#pragma once
#include "common/Animator/ssplayer_matrix.h"
#include "common/SS5PlayerLibs/SS5PlayerPartState.h"
#include "common/Animator/ssplayer_PartState.h"
#include "common/Loader/sstypes.h"

namespace ss{
class Player;
class SsEffectRenderV2;

/**
 * CustomSprite
 */
class CustomSprite
{
private:
	bool				_useCustomShaderProgram;
	float				_opacity;
	int					_hasPremultipliedAlpha;
	int					_colorBlendFuncNo;
	bool				_flipX;
	bool				_flipY;

public:
	SSMatrix			_mat;
	State				_state;
	bool				_isStateChanged;
	CustomSprite*		_parent;
	Player*				_ssplayer;
	float				_liveFrame;

	//エフェクト用パラメータ
	SsEffectRenderV2*	refEffect;
	SsPartState			partState;

	//モーションブレンド用ステータス
	State				_orgState;

	//エフェクト制御用ワーク
	bool effectAttrInitialized;
	float effectTimeTotal;

	//Ver4互換用ワーク
	SsVector3		_temp_position;
	SsVector3		_temp_rotation;
	SsVector2		_temp_scale;

public:
	CustomSprite();
	virtual ~CustomSprite();

	static CustomSprite* create();

	void initState()
	{
		_state.init();
		_isStateChanged = true;
	}

	void setStateValue(float& ref, float value)
	{
		if (ref != value)
		{
			ref = value;
			_isStateChanged = true;
		}
	}

	void setStateValue(int& ref, int value)
	{
		if (ref != value)
		{
			ref = value;
			_isStateChanged = true;
		}
	}

	void setStateValue(bool& ref, bool value)
	{
		if (ref != value)
		{
			ref = value;
			_isStateChanged = true;
		}
	}

	void setStateValue(SSV3F_C4B_T2F_Quad& ref, SSV3F_C4B_T2F_Quad value)
	{
		//		if (ref != value)
		{
			ref = value;
			_isStateChanged = true;
		}
	}

	void setState(const State& state)
	{
		setStateValue(_state.flags, state.flags);
		setStateValue(_state.cellIndex, state.cellIndex);
		setStateValue(_state.x, state.x);
		setStateValue(_state.y, state.y);
		setStateValue(_state.z, state.z);
		setStateValue(_state.pivotX, state.pivotX);
		setStateValue(_state.pivotY, state.pivotY);
		setStateValue(_state.rotationX, state.rotationX);
		setStateValue(_state.rotationY, state.rotationY);
		setStateValue(_state.rotationZ, state.rotationZ);
		setStateValue(_state.scaleX, state.scaleX);
		setStateValue(_state.scaleY, state.scaleY);
		setStateValue(_state.opacity, state.opacity);
		setStateValue(_state.size_X, state.size_X);
		setStateValue(_state.size_Y, state.size_Y);
		setStateValue(_state.uv_move_X, state.uv_move_X);
		setStateValue(_state.uv_move_Y, state.uv_move_Y);
		setStateValue(_state.uv_rotation, state.uv_rotation);
		setStateValue(_state.uv_scale_X, state.uv_scale_X);
		setStateValue(_state.uv_scale_Y, state.uv_scale_Y);
		setStateValue(_state.boundingRadius, state.boundingRadius);
		setStateValue(_state.isVisibled, state.isVisibled);
		setStateValue(_state.flipX, state.flipX);
		setStateValue(_state.flipY, state.flipY);
		setStateValue(_state.blendfunc, state.blendfunc);
		setStateValue(_state.colorBlendFunc, state.colorBlendFunc);
		setStateValue(_state.colorBlendType, state.colorBlendType);

		setStateValue(_state.quad, state.quad);
		_state.texture = state.texture;
		_state.rect = state.rect;
		_state.mat = state.mat;

		setStateValue(_state.instanceValue_curKeyframe, state.instanceValue_curKeyframe);
		setStateValue(_state.instanceValue_startFrame, state.instanceValue_startFrame);
		setStateValue(_state.instanceValue_endFrame, state.instanceValue_endFrame);
		setStateValue(_state.instanceValue_loopNum, state.instanceValue_loopNum);
		setStateValue(_state.instanceValue_speed, state.instanceValue_speed);
		setStateValue(_state.instanceValue_loopflag, state.instanceValue_loopflag);
		setStateValue(_state.effectValue_curKeyframe, state.effectValue_curKeyframe);
		setStateValue(_state.effectValue_startTime, state.effectValue_startTime);
		setStateValue(_state.effectValue_speed, state.effectValue_speed);
		setStateValue(_state.effectValue_loopflag, state.effectValue_loopflag);

		_state.Calc_rotationX = state.Calc_rotationX;
		_state.Calc_rotationY = state.Calc_rotationY;
		_state.Calc_rotationZ = state.Calc_rotationZ;
		_state.Calc_scaleX = state.Calc_scaleX;
		_state.Calc_scaleY = state.Calc_scaleY;
		_state.Calc_opacity = state.Calc_opacity;

	}


	// original functions
	void setFlippedX(bool flip);
	void setFlippedY(bool flip);
	bool isFlippedX();
	bool isFlippedY();

public:
};

} //namespace ss

