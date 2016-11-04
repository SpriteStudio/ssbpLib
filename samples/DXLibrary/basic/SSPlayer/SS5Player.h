﻿//-----------------------------------------------------------
// ssbpLib v1.3.3
//
// Copyright(C) Web Technology Corp.
// http://www.webtech.co.jp/
//-----------------------------------------------------------
//
// SS5Player.h
//


/************************************************************
対応するssbpフォーマットはバージョン4です。
Ss5ConverterのフォーマットバージョンはSpriteStudioSDKを参照してください。
https://github.com/SpriteStudio/SpriteStudio5-SDK/wiki/%E3%82%B3%E3%83%B3%E3%83%90%E3%83%BC%E3%82%BF%E3%81%AE%E4%BD%BF%E3%81%84%E6%96%B9

- Quick start
 
  #include "SS5Player.h"

  
  // SS5プレイヤーの宣言
  ss::Player *ssplayer;
  ss::ResourceManager *resman;


  //リソースマネージャの作成
  resman = ss::ResourceManager::getInstance();
  //プレイヤーの作成
  ssplayer = ss::Player::create();

  //アニメデータをリソースに追加
  //それぞれのプラットフォームに合わせたパスへ変更してください。
  resman->addData("character_template_comipo\\character_template1.ssbp");
  //プレイヤーにリソースを割り当て
  ssplayer->setData("character_template1");					// ssbpファイル名（拡張子不要）
  //再生するモーションを設定
  ssplayer->play("character_template_3head/stance");		// アニメーション名を指定(ssae名/アニメーション名)


  //表示位置を設定
  ssplayer->setPosition(1280/2, 720);
  //スケール設定
  ssplayer->setScale(0.5f, 0.5f);
  //回転を設定
  ssplayer->setRotation(0.0f, 0.0f, 0.0f);
  //透明度を設定
  ssplayer->setAlpha(255);
  //反転を設定
  ssplayer->setFlip(false, false);


  //メインループで呼び出してください。
  ssplayer->update(dt);					//プレイヤーの更新
  ssplayer->draw();						//プレイヤーの描画


  終了処理で resman、player を delete してください。
  //SS5Playerの削除
  delete (ssplayer);
  delete (resman);

  使用するアニメーションに合わせて Playerクラス定義部分にある設定用定数を変更してください。

  ssbpLibの制限についてはこちらのページを参照してください。
  https://github.com/SpriteStudio/ssbpLib/wiki

  使用方法についてはPlayerクラスのコメントを参照してください。

*************************************************************/

#ifndef SS5Player_h
#define SS5Player_h

#include "./common/SS5PlayerLibs/SS5PlayerData.h"
#include "./common/SS5PlayerLibs/SS5PlayerTypes.h"
#include "./common/SS5PlayerLibs/SS5PlayerPartState.h"
#include "SS5PlayerPlatform.h"
#include <map>
#include <vector>
#include <string>
#include <stdarg.h>
#include <assert.h>
#include <time.h>

//エフェクト関連
#include "./common/loader/ssloader.h"
#include "./common/Animator/ssplayer_macro.h"
#include "./common/Animator/ssplayer_matrix.h"
#include "./common/Animator/ssplayer_effectfunction.h"
#include "./common/Animator/ssplayer_cellmap.h"
#include "./common/Animator/ssplayer_PartState.h"
//#include "./Common/Animator/MersenneTwister.h"

#pragma warning(disable : 4996)

namespace ss
{
class CustomSprite;
class CellCache;
class CellRef;
class AnimeCache;
class AnimeRef;
class ResourceSet;
struct ProjectData;
class SSSize;
class Player;
class ResourceManager;

//関数定義
extern void get_uv_rotation(float *u, float *v, float cu, float cv, float deg);



/**
 * UserData
 */
struct UserData
{
	enum {
		FLAG_INTEGER	= 1 << 0,
		FLAG_RECT		= 1 << 1,
		FLAG_POINT		= 1 << 2,
		FLAG_STRING		= 1 << 3
	};

	const char*	partName;		/// Part name
	int			frameNo;		/// Frame no

