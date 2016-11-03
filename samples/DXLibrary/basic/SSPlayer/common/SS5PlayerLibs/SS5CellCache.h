#pragma once
#include <string>
#include <vector>
#include "SS5PlayerTypes.h"
#include "common/Loader/sstypes.h"


namespace ss{
class Cell;
struct ProjectData;


/**
 * CellRef
 */
struct CellRef{
	const Cell* cell;
	TextuerData texture;
	SSRect rect;
	std::string texname;
};


/**
 * CellCache
 */
class CellCache{
public:
	CellCache();
	~CellCache();
	
	static CellCache* create(const ProjectData* data, const std::string& imageBaseDir);

	CellRef* getReference(int index);

	//指定した名前のセルの参照テクスチャを変更する
	bool setCellRefTexture(const ProjectData* data, const char* cellName, long texture);


	//指定したデータのテクスチャを破棄する
	bool releseTexture(const ProjectData* data);

private:
	void init(const ProjectData* data, const std::string& imageBaseDir);

	//キャッシュの削除
	void releseReference(void);

	void addTexture(const std::string& imagePath, const std::string& imageBaseDir, SsTexWrapMode::_enum  wrapmode, SsTexFilterMode::_enum filtermode);

protected:
	std::vector<std::string>			_texname;
	std::vector<TextuerData>			_textures;
	std::vector<CellRef*>				_refs;
};




} //namespace ss;


