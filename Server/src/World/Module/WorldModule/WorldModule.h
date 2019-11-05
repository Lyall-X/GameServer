#pragma once

class PlayerRecord
{
public:
	uint32 GetUserId();
	const std::string& GetName();
	bool GetOnline();
public:
	uint32 mUserId = 0;
	std::string mName;
	Player* mPlayer = NULL;
};
typedef std::map<uint32, PlayerRecord*> MapPlayerRecord;
typedef std::map<std::string, PlayerRecord*> MapPlayerNameRecord;

class WorldModule : public Module
{
	DECLARE_CLASS(WorldModule)
public:
	WorldModule();
	~WorldModule();
protected:
	virtual bool Initialize();
	virtual bool Update(float time, float delay);
	virtual bool Destroy();
	virtual bool onEnterWorld(Player* player, Dictionary& dict);
	virtual bool onLeaveWorld(Player* player, Dictionary& dict);
public:
	void sendPacketToAll(Packet& packet);
	void sendPacketToTarget(EnumChannel ec, Packet& packet, Player* self, uint32 tarUserId = 0);
	void sendPacketToMsg(EnumChannel ec, const std::string& msg, Player* self);
public:
	void removeEntity(Entity* ent);
	void removeEntity(Guid guid);
	void removePlayer(uint32 accId);
	void removePlayer(Player* plr);
	void removePlayerToUserId(uint32 userId);
	void removeNpc(uint32 npcId);
	void removeNpc(Npc* npc);

	bool ChangeName(Entity* ent, cstring& sname);
	Entity* addEntity(Entity* ent);
	Entity* addEntityByName(Entity* ent);

	Entity* FindEntByName(cstring& name);
	Entity* FindEntByGuid(Guid guid);
	
	Player* addPlayerByAccId(Player* plr);
	Player* addPlayerByName(Player* plr);
	Player* addPlayerToUserId(Player* plr);

	Player* FindPlrByName(cstring& name);
	Player* FindPlrByGuid(Guid guid);
	Player* FindPlrByAccId(uint32 accId);
	Player* FindPlrByUserId(uint32 userId);

	PlayerRecord* AddPlrRecord(PlayerRecord* aPlrRecord);
	PlayerRecord* FindPlrRecord(uint32 userId);
	PlayerRecord* FindPlrRecord(cstring& name);

	Npc* addNpc(Npc* npc);
	
	Npc* FindNpcByCfgId(uint32 cfgId);
	Npc* FindNpcByName(cstring& name);
	Npc* FindNpcByGuid(Guid guid);

	void DestroyEnt(Guid guid);
	void DestroyEnt(Entity* ent);
	void DelPlrRecord(uint32 userId);
	void ClearPlayerRecord();
	void ClearEntity();

	uint32 GetPlrCount() { return (uint32)mMapPlayer.size(); }

	std::map<uint32, Player*>& getMapPlayer() { return mMapPlayer; }
	MapPlayerRecord& GetMapPlayer() { return mMapPlrRecords; }
protected:
	int32 onRedisAuth(Event& e);
	int32 onRedisAllPlr(RedisEvent& e);
protected:
	std::map<Guid, Entity*>							mMapEntity;
	std::map<std::string, Entity*>					mMapNameEntity;
	std::map<uint32, Player*>						mMapPlayer;
	std::map<uint32, Player*>						mMapUserIdPlayer;
	std::map<uint32, Npc*>							mMapNpc;
	MapPlayerRecord				mMapPlrRecords;
	MapPlayerNameRecord			mMapPlayerNameRecord;
};