	int			flags;			/// Flags of valid data
	int			integer;		/// Integer
	int			rect[4];		/// Rectangle Left, Top, Right, Bottom
	int			point[2];		/// Position X, Y
	const char*	str;			/// String (zero terminated)
	int			strSize;		/// String size (byte count)
};


/**
* LabelData
*/
struct LabelData
{
	std::string	str;			/// String (zero terminated)
	int			strSize;		/// String size (byte count)
	int			frameNo;		/// Frame no
};

//インスタンスデータ
struct Instance
{
	int			refStartframe;		//開始フレーム
	int			refEndframe;		//終了フレーム
	float		refSpeed;			//再生速度
	int			refloopNum;			//ループ回数
	bool		infinity;			//無限ループ
	bool		reverse;			//逆再選
	bool		pingpong;			//往復
	bool		independent;		//独立動作
	void clear(void)
	{
		refStartframe = 0;			//開始フレーム
		refEndframe = 1;			//終了フレーム
		refSpeed = 1;				//再生速度
		refloopNum = 1;				//ループ回数
		infinity = false;			//無限ループ
		reverse = false;			//逆再選
		pingpong = false;			//往復
		independent = false;		//独立動作
	}
};


/**
* ResluteState
* ゲーム側に返すパーツステータス。
* 必要に応じてカスタマイズしてください。
*/
struct ResluteState
{
	int flags;						/// このフレームで更新が行われるステータスのフラグ
	int cellIndex;					/// パーツに割り当てられたセルの番号
	float x;						/// SS5アトリビュート：X座標
	float y;						/// SS5アトリビュート：Y座標
	float z;						/// SS5アトリビュート：Z座標
	float pivotX;					/// 原点Xオフセット＋セルに設定された原点オフセットX
	float pivotY;					/// 原点Yオフセット＋セルに設定された原点オフセットY
	float rotationX;				/// X回転（親子関係計算済）
	float rotationY;				/// Y回転（親子関係計算済）
	float rotationZ;				/// Z回転（親子関係計算済）
	float scaleX;					/// Xスケール（親子関係計算済）
	float scaleY;					/// Yスケール（親子関係計算済）
	int opacity;					/// 不透明度（0～255）（親子関係計算済）
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

	int	part_type;					/// パーツ種別
	int	part_boundsType;			/// 当たり判定種類
	int	part_alphaBlendType;		/// BlendType
	int	part_labelcolor;			/// ラベルカラー
};

//プロジェクトフラグ
enum {
	HEAD_FLAG_rootPartFunctionAsVer4 = 1 << 0,		//不透明度・反転・非表示アトリビュートの継承方法をVer.4と同様にする
	HEAD_FLAG_dontUseMatrixForTransform = 1 << 1,	//親子の座標変換にマトリックスを使用しない（Ver4互換）
};

/**
* 再生するフレームに含まれるパーツデータのフラグ
*/
enum {
	PART_FLAG_INVISIBLE			= 1 << 0,		/// 非表示
	PART_FLAG_FLIP_H			= 1 << 1,		/// 横反転
	PART_FLAG_FLIP_V			= 1 << 2,		/// 縦反転

	// optional parameter flags
	PART_FLAG_CELL_INDEX		= 1 << 3,		/// セル番号
	PART_FLAG_POSITION_X		= 1 << 4,		/// X座標
	PART_FLAG_POSITION_Y		= 1 << 5,		/// Y座標
	PART_FLAG_POSITION_Z		= 1 << 6,		/// Z座標
	PART_FLAG_PIVOT_X			= 1 << 7,		/// 原点オフセットX
	PART_FLAG_PIVOT_Y			= 1 << 8,		/// 原点オフセットY
	PART_FLAG_ROTATIONX			= 1 << 9,		/// X回転
	PART_FLAG_ROTATIONY			= 1 << 10,		/// Y回転
	PART_FLAG_ROTATIONZ			= 1 << 11,		/// Z回転
	PART_FLAG_SCALE_X			= 1 << 12,		/// スケールX
	PART_FLAG_SCALE_Y			= 1 << 13,		/// スケールY
	PART_FLAG_OPACITY			= 1 << 14,		/// 不透明度
	PART_FLAG_COLOR_BLEND		= 1 << 15,		/// カラーブレンド
	PART_FLAG_VERTEX_TRANSFORM	= 1 << 16,		/// 頂点変形

