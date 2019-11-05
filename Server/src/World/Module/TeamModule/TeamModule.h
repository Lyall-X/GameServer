#pragma once

class Entity;
class Player;
class Packet;
class Team;

class TeamModule : public Module
{
	DECLARE_CLASS(TeamModule)
public:
	TeamModule();
	~TeamModule();
	virtual bool Initialize() { return true; }
	virtual bool Update(float time, float delay);
	virtual bool Destroy();
protected:
	virtual bool onEnterWorld(Player* player, Dictionary& dict);
	virtual bool onLeaveWorld(Player* player, Dictionary& dict);
public:
	void DestroyTeam(Team* team);
	void RemoveTeam(uint32 teamId);
	void RemovePlayerTeam(uint32 userid);

	Team* CreateTeam(Player* leader);
	Team* AddTeam(Team* team);
	Team* FindTeamById(uint32 teamId);
	Team* FindTeamByUserId(uint32 userId);
	bool AddPlayerTeam(uint32 userid, Team* team);
public:
	bool DoPlayerAddTeam(Player* player, Team* team, bool isLeader = false);
	bool DoPlayerRemoveTeam(uint32 userId, Team* team);
	bool DoFreeTeam(uint32 teamId);
public:
	void DoCreateTeamReq(Player* aPlr);
	void DoOrganizeTeamReq(Player* aPlr, cstring& tarName);
	void DoAgreeTeamReq(Player* aPlr, cstring& tarName, uint8 isJoin);
public:
	void sendPacketToAll(Packet& packet);
	void sendPacketToTeam(Packet& packet, Player* player);
protected:
	void ClearDelTeamArray();
protected:
	std::map<uint32, Team*>								mMapTeam;
	std::map<uint32, Team*>								mMapPlayerTeam;
	std::set<Team*>										mDelTeamList;
};