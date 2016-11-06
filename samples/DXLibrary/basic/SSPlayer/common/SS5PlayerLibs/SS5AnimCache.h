#pragma once
#include <string>
#include <map>

namespace ss{
struct AnimationData;
struct AnimePackData;
struct PartData;
struct ProjectData;
class ToPointer;

/**
 * AnimeRef
 *  AnimationDataへのアクセスを提供
 *  パーツデータはssae単位で共通のものだが、利便性のためここで抱えておく
 */
struct AnimeRef{
	std::string				packName;
	std::string				animeName;
	const AnimationData*	animationData;

	//const AnimePackData*	animePackData;	//属するパッケージ
	const PartData*			partDatas;		//パーツデータへのショートカット aniemPackData->partDatas[]
	int						numParts;		//パーツデータ数
};


/**
 * AnimeCache
 *  ProjectDataからAnimationDataへのアクセスを構築する
 */
class AnimeCache{
public:
	/** dataを元にAnimeRefを構築 */
	AnimeCache(const ProjectData* data);
	~AnimeCache();

	/** packNameとanimeNameを指定してAnimeRefを得る */
	AnimeRef* getReference(const std::string& packName, const std::string& animeName);

	/** animeNameのみ指定してAnimeRefを得る */
	AnimeRef* getReference(const std::string& animeName);

	/** デバッグ用 */
	void dump();

private:
	/** dataを元にAnimeRefを構築 */
	void init(const ProjectData* data);
	void addAnimationData(ToPointer ptr, const AnimePackData* pack);

	/** ファイルパス生成 */
	static std::string toPackAnimeKey(const std::string& packName, const std::string& animeName);


private:
	std::map<std::string, AnimeRef>	_dic;
};




} //namespace ss