	PART_FLAG_SIZE_X			= 1 << 17,		/// サイズX
	PART_FLAG_SIZE_Y			= 1 << 18,		/// サイズY

	PART_FLAG_U_MOVE			= 1 << 19,		/// UV移動X
	PART_FLAG_V_MOVE			= 1 << 20,		/// UV移動Y
	PART_FLAG_UV_ROTATION		= 1 << 21,		/// UV回転
	PART_FLAG_U_SCALE			= 1 << 22,		/// UVスケールX
	PART_FLAG_V_SCALE			= 1 << 23,		/// UVスケールY
	PART_FLAG_BOUNDINGRADIUS	= 1 << 24,		/// 当たり半径

	PART_FLAG_INSTANCE_KEYFRAME = 1 << 25,		/// インスタンス
	PART_FLAG_EFFECT_KEYFRAME   = 1 << 26,		/// エフェクト

	NUM_PART_FLAGS
};

/**
* 頂点変形フラグ
*/
enum {
	VERTEX_FLAG_LT = 1 << 0,
	VERTEX_FLAG_RT = 1 << 1,
	VERTEX_FLAG_LB = 1 << 2,
	VERTEX_FLAG_RB = 1 << 3,
	VERTEX_FLAG_ONE = 1 << 4	// color blend only
};

/**
* インスタンスループ設定フラグ
*/
enum {
	INSTANCE_LOOP_FLAG_INFINITY = 1 << 0,		//
	INSTANCE_LOOP_FLAG_REVERSE = 1 << 1,
	INSTANCE_LOOP_FLAG_PINGPONG = 1 << 2,
	INSTANCE_LOOP_FLAG_INDEPENDENT = 1 << 3,
};

//エフェクトアトリビュートのループフラグ
enum {
	EFFECT_LOOP_FLAG_INDEPENDENT = 1 << 0,
};

/**
* Animation Part Type
*/
enum
{
	PARTTYPE_INVALID = -1,
	PARTTYPE_NULL,			/// null。領域を持たずSRT情報のみ。ただし円形の当たり判定は設定可能。
	PARTTYPE_NORMAL,		/// 通常パーツ。領域を持つ。画像は無くてもいい。
	PARTTYPE_TEXT,			/// テキスト(予約　未実装）
	PARTTYPE_INSTANCE,		/// インスタンス。他アニメ、パーツへの参照。シーン編集モードの代替になるもの
	PARTTYPE_EFFECT,		// ss5.5対応エフェクトパーツ
	PARTTYPE_NUM
};

/*
* 当たり判定の種類
*/
enum
{
	INVALID = -1,
	NONE,			///< 当たり判定として使わない。
	QUAD,			///< 自在に変形する四辺形。頂点変形など適用後の４角を結んだ領域。最も重い。
	AABB,			///< 回転しない全体を囲む矩形で交差判定
	CIRCLE,			///< 真円の半径で距離により判定する
	CIRCLE_SMIN,	///< 真円の半径で距離により判定する (スケールはx,yの最小値をとる）
	CIRCLE_SMAX,	///< 真円の半径で距離により判定する (スケールはx,yの最大値をとる）
	num
};

/**
* αブレンド方法
*/
enum BlendType
{
	BLEND_MIX,		///< 0 ブレンド（ミックス）
	BLEND_MUL,		///< 1 乗算
	BLEND_ADD,		///< 2 加算
	BLEND_SUB		///< 3 減算
};

/*
Common\Loader\sstypes.hに実際の定義があります。
/// テクスチャラップモード
namespace SsTexWrapMode
{
	enum _enum
	{
		invalid = -1,	/// なし
		clamp,			/// クランプする
		repeat,			/// リピート
		mirror,			/// ミラー
		num
	};
};

/// テクスチャフィルターモード 画素補間方法
namespace SsTexFilterMode
{
	enum _enum
	{
		invalid = -1,
		nearlest,	///< ニアレストネイバー
		linear,		///< リニア、バイリニア
		num
	};
};
*/

//カラーラベル定数
#define COLORLABELSTR_NONE		""
#define COLORLABELSTR_RED		"Red"
#define COLORLABELSTR_ORANGE	"Orange"
#define COLORLABELSTR_YELLOW	"Yellow"
#define COLORLABELSTR_GREEN		"Green"
#define COLORLABELSTR_BLUE		"Blue"
#define COLORLABELSTR_VIOLET	"Violet"
#define COLORLABELSTR_GRAY		"Gray"
enum
{
	COLORLABEL_NONE,		///< 0 なし
	COLORLABEL_RED,			///< 1 赤
	COLORLABEL_ORANGE,		///< 2 オレンジ
	COLORLABEL_YELLOW,		///< 3 黄色
	COLORLABEL_GREEN,		///< 4 緑
	COLORLABEL_BLUE,		///< 5 青
	COLORLABEL_VIOLET,		///< 6 紫
	COLORLABEL_GRAY,		///< 7 灰色
};

//------------------------------------------------------------------------------
//プレイヤーの設定定義
//使用するアニメーションに合わせて設定してください。


//プレイヤーで扱えるアニメに含まれるパーツの最大数
#define PART_VISIBLE_MAX (1024)

//座標系の設定：上方向がマイナスの場合は有効
//このサンプルでは有効にするとDXライブラリのスプライト表示機能を使用して描画します。
//スプライト機能を使用する場合、拡大率や反転に制限が付きます。
//無効にした場合は上方向をプラスとして計算を行います。
//このサンプルでは3D機能を使用して描画します。
//それぞれのプラットフォームに合わせた座標系で使用してください。
//DXライブラリで使用する場合はUP_MINUSを無効にしたままsetPositionで
//画面サイズから引いた座標を設定して運用するといいと思います。
//#define UP_MINUS

//描画に頂点データを使用する場合有効
//無効にした場合はアフィン変換を使用してスプライトの表示を行います。
//親パーツにマイナススケールを使用した場合に再現できない場合があります。
#define USE_VERTEX

//互換性設定の親子の座標変換にマトリクス使用しない（Ver4互換）を設定した場合に動作を再現します。
//互換性設定はssbp内に出力されており、設定によって処理が分岐します。
//設定がされていないデータはSS5の挙動を再現します。
#define USE_VER4TRANSFORM

//Ver1.2系と同等の挙動にする場合は
//#define USE_VERTEX
//#define USE_VER4TRANSFORM
//をコメントアウトしてください。

//------------------------------------------------------------------------------


/**
 * Player
 */
class Player
{
public:
	/**
	 * Playerインスタンスを構築します.
	 *
	 * @param  resman  使用するResourceManagerインスタンス. 省略時はデフォルトインスタンスが使用されます.
	 * @return Playerインスタンス
	 */
	static Player* create(ResourceManager* resman = NULL);

