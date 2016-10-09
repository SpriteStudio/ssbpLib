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
	
	void setState(const SSCellPartState& state)
	{	
		if(_state.isStateChanged(state)){
			_isStateChanged = true;
		}
		_state = state;
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

