#pragma once

#include <memory>
#include "common/Animator/ssplayer_matrix.h"
#include "SS5PlayerTypes.h"

namespace ss{

/**
 * State
 * パーツの情報を格納します。Stateの内容をもとに描画処理を作成してください。
 */
struct State
{
	int flags;						/// このフレームで更新が行われるステータスのフラグ
	int cellIndex;					/// パーツに割り当てられたセルの番号
	float x;						/// SS5アトリビュート：X座標
	float y;						/// SS5アトリビュート：Y座標
	float z;						/// SS5アトリビュート：Z座標
	float pivotX;					/// 原点Xオフセット＋セルに設定された原点オフセットX
	float pivotY;					/// 原点Yオフセット＋セルに設定された原点オフセットY
	float rotationX;				/// X回転
	float rotationY;				/// Y回転
	float rotationZ;				/// Z回転
	float scaleX;					/// Xスケール
	float scaleY;					/// Yスケール
	int opacity;					/// 不透明度（0～255）
	float size_X;					/// SS5アトリビュート：Xサイズ
	float size_Y;					/// SS5アトリビュート：Xサイズ
	float uv_move_X;				/// SS5アトリビュート：UV X移動
	float uv_move_Y;				/// SS5アトリビュート：UV Y移動
	float uv_rotation;				/// SS5アトリビュート：UV 回転
	float uv_scale_X;				/// SS5アトリビュート：UV Xスケール
	float uv_scale_Y;				/// SS5アトリビュート：UV Yスケール
	float boundingRadius;			/// SS5アトリビュート：当たり半径
	int colorBlendFunc;				/// SS5アトリビュート：カラーブレンドのブレンド方法
	int colorBlendType;				/// SS5アトリビュート：カラーブレンドの単色か頂点カラーか。
	bool flipX;						/// 横反転（親子関係計算済）
	bool flipY;						/// 縦反転（親子関係計算済）
	bool isVisibled;				/// 非表示（親子関係計算済）
	SSV3F_C4B_T2F_Quad quad;		/// 頂点データ、座標、カラー値、UVが含まれる（頂点変形、サイズXY、UV移動XY、UVスケール、UV回転、反転が反映済）
	TextuerData texture;			/// セルに対応したテクスチャ番号（ゲーム側で管理している番号を設定する）
	SSRect rect;					/// セルに対応したテクスチャ内の表示領域（開始座標、幅高さ）
	int blendfunc;					/// パーツに設定されたブレンド方法
	SSMatrix mat;					/// パーツの位置を算出するためのマトリクス（親子関係計算済）
	//再生用パラメータ
	float Calc_rotationX;			/// X回転（親子関係計算済）
	float Calc_rotationY;			/// Y回転（親子関係計算済）
	float Calc_rotationZ;			/// Z回転（親子関係計算済）
	float Calc_scaleX;				/// Xスケール（親子関係計算済）
	float Calc_scaleY;				/// Yスケール（親子関係計算済）
	int Calc_opacity;				/// 不透明度（0～255）（親子関係計算済）
	//インスタンスアトリビュート
	int			instanceValue_curKeyframe;
	int			instanceValue_startFrame;
	int			instanceValue_endFrame;
	int			instanceValue_loopNum;
	float		instanceValue_speed;
	int			instanceValue_loopflag;
	//エフェクトアトリビュート
	int			effectValue_curKeyframe;
	int			effectValue_startTime;
	float		effectValue_speed;
	int			effectValue_loopflag;

	void init()
	{
		flags = 0;
		cellIndex = 0;
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		pivotX = 0.0f;
		pivotY = 0.0f;
		rotationX = 0.0f;
		rotationY = 0.0f;
		rotationZ = 0.0f;
		scaleX = 1.0f;
		scaleY = 1.0f;
		opacity = 255;
		size_X = 1.0f;
		size_Y = 1.0f;
		uv_move_X = 0.0f;
		uv_move_Y = 0.0f;
		uv_rotation = 0.0f;
		uv_scale_X = 1.0f;
		uv_scale_Y = 1.0f;
		boundingRadius = 0.0f;
		colorBlendFunc = 0;
		colorBlendType = 0;
		flipX = false;
		flipY = false;
		isVisibled = false;
		quad = SSV3F_C4B_T2F_Quad();
		texture.handle = 0;
		texture.size_w = 0;
		texture.size_h = 0;
		rect.size.height = 0;
		rect.size.width = 0;
		rect.origin.x = 0;
		rect.origin.y = 0;
		blendfunc = 0;
		mat.setupIdentity();
		instanceValue_curKeyframe = 0;
		instanceValue_startFrame = 0;
		instanceValue_endFrame = 0;
		instanceValue_loopNum = 0;
		instanceValue_speed = 0;
		instanceValue_loopflag = 0;
		effectValue_curKeyframe = 0;
		effectValue_startTime = 0;
		effectValue_speed = 0;
		effectValue_loopflag = 0;

		Calc_rotationX = 0.0f;
		Calc_rotationY = 0.0f;
		Calc_rotationZ = 0.0f;
		Calc_scaleX = 1.0f;
		Calc_scaleY = 1.0f;
		Calc_opacity = 255;

	}

	State() { init(); }
};


};	// namespace ss