	/**
	 * 使用するResourceManagerインスタンスを設定します.
	 *
	 * @param  resman  使用するResourceManagerインスタンス. 省略時はデフォルトインスタンスが使用されます.
	 */
	void setResourceManager(ResourceManager* resman = NULL);

	/**
	 * 使用中のResourceManagerインスタンスを解放します.
	 * 再度ResourceManagerインスタンスを設定するまでは再生できなくなります.
	 */
	void releaseResourceManager();

	/**
	 * 再生するssbpデータのdataKeyを設定します.
	 *
	 * @param  dataKey  再生するデータのdataKey
	 */
	void setData(const std::string& dataKey);


	/**
	* アニメーションの再生を開始します.
	*
	* @param  ssaeName      パック名(ssae名）
	* @param  motionName    再生するモーション名
	* @param  loop          再生ループ数の指定. 省略時は0
	* @param  startFrameNo  再生を開始するフレームNoの指定. 省略時は0
	*/
	void play(const std::string& ssaeName, const std::string& motionName, int loop = 0, int startFrameNo = 0);

	/**
	* アニメーションの再生を開始します.
	* アニメーション名から再生するデータを選択します.
	* "ssae名/モーション名で指定してください.
	* sample.ssaeのanime_1を指定する場合、sample/anime_1となります.
	* ※ver1.1からモーション名のみで指定する事はできなくなりました。
	*
	* @param  animeName     再生するアニメーション名
	* @param  loop          再生ループ数の指定. 省略時は0
	* @param  startFrameNo  再生を開始するフレームNoの指定. 省略時は0
	*/
	void play(const std::string& animeName, int loop = 0, int startFrameNo = 0);

