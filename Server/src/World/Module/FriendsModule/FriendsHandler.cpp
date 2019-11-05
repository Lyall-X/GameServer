#include "stdafx.h"

FriendsHandler::FriendsHandler()
{
	RegWorldEvent(ID_NetAddFriendReq, &FriendsHandler::onNetAddFriendReq, this);
}

int32 FriendsHandler::onNetAddFriendReq(Player* aPlr, NetAddFriendReq* req)
{
	Player* tarPlr = sWorld.FindPlrByName(req->tarName);
	if (tarPlr == NULL) {
		return 0;
	}
	req->tarName = aPlr->getName();
	tarPlr->sendPacket(*req);
	return 0;
}

int32 FriendsHandler::onNetAddFriendRes(Player* aPlr, NetAddFriendRes* res)
{
	Player* dstPlr = sWorld.FindPlrByName(res->tarName);
	if (dstPlr == NULL)
	{
		return 0;

	}

	if (res->result == NResultFail)
	{
		res->tarName = aPlr->getName();
		dstPlr->sendPacket(*res);
		return 0;
	}

	sFriends.MutualBindFriend(dstPlr, aPlr);
	return 0;

}

int32 FriendsHandler::onNetFriendListReq(Player* aPlr, NetFriendListReq* req)
{

	sFriends.DoFriendsList(aPlr);
	return 0;
}
