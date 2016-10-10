#include "SSCellPartState.h"
#include <memory>
#include "common/Animator/ssplayer_matrix.h"
#include "SS5PlayerTypes.h"
#include "DataArrayReader.h"
#include "SS5Player.h"

namespace ss{

void SSCellPartState::init()
{
	m_flags = 0;
	m_cellIndex = 0;
	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;
	m_pivotX = 0.0f;
	m_pivotY = 0.0f;
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
	m_scaleX = 1.0f;
	m_scaleY = 1.0f;
	m_opacity = 255;
	m_size_X = 1.0f;
	m_size_Y = 1.0f;
	m_uv_move_X = 0.0f;
	m_uv_move_Y = 0.0f;
	m_uv_rotation = 0.0f;
	m_uv_scale_X = 1.0f;
	m_uv_scale_Y = 1.0f;
	m_boundingRadius = 0.0f;
	m_colorBlendFunc = 0;
	m_colorBlendType = 0;
	m_flipX = false;
	m_flipY = false;
	m_isVisibled = false;
	m_quad = SSV3F_C4B_T2F_Quad();
	m_texture.handle = 0;
	m_texture.size_w = 0;
	m_texture.size_h = 0;
	m_rect.size.height = 0;
	m_rect.size.width = 0;
	m_rect.origin.x = 0;
	m_rect.origin.y = 0;
	m_blendfunc = 0;
	m_mat.setupIdentity();
	m_instanceValue_curKeyframe = 0;
	m_instanceValue_startFrame = 0;
	m_instanceValue_endFrame = 0;
	m_instanceValue_loopNum = 0;
	m_instanceValue_speed = 0;
	m_instanceValue_loopflag = 0;
	m_effectValue_curKeyframe = 0;
	m_effectValue_startTime = 0;
	m_effectValue_speed = 0;
	m_effectValue_loopflag = 0;

	m_Calc_rotationX = 0.0f;
	m_Calc_rotationY = 0.0f;
	m_Calc_rotationZ = 0.0f;
	m_Calc_scaleX = 1.0f;
	m_Calc_scaleY = 1.0f;
	m_Calc_opacity = 255;
}
	


//readerを介してデータを読み取る
void SSCellPartState::readData(DataArrayReader &reader, const AnimationInitialData *init)
{
	// optional parameters
	int flags = reader.readU32();

	m_flags = flags;
	m_cellIndex = flags & PART_FLAG_CELL_INDEX ? reader.readS16() : init->cellIndex;
	m_x         = flags & PART_FLAG_POSITION_X ? reader.readFloat() : init->positionX;
#ifdef UP_MINUS
	m_y         = flags & PART_FLAG_POSITION_Y ? -reader.readFloat() : -init->positionY;		//上がマイナスなので反転させる
#else
	m_y         = flags & PART_FLAG_POSITION_Y ? reader.readFloat() : init->positionY;
#endif
	m_z         = flags & PART_FLAG_POSITION_Z ? reader.readFloat() : init->positionZ;
	m_pivotX    = flags & PART_FLAG_PIVOT_X ? reader.readFloat() : init->pivotX;
#ifdef UP_MINUS
	m_pivotY    = flags & PART_FLAG_PIVOT_Y ? -reader.readFloat() : -init->pivotY;
#else
	m_pivotY    = flags & PART_FLAG_PIVOT_Y ? reader.readFloat() : init->pivotY;
#endif
#ifdef UP_MINUS
	m_rotationX = flags & PART_FLAG_ROTATIONX ? -reader.readFloat() : -init->rotationX;
	m_rotationY = flags & PART_FLAG_ROTATIONY ? -reader.readFloat() : -init->rotationY;
	m_rotationZ = flags & PART_FLAG_ROTATIONZ ? -reader.readFloat() : -init->rotationZ;
#else
	m_rotationX = flags & PART_FLAG_ROTATIONX ? reader.readFloat() : init->rotationX;
	m_rotationY = flags & PART_FLAG_ROTATIONY ? reader.readFloat() : init->rotationY;
	m_rotationZ = flags & PART_FLAG_ROTATIONZ ? reader.readFloat() : init->rotationZ;
#endif
	m_scaleX    = flags & PART_FLAG_SCALE_X ? reader.readFloat() : init->scaleX;
	m_scaleY    = flags & PART_FLAG_SCALE_Y ? reader.readFloat() : init->scaleY;
	m_opacity   = flags & PART_FLAG_OPACITY ? reader.readU16() : init->opacity;
	m_size_X    = flags & PART_FLAG_SIZE_X ? reader.readFloat() : init->size_X;
	m_size_Y    = flags & PART_FLAG_SIZE_Y ? reader.readFloat() : init->size_Y;
	m_uv_move_X = flags & PART_FLAG_U_MOVE ? reader.readFloat() : init->uv_move_X;
	m_uv_move_Y = flags & PART_FLAG_V_MOVE ? reader.readFloat() : init->uv_move_Y;
	m_uv_rotation = flags & PART_FLAG_UV_ROTATION ? reader.readFloat() : init->uv_rotation;
	m_uv_scale_X  = flags & PART_FLAG_U_SCALE ? reader.readFloat() : init->uv_scale_X;
	m_uv_scale_Y  = flags & PART_FLAG_V_SCALE ? reader.readFloat() : init->uv_scale_Y;
	m_boundingRadius = flags & PART_FLAG_BOUNDINGRADIUS ? reader.readFloat() : init->boundingRadius;

	//インスタンスアトリビュート
	m_instanceValue_curKeyframe = flags & PART_FLAG_INSTANCE_KEYFRAME ? reader.readS32() : init->instanceValue_curKeyframe;
	m_instanceValue_startFrame  = flags & PART_FLAG_INSTANCE_KEYFRAME ? reader.readS32() : init->instanceValue_startFrame;
	m_instanceValue_endFrame    = flags & PART_FLAG_INSTANCE_KEYFRAME ? reader.readS32() : init->instanceValue_endFrame;
	m_instanceValue_loopNum     = flags & PART_FLAG_INSTANCE_KEYFRAME ? reader.readS32() : init->instanceValue_loopNum;
	m_instanceValue_speed       = flags & PART_FLAG_INSTANCE_KEYFRAME ? reader.readFloat() : init->instanceValue_speed;
	m_instanceValue_loopflag    = flags & PART_FLAG_INSTANCE_KEYFRAME ? reader.readS32() : init->instanceValue_loopflag;
	//エフェクトアトリビュート
	m_effectValue_curKeyframe = flags & PART_FLAG_EFFECT_KEYFRAME ? reader.readS32() : init->effectValue_curKeyframe;
	m_effectValue_startTime   = flags & PART_FLAG_EFFECT_KEYFRAME ? reader.readS32() : init->effectValue_startTime;
	m_effectValue_speed       = flags & PART_FLAG_EFFECT_KEYFRAME ? reader.readFloat() : init->effectValue_speed;
	m_effectValue_loopflag    = flags & PART_FLAG_EFFECT_KEYFRAME ? reader.readS32() : init->effectValue_loopflag;


	m_flipX = (bool)(flags & PART_FLAG_FLIP_H);
	m_flipY = (bool)(flags & PART_FLAG_FLIP_V);
	
	m_isVisibled = !(flags & PART_FLAG_INVISIBLE);
}



//頂点計算
void SSCellPartState::vertexCompute(SSV3F_C4B_T2F_Quad *q, const SSRect &cellRect) const
{
	//memo:scaleとかrotateとかは後でmatrixを掛けて反映させる。ここでは中心(0,0)基準の矩形(+頂点変化)を作るだけ

	//ひとまずrectをベースに矩形をセットする
	float width = cellRect.size.width;
	float height = cellRect.size.height;
	q->bl.vertices = SSVertex3F(0, 0, 0);
	q->br.vertices = SSVertex3F(width, 0, 0);
	q->tl.vertices = SSVertex3F(0, height, 0);
	q->tr.vertices = SSVertex3F(width, height, 0);  //yが上方向+と考えれば、左下基準(0,0)の矩形になる

	//サイズ変化しているなら、そちらに設定を合わせる
	//普通はscaleで指定されるので、このフラグがONになるのはレアケース
	//詳しくは http://www.webtech.co.jp/help/ja/spritestudio/window/attributewindow/
	//「アンカー機能を持つ特殊なパーツ」についての項目を参照
	if(m_flags & PART_FLAG_SIZE_X){
		q->br.vertices.x = m_size_X;
		q->tr.vertices.x = m_size_X;
	}
	if(m_flags & PART_FLAG_SIZE_Y){
		q->tl.vertices.y = m_size_Y;
		q->tr.vertices.y = m_size_Y;
	}

#ifdef UP_MINUS
	//座標系がy上方向が-になるように調整
	std::swap(q->tl.vertices.y, q->bl.vertices.y);
	std::swap(q->tr.vertices.y, q->br.vertices.y);
#endif


	//中心が(0,0)になるようにオフセットを追加
	SSVertex3F center = (q->bl.vertices + q->tr.vertices) / 2;
	q->bl.vertices -= center;
	q->br.vertices -= center;
	q->tl.vertices -= center;
	q->tr.vertices -= center;
}



bool SSCellPartState::isStateChanged(const SSCellPartState& s) const
{
	return !(
		m_flags == s.m_flags
		&& m_cellIndex == s.m_cellIndex
		&& m_x == s.m_x
		&& m_y == s.m_y
		&& m_z == s.m_z
		&& m_pivotX == s.m_pivotX
		&& m_pivotY == s.m_pivotY
		&& m_rotationX == s.m_rotationX
		&& m_rotationY == s.m_rotationY
		&& m_rotationZ == s.m_rotationZ
		&& m_scaleX == s.m_scaleX
		&& m_scaleY == s.m_scaleY
		&& m_opacity == s.m_opacity
		&& m_size_X == s.m_size_X
		&& m_size_Y == s.m_size_Y
		&& m_uv_move_X == s.m_uv_move_X
		&& m_uv_move_Y == s.m_uv_move_Y
		&& m_uv_rotation == s.m_uv_rotation
		&& m_uv_scale_X == s.m_uv_scale_X
		&& m_uv_scale_Y == s.m_uv_scale_Y
		&& m_boundingRadius == s.m_boundingRadius
		&& m_isVisibled == s.m_isVisibled
		&& m_flipX == s.m_flipX
		&& m_flipY == s.m_flipY
		&& m_blendfunc == s.m_blendfunc
		&& m_colorBlendFunc == s.m_colorBlendFunc
		&& m_colorBlendType == s.m_colorBlendType
		//&& m_quad == s.m_quad		//memo:CustomSpriteの実装には比較を行おうとした形跡がある
		&& m_instanceValue_curKeyframe == s.m_instanceValue_curKeyframe
		&& m_instanceValue_startFrame == s.m_instanceValue_startFrame
		&& m_instanceValue_endFrame == s.m_instanceValue_endFrame
		&& m_instanceValue_loopNum == s.m_instanceValue_loopNum
		&& m_instanceValue_speed == s.m_instanceValue_speed
		&& m_instanceValue_loopflag == s.m_instanceValue_loopflag
		&& m_effectValue_curKeyframe == s.m_effectValue_curKeyframe
		&& m_effectValue_startTime == s.m_effectValue_startTime
		&& m_effectValue_speed == s.m_effectValue_speed
		&& m_effectValue_loopflag == s.m_effectValue_loopflag
	);
}


}	// namespace ss
