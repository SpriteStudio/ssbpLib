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
	bool				_flipX;
	bool				_flipY;

public:
	SSMatrix			_mat;
	SSCellPartState		_state;
	bool				_isStateChanged;
	CustomSprite*		_parent;
	Player*				_ssplayer;
	float				_liveFrame;

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


	void setFlippedX(bool flip);
	void setFlippedY(bool flip);
	bool isFlippedX();
	bool isFlippedY();
};




};	// namespace ss

