#pragma once

class FriendsHandler : public Object
{
public:
	FriendsHandler();

protected:
	int32 onNetAddFriendReq(Player* aPlr, NetAddFriendReq* req);
	int32 onNetAddFriendRes(Player* aPlr, NetAddFriendRes* res);
	int32 onNetFriendListReq(Player* aPlr, NetFriendListReq* req);
private:
};