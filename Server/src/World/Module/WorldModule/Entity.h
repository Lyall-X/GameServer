#pragma once

enum EntityType
{
	ET_None,
	ET_Player,
	ET_Npc,
};

class Player;
class Entity;

typedef std::set<Entity*> EntitySet;

class Entity : public Object
{
	friend class Map;
	friend class Player;
public:
	Entity(int8 type = 0);
	~Entity();
public:
	virtual bool Initialize();
	virtual bool Update(float time, float delay);
	virtual bool Destroy();
	virtual const char* getName() { return mName.c_str(); }
	virtual const std::string& GetNameStr() { return mName; }
	virtual void setName(const std::string& name) { mName = name; }
	void setLine(uint8 l) { mLine = l; }
	uint8 getLine() { return mLine; }
	virtual	bool CanDestroy();
	virtual void SetDestroy(bool isDestroy) { mCanDestroy = isDestroy; }
public:
	virtual	void sendPacket(Packet& packet) {}
	virtual void sendBuffer(void* data, int32 count) {}
	virtual void sendPacketToView(Packet& packet) {}
	virtual void sendPacketToMap(Packet& packet) {}
	virtual void sendPacketToWorld(Packet& packet) {}
	virtual void sendPacketToTeam(Packet& packet) {}
	virtual void sendPacketToRoom(Packet& packet) {}
	virtual void sendPacketToTarget(Packet& packet, Entity* tar) {}
	virtual void sendRespnoseMsg(int32 msgId, std::vector<std::string>* msgParams = NULL) {}
	virtual void sendPacketToMsg(const std::string& msg) {}
	virtual void sendPacketToMsg(EnumChannel ec, const std::string& msg) {}
	virtual bool changeMapByMapInsId(int32 mapInsId);
	virtual bool changeMapByMapId(int32 mapId);
	virtual bool changeMapByMap(Map* map);
protected:
	virtual bool onEnterMap() { return true; }
	virtual bool onLeaveMap() { return true; }

	// 进入目标视野;
	virtual bool onEnterView(Entity* tar);
	virtual bool onLeaveView(Entity* tar);

	virtual bool onEnterPlayerView(Player* plr) { return true; }
	virtual bool onLeavePlayerView(Player* plr) { return true; }
public:
	Guid									mGuid;
	uint8									entityType;
	uint32									mCharId;
	uint8									mSex;
	Map*									mMap;
	Point2									mPos;
	Point2									mTarPos;
	float32									mSpeed;
	FSM*									mStatus;
	int8									mDirPosition;
	std::string								mName;
	std::list<Point2>						mPoints;
	float32									mCheckTime;
	Timer									mTimer;
	uint8									mLine;
public:
	Vector2 mPosition;
	Vector2 mTarPosition;
	Vector2 mDir;

	Vector2& getPos() { return mPosition; }
	Vector2& getTarPos() { return mTarPosition; }
	Vector2& getDir() { return mDir; }

	void setPos(const Vector2& pos) { mPosition = pos; }
	void setTarPos(const Vector2& tarPos) { mTarPosition = tarPos; }
	void setDir(const Vector2& dir) { mDir = dir; }
public:
	void setSpeed(float32 speed);
	void setDirPosition(int8 dir);
	void setCellPosition(Point2& pos);
	void setCellX(int32 x);
	void setCellY(int32 y);
	void setCellTarPos(Point2& pos) { mTarPos = pos; }
	void setCellTarX(int32 x) { mTarPos.x = x; }
	void setCellTarY(int32 y) { mTarPos.y = y; }
	void ChangeStatus(int32 status);
	void addChangeValue(const Variant& key, const Variant& value);
	void setCheckView(bool checkView) { mCheckView = checkView; }

	int8 getDirPosition() { return mDirPosition; }
	
	int32 getCellX() { return mPos.x; }
	int32 getCellY() { return mPos.y; }
	int32 getCellTarX() { return mTarPos.x; }
	int32 getCellTarY() { return mTarPos.y; }

	float32 getSpeed() { return mSpeed; }

	Point2& getCellPos() { return mPos; }
	Point2& getCellTarPos() { return mTarPos; }
	std::list<Point2>& getPoints() { return mPoints; }
public:
	virtual uint32 getCharId() { return mCharId; }

	void setMap(Map* map) { mMap = map; }
	void setMapId(uint32 mapId);
	void setMapInsId(uint32 mapInsId);
	void setInstanceId(Guid uId) { mGuid = uId; }
	void setCharId(uint32 charId) { mCharId = charId; }
	void setSex(uint8 sex) { mSex = sex; }
	void removeProperty(const std::string& name);
	void addVariant(const Variant& key, const Variant& value);
	void removeVariant(const Variant& key);

	bool ContainsVariantKey(const Variant& key);
	bool serializeProperty(BinaryStream& stream);
	bool deSerializeProperty(BinaryStream& stream);
	bool MoveTo();

	uint8 getEntityType() { return entityType; }
	uint8 getSex() { return mSex; }

	uint32 getMapId();
	uint32 getMapInsId();

	Guid getGuid() { return mGuid; }
	cstring getGuidStr();

	Property* getProperty(const std::string& name);
	Property* addProperty(Property* property);
	Variant& getVariant(const Variant& value);

	Dictionary& getDictProperty() { mDictProperty; }
	Map* getMap() { return mMap; }
	Visual& GetVisual() { return mVisual; }
protected:
	int32 onTimerChangeProperty(TimerEvent& e);
	int32 onTimerCheckView(TimerEvent& e);
protected:
	void ChangePos(int32 lastX, int32 lastY, int32 x, int32 y);
	bool isMoveTo();
protected:
	std::map<std::string, Property*>		mMapProperty;
	Dictionary								mDictProperty;
	Dictionary								mDictChangePro;
	bool									mCheckView;
	bool									mCanDestroy;
	Visual									mVisual;
};

#define EntityGetProperty(tar, class_name) (class_name*)(tar)->getProperty(#class_name)

#define CastProperty(tar, class_name) (class_name*)(tar)->getProperty(#class_name)