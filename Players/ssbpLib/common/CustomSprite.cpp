#include "CustomSprite.h"
#include "SS5Player.h"

namespace ss{

/**
 * CustomSprite
 */
 //カラーブレンド用のシェーダー処理は汎用的に使用する事ができないためすべてコメントにしてあります。
 //カラーブレンドを再現するための参考にしてください。

unsigned int CustomSprite::ssSelectorLocation = 0;
unsigned int CustomSprite::ssAlphaLocation = 0;
unsigned int CustomSprite::sshasPremultipliedAlpha = 0;

//static const GLchar * ssPositionTextureColor_frag =
//#include "ssShader_frag.h"

CustomSprite::CustomSprite():
//	: _defaultShaderProgram(NULL)
	  _useCustomShaderProgram(false)
	, _opacity(1.0f)
	, _colorBlendFuncNo(0)
	, _liveFrame(0.0f)
	, _hasPremultipliedAlpha(0)
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

void CustomSprite::sethasPremultipliedAlpha(int PremultipliedAlpha){
	_hasPremultipliedAlpha = PremultipliedAlpha;
}

bool CustomSprite::isCustomShaderProgramEnabled() const{
	return _useCustomShaderProgram;
}

void CustomSprite::setColorBlendFunc(int colorBlendFuncNo){
	_colorBlendFuncNo = colorBlendFuncNo;
}

SSV3F_C4B_T2F_Quad& CustomSprite::getAttributeRef(){
	return _sQuad;
}

void CustomSprite::setOpacity(unsigned char opacity){
//	CCSprite::setOpacity(opacity);
	_opacity = static_cast<float>(opacity) / 255.0f;
}


#if 1
void CustomSprite::draw(void)
{
/*
	CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "SSSprite - draw");


	if (!_useCustomShaderProgram)
	{
		CCSprite::draw();
		return;
	}


	SS_ASSERT2(!m_pobBatchNode, "If CCSprite is being rendered by CCSpriteBatchNode, CCSprite#draw SHOULD NOT be called");

	CC_NODE_DRAW_SETUP();

	ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);

	if (m_pobTexture != NULL)
	{
		ccGLBindTexture2D(m_pobTexture->getName());
	}
	else
	{
		ccGLBindTexture2D(0);
	}

	glUniform1i(ssSelectorLocation, _colorBlendFuncNo);
	glUniform1f(ssAlphaLocation, _opacity);
	glUniform1i(sshasPremultipliedAlpha, _hasPremultipliedAlpha);

	//
	// Attributes
	//

	ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex);

#define kQuadSize sizeof(m_sQuad.bl)
	long offset = (long)&m_sQuad;

	// vertex
	int diff = offsetof(ccV3F_C4B_T2F, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

	// texCoods
	diff = offsetof(ccV3F_C4B_T2F, texCoords);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

	// color
	diff = offsetof(ccV3F_C4B_T2F, colors);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));


	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

//	CHECK_GL_ERROR_DEBUG();


#if CC_SPRITE_DEBUG_DRAW == 1
	// draw bounding box
	CCPoint vertices[4] = {
		ccp(m_sQuad.tl.vertices.x, m_sQuad.tl.vertices.y),
		ccp(m_sQuad.bl.vertices.x, m_sQuad.bl.vertices.y),
		ccp(m_sQuad.br.vertices.x, m_sQuad.br.vertices.y),
		ccp(m_sQuad.tr.vertices.x, m_sQuad.tr.vertices.y),
	};
	ccDrawPoly(vertices, 4, true);
#elif CC_SPRITE_DEBUG_DRAW == 2
	// draw texture box
	CCSize s = this->getTextureRect().size;
	CCPoint offsetPix = this->getOffsetPosition();
	CCPoint vertices[4] = {
		ccp(offsetPix.x,offsetPix.y), ccp(offsetPix.x+s.width,offsetPix.y),
		ccp(offsetPix.x+s.width,offsetPix.y+s.height), ccp(offsetPix.x,offsetPix.y+s.height)
	};
	ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW

	CC_INCREMENT_GL_DRAWS(1);

	CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
*/
}
#endif

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
