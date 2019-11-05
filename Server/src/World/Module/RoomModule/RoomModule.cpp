#include "stdafx.h"

IMPLEMENT_CLASS(RoomModule, Module)
RoomModule::RoomModule()
{

}

RoomModule::~RoomModule()
{
	Destroy();
}

void RoomModule::sendPacketToRoom(Packet& packet, Player* player)
{
	Room* aRoom = FindPlayerRoom(player->getUserId());
	if (aRoom == NULL) return;
	aRoom->sendPacketToAll(packet);
}

Room* RoomModule::Create(Player* aPlr)
{
	if (FindPlayerRoom(aPlr->getUserId())) return NULL;
	Room* aRoom = new Room();
	AddRoom(aRoom);
	aRoom->OnCreate(aPlr->getUserId());
	return aRoom;
}

void RoomModule::Remove(uint32 roomId)
{
	Room* aRoom = FindRoom(roomId);
	if (aRoom == NULL) return;
	Remove(aRoom);
}

void RoomModule::Remove(Room* aRoom)
{
	for (uint32 i = 0; i < aRoom->GetRoomPlayerCount(); ++i)
	{
		RoomPlayer* aRoomPlayer = aRoom->GetRoomPlayer(i);
		if (aRoomPlayer == NULL) continue;

		Player* aPlr = aRoomPlayer->mPlayer;
		if (aPlr == NULL) continue;

		NetLeaveRoomRes res;
		res.roomId = aRoom->GetInsId();
		res.userId = aPlr->getUserId();
		aRoom->sendPacketToAll(res);
		aPlr->sendPacket(res);
	}

	RemoveRoom(aRoom->GetInsId());
}

bool RoomModule::EnterRoom(uint32 roomId, Player* aPlr, bool isMaster /* = false */)
{
	Room* aRoom = FindRoom(roomId);
	if (aRoom == NULL) return false;
	return EnterRoom(aRoom, aPlr, isMaster);
}

bool RoomModule::EnterRoom(Room* aRoom, Player* aPlr, bool isMaster /* = false */)
{
	if (FindPlayerRoom(aPlr->getUserId()))
		return false;
	if (aRoom->FindPlayer(aPlr->getUserId()))
		return false;
	if (!aRoom->IsCanAdd(aPlr))
	{
		return false;
	}
	RoomPlayer* curPlayer = aRoom->DoEnter(aPlr, isMaster);
	if (curPlayer == NULL)
		return false;

	AddPlayerRoom(aPlr->getUserId(), aRoom);
	NetEnterRoomRes res;
	(*aRoom) >> res.roomInfo;
	aPlr->sendPacket(res);

	NetEnterRoomNotify nfy;
	*curPlayer >> nfy.roomPlayerInfo;
	for (uint32 i = 0; i < aRoom->GetRoomPlayerCount(); ++i)
	{
		RoomPlayer* roomPlayer = aRoom->GetRoomPlayer(i);
		if (roomPlayer == NULL) continue;

		if (roomPlayer == curPlayer) continue;

		Player* aSndPlr = sWorld.FindPlrByUserId(roomPlayer->mUserId);
		if (aSndPlr == NULL) continue;
		aSndPlr->sendPacket(nfy);
	}

	aRoom->OnEnter(aPlr->getUserId());
	return true;
}

bool RoomModule::LeaveRoom(uint32 roomId, uint32 userId)
{
	Room* aRoom = FindPlayerRoom(userId);
	if (aRoom == NULL) return false;

	if (aRoom->GetInsId() != roomId) return false;
	LeaveRoom(aRoom, userId);

	return true;
}

bool RoomModule::LeaveRoom(Room* aRoom, uint32 userId)
{
	if (aRoom->FindPlayer(userId) == NULL)
	{
		// 不在房间
		return false;
	}

	NetLeaveRoomRes res;
	res.roomId = aRoom->GetInsId();
	res.userId = userId;
	aRoom->sendPacketToAll(res);

	aRoom->OnLeave(userId);

	aRoom->DoLeave(userId);
	RemovePlayerRoom(userId);
	if (aRoom->GetRoomPlayerCount() <= 0) {
		aRoom->OnClose();
		RemoveRoom(aRoom->GetInsId());
	}
	return true;
}

bool RoomModule::ChangeMaster(Room* aRoom, uint32 oldUserId, uint32 newUserId)
{
	RoomPlayer* oldRoomPlayer = aRoom->GetRoomPlayer(oldUserId);
	RoomPlayer* newRoomPlayer = aRoom->GetRoomPlayer(newUserId);
	if (oldRoomPlayer == NULL)
	{
		return false;
	}

	if (newRoomPlayer == NULL)
	{
		return false;
	}

	aRoom->SetMaster(newRoomPlayer);
	NetChangeRoomMasterRes res;
	res.roomId = aRoom->GetInsId();
	res.masterUserId = newRoomPlayer->mUserId;
	aRoom->sendPacketToAll(res);
	aRoom->OnChangeMaster(oldUserId, newUserId);
	return true;
}

