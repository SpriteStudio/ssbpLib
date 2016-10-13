#pragma once

#include "SS5PlayerData.h"

namespace ss{

/**
 * ProjectDataから各データへのアクセサ
 *  特定の型へのアクセスしかしないので、読みやすさのためにもアクセス方法を提供しておく
 */
class ToPointer{
public:
	explicit ToPointer(const ProjectData* data)
		: _base(static_cast<const char*>(static_cast<const void*>(data))) {}
	

	const Cell* toCell(const ProjectData *adr) const{
		return toClass<Cell>( adr->cells );					//return static_cast<const Cell*>(getAddress( adr->cells ));
	}
	const CellMap* toCellMap(const Cell *adr) const{
		return toClass<CellMap>( adr->cellMap );
	}
	
	
	const AnimePackData* toAnimePackData(const ProjectData *adr) const{
		return toClass<AnimePackData>( adr->animePacks );	//return static_cast<const AnimePackData*>(getAddress( adr->animePacks));
	}
	const PartData* toPartData(const AnimePackData *adr) const{
		return toClass<PartData>( adr->parts );
	}
	const AnimationData* toAnimationData(const AnimePackData *adr) const{
		return toClass<AnimationData>( adr->animations );
	}
	const AnimationInitialData* toAnimationInitialData(const AnimationData *adr) const{
		return toClass<AnimationInitialData>( adr->defaultData );
	}


	//ほかのアクセスはほとんどconst char*なのでこれはoffsetを取るようにしとく
	const char* toString(ss_offset offset) const{
		return toClass<char>( offset );
	}

		
		
		
//private:	
	const void* operator()(ss_offset offset) const{
		return getAddress(offset);
	}
	
private:
	//キャストのテンプレート化
	template<class T>
	const T* toClass(ss_offset offset) const{
		return static_cast<const T*>(getAddress( offset ));
	}

	const void* getAddress(ss_offset offset) const{
		return (_base + offset);
	}


	const char*	_base;			//
};


} //namespace ss
