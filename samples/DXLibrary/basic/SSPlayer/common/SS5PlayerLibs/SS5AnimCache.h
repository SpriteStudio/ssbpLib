#pragma once
#include <string>
#include <map>

namespace ss{
struct AnimationData;
struct AnimePackData;
struct ProjectData;

/**
 * AnimeRef
 */
struct AnimeRef{
	std::string				packName;
	std::string				animeName;
	const AnimationData*	animationData;
	const AnimePackData*	animePackData;
};


/**
 * AnimeCache
 */
class AnimeCache{
public:
	AnimeCache();
	~AnimeCache();
	static AnimeCache* create(const ProjectData* data);

	/**
	 * packNameとanimeNameを指定してAnimeRefを得る
	 */
	AnimeRef* getReference(const std::string& packName, const std::string& animeName);

	/**
	 * animeNameのみ指定してAnimeRefを得る
	 */
	AnimeRef* getReference(const std::string& animeName);

	void dump();

private:
	void init(const ProjectData* data);

	static std::string toPackAnimeKey(const std::string& packName, const std::string& animeName);

	//キャッシュの削除
	void releseReference(void);

private:
	std::map<std::string, AnimeRef*>	_dic;
};




} //namespace ss

