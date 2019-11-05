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

struct TaskStepJson
{
	uint32				ID;
	std::string			Name;
	uint32				TaskId;
	uint32				TaskStepId;
};
typedef std::map<int32, TaskStepJson*> MapTaskStepJson;

struct TaskJson
{
	uint32 ID;
	std::string			Name;
	uint32				NextTaskId;
	uint32				PrevTaskId;
	bool				IsMain;
};

struct CardJson
{
	uint32				ID;
	uint32				Number;
	uint8				Color;
	std::string			Name;
};

struct GameLevelJson
{
	uint32				ID;
	uint32				Mingold;
	uint8				Maxgold;
	uint8				Poundage;
	uint32				Maxround;
	std::string			Desc;
};

struct ProductJson
{
	uint32 ID;
	std::string Desc;
	uint32 Price;
};
