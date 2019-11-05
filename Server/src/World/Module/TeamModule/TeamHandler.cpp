#include "stdafx.h"

TeamHandler::TeamHandler()
{
	RegWorldEvent(ID_NetOrganizeTeamReq, &TeamHandler::onNetOrganizeTeamReq, this);
	RegWorldEvent(ID_NetAgreeTeamReq, &TeamHandler::onNetAgreeTeamReq, this);
	RegWorldEvent(ID_NetTeamListReq, &TeamHandler::onNetTeamListReq, this);
}


int32 TeamHandler::onNetCreateTeamReq(Player* aPlr, NetCreateTeamReq* req)
{
	sTeam.DoCreateTeamReq(aPlr);
	return 0;
}

int32 TeamHandler::onNetOrganizeTeamReq(Player* player, NetOrganizeTeamReq* req)
{
	sTeam.DoOrganizeTeamReq(player, req->name);
	return 0;
}

int32 TeamHandler::onNetAgreeTeamReq(Player* player, NetAgreeTeamReq* req)
{
	sTeam.DoAgreeTeamReq(player, req->name, req->isJoin);
	return 0;
}

int32 TeamHandler::onNetTeamListReq(Player* player, NetTeamListReq* req)
{
	return 1;
}