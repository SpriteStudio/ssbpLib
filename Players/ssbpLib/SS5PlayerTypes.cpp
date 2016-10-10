#include "SS5PlayerTypes.h"
#include "SS5Player.h"
#include "common/DataArrayReader.h"

namespace ss{

//カラーの読み取り
void SSColor4B::readColor(DataArrayReader &reader){
	unsigned int raw = reader.readU32();
	a = static_cast<unsigned char>(raw >> 24);
	r = static_cast<unsigned char>(raw >> 16);
	g = static_cast<unsigned char>(raw >> 8);
	b = static_cast<unsigned char>(raw);
}

//rateを考慮して読む
void SSColor4B::readColorWithRate(DataArrayReader &reader){
	float blend_rate = reader.readFloat();					//レート読み込み
	readColor(reader);

	//ssbpではカラーブレンドのレート（％）は使用できません。
	//制限となります。
	//a = static_cast<int>(blend_rate * 255);	//レートをアルファ値として設定
}


//unsigned long化
unsigned long SSColor4B::pack() const{
	//memo:参考		D3DCOLOR_ARGB(a,r,g,b)	((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
	return ((unsigned long)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)));
}



//頂点オフセットの読み取り
void SSQuad3::readVertexTransform(DataArrayReader &reader){
	int vt_flags = reader.readU16();
	if(vt_flags & VERTEX_FLAG_LT){
		tl.x = reader.readS16();
		tl.y = reader.readS16();
	}
	if(vt_flags & VERTEX_FLAG_RT){
		tr.x = reader.readS16();
		tr.y = reader.readS16();
	}
	if(vt_flags & VERTEX_FLAG_LB){
		bl.x = reader.readS16();
		bl.y = reader.readS16();
	}
	if(vt_flags & VERTEX_FLAG_RB){
		br.x = reader.readS16();
		br.y = reader.readS16();
	}

#ifdef UP_MINUS
	//上がマイナスなので反転させる
	tl.y = -tl.y;
	tr.y = -tr.y;
	bl.y = -bl.y;
	br.y = -br.y;
#endif
}


}	// namespace ss

