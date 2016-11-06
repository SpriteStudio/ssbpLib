#pragma once
#include <string>
#include <map>

namespace ss{
struct ProjectData;
class ResourceSet;

/**
 * ResourceManager
 */
class ResourceManager{
public:
	static const std::string s_null;

	/**
	 * デフォルトインスタンスを取得します.
	 *
	 * @return デフォルトのResourceManagerインスタンス
	 */
	static ResourceManager* getInstance();

#if 0
	/**
	 * ssbpファイルを読み込み管理対象とします.
	 * dataKeyはssbpのファイル名（拡張子なし）になります.
	 *
	 * @param  ssbpFilepath  ssbpファイルのパス
	 * @param  imageBaseDir  画像ファイルの読み込み元ルートパス. 省略時はssbpのある場所をルートとします.
	 * @return dataKey
	 */
	std::string addData(const std::string& ssbpFilepath, const std::string& imageBaseDir = s_null);

	/**
	 * ssbpファイルを読み込み管理対象とします.
	 *
	 * @param  dataKey       dataKeyの指定
	 * @param  ssbpFilepath  ssbpファイルのパス
	 * @param  imageBaseDir  画像ファイルの読み込み元ルートパス. 省略時はssbpのある場所をルートとします.
	 * @return dataKey
	 */
	std::string addDataWithKey(const std::string& dataKey, const std::string& ssbpFilepath, const std::string& imageBaseDir = s_null);

	/**
	 * 指定されたssbpデータを管理対象とします.
	 *
	 * @param  dataKey       dataKeyの指定
	 * @param  data          ssbpデータ
	 * @param  imageBaseDir  画像ファイルの読み込み元ルートパス. 省略時はssbpのある場所をルートとします.
	 * @return dataKey
	 */
	std::string addData(const std::string& dataKey, const ProjectData* data, const std::string& imageBaseDir = s_null);
#endif

	/**
	 * ssbpファイルを登録します
	 *
	 * @param  data			ssbpのデータ(中でコピーされます)
	 * @param  dataSize		dataのサイズ
	 * @param  dataKey		登録名
	 * @param  imageBaseDir 画像ファイルの読み込み元ルートパス. 省略時はコンバート時に指定されたパスを使用する
	 */
	bool addData(const void *data, size_t dataSize, const std::string &dataKey, const std::string &imageBaseDir = s_null);

	/**
	 * 指定データを解放します.
	 * パス、拡張子を除いたssbp名を指定してください。
	 *
	 * @param  dataKey
	 */
	void removeData(const std::string& dataKey);

	/**
	 * 全てのデータを解放します.
	 */
	void removeAllData();

	/**
	 * 名前に対応するデータ取得します.
	 */
	ResourceSet* getData(const std::string& dataKey);

	/**
	 * 指定したセルのテクスチャを変更します.
	 * @param  ssbpName       ssbp名（拡張子を除くファイル名）
	 * @param  ssceName       ssce名（拡張子を除くファイル名）
	 * @param  texture        変更後のテクスチャハンドル
	 * @return 変更を行ったか
	 */
	bool changeTexture(char* ssbpName, char* ssceName, long texture);

	/**
	 * 指定したデータのテクスチャを破棄します。
	 * @param  dataName       ssbp名（拡張子を除くファイル名）
	 * @return 成功失敗
	 */
	bool releseTexture(std::string);

	/**
	 * 読み込んでいるssbpからアニメーションの総フレーム数を取得します。
	 * @param  ssbpName       ssbp名（拡張子を除くファイル名）
	 * @param  animeName      ssae/モーション名
	 * @return アニメーションの総フレーム（存在しない場合はアサート）
	 */
	int getMaxFrame(std::string ssbpName, std::string animeName);

	/**
	 * 名前が登録されていればtrueを返します
	 *
	 * @param dataKey
	 * @return
	 */
	bool isDataKeyExists(const std::string& dataKey);

	/**
	 * 新たなResourceManagerインスタンスを構築します.
	 *
	 * @return ResourceManagerインスタンス
	 */
	static ResourceManager* create();

public:
	ResourceManager(void);
	virtual ~ResourceManager();

private:
	//imageBaseDirの指定がないときはdataの中を見てディレクトリを返す
	std::string getImageBaseDir(const std::string &imageBaseDir, const ProjectData *data) const;

	std::map<std::string, ResourceSet*>	_dataDic;
};


} //namespace ss

