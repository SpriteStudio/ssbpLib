#pragma once
#include "SS5PlayerTypes.h"
#include "SSCellPartState.h"
#include "Animator/ssplayer_matrix.h"
#include "Animator/ssplayer_PartState.h"
#include "Loader/sstypes.h"

namespace ss{
class Player;
class SsEffectRenderV2;

/**
 * CustomSprite
 */
class CustomSprite{
private:
	static unsigned int ssSelectorLocation;
	static unsigned int	ssAlphaLocation;
	static unsigned int	sshasPremultipliedAlpha;

	//	static CCGLProgram* getCustomShaderProgram();

private:
	//	CCGLProgram*	_defaultShaderProgram;
	bool				_useCustomShaderProgram;
	float				_opacity;
	int					_hasPremultipliedAlpha;
	int					_colorBlendFuncNo;
	bool				_flipX;
	bool				_flipY;

public:
	SSMatrix			_mat;
	SSCellPartState		_state;
	bool				_isStateChanged;
	CustomSprite*		_parent;
	Player*				_ssplayer;
	float				_liveFrame;
	SSV3F_C4B_T2F_Quad	_sQuad;

	//エフェクト用パラメータ
	SsEffectRenderV2*	refEffect;
	SsPartState			partState;

	//モーションブレンド用ステータス
	SSCellPartState		_orgState;

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

	void setState(const SSCellPartState& state)
	{
		setStateValue(_state.m_flags, state.m_flags);
		setStateValue(_state.m_cellIndex, state.m_cellIndex);
		setStateValue(_state.m_x, state.m_x);
		setStateValue(_state.m_y, state.m_y);
		setStateValue(_state.m_z, state.m_z);
		setStateValue(_state.m_pivotX, state.m_pivotX);
		setStateValue(_state.m_pivotY, state.m_pivotY);
		setStateValue(_state.m_rotationX, state.m_rotationX);
		setStateValue(_state.m_rotationY, state.m_rotationY);
		setStateValue(_state.m_rotationZ, state.m_rotationZ);
		setStateValue(_state.m_scaleX, state.m_scaleX);
		setStateValue(_state.m_scaleY, state.m_scaleY);
		setStateValue(_state.m_opacity, state.m_opacity);
		setStateValue(_state.m_size_X, state.m_size_X);
		setStateValue(_state.m_size_Y, state.m_size_Y);
		setStateValue(_state.m_uv_move_X, state.m_uv_move_X);
		setStateValue(_state.m_uv_move_Y, state.m_uv_move_Y);
		setStateValue(_state.m_uv_rotation, state.m_uv_rotation);
		setStateValue(_state.m_uv_scale_X, state.m_uv_scale_X);
		setStateValue(_state.m_uv_scale_Y, state.m_uv_scale_Y);
		setStateValue(_state.m_boundingRadius, state.m_boundingRadius);
		setStateValue(_state.m_isVisibled, state.m_isVisibled);
		setStateValue(_state.m_flipX, state.m_flipX);
		setStateValue(_state.m_flipY, state.m_flipY);
		setStateValue(_state.m_blendfunc, state.m_blendfunc);
		setStateValue(_state.m_colorBlendFunc, state.m_colorBlendFunc);
		setStateValue(_state.m_colorBlendType, state.m_colorBlendType);

		setStateValue(_state.m_quad, state.m_quad);
		_state.m_texture = state.m_texture;
		_state.m_rect = state.m_rect;
		_state.m_mat = state.m_mat;

		setStateValue(_state.m_instanceValue_curKeyframe, state.m_instanceValue_curKeyframe);
		setStateValue(_state.m_instanceValue_startFrame, state.m_instanceValue_startFrame);
		setStateValue(_state.m_instanceValue_endFrame, state.m_instanceValue_endFrame);
		setStateValue(_state.m_instanceValue_loopNum, state.m_instanceValue_loopNum);
		setStateValue(_state.m_instanceValue_speed, state.m_instanceValue_speed);
		setStateValue(_state.m_instanceValue_loopflag, state.m_instanceValue_loopflag);
		setStateValue(_state.m_effectValue_curKeyframe, state.m_effectValue_curKeyframe);
		setStateValue(_state.m_effectValue_startTime, state.m_effectValue_startTime);
		setStateValue(_state.m_effectValue_speed, state.m_effectValue_speed);
		setStateValue(_state.m_effectValue_loopflag, state.m_effectValue_loopflag);

		_state.m_Calc_rotationX = state.m_Calc_rotationX;
		_state.m_Calc_rotationY = state.m_Calc_rotationY;
		_state.m_Calc_rotationZ = state.m_Calc_rotationZ;
		_state.m_Calc_scaleX = state.m_Calc_scaleX;
		_state.m_Calc_scaleY = state.m_Calc_scaleY;
		_state.m_Calc_opacity = state.m_Calc_opacity;

	}


	// override
	virtual void draw(void);
	virtual void setOpacity(unsigned char opacity);

	// original functions
	void changeShaderProgram(bool useCustomShaderProgram);
	bool isCustomShaderProgramEnabled() const;
	void setColorBlendFunc(int colorBlendFuncNo);
	SSV3F_C4B_T2F_Quad& getAttributeRef();

	void setFlippedX(bool flip);
	void setFlippedY(bool flip);
	bool isFlippedX();
	bool isFlippedY();
	void sethasPremultipliedAlpha(int PremultipliedAlpha);

public:
};




};	// namespace ss

