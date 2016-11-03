#include "SS5CustomSprite.h"
#include "common/Helper/Util.h"

namespace ss{


/**
 * CustomSprite
 */
//カラーブレンド用のシェーダー処理は汎用的に使用する事ができないためすべてコメントにしてあります。
//カラーブレンドを再現するための参考にしてください。

//static const GLchar * ssPositionTextureColor_frag =
//#include "ssShader_frag.h"

CustomSprite::CustomSprite() :
//	: _defaultShaderProgram(NULL)
_useCustomShaderProgram(false)
, _opacity(1.0f)
, _colorBlendFuncNo(0)
, _liveFrame(0.0f)
, _hasPremultipliedAlpha(0)
, refEffect(0)
, _ssplayer(0)
, effectAttrInitialized(false)
, effectTimeTotal(0)
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
		return pSprite;
	}
	SS_SAFE_DELETE(pSprite);
	return NULL;
}

void CustomSprite::setFlippedX(bool flip)
{
	_flipX = flip;
}
void CustomSprite::setFlippedY(bool flip)
{
	_flipY = flip;
}
bool CustomSprite::isFlippedX()
{
	return (_flipX);
}
bool CustomSprite::isFlippedY()
{
	return (_flipY);
}


} //namespace ss
