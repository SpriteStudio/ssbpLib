#pragma once
#include <memory>
#include <vector>
#include "SS5PlayerData.h"
#include "SS5EffectCache.h"
#include "SS5CellCache.h"
#include "SS5AnimCache.h"

namespace ss{


/**
 * ResourceSet
 */
class ResourceSet{
private:
	static const ss_u32 DATA_ID = 0x42505353;	//データのチェック値
	static const ss_u32 DATA_VERSION = 4;		//データのチェック値

	std::vector<char> m_src;		//データの実体

public:
	const ProjectData* m_data;		//データを指すだけ
	std::unique_ptr<EffectCache> m_effectCache;
	std::unique_ptr<CellCache>   m_cellCache;	//cell周りの構造へのアクセサ
	std::unique_ptr<AnimeCache>  m_animeCache;	//anim周りの構造へのアクセサ

	/** dataはコピーされます */
	ResourceSet(const char *data, size_t dataSize, const std::string &imageBaseDir)
		: m_src(data, data + dataSize)	//コピー
		, m_data(nullptr)
		, m_effectCache(nullptr)
		, m_cellCache(nullptr)
		, m_animeCache(nullptr)
	{
		SS_ASSERT2(data, "Invalid data");
		SS_ASSERT2(dataSize > 0, "dataSize is zero");

		m_data = reinterpret_cast<const ProjectData*>(&m_src[0]);
		SS_ASSERT2(m_data->dataId == DATA_ID, "Not data id matched");
		SS_ASSERT2(m_data->version == DATA_VERSION, "Version number of data does not match");

		//アニメはエフェクトを参照し、エフェクトはセルを参照するのでこの順番で生成する必要がある
		m_cellCache.reset(new CellCache(m_data, imageBaseDir));
		m_effectCache.reset(new EffectCache(m_data, imageBaseDir, m_cellCache.get()));
		m_animeCache.reset(new AnimeCache(m_data));
	}

	~ResourceSet()
	{
	}
};



} //namespace ss

