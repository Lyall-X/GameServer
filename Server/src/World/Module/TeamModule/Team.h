#pragma once

class Player;
class Packet;
#define TEAM_MAX_COUNT 5

class TeamEntity
{
public:
	void setPlayer(Player* plr) { mPlayer = plr; }
	Player* getPlayer() { return mPlayer; }
	void setName(const std::string& name) { mName = name; }
	const std::string& getName() { return mName; }
	uint32 getUserId() { return mUserId; }
	void setUserId(uint32 rId) { mUserId = rId; }
protected:
	Player*			mPlayer;
	std::string		mName;
	uint32			mUserId;
};

typedef std::list<TeamEntity*> TeamEntityArray;
typedef std::set<TeamEntity*> TeamEntitySet;
class Team : public Object
{
	friend class TeamModule;
public:
	Team();
	~Team();
	uint32 GetInsId() { return mInstanceId; }

	void Update();
	void sendPacketToAll(Packet& packet);
	Player* GetLeader() { return mLeader ? mLeader->getPlayer() : NULL; }
	TeamEntity* GetLeaderEntity() { return mLeader; }
	TeamEntity* GetTeamEntity(uint32 userId);
	TeamEntity* FindTeamEntity(uint32 idx);

	bool addPlayer(Player* player, bool isLeader = false);
	bool RemovePlayer(uint32 userId);
	bool ChangeLeader(Player* newLeader);

	int32 CanAddTeam(Player* tar);

	void onEnterWorld(Player* player);
	void onLeaveWorld(Player* player);

	uint32 GetPlayerCount();
	bool GetValid() { return mValid; }
	void SetValid(bool valid) { mValid = valid; }
	bool IsFull();
protected:
	TeamEntity*	ChooseLeader();
	void ClearDelTeamEntitySet();
protected:
	bool OnCreate(uint32 userId);
	bool OnEnter(uint32 userId);
	bool OnLeave(uint32 userId);
	bool OnChangeLeader(uint32 oldUserId, uint32 newUserId);
protected:
	static uint32 sId;
protected:
	uint32						mInstanceId;
	TeamEntity*					mLeader;
	TeamEntityArray				mTeamEntityList;
	TeamEntitySet				mDelTeamEntitySet;
	bool						mValid = true;
};