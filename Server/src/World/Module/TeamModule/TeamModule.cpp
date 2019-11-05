#include "stdafx.h"

IMPLEMENT_CLASS(TeamModule, Module)
TeamModule::TeamModule()
{

}

TeamModule::~TeamModule()
{
	Destroy();
}

bool TeamModule::Update(float time, float delay)
{
	for (auto itr : mMapTeam)
	{
		Team* team = itr.second;
		team->Update();
		if (team->GetPlayerCount() <= 0 || !team->GetValid())
			DestroyTeam(team);
	}

	ClearDelTeamArray();
	return true;
}

bool TeamModule::Destroy()
{
	for (auto itr : mMapTeam)
		delete itr.second;
	mMapTeam.clear();
	ClearDelTeamArray();
	return true;
}

bool TeamModule::onEnterWorld(Player* player, Dictionary& dict)
{
	Team* tm = FindTeamByUserId(player->getUserId());
	if (tm == NULL)
		return true;

	tm->onEnterWorld(player);
	return true;
}

bool TeamModule::onLeaveWorld(Player* player, Dictionary& dict)
{
	Team* tm = FindTeamByUserId(player->getUserId());
	if (tm == NULL)
		return true;

	tm->onLeaveWorld(player);
	return true;
}

Team* TeamModule::CreateTeam(Player* leader)
{
	if (FindTeamByUserId(leader->getUserId())) return NULL;
	Team* t = new Team();
	AddTeam(t);
	t->OnCreate(leader->getUserId());
	return NULL;
}

void TeamModule::DestroyTeam(Team* team)
{
	mDelTeamList.insert(team);
}

Team* TeamModule::AddTeam(Team* team)
{
	if (FindTeamById(team->GetInsId()))
		return NULL;
	mMapTeam.insert(std::make_pair(team->GetInsId(), team));
	return team;
}

void TeamModule::RemoveTeam(uint32 teamId)
{
	mMapTeam.erase(teamId);
}

Team* TeamModule::FindTeamById(uint32 teamId)
{
	auto itr = mMapTeam.find(teamId);
	if (itr != mMapTeam.end())
		return itr->second;
	return NULL;
}

Team* TeamModule::FindTeamByUserId(uint32 userId)
{
	auto itr = mMapPlayerTeam.find(userId);
	if (itr != mMapPlayerTeam.end()) {
		Team* aTeam = itr->second;
		if (aTeam && aTeam->GetValid())
			return aTeam;
	}

	return NULL;
}

bool TeamModule::AddPlayerTeam(uint32 userid, Team* team)
{
	auto itr = mMapPlayerTeam.find(userid);
	if (itr != mMapPlayerTeam.end())
		return false;

	mMapPlayerTeam[userid] = team;
	return true;
}

void TeamModule::RemovePlayerTeam(uint32 userid)
{
	mMapPlayerTeam.erase(userid);
}

bool TeamModule::DoPlayerAddTeam(Player* player, Team* team, bool isLeader /* = false */)
{
	if (FindTeamByUserId(player->getUserId()))
		return false;
	if (!team->addPlayer(player, isLeader))
		return false;
	AddPlayerTeam(player->getUserId(), team);
	team->OnEnter(player->getUserId());
	return true;
}

bool TeamModule::DoPlayerRemoveTeam(uint32 userId, Team* team)
{
	TeamEntity* teny = team->GetTeamEntity(userId);
	if (teny && teny->getPlayer())
		team->OnLeave(teny->getPlayer()->getUserId());

	if (!team->RemovePlayer(userId))
		return false;
	RemovePlayerTeam(userId);

	if (team->GetPlayerCount() <= 0)
		team->SetValid(false);
	return true;
}

bool TeamModule::DoFreeTeam(uint32 teamId)
{
	Team* aTeam = FindTeamById(teamId);
	if (aTeam == NULL) return false;
	std::list<uint32> plrIDs;
	for (uint32 i = 0; i < aTeam->GetPlayerCount(); ++i)
	{
		TeamEntity* aEnt = aTeam->FindTeamEntity(i);
		if (aEnt == NULL) continue;
		plrIDs.push_back(aEnt->getUserId());
	}

	for (uint32 plrId : plrIDs)
	{
		DoPlayerRemoveTeam(plrId, aTeam);
	}
	return true;
}