	/**
	* 現在再生しているモーションとブレンドしながら再生します。
	* アニメーション名から再生するデータを選択します.
	* "ssae名/モーション名で指定してください.
	* sample.ssaeのanime_1を指定する場合、sample/anime_1となります.
	* ※ver1.1からモーション名のみで指定する事はできなくなりました。
	*
	* ブレンドするアニメーションの条件は以下になります。
	* ・同じssbp内に含まれている事
	* ・同じパーツ構成（パーツ順、パーツ数）である事
	* SpriteStudioのフレームコントロールに並ぶパーツを上から順にブレンドしていきます。
	* パーツ名等のチェックは行なっていませんので遷移元と遷移先アニメのパーツの順番を同じにする必要があります。
	* 遷移元と遷移先のパーツ構成があっていない場合、正しくブレンドされませんのでご注意ください。
	*
	* 合成されるアトリビュートは
	* 座標X、座標Y、X回転、Y回転、Z回転、スケールX、スケールYのみです。
	* それ以外のアトリビュートは遷移先アニメの値が適用されます。
	* インスタンスパーツが参照しているソースアニメはブレンドされません。
	* エフェクトパーツから発生したパーティクルはブレンドされません。
	* 
	*
	* @param  animeName     再生するアニメーション名
	* @param  loop          再生ループ数の指定. 省略時は0
	* @param  startFrameNo  再生を開始するフレームNoの指定. 省略時は0
	* @param  blendTime		モーションブレンドを行う時間、単位は秒　省略時は1秒
	*/
	void motionBlendPlay(const std::string& animeName, int loop = 0, int startFrameNo = 0, float blendTime = 0.1f);

	/**
	 * 再生を中断します.
	 */
	void animePause();

	/**
	 * 再生を再開します.
	 */
	void animeResume();

	/**
	 * 再生を停止します.
	 * ゲーム側でアニメーションの表示フレームを制御する場合はstop()を呼び出した後
	 * ゲーム側の更新処理でsetFrameNo()を呼び出し指定のフレームを表示してください。
	 */
	void stop();

	/**
	 * 再生しているアニメーションのパック名(ssae)を返します.
	 *
	 * @return パック名(ssae)
	 */
	const std::string& getPlayPackName() const;

	/**
	 * 再生しているアニメーション名を返します.
	 *
	 * @return アニメーション名
	 */
	const std::string& getPlayAnimeName() const;
	
	/**
	* アニメーションの総フレームを取得します.
	*
	* @return 総フレーム
	*/
	int getMaxFrame() const;

	/**
	 * 再生フレームNoを取得します.
	 * Get frame no of playing.
	 *
	 * @return 再生フレームNo. frame no.
	 */
	int getFrameNo() const;

	/**
	 * 再生フレームNoを設定します.
	 * Set frame no of playing.
	 *
	 * @param 再生フレームNo. frame no.
	 */
	void setFrameNo(int frameNo);

	/**
	 * 再生スピードを取得します. (1.0f:標準)
	 * Set speed to play. (1.0f:normal speed)
	 */
	float getStep() const;

	/**
	 * 再生スピードを設定します. (1.0f:標準)
	 * Get speed to play. (1.0f:normal speed)
	 */
	void setStep(float step);
	
	/** 
	 * 指定されている再生ループ回数を取得します. (0:指定なし)
	 * Get a playback loop count of specified. (0:not specified)
	 */
	int getLoop() const;

	/** 
	 * 再生ループ回数を設定します. (0:指定なし)
	 * Set a playback loop count.  (0:not specified)
	 */
	void setLoop(int loop);

	/** 
	 * 現在までのループ再生回数を取得します.
	 * Get repeat count a playback.
	 */
	int getLoopCount() const;

	/** 
	 * 現在までのループ再生回数をクリアします.
	 * Clear repeat count a playback.
	 */
	void clearLoopCount();

	/**
	 * フレームスキップ（フレームレートに合わせ再生フレームをスキップする）の設定をします. (default: true)
	 * Set frame-skip(to skip the playback frame according to the frame rate). (default: true)
	 */
	void setFrameSkipEnabled(bool enabled);
	
	/** 
	 * フレームスキップの設定状態を返します.
	 * Get frame-skip setting.
	 */
	bool isFrameSkipEnabled() const;

	/**
	* ラベル名からフレーム位置を取得します.
	*/
	int getLabelToFrame(char* findLabelName);

