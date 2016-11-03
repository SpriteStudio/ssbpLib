#pragma once
#include "SS5PlayerData.h"
#include "SS5EffectCache.h"
#include "SS5CellCache.h"
#include "SS5AnimCache.h"

namespace ss{


/**
 * ResourceSet
 */
class ResourceSet{
public:
	const ProjectData* data;
	EffectCache* effectCache;
	CellCache* cellCache;
	AnimeCache* animeCache;

	virtual ~ResourceSet()
	{
		delete data;
		data = NULL;
		
		if (animeCache)
		{
			delete animeCache;
			animeCache = NULL;
		}
		if (cellCache)
		{
			delete cellCache;
			cellCache = NULL;
		}
		if (effectCache)
		{
			delete effectCache;
			effectCache = NULL;
		}
	}
};



} //namespace ss