Room* RoomModule::FindPlayerRoom(uint32 userId)
{
	auto itr = mMapPlayerRoom.find(userId);
	if (itr != mMapPlayerRoom.end())
		return itr->second;
	return NULL;
}

Room* RoomModule::AddPlayerRoom(uint32 userId, Room* aRoom)
{
	if (FindPlayerRoom(userId)) return NULL;
	mMapPlayerRoom[userId] = aRoom;
	return aRoom;
}

void RoomModule::RemovePlayerRoom(uint32 userId)
{
	auto itr = mMapPlayerRoom.find(userId);
	if (itr != mMapPlayerRoom.end())
		mMapPlayerRoom.erase(itr);
}

Room* RoomModule::FindRoom(uint32 roomId)
{
	auto itr = mMapRoom.find(roomId);
	if (itr != mMapRoom.end())
		return itr->second;
	return NULL;
}

Room* RoomModule::AddRoom(Room* aRoom)
{
	if (FindRoom(aRoom->GetInsId())) return NULL;
	mMapRoom[aRoom->GetInsId()] = aRoom;
	return aRoom;
}

void RoomModule::RemoveRoom(uint32 roomId)
{
	auto itr = mMapRoom.find(roomId);
	if (itr != mMapRoom.end()) {
		mSetDelRoom.insert(itr->second);
		mMapRoom.erase(itr);
	}
}

void RoomModule::DoCreateRoom(Player* aPlr)
{
	Room* aRoom = FindPlayerRoom(aPlr->getUserId());
	if (aRoom)
	{
		//
		return;
	}

	aRoom = Create(aPlr);
	EnterRoom(aRoom, aPlr, true);
}

void RoomModule::DoEnterRoom(Player* aPlr, uint32 roomId)
{
	if (FindPlayerRoom(aPlr->getUserId()))
	{
		// 已经有房间
		return;
	}

	Room* aRoom = FindRoom(roomId);
	if (aRoom == NULL)
	{
		// 房间不存在
		return;
	}

	EnterRoom(aRoom, aPlr);
}

void RoomModule::DoLeaveRoom(Player* aPlr, uint32 roomId)
{
	Room* aRoom = FindRoom(roomId);
	if (aRoom == NULL)
	{
		// 房间不存在
		return;
	}

	LeaveRoom(aRoom, aPlr->getUserId());
}

void RoomModule::DoLeaveRoom(uint32 userId, uint32 roomId)
{
	LeaveRoom(roomId, userId);
}

void RoomModule::DoChangeRoomMaster(Player* aPlr, uint32 roomId, uint32 newUserId)
{
	Room* aRoom = FindRoom(roomId);
	if (aRoom == NULL)
	{
		// 房间不存在
		return ;
	}
	ChangeMaster(aRoom, aPlr->getUserId(), newUserId);
}

void RoomModule::DoRoomList(Player* aPlr, uint32 start, uint32 count)
{
	NetRoomListRes res;

	std::udmap<uint32, Room*>& mapRooms = GetMapRoom();
	uint32 idx = 0;
	for (auto& itr : mapRooms)
	{
		if (idx >= start)
		{
			Room* aRoom = itr.second;
			if (aRoom == NULL) {
				idx++;
				continue;
			}

			RoomInfo info;
			*aRoom >> info;
			res.roomInfoInfos.push_back(info);
			if ((uint32)res.roomInfoInfos.size() >= count)
				break;
		}
		idx++;
	}

	aPlr->sendPacket(res);
}

void RoomModule::DoRoomReady(Player* aPlr, uint8 isReady)
{
	NetRoomReadyRes res;
	Room* aRoom = FindPlayerRoom(aPlr->getUserId());
	if (aRoom == NULL) {
		goto fial;
	}

	RoomPlayer* aRoomPlayer = aRoom->FindPlayer(aPlr->getUserId());
	if (aRoomPlayer == NULL) {
		goto fial;
	}

	if (aRoom->GetMaster() == aRoomPlayer) {
		goto fial;
	}
	if (aRoomPlayer->GetState() == RPS_Game) {
		goto fial;
	}
	uint8 oldState = aRoomPlayer->GetState();
	isReady == 0 ? aRoomPlayer->SetState(RPS_None) : aRoomPlayer->SetState(RPS_Ready);

	aRoom->OnChangeState(aPlr->getUserId(), oldState, aRoomPlayer->GetState());

	res.userId = aPlr->getUserId();
	res.result = NResultSuccess;
	res.isReady = aRoomPlayer->GetState();
	aRoom->sendPacketToAll(res);
	return;
fial:
	res.result = NResultFail;
	aPlr->sendPacket(res);
}