	/**
	* 再生しているアニメーションに含まれるパーツ数を取得します.
	*/
	int getPartsCount(void);

	/**
	* indexからパーツ名を取得します.
	*
	* @param  result        パーツ情報を受け取るバッファ
	* @param  name          取得するパーツ名
	* @param  frameNo       取得するフレーム番号 -1の場合は現在再生しているフレームが適用される
	*/
	const char* getPartName(int partId) const;

	/**
	* パーツ名からindexを取得します.
	*/
	int indexOfPart(const char* partName) const;

	/**
	* パーツの名から、パーツ情報を取得します.
	*
	* @param  result        パーツ情報を受け取るバッファ
	* @param  name          取得するパーツ名
	* @param  frameNo       取得するフレーム番号 -1の場合は現在再生しているフレームが適用される
	*/
	bool getPartState(ResluteState& result, const char* name, int frameNo = -1);

	/**
	* パーツ名からパーツの表示、非表示を設定します.
	* コリジョン用のパーツや差し替えグラフィック等、SS5上で表示を行うがゲーム中では非表示にする場合に使用します。
	* SSの非表示アトリビュート設定するわけではないので注意してください。
	*/
	void setPartVisible(std::string partsname, bool flg);

	/**
	* パーツ名からパーツに割り当たるセルを変更します.
	* この関数で設定したパーツは参照セルアトリビュートの影響をうけません。
	* アニメに設定されたセルに戻す場合は、セル名に""を指定してください。
	*
	* @param  partsname         パーツ名
	* @param  sscename          セルマップ名
	* @param  cellname          表示させたいセル名
	*/
	void setPartCell(std::string partsname, std::string sscename, std::string cellname);

	/*
	* プレイヤー本体の位置を設定します。
	*/
	void  setPosition(float x, float y);

	/*
	* プレイヤー本体の回転角度を設定します。2Dの回転はZに値を設定してください。
	*/

	void  setRotation(float x, float y, float z);
	/*
	* プレイヤー本体のスケールを設定します。
	*/
	void  setScale(float x, float y);

	/*
	* プレイヤー本体の透明度を設定します。
	*/
	void  setAlpha(int a);

	/*
	* アニメの輝度を設定します.
	* setColor(Color3B)ではなくこちらを使用してください。
	* 制限としてカラーブレンドが適用されたパーツの色は変更できませんので注意してください。
	*
	* @param  r          赤成分(0～255)
	* @param  g          緑成分(0～255)
	* @param  b          青成分(0～255)
	*/
	void setColor(int r, int g, int b);

	/*
	* 名前を指定してパーツの再生するインスタンスアニメを変更します。
	* 指定したパーツがインスタンスパーツでない場合、falseを返します.
	* インスタンスパーツ名はディフォルトでは「ssae名:モーション名」とつけられています。
	* 再生するアニメの名前は"ssae名/アニメーション名"として再生してください。
	* 現在再生しているアニメを指定することは入れ子となり無限ループとなるためできません。
	* 変更するアニメーションは同じssbpに含まれる必要があります。
	*
	* インスタンスキーを手動で設定する事が出来ます。
	* アニメーションに合わせて開始フレーム、終了フレーム等のインスタンスアトリビュート情報を設定してください。
	* 終了フレーム最大値は総フレーム-1になります。
	* 上書きフラグがfalseの場合、SS上に設定されたインスタンスアトリビュートの設定を使用します。
	* 使用例：
	* ss::Instance param;
	* param.clear();
	* param.refEndframe = resman->getMaxFrame("ssbp名","ssae名/モーション名") - 1;	//アニメーションの長さを取得
	* param.infinity = true;														//無限ループを設定
	* ssplayer->changeInstanceAnime("再生しているアニメーションに含まれるインスタンスパーツ名", "ssae名/モーション名", true, param);
	*
	* @param  partsname			SS上のパーツ名
	* @param  animeName			参照するアニメ名
	* @param  overWrite			インスタンスキーの上書きフラグ
	* @param  keyParam			インスタンスキーのパラメータ
	*/
	bool changeInstanceAnime(std::string partsname, std::string animeName, bool overWrite, Instance keyParam);

