#pragma once

enum EnumProperty
{
	ep_none,
	ep_entityType,
	ep_name,
	ep_insId,
	ep_CharId,
	ep_mapId,
	ep_posX,
	ep_posY,
	ep_speed,
	ep_dirPos,
	ep_Level,
	ep_MaxHp,
	ep_MaxMp,
	ep_Hp,
	ep_Mp,
	ep_Attack,
	ep_Defense,
	ep_Gold,
	ep_Diamond,
};

class Property : public Object
{
	DECLARE_CLASS(Property);
public:
	static std::map<int32, std::string> sMapPropertyType;
	static void Register(int32 type, const std::string& name);
	static std::string GetPropertyName(int32 type);
};

#define REGISTER_PROPERTY(type) struct type##Struct \
{\
	type##Struct()\
	{\
		Property::Register(type, #type);\
	}\
};\
static type##Struct s_##type##Struct;




class EntityProperty :  public Property
{
	DECLARE_CLASS(EntityProperty);
protected:
	virtual bool operator>>(BinaryStream& bytes);
	virtual bool operator<<(BinaryStream& bytes);
public:
	int32			accId = 0;
	int32			roleId = 0;
	int32			guid = 0;
	int32			CharId = 0;
	std::string		name;
	int32			mapId = 0;
	Vector2			pos;
	int8			dir = 0;
	
	uint8			mLevel = 0;
	int32			mMaxHp = 0;
	int32			mMaxMp = 0;
	int32			mHp = 0;
	int32			mMp = 0;
	int32			mAttack = 0;
	int32			mDefense = 0;
};

class PlayerProperty : public EntityProperty
{
	DECLARE_CLASS(PlayerProperty);
protected:
	virtual bool operator >> (BinaryStream& bytes);
	virtual bool operator << (BinaryStream& bytes);
public:
	uint64			mGold = 0;			// ½ð±Ò
	uint64			mDiamond = 0;		// ×êÊ¯
};

class Entity;
class Player;

class PropertyHelper
{
public:
	void setLevel(Entity* ent, uint8 value);
	uint8 getLevel(Entity* ent);

	void setSpeed(Entity* ent, float32 value);
	float32 getSpeed(Entity* ent);

	void setMaxHp(Entity* entity, int32 value);
	int32 getMaxHp(Entity* entity);

	void setMaxMp(Entity* entity, int32 value);
	int32 getMaxMp(Entity* entity);

	void setHp(Entity* entity, int32 value);
	int32 getHp(Entity* entity);

	void setMp(Entity* entity, int32 value);
	int32 getMp(Entity* entity);

	void setAttack(Entity* entity, int32 value);
	int32 getAttack(Entity* entity);

	void setDefense(Entity* entity, int32 value);
	int32 getDefense(Entity* entity);

	void setGold(Player* aPlr, int32 value);
	void addGold(Player* aPlr, int32 value);
	int32 getGold(Player* aPlr);
	bool hasGold(Player* aPlr, int32 value);

	void setDiamond(Player* aPlr, int32 value);
	void addDiamond(Player* aPlr, int32 value);
	int32 getDiamond(Player* aPlr);
	bool hasDiamond(Player* aPlr, int32 value);

	uint32 CalculateMaxHp(Entity* ent);
	uint32 CalculateMaxMp(Entity* ent);
	uint32 CalculateAttack(Entity* ent);
	uint32 CalculateDefense(Entity* ent);
};