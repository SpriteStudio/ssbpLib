#include "CustomSprite.h"
#include "SS5Player.h"

namespace ss{


CustomSprite::CustomSprite()
	: _liveFrame(0.0f)
	, refEffect(0)
	, _ssplayer(0)
	,effectAttrInitialized(false)
	,effectTimeTotal(0)
{
	_temp_position = SsVector3(0, 0, 0);
	_temp_rotation = SsVector3(0, 0, 0);
	_temp_scale = SsVector2(1.0f, 1.0f);
}

CustomSprite::~CustomSprite()
{
	//エフェクトクラスがある場合は解放する
	SS_SAFE_DELETE(refEffect);
	SS_SAFE_DELETE(_ssplayer);
}


CustomSprite* CustomSprite::create()
{
	CustomSprite *pSprite = new CustomSprite();
	if (pSprite)
	{
		pSprite->initState();
//		pSprite->_defaultShaderProgram = pSprite->getShaderProgram();
//		pSprite->autorelease();
		return pSprite;
	}
	SS_SAFE_DELETE(pSprite);
	return NULL;
}



void CustomSprite::setFlippedX(bool flip){
	_flipX = flip;
}
void CustomSprite::setFlippedY(bool flip){
	_flipY = flip;
}
bool CustomSprite::isFlippedX(){
	return _flipX;
}
bool CustomSprite::isFlippedY(){
	return _flipY;
}


} //namespace ss