	/*
	* プレイヤーにインスタンスパラメータを設定します。
	*
	* @param  overWrite			インスタンスキーの上書きフラグ
	* @param  keyParam			インスタンスキーのパラメータ
	*/
	void setInstanceParam(bool overWrite, Instance keyParam);

	/*
	* プレイヤーからインスタンスパラメータを取得します。
	*
	* @param  overWrite			インスタンスキーの上書きフラグ
	* @param  keyParam			インスタンスキーのパラメータ
	*/
	void getInstanceParam(bool *overWrite, Instance *keyParam);

	/*
	* アニメーションのループ範囲（再生位置）を上書きします。
	*
	* @param  frame			開始フレーム（-1で上書き解除）
	*/
	void setStartFrame(int frame);

	/*
	* アニメーションのループ範囲（終了位置）を上書きします。
	* SpriteStudioのフレーム数+1を設定してください。
	*
	* @param  frame			終了フレーム（-1で上書き解除）
	*/
	void setEndFrame(int frame);

	/*
	* アニメーションのループ範囲（再生位置）を上書きします。
	*
	* @param  labelname			開始フレームとなるラベル名（""で上書き解除）
	*/
	void setStartFrameToLabelName(char *findLabelName);

	/*
	* アニメーションのループ範囲（終了位置）を上書きします。
	*
	* @param  labelname			終了フレームとなるラベル名（""で上書き解除）
	*/
	void setEndFrameToLabelName(char *findLabelName);

	/*
	* プレイヤー本体の反転を設定します。
	*/
	void  setFlip(bool flipX, bool flipY);

	/*
	* パーツ番号に対応したスプライト情報を取得します。
	* 
	* @param  partIndex			パーツ番号
	*/
	CustomSprite* getSpriteData(int partIndex);

	/*
	* 表示を行ったスプライト数を取得します。パフォーマンスの参考に使用してください。
	*/
	int getDrawSpriteCount(void);

	/*
	* プレイヤーの更新を行います。ゲームの更新タイミングで呼び出してください。
	*/
	void update(float dt);

	/*
	* プレイヤーの表示を行います。ゲームの表示タイミングで呼び出してください。
	*/
	void draw();

public:
	Player(void);
	~Player();
	bool init();


protected:
	void allocParts(int numParts);
	void releaseParts();
	void setPartsParentage();

	void play(AnimeRef* animeRef, int loop, int startFrameNo);
	void updateFrame(float dt);
	void setFrame(int frameNo, float dt = 0.0f);
	void checkUserData(int frameNo);
	float parcentVal(float val1, float val2, float parcent);
	float parcentValRot(float val1, float val2, float parcent);
	void update_matrix_ss4(CustomSprite *sprite, CustomSprite *parent, const PartData *partData);

protected:
	ResourceManager*	_resman;
	ResourceSet*		_currentRs;
	std::string			_currentdataKey;
	std::string			_currentAnimename;
	AnimeRef*			_currentAnimeRef;
	std::vector<CustomSprite *>	_parts;

	Player*				_motionBlendPlayer;
	float				_blendTime;
	float				_blendTimeMax;

	bool				_frameSkipEnabled;
	float				_playingFrame;
	float				_step;
	int					_loop;
	int					_loopCount;
	bool				_isPlaying;
	bool				_isPausing;
	bool				_isPlayFirstUserdataChack;
	int					_prevDrawFrameNo;
	bool				_partVisible[PART_VISIBLE_MAX];
	int					_cellChange[PART_VISIBLE_MAX];
	int					_partIndex[PART_VISIBLE_MAX];
	int					_animefps;
	int					_col_r;
	int					_col_g;
	int					_col_b;
	bool				_instanceOverWrite;				//インスタンス情報を上書きするか？
	Instance			_instanseParam;					//インスタンスパラメータ
	int					_startFrameOverWrite;			//開始フレームの上書き設定
	int					_endFrameOverWrite;				//終了フレームの上書き設定
	int					_seedOffset;					//エフェクトシードオフセット
	bool				_rootPartFunctionAsVer4;		//不透明度・反転・非表示アトリビュートの継承方法をVer.4と同様にする
	bool				_dontUseMatrixForTransform;		//親子の座標変換にマトリックスを使用しない（Ver4互換）
	int					_draw_count;					//表示スプライト数

	UserData			_userData;

	State				_state;
};


};	// namespace ss

#endif
