#pragma once
#include <string>
#include <map>

namespace ss{
struct ProjectData;
class SsEffectModel;
class CellCache;

/**
 * EffectCache
 */
class EffectCache{
public:
	EffectCache(const ProjectData* data, const std::string& imageBaseDir, CellCache* cellCache);
	~EffectCache();
	
	/**
	 * エフェクトファイル名を指定してEffectRefを得る
	 */
	SsEffectModel* getReference(const std::string& name);

	void dump();

private:
	void init(const ProjectData* data, const std::string& imageBaseDir, CellCache* cellCache);

	//エフェクトファイル情報の削除
	void releseReference(void);

private:
	std::map<std::string, SsEffectModel*>		_dic;
};


} //namespace ss
