#pragma once

namespace Json {
	class Reader;
	class Value;
}

class ConfigManager
{

public:
	ConfigManager();
	~ConfigManager();
	bool reloadConfig();
	ServerConfig& getConfig(const std::string& Name);
	const std::string getMsg(int32 id);

	bool loadJson(const std::string& path, Json::Reader& jsonReader, Json::Value& jsonRoot);
	void loadConfig(const std::string& path);
	void loadMsgConfig(const std::string& path);
	void loadMapJson(const std::string& path);
	void loadCharJson(const std::string& path);
	void loadNpcJson(const std::string& path);
	void loadPropertyJson(const std::string& path);
	void loadRobotJson(const std::string& path);
	void loadTaskJson(const std::string& path);
	void loadTaskStepJson(const std::string& path);
	void loadCardJson(cstring& path);
	void loadGameLevelJson(cstring& path);
	void loadProductJson(cstring& path);

	MapJson* getMapJson(uint32 id);
	std::map<int32, MapJson>& getMapMapJson() { return mMapMapJson; }
	CharJson* getCharJson(uint32 id);
	uint32 getCharJsonRandId();
	NpcJson* getNpcJson(uint32 npcId);
	std::vector<NpcJson*>* getNpcJsons(uint32 mapId);
	PropertyJson* getPropertyJson(uint32 pId);
	RobotJson* getRobotJson(uint32 id);
	std::map<int32, RobotJson>& getMapRobotJson() { return mMapRobotJson; }
	TaskJson* getTaskJson(uint32 id);
	TaskStepJson* getTaskStepJson(uint32 id);
	MapTaskStepJson* GetTaskSteps(uint32 taskId);
	bool GetTaskStepIsEndStep(uint32 ID);
	CardJson* getCardJson(uint32 id);
	CardJson* getCardJsonByName(cstring& name);
	GameLevelJson* getGameLevelJson(uint32 id);
	ProductJson* getProductJson(uint32 id);

	void loadAllFiles();
protected:
	std::map<std::string, std::pair<int8*, int32>>		mMapFilesStream;
	std::map<std::string, ServerConfig>					mMapServerConfig;
	std::map<int32, std::string>						mMapMsgConfig;
	std::map<int32, MapJson>							mMapMapJson;
	std::map<int32, CharJson>							mMapCharJson;
	std::map<int32, NpcJson>							mMapNpcJson;
	std::map<int32, std::vector<NpcJson*>>				mMapMapNpcJson;
	std::map<int32, PropertyJson>						mPropertyJson;
	std::map<int32, RobotJson>							mMapRobotJson;
	std::map<int32, TaskJson>							mMapTaskJson;
	std::map<int32, TaskStepJson>						mMapTaskStepJson;
	std::map<int32, MapTaskStepJson>					mMapTaskStepJsonMap;
	std::map<int32, CardJson>							mMapCard;
	std::map<std::string, CardJson*>					mMapNameCard;
	std::map<int32, GameLevelJson>						mMapGameLevelJson;
	std::map<int32, ProductJson>						mMapProductJson;
};

