#include "SS5PlayerTypes.h"
#include "SS5Player.h"
#include "common/DataArrayReader.h"

namespace ss{


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

