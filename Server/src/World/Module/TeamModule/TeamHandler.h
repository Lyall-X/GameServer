#pragma once

class TeamHandler : public Object
{
public:
	TeamHandler();
protected:
	int32 onNetCreateTeamReq(Player* aPlr, NetCreateTeamReq* req);
	int32 onNetOrganizeTeamReq(Player* player, NetOrganizeTeamReq* req);
	int32 onNetAgreeTeamReq(Player* player, NetAgreeTeamReq* req);
	int32 onNetTeamListReq(Player* player, NetTeamListReq* req);
};