void RoomModule::DoRoomStartGame(Player* aPlr)
{
	NetRoomStartGameRes res;
	Room* aRoom = FindPlayerRoom(aPlr->getUserId());
	if (aRoom == NULL) {
		goto fial;
	}

	if (!aRoom->IsCanStart())
	{
		goto fial;
	}

	RoomPlayer* aRoomPlayer = aRoom->FindPlayer(aPlr->getUserId());
	if (aRoomPlayer == NULL)
	{
		goto fial;
	}
	if (aRoomPlayer != aRoom->GetMaster())
	{
		goto fial;
	}
	aRoomPlayer->SetState(RPS_Ready);
	res.result = NResultSuccess;
	aRoom->sendPacketToAll(res);

	sGame.DoStartGame(aRoom);
fial:
	res.result = NResultFail;
	aPlr->sendPacket(res);
}

void RoomModule::DoRoomClose(Player* aPlr)
{
	Room* aRoom = FindPlayerRoom(aPlr->getUserId());
	if (aRoom == NULL)
	{
		LOG_ERROR("%s 没有房间", aPlr->getName());
		return;
	}
	uint32 roomInsId = aRoom->GetInsId();
	sGame.DoCloseGame(aRoom->GetInsId());
	std::queue<uint32> delQueue;
	for (uint32 i = 0; i < aRoom->GetRoomPlayerCount(); ++i)
	{
		RoomPlayer* aRoomPlayer = aRoom->GetRoomPlayer(i);
		if (aRoomPlayer == NULL) continue;
		delQueue.push(aRoomPlayer->mUserId);
	}
	while (delQueue.size())
	{
		uint32 userId = delQueue.front();
		delQueue.pop();
		DoLeaveRoom(userId, roomInsId);
	}

	RemoveRoom(roomInsId);
}

void RoomModule::DoAutoMatch(Player* aPlr)
{
	RoomMatch* aMatch = FindRoomMatch(aPlr->getUserId());
	if (aMatch)
	{
		// 已经在匹配
		return;
	}
	RoomMatch rMatch;
	rMatch.userId = aPlr->getUserId();
	rMatch.stateTime = DateTime::Now();
	AddRoomMatch(rMatch);
}

bool RoomModule::Initialize()
{
	return true;
}

bool RoomModule::Update(float time, float delay)
{
	MatchUpdate();
	FreeRooms();
	return true;
}

bool RoomModule::Destroy()
{
	ClearRoom();
	return true;
}

bool RoomModule::onEnterWorld(Player* player, Dictionary& dict)
{
	return true;
}

bool RoomModule::onLeaveWorld(Player* player, Dictionary& dict)
{
	return true;
}

void RoomModule::ClearRoom()
{
	for (auto& itr : mMapRoom)
		delete itr.second;
	mMapRoom.clear();
	FreeRooms();
}

RoomMatch* RoomModule::FindRoomMatch(uint32 userId)
{
	for (RoomMatch& rMatch : mLstAutoMatch)
	{
		if (rMatch.userId == userId)
		{
			return &rMatch;
		}
	}
	return NULL;
}

RoomMatch* RoomModule::AddRoomMatch(RoomMatch& rMatch)
{
	mLstAutoMatch.push_back(rMatch);
	return FindRoomMatch(rMatch.userId);
}

void RoomModule::DelRoomMatch(uint32 userId)
{
	for (auto itr = mLstAutoMatch.begin(); 
		 itr != mLstAutoMatch.end();
		 ++itr)
	{
		if (userId == (*itr).userId)
		{
			mLstAutoMatch.erase(itr);
			break;
		}
	}
}

void RoomModule::MatchUpdate()
{
	if (mLstAutoMatch.size() == 0) return;

	RoomMatch rMatch = mLstAutoMatch.front();
	mLstAutoMatch.pop_front();
	Player* aPlr = sWorld.FindPlrByUserId(rMatch.userId);
	if (aPlr == NULL) return;

	if (GetRoomCount() == 0)
	{
		DoCreateRoom(aPlr);
	}
	else
	{
		for (auto& itr : mMapRoom)
		{
			Room* aRoom = itr.second;
			if (aRoom == NULL) continue;

			if (!aRoom->IsCanAdd(aPlr))
				continue;
			DoEnterRoom(aPlr, aRoom->GetInsId());
			DoRoomReady(aPlr, RPS_Ready);
			break;
		}
	}
}

void RoomModule::FreeRooms()
{
	for (auto itr : mSetDelRoom) {
		delete itr;
	}
	mSetDelRoom.clear();
}

