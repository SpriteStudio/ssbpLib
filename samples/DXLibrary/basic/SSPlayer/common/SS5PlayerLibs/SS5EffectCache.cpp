#include "SS5EffectCache.h"
#include "SS5Player.h"
#include "SS5PlayerToPointer.h"
#include "SS5CellCache.h"
#include "SS5PlayerDataArrayReader.h"

namespace ss{
	

EffectCache::EffectCache(const ProjectData* data, const std::string& imageBaseDir, CellCache* cellCache)
{
	init(data, imageBaseDir, cellCache);
}

EffectCache::~EffectCache()
{
	releseReference();
}


//エフェクトファイル名を指定してEffectRefを得る
SsEffectModel* EffectCache::getReference(const std::string& name)
{
	SsEffectModel* ref = _dic.at(name);
	return ref;
}

void EffectCache::dump()
{
	std::map<std::string, SsEffectModel*>::iterator it = _dic.begin();
	while (it != _dic.end())
	{
		SSLOG("%s", (*it).second);
		++it;
	}
}


void EffectCache::init(const ProjectData* data, const std::string& imageBaseDir, CellCache* cellCache)
{
	SS_ASSERT2(data != NULL, "Invalid data");

	ToPointer ptr(data);

	//ssbpからエフェクトファイル配列を取得
	const EffectFile* effectFileArray = static_cast<const EffectFile*>(ptr(data->effectFileList));

	for (int listindex = 0; listindex < data->numEffectFileList; listindex++)
	{
		//エフェクトファイル配列からエフェクトファイルを取得
		const EffectFile* effectFile = &effectFileArray[listindex];

		//保持用のエフェクトファイル情報を作成
		SsEffectModel *effectmodel = new SsEffectModel();
		std::string effectFileName = static_cast<const char*>(ptr(effectFile->name));

		//エフェクトファイルからエフェクトノード配列を取得
		const EffectNode* effectNodeArray = static_cast<const EffectNode*>(ptr(effectFile->effectNode));
		for (int nodeindex = 0; nodeindex < effectFile->numNodeList; nodeindex++)
		{
			const EffectNode* effectNode = &effectNodeArray[nodeindex];		//エフェクトノード配列からエフェクトノードを取得

			SsEffectNode *node = new SsEffectNode();
			node->arrayIndex = effectNode->arrayIndex;
			node->parentIndex = effectNode->parentIndex;
			node->type = (SsEffectNodeType::_enum)effectNode->type;
			node->visible = true;

			SsEffectBehavior behavior;
			//セル情報を作成
			behavior.CellIndex = effectNode->cellIndex;
			CellRef* cellRef = behavior.CellIndex >= 0 ? cellCache->getReference(behavior.CellIndex) : NULL;
			if (cellRef)
			{
				behavior.refCell.pivot_X = cellRef->cell->pivot_X;
				behavior.refCell.pivot_Y = cellRef->cell->pivot_Y;
				behavior.refCell.texture = cellRef->texture;
				behavior.refCell.texname = cellRef->texname;
				behavior.refCell.rect = cellRef->rect;
				behavior.refCell.cellIndex = behavior.CellIndex;
				std::string name = static_cast<const char*>(ptr(cellRef->cell->name));
				behavior.refCell.cellName = name;

			}
			//				behavior.CellName;
			//				behavior.CellMapName;
			behavior.blendType = (SsRenderBlendType::_enum)effectNode->blendType;

			//エフェクトノードからビヘイビア配列を取得
			const ss_offset* behaviorArray = static_cast<const ss_offset*>(ptr(effectNode->Behavior));
			for (int behaviorindex = 0; behaviorindex < effectNode->numBehavior; behaviorindex++)
			{
				//ビヘイビア配列からビヘイビアを取得
				const ss_u16* behavior_adr = static_cast<const ss_u16*>(ptr(behaviorArray[behaviorindex]));
				DataArrayReader reader(behavior_adr);

				//パラメータを作ってpush_backで登録していく
				int type = reader.readS32();
				switch (type)
				{
				case SsEffectFunctionType::Basic:
				{
					//基本情報
					EffectParticleElementBasic readparam;
					readparam.priority = reader.readU32();			//表示優先度
					readparam.maximumParticle = reader.readU32();		//最大パーティクル数
					readparam.attimeCreate = reader.readU32();		//一度に作成するパーティクル数
					readparam.interval = reader.readU32();			//生成間隔
					readparam.lifetime = reader.readU32();			//エミッター生存時間
					readparam.speedMinValue = reader.readFloat();		//初速最小
					readparam.speedMaxValue = reader.readFloat();		//初速最大
					readparam.lifespanMinValue = reader.readU32();	//パーティクル生存時間最小
					readparam.lifespanMaxValue = reader.readU32();	//パーティクル生存時間最大
					readparam.angle = reader.readFloat();				//射出方向
					readparam.angleVariance = reader.readFloat();		//射出方向範囲

					ParticleElementBasic *effectParam = new ParticleElementBasic();
					effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
					effectParam->priority = readparam.priority;							//表示優先度
					effectParam->maximumParticle = readparam.maximumParticle;			//最大パーティクル数
					effectParam->attimeCreate = readparam.attimeCreate;					//一度に作成するパーティクル数
					effectParam->interval = readparam.interval;							//生成間隔
					effectParam->lifetime = readparam.lifetime;							//エミッター生存時間
					effectParam->speed.setMinMax(readparam.speedMinValue, readparam.speedMaxValue);				//初速
					effectParam->lifespan.setMinMax(readparam.lifespanMinValue, readparam.lifespanMaxValue);	//パーティクル生存時間
					effectParam->angle = readparam.angle;								//射出方向
					effectParam->angleVariance = readparam.angleVariance;				//射出方向範囲

					behavior.plist.push_back(effectParam);												//パラメータを追加
					break;
				}
				case SsEffectFunctionType::RndSeedChange:
				{
					//シード上書き
					EffectParticleElementRndSeedChange readparam;
					readparam.Seed = reader.readU32();				//上書きするシード値

					ParticleElementRndSeedChange *effectParam = new ParticleElementRndSeedChange();
					effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
					effectParam->Seed = readparam.Seed;							//上書きするシード値

					behavior.plist.push_back(effectParam);												//パラメータを追加
					break;
				}
				case SsEffectFunctionType::Delay:
				{
					//発生：タイミング
					EffectParticleElementDelay readparam;
					readparam.DelayTime = reader.readU32();			//遅延時間

					ParticleElementDelay *effectParam = new ParticleElementDelay();
					effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
					effectParam->DelayTime = readparam.DelayTime;			//遅延時間

					behavior.plist.push_back(effectParam);												//パラメータを追加
					break;
				}
				case SsEffectFunctionType::Gravity:
				{
					//重力を加える
					EffectParticleElementGravity readparam;
					readparam.Gravity_x = reader.readFloat();			//X方向の重力
					readparam.Gravity_y = reader.readFloat();			//Y方向の重力

					ParticleElementGravity *effectParam = new ParticleElementGravity();
					effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
					effectParam->Gravity.x = readparam.Gravity_x;			//X方向の重力
					effectParam->Gravity.y = readparam.Gravity_y;			//Y方向の重力

					behavior.plist.push_back(effectParam);												//パラメータを追加
					break;
				}
				case SsEffectFunctionType::Position:
				{
					//座標：生成時
					EffectParticleElementPosition readparam;
					readparam.OffsetXMinValue = reader.readFloat();	//X座標に加算最小
					readparam.OffsetXMaxValue = reader.readFloat();	//X座標に加算最大
					readparam.OffsetYMinValue = reader.readFloat();	//X座標に加算最小
					readparam.OffsetYMaxValue = reader.readFloat();	//X座標に加算最大

					ParticleElementPosition *effectParam = new ParticleElementPosition();
					effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
					effectParam->OffsetX.setMinMax(readparam.OffsetXMinValue, readparam.OffsetXMaxValue); 	//X座標に加算最小
					effectParam->OffsetY.setMinMax(readparam.OffsetYMinValue, readparam.OffsetYMaxValue);	//X座標に加算最小

					behavior.plist.push_back(effectParam);												//パラメータを追加
					break;
				}
				case SsEffectFunctionType::Rotation:
				{
					//Z回転を追加
					EffectParticleElementRotation readparam;
					readparam.RotationMinValue = reader.readFloat();		//角度初期値最小
					readparam.RotationMaxValue = reader.readFloat();		//角度初期値最大
					readparam.RotationAddMinValue = reader.readFloat();	//角度初期加算値最小
					readparam.RotationAddMaxValue = reader.readFloat();	//角度初期加算値最大

					ParticleElementRotation *effectParam = new ParticleElementRotation();
					effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
					effectParam->Rotation.setMinMax(readparam.RotationMinValue, readparam.RotationMaxValue);		//角度初期値最小
					effectParam->RotationAdd.setMinMax(readparam.RotationAddMinValue, readparam.RotationAddMaxValue);	//角度初期加算値最小

					behavior.plist.push_back(effectParam);												//パラメータを追加
					break;
				}
				case SsEffectFunctionType::TransRotation:
				{
					//Z回転速度変更
					EffectParticleElementRotationTrans readparam;
					readparam.RotationFactor = reader.readFloat();		//角度目標加算値
					readparam.EndLifeTimePer = reader.readFloat();		//到達時間

					ParticleElementRotationTrans *effectParam = new ParticleElementRotationTrans();
					effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
					effectParam->RotationFactor = readparam.RotationFactor;		//角度目標加算値
					effectParam->EndLifeTimePer = readparam.EndLifeTimePer;		//到達時間

					behavior.plist.push_back(effectParam);												//パラメータを追加
					break;
				}
				case SsEffectFunctionType::TransSpeed:
				{
					//速度：変化
					EffectParticleElementTransSpeed readparam;
					readparam.SpeedMinValue = reader.readFloat();			//速度目標値最小
					readparam.SpeedMaxValue = reader.readFloat();			//速度目標値最大

					ParticleElementTransSpeed *effectParam = new ParticleElementTransSpeed();
					effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
					effectParam->Speed.setMinMax(readparam.SpeedMinValue, readparam.SpeedMaxValue);			//速度目標値最小

					behavior.plist.push_back(effectParam);												//パラメータを追加
					break;
				}
				case SsEffectFunctionType::TangentialAcceleration:
				{
					//接線加速度
					EffectParticleElementTangentialAcceleration readparam;
					readparam.AccelerationMinValue = reader.readFloat();	//設定加速度最小
					readparam.AccelerationMaxValue = reader.readFloat();	//設定加速度最大

					ParticleElementTangentialAcceleration *effectParam = new ParticleElementTangentialAcceleration();
					effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
					effectParam->Acceleration.setMinMax(readparam.AccelerationMinValue, readparam.AccelerationMaxValue);	//設定加速度最小

					behavior.plist.push_back(effectParam);												//パラメータを追加
					break;
				}
				case SsEffectFunctionType::InitColor:
				{
					//カラーRGBA：生成時
					EffectParticleElementInitColor readparam;
					readparam.ColorMinValue = reader.readU32();			//設定カラー最小
					readparam.ColorMaxValue = reader.readU32();			//設定カラー最大

					ParticleElementInitColor *effectParam = new ParticleElementInitColor();
					effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類

					int a = (readparam.ColorMinValue & 0xFF000000) >> 24;
					int r = (readparam.ColorMinValue & 0x00FF0000) >> 16;
					int g = (readparam.ColorMinValue & 0x0000FF00) >> 8;
					int b = (readparam.ColorMinValue & 0x000000FF) >> 0;
					SsU8Color mincol(r, g, b, a);
					a = (readparam.ColorMaxValue & 0xFF000000) >> 24;
					r = (readparam.ColorMaxValue & 0x00FF0000) >> 16;
					g = (readparam.ColorMaxValue & 0x0000FF00) >> 8;
					b = (readparam.ColorMaxValue & 0x000000FF) >> 0;
					SsU8Color maxcol(r, g, b, a);
					effectParam->Color.setMinMax(mincol, maxcol);			//設定カラー最小

					behavior.plist.push_back(effectParam);												//パラメータを追加
					break;
				}
				case SsEffectFunctionType::TransColor:
				{
					//カラーRGB：変化
					EffectParticleElementTransColor readparam;
					readparam.ColorMinValue = reader.readU32();			//設定カラー最小
					readparam.ColorMaxValue = reader.readU32();			//設定カラー最大

					ParticleElementTransColor *effectParam = new ParticleElementTransColor();
					effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類

					int a = (readparam.ColorMinValue & 0xFF000000) >> 24;
					int r = (readparam.ColorMinValue & 0x00FF0000) >> 16;
					int g = (readparam.ColorMinValue & 0x0000FF00) >> 8;
					int b = (readparam.ColorMinValue & 0x000000FF) >> 0;
					SsU8Color mincol(r, g, b, a);
					a = (readparam.ColorMaxValue & 0xFF000000) >> 24;
					r = (readparam.ColorMaxValue & 0x00FF0000) >> 16;
					g = (readparam.ColorMaxValue & 0x0000FF00) >> 8;
					b = (readparam.ColorMaxValue & 0x000000FF) >> 0;
					SsU8Color maxcol(r, g, b, a);
					effectParam->Color.setMinMax(mincol, maxcol);			//設定カラー最小

					behavior.plist.push_back(effectParam);												//パラメータを追加
					break;
				}
				case SsEffectFunctionType::AlphaFade:
				{
					//フェード
					EffectParticleElementAlphaFade readparam;
					readparam.disprangeMinValue = reader.readFloat();		//表示区間開始
					readparam.disprangeMaxValue = reader.readFloat();		//表示区間終了

					ParticleElementAlphaFade *effectParam = new ParticleElementAlphaFade();
					effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
					effectParam->disprange.setMinMax(readparam.disprangeMinValue, readparam.disprangeMaxValue);		//表示区間開始

					behavior.plist.push_back(effectParam);												//パラメータを追加
					break;
				}
				case SsEffectFunctionType::Size:
				{
					//スケール：生成時
					EffectParticleElementSize readparam;
					readparam.SizeXMinValue = reader.readFloat();			//幅倍率最小
					readparam.SizeXMaxValue = reader.readFloat();			//幅倍率最大
					readparam.SizeYMinValue = reader.readFloat();			//高さ倍率最小
					readparam.SizeYMaxValue = reader.readFloat();			//高さ倍率最大
					readparam.ScaleFactorMinValue = reader.readFloat();		//倍率最小
					readparam.ScaleFactorMaxValue = reader.readFloat();		//倍率最大

					ParticleElementSize *effectParam = new ParticleElementSize();
					effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
					effectParam->SizeX.setMinMax(readparam.SizeXMinValue, readparam.SizeXMaxValue);			//幅倍率最小
					effectParam->SizeY.setMinMax(readparam.SizeYMinValue, readparam.SizeYMaxValue);			//高さ倍率最小
					effectParam->ScaleFactor.setMinMax(readparam.ScaleFactorMinValue, readparam.ScaleFactorMaxValue);		//倍率最小

					behavior.plist.push_back(effectParam);												//パラメータを追加
					break;
				}
				case SsEffectFunctionType::TransSize:
				{
					//スケール：変化
					EffectParticleElementTransSize readparam;
					readparam.SizeXMinValue = reader.readFloat();			//幅倍率最小
					readparam.SizeXMaxValue = reader.readFloat();			//幅倍率最大
					readparam.SizeYMinValue = reader.readFloat();			//高さ倍率最小
					readparam.SizeYMaxValue = reader.readFloat();			//高さ倍率最大
					readparam.ScaleFactorMinValue = reader.readFloat();		//倍率最小
					readparam.ScaleFactorMaxValue = reader.readFloat();		//倍率最大

					ParticleElementTransSize *effectParam = new ParticleElementTransSize();
					effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
					effectParam->SizeX.setMinMax(readparam.SizeXMinValue, readparam.SizeXMaxValue);			//幅倍率最小
					effectParam->SizeY.setMinMax(readparam.SizeYMinValue, readparam.SizeYMaxValue);			//高さ倍率最小
					effectParam->ScaleFactor.setMinMax(readparam.ScaleFactorMinValue, readparam.ScaleFactorMaxValue);		//倍率最小

					behavior.plist.push_back(effectParam);												//パラメータを追加
					break;
				}
				case SsEffectFunctionType::PointGravity:
				{
					//重力点の追加
					EffectParticlePointGravity readparam;
					readparam.Position_x = reader.readFloat();				//重力点X
					readparam.Position_y = reader.readFloat();				//重力点Y
					readparam.Power = reader.readFloat();					//パワー

					ParticlePointGravity *effectParam = new ParticlePointGravity();
					effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
					effectParam->Position.x = readparam.Position_x;				//重力点X
					effectParam->Position.y = readparam.Position_y;				//重力点Y
					effectParam->Power = readparam.Power;					//パワー

					behavior.plist.push_back(effectParam);												//パラメータを追加
					break;
				}
				case SsEffectFunctionType::TurnToDirectionEnabled:
				{
					//進行方向に向ける
					EffectParticleTurnToDirectionEnabled readparam;
					readparam.Rotation = reader.readFloat();					//フラグ

					ParticleTurnToDirectionEnabled *effectParam = new ParticleTurnToDirectionEnabled();
					effectParam->Rotation = readparam.Rotation;
					effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類

					behavior.plist.push_back(effectParam);												//パラメータを追加
					break;
				}
				case SsEffectFunctionType::InfiniteEmitEnabled:
				{
					EffectParticleInfiniteEmitEnabled readparam;
					readparam.flag = reader.readS32();					//フラグ

					ParticleInfiniteEmitEnabled *effectParam = new ParticleInfiniteEmitEnabled();
					effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類

					behavior.plist.push_back(effectParam);												//パラメータを追加
					break;
				}
				default:
					break;
				}
			}
			node->behavior = behavior;
			effectmodel->nodeList.push_back(node);
			if (nodeindex == 0)
			{
			}
		}
		//ツリーの構築
		if (effectmodel->nodeList.size() > 0)
		{
			effectmodel->root = effectmodel->nodeList[0];	//rootノードを追加
			for (size_t i = 1; i < effectmodel->nodeList.size(); i++)
			{
				int pi = effectmodel->nodeList[i]->parentIndex;
				if (pi >= 0)
				{
					effectmodel->nodeList[pi]->addChildEnd(effectmodel->nodeList[i]);
				}
			}
		}
		effectmodel->lockRandSeed = effectFile->lockRandSeed; 	 // ランダムシード固定値
		effectmodel->isLockRandSeed = effectFile->isLockRandSeed;  // ランダムシードを固定するか否か
		effectmodel->fps = effectFile->fps;             //
		effectmodel->effectName = effectFileName;
		effectmodel->layoutScaleX = effectFile->layoutScaleX;	//レイアウトスケールX
		effectmodel->layoutScaleY = effectFile->layoutScaleY;	//レイアウトスケールY



		SSLOG("effect key: %s", effectFileName.c_str());
		_dic.insert(std::map<std::string, SsEffectModel*>::value_type(effectFileName, effectmodel));
	}
}


//エフェクトファイル情報の削除
void EffectCache::releseReference(void)
{
	std::map<std::string, SsEffectModel*>::iterator it = _dic.begin();
	while (it != _dic.end())
	{
		SsEffectModel* effectmodel = it->second;

		if (effectmodel)
		{
			for (int nodeindex = 0; nodeindex < effectmodel->nodeList.size(); nodeindex++)
			{
				SsEffectNode* node = effectmodel->nodeList.at(nodeindex);
				for (int behaviorindex = 0; behaviorindex < node->behavior.plist.size(); behaviorindex++)
				{
					SsEffectElementBase* eb = node->behavior.plist.at(behaviorindex);
					delete eb;
				}
				node->behavior.plist.clear();
			}
			if (effectmodel->nodeList.size() > 0)
			{
				SsEffectNode* node = effectmodel->nodeList.at(0);
				delete node;
				effectmodel->nodeList.clear();
			}
			effectmodel->root = 0;

		}
		delete effectmodel;
		it++;
	}
	_dic.clear();
}


} //namespace ss
