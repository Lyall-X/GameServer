#pragma once

class RoomHandler : public Object
{
public:
	RoomHandler();
protected:
	int32 onNetCreateRoomReq(Player* aPlr, NetCreateRoomReq* req);
	int32 onNetEnterRoomReq(Player* aPlr, NetEnterRoomReq* req);
	int32 onNetLeaveRoomReq(Player* aPlr, NetLeaveRoomReq* req);
	int32 onNetChangeRoomMasterReq(Player* aPlr, NetChangeRoomMasterReq* req);
	int32 onNetRoomListReq(Player* aPlr, NetRoomListReq* req);
	int32 onNetRoomReadyReq(Player* aPlr, NetRoomReadyReq* req);
	int32 onNetRoomStartGameReq(Player* aPlr, NetRoomStartGameReq* req);
private:
};