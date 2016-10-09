#pragma once

#include "common/Animator/ssplayer_matrix.h"
#include "SS5PlayerTypes.h"

namespace ss{
class DataArrayReader;
struct AnimationInitialData;


/**
 * SSCellPartState
 * パーツの情報を格納します。SSCellPartStateの内容をもとに描画処理を作成してください。
 */
struct SSCellPartState
{
	int   m_flags;					/// このフレームで更新が行われるステータスのフラグ
	int   m_cellIndex;				/// パーツに割り当てられたセルの番号
	float m_x;						/// SS5アトリビュート：X座標
	float m_y;						/// SS5アトリビュート：Y座標
	float m_z;						/// SS5アトリビュート：Z座標
	float m_pivotX;					/// 原点Xオフセット＋セルに設定された原点オフセットX
	float m_pivotY;					/// 原点Yオフセット＋セルに設定された原点オフセットY
	float m_rotationX;				/// X回転
	float m_rotationY;				/// Y回転
	float m_rotationZ;				/// Z回転
	float m_scaleX;					/// Xスケール
	float m_scaleY;					/// Yスケール
	int   m_opacity;				/// 不透明度（0～255）
	float m_size_X;					/// SS5アトリビュート：Xサイズ
	float m_size_Y;					/// SS5アトリビュート：Xサイズ
	float m_uv_move_X;				/// SS5アトリビュート：UV X移動
	float m_uv_move_Y;				/// SS5アトリビュート：UV Y移動
	float m_uv_rotation;			/// SS5アトリビュート：UV 回転
	float m_uv_scale_X;				/// SS5アトリビュート：UV Xスケール
	float m_uv_scale_Y;				/// SS5アトリビュート：UV Yスケール
	float m_boundingRadius;			/// SS5アトリビュート：当たり半径
	int   m_colorBlendFunc;			/// SS5アトリビュート：カラーブレンドのブレンド方法
	int   m_colorBlendType;			/// SS5アトリビュート：カラーブレンドの単色か頂点カラーか。
	bool  m_flipX;					/// 横反転（親子関係計算済）
	bool  m_flipY;					/// 縦反転（親子関係計算済）
	bool  m_isVisibled;				/// 非表示（親子関係計算済）
	SSV3F_C4B_T2F_Quad m_quad;		/// 頂点データ、座標、カラー値、UVが含まれる（頂点変形、サイズXY、UV移動XY、UVスケール、UV回転、反転が反映済）
	TextuerData m_texture;			/// セルに対応したテクスチャ番号（ゲーム側で管理している番号を設定する）
	SSRect m_rect;					/// セルに対応したテクスチャ内の表示領域（開始座標、幅高さ）
	int m_blendfunc;				/// パーツに設定されたブレンド方法
	SSMatrix m_mat;					/// パーツの位置を算出するためのマトリクス（親子関係計算済）
	//再生用パラメータ
	float m_Calc_rotationX;			/// X回転（親子関係計算済）
	float m_Calc_rotationY;			/// Y回転（親子関係計算済）
	float m_Calc_rotationZ;			/// Z回転（親子関係計算済）
	float m_Calc_scaleX;			/// Xスケール（親子関係計算済）
	float m_Calc_scaleY;			/// Yスケール（親子関係計算済）
	int   m_Calc_opacity;			/// 不透明度（0～255）（親子関係計算済）
	//インスタンスアトリビュート
	int			m_instanceValue_curKeyframe;
	int			m_instanceValue_startFrame;
	int			m_instanceValue_endFrame;
	int			m_instanceValue_loopNum;
	float		m_instanceValue_speed;
	int			m_instanceValue_loopflag;
	//エフェクトアトリビュート
	int			m_effectValue_curKeyframe;
	int			m_effectValue_startTime;
	float		m_effectValue_speed;
	int			m_effectValue_loopflag;


	
	//readerを介してデータを読み取る
	void readData(DataArrayReader &reader, const AnimationInitialData *init);

	bool isStateChanged(const SSCellPartState& s) const;
	void init();

	SSCellPartState() { init(); }
};


}	// namespace ss
