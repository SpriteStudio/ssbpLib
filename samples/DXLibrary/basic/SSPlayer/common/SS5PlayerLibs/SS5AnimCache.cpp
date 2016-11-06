#include "SS5AnimCache.h"
#include "SS5Player.h"
#include "SS5PlayerToPointer.h"

namespace ss{


AnimeCache::AnimeCache(const ProjectData* data)
{
	init(data);
}

AnimeCache::~AnimeCache()
{
}
		
//packNameとanimeNameを指定してAnimeRefを得る
AnimeRef* AnimeCache::getReference(const std::string& packName, const std::string& animeName)
{
	std::string key = toPackAnimeKey(packName, animeName);
	AnimeRef* ref = &(_dic.at(key));
	return ref;
}

		
//animeNameのみ指定してAnimeRefを得る
AnimeRef* AnimeCache::getReference(const std::string& animeName)
{
	AnimeRef* ref = &(_dic.at(animeName));
	return ref;
}

void AnimeCache::dump()
{
	for(auto &str_aref : _dic){
		SSLOG("%s", str_aref.first);
	}
}


void AnimeCache::init(const ProjectData* data)
{
	SS_ASSERT2(data != NULL, "Invalid data");

	ToPointer ptr(data);
	const AnimePackData* animePacks = ptr.toAnimePackData(data);

	for(int packIndex = 0; packIndex < data->numAnimePacks; packIndex++){
		const AnimePackData* pack = &animePacks[packIndex];
		addAnimationData(ptr, pack);		//ssaeからAnimationDataを登録する
	}
}

//ssaeからAnimationDataを登録する
void AnimeCache::addAnimationData(ToPointer ptr, const AnimePackData* pack)
{
	const char* packName = ptr.toString(pack->name);

	const PartData* partDatas = ptr.toPartData(pack);				//array
	const AnimationData* animations = ptr.toAnimationData(pack);	//array
	
	for (int animeIndex = 0; animeIndex < pack->numAnimations; animeIndex++){
		const AnimationData* anime = &animations[animeIndex];
		const char* animeName = ptr.toString(anime->name);

		AnimeRef ref = {
			packName, animeName, anime, partDatas, pack->numParts
		};

		// packName + animeNameでの登録
		std::string key = toPackAnimeKey(packName, animeName);
		SSLOG("anime key: %s", key.c_str());
		_dic.insert(std::make_pair(key, ref));

		// animeNameのみでの登録
		_dic.insert(std::make_pair(animeName, ref));
	}
}

std::string AnimeCache::toPackAnimeKey(const std::string& packName, const std::string& animeName)
{
	return packName + "/" + animeName;		//return Format("%s/%s", packName.c_str(), animeName.c_str());
}


} //namespace ss;

