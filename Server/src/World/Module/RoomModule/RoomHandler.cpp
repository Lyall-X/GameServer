#include "stdafx.h"

RoomHandler::RoomHandler()
{
	RegWorldEvent(ID_NetCreateRoomReq, &RoomHandler::onNetChangeRoomMasterReq, this);
	RegWorldEvent(ID_NetEnterRoomReq, &RoomHandler::onNetEnterRoomReq, this);
	RegWorldEvent(ID_NetLeaveRoomReq, &RoomHandler::onNetLeaveRoomReq, this);
	RegWorldEvent(ID_NetChangeRoomMasterReq, &RoomHandler::onNetChangeRoomMasterReq, this);
	RegWorldEvent(ID_NetRoomListReq, &RoomHandler::onNetRoomListReq, this);
	RegWorldEvent(ID_NetRoomReadyReq, &RoomHandler::onNetRoomReadyReq, this);
	RegWorldEvent(ID_NetRoomStartGameReq, &RoomHandler::onNetRoomStartGameReq, this);
}

int32 RoomHandler::onNetCreateRoomReq(Player* aPlr, NetCreateRoomReq* req)
{
	sRoom.DoCreateRoom(aPlr);
	return 0;
}

int32 RoomHandler::onNetEnterRoomReq(Player* aPlr, NetEnterRoomReq* req)
{
	sRoom.DoEnterRoom(aPlr, req->roomId);
	return 0;
}

int32 RoomHandler::onNetLeaveRoomReq(Player* aPlr, NetLeaveRoomReq* req)
{
	sRoom.DoLeaveRoom(aPlr, req->roomId);
	
	return 0;
}

int32 RoomHandler::onNetChangeRoomMasterReq(Player* aPlr, NetChangeRoomMasterReq* req)
{
	sRoom.DoChangeRoomMaster(aPlr, req->roomId, req->userId);
	return 0;
}

int32 RoomHandler::onNetRoomListReq(Player* aPlr, NetRoomListReq* req)
{
	sRoom.DoRoomList(aPlr, req->start, req->count);
	return 0;
}

int32 RoomHandler::onNetRoomReadyReq(Player* aPlr, NetRoomReadyReq* req)
{
	sRoom.DoRoomReady(aPlr, req->isReady);
	return 0;
}

int32 RoomHandler::onNetRoomStartGameReq(Player* aPlr, NetRoomStartGameReq* req)
{
	sRoom.DoRoomStartGame(aPlr);
	return 0;
}