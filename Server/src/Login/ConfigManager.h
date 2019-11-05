#pragma once


struct ServerConfig
{
	std::string Name;
	std::string Host;
	int16		Port;
	std::string User;
	std::string Password;
	std::string DbName;
};

struct MapJson
{
	uint32				ID;
	std::string			Name;
	uint8				Type;
	uint32				Row;
	uint32				Col;
	float32				Cell;
	uint32				LogicRow;
	uint32				LogicCol;
	uint32				ViewCell;
	uint32				LineMax;
};

struct ActorJson
{
	uint32				ID;
};

struct CharJson
{
	uint32				ID;
	std::string			Name;
	uint32				ActorId;
	uint32				PropertyId;
	uint8				Type;
	uint8				Sex;
};

struct NpcJson
{
	uint32				ID;
	std::string			Name;
	uint8				Type;
	uint32				CharId;
	uint32				MapId;
	int32				PosX;
	int32				PosY;
	std::string			script;
};

struct PropertyJson
{
	uint32				ID;
	float32				Speed;
	uint32				MaxHp;
	uint32				MaxMp;
	uint32				Attack;
	uint32				Defense;
	float32				SpeedUp;
	uint32				MaxHpUp;
	uint32				MaxMpUp;
	uint32				AttackUp;
	uint32				DefenseUp;
};

struct RobotJson
{
	uint32				ID;
	std::string			User;
	std::string			Password;
};


struct TaskJson
{
	uint32				ID;
	uint32				Next;
	std::string			Name;
};

struct TaskStepJson
{
	uint32				ID;
	uint32				TaskId;
	std::string			Name;
	uint32				Step;
};

namespace Json {
	class Reader;
	class Value;
}

class ConfigManager
{

public:


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
	std::map<int32, std::map<int32, TaskStepJson*>>		mMapTaskStepJsonMap;
};

