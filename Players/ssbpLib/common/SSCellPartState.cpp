#include "SSCellPartState.h"
#include <memory>
#include "common/Animator/ssplayer_matrix.h"
#include "SS5PlayerTypes.h"
#include "DataArrayReader.h"
#include "SS5Player.h"

namespace ss{

void SSCellPartState::init()
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
	memset(&quad, 0, sizeof(quad));
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
	


//readerを介してデータを読み取る
void SSCellPartState::readData(DataArrayReader &reader, const AnimationInitialData *init)
{
	// optional parameters
	flags = reader.readU32();
	cellIndex = flags & PART_FLAG_CELL_INDEX ? reader.readS16() : init->cellIndex;
	x = flags & PART_FLAG_POSITION_X ? reader.readFloat() : init->positionX;
#ifdef UP_MINUS
	y = flags & PART_FLAG_POSITION_Y ? -reader.readFloat() : -init->positionY;		//上がマイナスなので反転させる
#else
	y = flags & PART_FLAG_POSITION_Y ? reader.readFloat() : init->positionY;
#endif
	z = flags & PART_FLAG_POSITION_Z ? reader.readFloat() : init->positionZ;
	pivotX = flags & PART_FLAG_PIVOT_X ? reader.readFloat() : init->pivotX;
#ifdef UP_MINUS
	pivotY = flags & PART_FLAG_PIVOT_Y ? -reader.readFloat() : -init->pivotY;
#else
	pivotY = flags & PART_FLAG_PIVOT_Y ? reader.readFloat() : init->pivotY;
#endif
#ifdef UP_MINUS
	rotationX = flags & PART_FLAG_ROTATIONX ? -reader.readFloat() : -init->rotationX;
	rotationY = flags & PART_FLAG_ROTATIONY ? -reader.readFloat() : -init->rotationY;
	rotationZ = flags & PART_FLAG_ROTATIONZ ? -reader.readFloat() : -init->rotationZ;
#else
	rotationX = flags & PART_FLAG_ROTATIONX ? reader.readFloat() : init->rotationX;
	rotationY = flags & PART_FLAG_ROTATIONY ? reader.readFloat() : init->rotationY;
	rotationZ = flags & PART_FLAG_ROTATIONZ ? reader.readFloat() : init->rotationZ;
#endif
	scaleX = flags & PART_FLAG_SCALE_X ? reader.readFloat() : init->scaleX;
	scaleY = flags & PART_FLAG_SCALE_Y ? reader.readFloat() : init->scaleY;
	opacity = flags & PART_FLAG_OPACITY ? reader.readU16() : init->opacity;
	size_X = flags & PART_FLAG_SIZE_X ? reader.readFloat() : init->size_X;
	size_Y = flags & PART_FLAG_SIZE_Y ? reader.readFloat() : init->size_Y;
	uv_move_X = flags & PART_FLAG_U_MOVE ? reader.readFloat() : init->uv_move_X;
	uv_move_Y = flags & PART_FLAG_V_MOVE ? reader.readFloat() : init->uv_move_Y;
	uv_rotation = flags & PART_FLAG_UV_ROTATION ? reader.readFloat() : init->uv_rotation;
	uv_scale_X = flags & PART_FLAG_U_SCALE ? reader.readFloat() : init->uv_scale_X;
	uv_scale_Y = flags & PART_FLAG_V_SCALE ? reader.readFloat() : init->uv_scale_Y;
	boundingRadius = flags & PART_FLAG_BOUNDINGRADIUS ? reader.readFloat() : init->boundingRadius;

	//インスタンスアトリビュート
	instanceValue_curKeyframe = flags & PART_FLAG_INSTANCE_KEYFRAME ? reader.readS32() : init->instanceValue_curKeyframe;
	instanceValue_startFrame = flags & PART_FLAG_INSTANCE_KEYFRAME ? reader.readS32() : init->instanceValue_startFrame;
	instanceValue_endFrame = flags & PART_FLAG_INSTANCE_KEYFRAME ? reader.readS32() : init->instanceValue_endFrame;
	instanceValue_loopNum = flags & PART_FLAG_INSTANCE_KEYFRAME ? reader.readS32() : init->instanceValue_loopNum;
	instanceValue_speed = flags & PART_FLAG_INSTANCE_KEYFRAME ? reader.readFloat() : init->instanceValue_speed;
	instanceValue_loopflag = flags & PART_FLAG_INSTANCE_KEYFRAME ? reader.readS32() : init->instanceValue_loopflag;
	//エフェクトアトリビュート
	effectValue_curKeyframe = flags & PART_FLAG_EFFECT_KEYFRAME ? reader.readS32() : init->effectValue_curKeyframe;
	effectValue_startTime = flags & PART_FLAG_EFFECT_KEYFRAME ? reader.readS32() : init->effectValue_startTime;
	effectValue_speed = flags & PART_FLAG_EFFECT_KEYFRAME ? reader.readFloat() : init->effectValue_speed;
	effectValue_loopflag = flags & PART_FLAG_EFFECT_KEYFRAME ? reader.readS32() : init->effectValue_loopflag;


	flipX = (bool)(flags & PART_FLAG_FLIP_H);
	flipY = (bool)(flags & PART_FLAG_FLIP_V);
	
	isVisibled = !(flags & PART_FLAG_INVISIBLE);
}


}	// namespace ss
