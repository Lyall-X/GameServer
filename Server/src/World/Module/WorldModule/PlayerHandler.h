#pragma once


class Player;
class RedisEvent;

class PlayerHandler : public Object
{
public:
	PlayerHandler();
protected:
	
	int32 onNetFirst(Player* player, NetFirst* nfy);

	int32 onNetCreateRoleReq(Player* aPlr, NetCreateRoleReq* req);
	int32 onNetSelectRoleReq(Player* aPlr, NetSelectRoleReq* req);
	int32 onNetQueryRoleReq(Player* aPlr, NetQueryRoleReq* req);

	int32 onNetChangeNameReq(Player* player, NetChangeNameReq* req);
	int32 onNetChatMsgNotify(Player* player, NetChatMsgNotify* nfy);
	int32 onNetGmMsg(Player* aPlr, NetGmMsg* msg);
	int32 onNetEntityMoveNotify(Player* player, NetEntityMoveNotify* nfy);
	int32 onNetEntityMoveToNotify(Player* player, NetEntityMoveToNotify* nfy);
};