#include "stdafx.h"

Room::Room()
{
	static uint32 sRoomId = 0;
	mId = ++sRoomId;
	mMaxCount = 8;
	mMaster = NULL;
}

Room::~Room()
{

}

void Room::sendPacketToAll(Packet& packet)
{
	for (auto & itr : mRoomPlayers)
	{
		RoomPlayer* rRoomPlr = itr.second;
		if (rRoomPlr->mPlayer == NULL) continue;
		rRoomPlr->mPlayer->sendPacket(packet);
	}
}

bool Room::operator >> (RoomInfo& info)
{
	info.name = GetName();
	info.roomId = GetInsId();
	info.maxCount = GetMaxCount();

	for (auto & itr : mRoomPlayers)
	{
		RoomPlayer* rRoomPlr = itr.second;
		RoomPlayerInfo aPlrInfo;
		(*rRoomPlr) >> aPlrInfo;
		info.roomPlayerInfos.push_back(aPlrInfo);
	}

	return true;
}

RoomPlayer* Room::DoEnter(Player* aPlr, bool isMaster /* = false */)
{
	if (IsFull())
	{
		return NULL;
	}

	if (FindPlayer(aPlr->getUserId()))
	{
		return NULL;
	}

	RoomPlayer* roomPlr = new RoomPlayer;
	roomPlr->mUserId = aPlr->getUserId();
	roomPlr->mName = aPlr->getName();
	roomPlr->mPlayer = aPlr;
	roomPlr->SetPos(GetEnterPos());
	if (isMaster)
		SetMaster(roomPlr);

	return AddPlayer(roomPlr);
}

bool Room::DoLeave(Player* aPlr)
{
	RoomPlayer* roomPlr = FindPlayer(aPlr->getUserId());
	if (roomPlr == NULL) return false;

	RemovePlayer(aPlr->getUserId());
	return true;
}

bool Room::DoLeave(uint32 userId)
{
	RemovePlayer(userId);
	return true;
}

RoomPlayer* Room::FindPlayer(uint32 userId)
{
	for (auto & itr : mRoomPlayers)
	{
		RoomPlayer* rRoomPlr = itr.second;
		if (rRoomPlr && rRoomPlr->mUserId == userId)
		{
			return rRoomPlr;
		}
	}
	return NULL;
}

void Room::RemovePlayer(uint32 userId)
{
	for (auto & itr : mRoomPlayers)
	{
		RoomPlayer* rRoomPlr = itr.second;
		if (rRoomPlr && rRoomPlr->mUserId == userId)
		{
			mRoomPlayers.erase(rRoomPlr->GetPos());
			break;
		}
	}
}

RoomPlayer* Room::AddPlayer(RoomPlayer* roomPlr)
{
	if (GetRoomPlayer(roomPlr->GetPos()))
		return NULL;
	mRoomPlayers[roomPlr->GetPos()] = roomPlr;
	return roomPlr;
}

RoomPlayer* Room::GetRoomPlayer(uint32 idx)
{
	auto itr = mRoomPlayers.find(idx);
	if (itr != mRoomPlayers.end())
		return itr->second;
	
	return NULL;
}

bool Room::IsFull()
{
	if (GetRoomPlayerCount() >= mMaxCount)
		return true;
	return false;
}

bool Room::IsCanStart()
{
	uint32 canStart = 0;
	for (auto & itr : mRoomPlayers)
	{
		RoomPlayer* aRoomPlayer = itr.second;
		if (aRoomPlayer->GetState() == RPS_Ready) {
			canStart++;
			continue;
		}
		if (aRoomPlayer == GetMaster() && aRoomPlayer->GetState() == RPS_None)
		{
			canStart++;
			continue;
		}
		if (aRoomPlayer->GetState() == RPS_None && aRoomPlayer != GetMaster()) {
			return false;
		}
	}
	// 一个人不能开始
	if (canStart < 2)
		return false;
	return true;
}

bool Room::DoAllStart()
{
	for (auto & itr : mRoomPlayers)
	{
		RoomPlayer* aRoomPlayer = itr.second;
		if (aRoomPlayer->GetState() == RPS_Ready) {
			uint8 lastState = aRoomPlayer->GetState();
			aRoomPlayer->SetState(RPS_Game);
			OnChangeState(aRoomPlayer->mUserId, lastState, aRoomPlayer->GetState());
		}
	}
	return true;
}

bool Room::IsCanAdd(Player* aPlr)
{
	if (IsFull())
		return false;
	return true;
}

uint8 Room::GetEnterPos()
{
	for (uint8 i = 0 ; i < GetMaxCount(); ++i)
	{
		if (GetRoomPlayer(i) == NULL)
		{
			return i;
		}
	}
	return -1;
}

void Room::OnCreate(uint32 userId)
{
	LuaEngine::Call(sScriptRoom, "OnCreate", GetInsId(), userId);
}

void Room::OnClose()
{
	LuaEngine::Call(sScriptRoom, "OnClose", GetInsId());
}

void Room::OnEnter(uint32 userId)
{
	LuaEngine::Call(sScriptRoom, "OnEnter", GetInsId(), userId);
}

void Room::OnLeave(uint32 userId)
{
	LuaEngine::Call(sScriptRoom, "OnLeave", GetInsId(), userId);
}

void Room::OnChangeMaster(uint32 oldUserId, uint32 newUserId)
{
	LuaEngine::Call(sScriptRoom, "OnChangeMaster", GetInsId(), oldUserId, newUserId);
}

void Room::OnChangeState(uint32 userId, uint8 oldState, uint8 state)
{
	LuaEngine::Call(sScriptRoom, "OnChangeState", GetInsId(), userId, oldState, state);
}

bool RoomPlayer::operator<<(RoomPlayerInfo& info)
{
	return true;
}

bool RoomPlayer::operator >> (RoomPlayerInfo& info)
{
	info.insId = mInsId;
	info.userId = mUserId;
	info.name = mName;
	info.state = mState;
	info.pos = GetPos();
	return true;
}
