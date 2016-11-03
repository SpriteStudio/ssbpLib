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
	const ProjectData* m_data;
	EffectCache* m_effectCache;
	CellCache* m_cellCache;
	AnimeCache* m_animeCache;

	~ResourceSet()
	{
		delete m_data;
		m_data = NULL;
		
		if (m_animeCache)
		{
			delete m_animeCache;
			m_animeCache = NULL;
		}
		if (m_cellCache)
		{
			delete m_cellCache;
			m_cellCache = NULL;
		}
		if (m_effectCache)
		{
			delete m_effectCache;
			m_effectCache = NULL;
		}
	}
};



} //namespace ss

