#include "CustomSprite.h"
#include "SS5Player.h"

namespace ss{

/**
 * CustomSprite
 */
 //カラーブレンド用のシェーダー処理は汎用的に使用する事ができないためすべてコメントにしてあります。
 //カラーブレンドを再現するための参考にしてください。

//static const GLchar * ssPositionTextureColor_frag =
//#include "ssShader_frag.h"

CustomSprite::CustomSprite()
	: _opacity(1.0f)
	, _colorBlendFuncNo(0)
	, _liveFrame(0.0f)
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

/*
CCGLProgram* CustomSprite::getCustomShaderProgram()
{
	using namespace cocos2d;

	static CCGLProgram* p = NULL;
	static bool constructFailed = false;
	if (!p && !constructFailed)
	{
		p = new CCGLProgram();
		p->initWithVertexShaderByteArray(
			ccPositionTextureColor_vert,
			ssPositionTextureColor_frag);
		p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

		if (!p->link())
		{
			constructFailed = true;
			return NULL;
		}
		
		p->updateUniforms();
		
		ssSelectorLocation = glGetUniformLocation(p->getProgram(), "u_selector");
		ssAlphaLocation = glGetUniformLocation(p->getProgram(), "u_alpha");
		sshasPremultipliedAlpha = glGetUniformLocation(p->getProgram(), "u_hasPremultipliedAlpha");
		if (ssSelectorLocation == GL_INVALID_VALUE
		 || ssAlphaLocation == GL_INVALID_VALUE)
		{
			delete p;
			p = NULL;
			constructFailed = true;
			return NULL;
		}

		glUniform1i(ssSelectorLocation, 0);
		glUniform1f(ssAlphaLocation, 1.0f);
		glUniform1i(sshasPremultipliedAlpha, 0);
	}
	return p;
}
*/

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

void CustomSprite::changeShaderProgram(bool useCustomShaderProgram)
{
/*
	if (useCustomShaderProgram != _useCustomShaderProgram)
	{
		if (useCustomShaderProgram)
		{
			CCGLProgram *shaderProgram = getCustomShaderProgram();
			if (shaderProgram == NULL)
			{
				// Not use custom shader.
				shaderProgram = _defaultShaderProgram;
				useCustomShaderProgram = false;
			}
			this->setShaderProgram(shaderProgram);
			_useCustomShaderProgram = useCustomShaderProgram;
		}
		else
		{
			this->setShaderProgram(_defaultShaderProgram);
			_useCustomShaderProgram = false;
		}
	}
*/
}



void CustomSprite::setColorBlendFunc(int colorBlendFuncNo){
	_colorBlendFuncNo = colorBlendFuncNo;
}


void CustomSprite::setOpacity(unsigned char opacity){
	_opacity = static_cast<float>(opacity) / 255.0f;
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
