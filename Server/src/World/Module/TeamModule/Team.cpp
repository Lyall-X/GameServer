#include "stdafx.h"
#include "Application.h"

uint32 Team::sId = 0;
Team::Team():
mInstanceId(++sId),
mLeader(NULL)
{
}

Team::~Team()
{
	for (TeamEntity* tEnt: mTeamEntityList)
		delete tEnt;
	mTeamEntityList.clear();
	ClearDelTeamEntitySet();
}

void Team::Update()
{
	ClearDelTeamEntitySet();
}

void Team::sendPacketToAll(Packet& packet)
{
	static char sPacketBuffer[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(sPacketBuffer, PACKET_MAX_LENGTH);
	in << packet;
	for (TeamEntity* tEnt : mTeamEntityList)
	{
		if (tEnt->getPlayer())
			tEnt->getPlayer()->sendBuffer(in.datas(), in.wpos());
	}
}

bool Team::addPlayer(Player* player, bool isLeader /* = false */)
{
	if (GetTeamEntity(player->getUserId()))
		return false;
	if (IsFull()) return false;

	if (isLeader && mLeader)
		return false;
	TeamEntity* eny = new TeamEntity;
	eny->setPlayer(player);
	eny->setName(player->getName());
	eny->setUserId(player->getUserId());
	if (isLeader) {
		mLeader = eny;
	}
	mTeamEntityList.push_back(eny);
	return true;
}

bool Team::RemovePlayer(uint32 userId)
{
	TeamEntity* tEnt = GetTeamEntity(userId);
	if (tEnt == NULL) return false;
	mTeamEntityList.remove(tEnt);
	if (mTeamEntityList.size() == 0)
		mLeader = NULL;

	mDelTeamEntitySet.insert(tEnt);
	return true;
}

TeamEntity* Team::GetTeamEntity(uint32 userId)
{
	for (TeamEntity* tEnt : mTeamEntityList)
	{
		if (tEnt->getUserId() == userId) return tEnt;
	}
	return NULL;
}

void Team::onEnterWorld(Player* player)
{
	TeamEntity* teny = GetTeamEntity(player->getUserId());
	if (teny == NULL)
		return;
	teny->setPlayer(player);
}

void Team::onLeaveWorld(Player* player)
{
	TeamEntity* teny = GetTeamEntity(player->getUserId());
	if (teny == NULL)
		return;
	teny->setPlayer(NULL);

	if (GetLeader() == player)
	{
		TeamEntity* nteny = ChooseLeader();
		if (nteny == NULL) {
			sTeam.DoFreeTeam(GetInsId());
			return;
		}
		ChangeLeader(nteny->getPlayer());
	}
}


uint32 Team::GetPlayerCount()
{
	return (uint32)mTeamEntityList.size();
}

TeamEntity* Team::FindTeamEntity(uint32 idx)
{
	uint32 count = 0;
	for (TeamEntity* tEnt : mTeamEntityList)
	{
		if (count == idx)
			return tEnt;
		count++;
	}
	return NULL;
}

bool Team::IsFull()
{
	if (mTeamEntityList.size() >= TEAM_MAX_COUNT) return true;
	return false;
}

bool Team::ChangeLeader(Player* newLeader)
{
	TeamEntity* teny = GetTeamEntity(newLeader->getUserId());
	if (teny == NULL)
		return false;

	if (GetLeader() == newLeader)
		return false;
	
	Player* lastLader = GetLeader();
	mLeader = teny;
	OnChangeLeader(lastLader->getUserId(), newLeader->getUserId());
	return true;
}

int32 Team::CanAddTeam(Player* tar)
{
	if (GetTeamEntity(tar->getUserId()))
		return 1;
	if (IsFull()) return 1;

	return 0;
}

TeamEntity* Team::ChooseLeader()
{
	if (mLeader == NULL)
		return NULL;

	for (TeamEntity* tEnt : mTeamEntityList)
	{
		if (tEnt->getPlayer() && mLeader != tEnt)
			return tEnt;
	}
	return NULL;
}

void Team::ClearDelTeamEntitySet()
{
	for (TeamEntity* tEnt : mDelTeamEntitySet)
		delete tEnt;
	mDelTeamEntitySet.clear();
}

bool Team::OnCreate(uint32 userId)
{
	LuaEngine::Call(sScriptTeam, "OnCreate", GetInsId(), userId);
	return true;
}

bool Team::OnEnter(uint32 userId)
{
	LuaEngine::Call(sScriptTeam, "OnEnter", GetInsId(), userId);
	return true;
}

bool Team::OnLeave(uint32 userId)
{
	LuaEngine::Call(sScriptTeam, "OnLeave", GetInsId(), userId);
	return true;
}

bool Team::OnChangeLeader(uint32 oldUserId, uint32 newUserId)
{
	LuaEngine::Call(sScriptTeam, "OnChangeLeader", GetInsId(), oldUserId, newUserId);
	return true;
}