void TeamModule::DoCreateTeamReq(Player* aPlr)
{
	Team* aTeam = CreateTeam(aPlr);
	if (aTeam == NULL) return;
	DoPlayerAddTeam(aPlr, aTeam);
}

void TeamModule::DoOrganizeTeamReq(Player* aPlr, cstring& tarName)
{
	Player* target = sWorld.FindPlrByName(tarName);
	if (!target)
		return ;

	Team* tm0 = FindTeamByUserId(aPlr->getUserId());
	Team* tm1 = FindTeamByUserId(target->getUserId());
	if (tm0 && tm1)
	{
		// 已经有队伍 ;
		return ;
	}

	if (tm0 && !tm1)
	{
		Player* leader = tm0->GetLeader();
		if (leader == NULL)
			return ;

		NetOrganizeTeamRes res;
		res.name = leader->getName();
		res.isJoin = 0;
		target->sendPacket(res);
		return ;
	}

	if (!tm0 && tm1)
	{
		Player* leader = tm1->GetLeader();
		if (leader == NULL)
			return ;

		NetOrganizeTeamRes res;
		res.name = aPlr->getName();
		res.isJoin = 1;
		leader->sendPacket(res);

		return ;
	}

	if (!tm0 && !tm1)
	{
		Team* tm = CreateTeam(aPlr);
		if (tm == NULL)
			return ;
		DoPlayerAddTeam(aPlr, tm);
		DoPlayerAddTeam(target, tm);

		NetOrganizeTeamRes res;
		res.name = aPlr->getName();
		res.isJoin = 0;
		target->sendPacket(res);
		return ;
	}
}

void TeamModule::DoAgreeTeamReq(Player* aPlr, cstring& tarName, uint8 isJoin)
{
	Player* target = sWorld.FindPlrByName(tarName);
	if (!target)
		return;

	Team* tm0 = FindTeamByUserId(aPlr->getUserId());
	Team* tm1 = FindTeamByUserId(target->getUserId());

	if (tm0 && tm1)
		return;

	// 邀请;
	if (isJoin == 0)
	{
		if (tm0)
		{
			// 已经有队伍了;
			aPlr->sendRespnoseMsg(MC_InviterHaveTeam);
			return;
		}

		if (!tm1)
		{
			// 队伍不存在;
			aPlr->sendRespnoseMsg(MC_JoinerTeamNotExist);
			return;
		}

		if (tm1->CanAddTeam(aPlr) == 0)
		{
			DoPlayerAddTeam(aPlr, tm1);
			return;
		}
	}
	else
	{
		// 加入;
		if (tm1)
		{
			// 已经有队伍了;
			aPlr->sendRespnoseMsg(MC_HaveTeam);
			return;
		}
		if (!tm0)
		{
			// 已经没有队伍了;
			aPlr->sendRespnoseMsg(MC_TeamNotExist);
			return;
		}

		if (tm0->GetLeader() != aPlr)
		{
			// 不是队长不能同意加入;
			aPlr->sendRespnoseMsg(MC_AgreeAddTeamNotLeader);
			return;
		}
		if (tm0->CanAddTeam(target) == 0)
		{
			DoPlayerAddTeam(target, tm0);
			return;
		}
	}
	aPlr->sendRespnoseMsg(MC_TeamError);
}

void TeamModule::sendPacketToAll(Packet& packet)
{
	for (auto itr : mMapTeam)
		itr.second->sendPacketToAll(packet);
}

void TeamModule::sendPacketToTeam(Packet& packet, Player* player)
{
	Team* tm = FindTeamByUserId(player->getUserId());
	if (tm == NULL)
		return;
	tm->sendPacketToAll(packet);
}

void TeamModule::ClearDelTeamArray()
{
	for (Team* aTeam : mDelTeamList)
	{
		RemoveTeam(aTeam->GetInsId());
		delete aTeam;
	}
	mDelTeamList.clear();
}
