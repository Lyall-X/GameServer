#include "stdafx.h"

uint32 GameComponent::sId = 0;

GameComponent::GameComponent():
mInsId(++sId),
mRoomId(0),
mRoom(NULL)
{
}

GameComponent::~GameComponent()
{
	for (auto& itr : mLstGameEntity)
	{
		delete itr.second;
	}
	mLstGameEntity.clear();
}

bool GameComponent::Update(float time, float delay)
{
	mTimer.update(time, delay);
	return true;
}

GameEntity* GameComponent::AddGameEnt(GameEntity* aGameEnt)
{
	RoomPlayer* aRoomPlr = mRoom->FindPlayer(aGameEnt->GetUserId());
	if (aRoomPlr == NULL) return NULL;
	auto itr = mLstGameEntity.find(aRoomPlr->GetPos());
	if (itr != mLstGameEntity.end())
		return NULL;
	mLstGameEntity[aRoomPlr->GetPos()] = aGameEnt;
	return aGameEnt;
}

GameEntity* GameComponent::GetGameEnt(uint32 idx)
{
	auto itr = mLstGameEntity.find(idx);
	if (itr != mLstGameEntity.end())
		return NULL;
	return NULL;
}

GameEntity* GameComponent::FindGameEnt(uint32 userId)
{
	for (auto& itr : mLstGameEntity)
	{
		GameEntity* aGameEnt = itr.second;
		if (aGameEnt->userId == userId)
		{
			return aGameEnt;
		}
	}
	return NULL;
}

void GameComponent::DelGameEnt(uint32 userId)
{
	for (auto& itr : mLstGameEntity)
	{
		GameEntity* aGameEnt = itr.second;
		if (aGameEnt->userId == userId)
		{
			delete aGameEnt;
			mLstGameEntity.erase(itr.first);
			return;
		}
	}
}

uint32 GameComponent::GetPlrCard(uint32 userId, uint32 idx)
{
	GameEntity* aGameEnt = FindGameEnt(userId);
	if (aGameEnt == NULL) return 0;
	return aGameEnt->GetCard(idx);
}

std::string GameComponent::ToString()
{
	std::string str;
	char szBuffer[256] = { 0 };
	for (uint32 i = 0; i < mLstGameEntity.size(); ++i)
	{
		str += mLstGameEntity[i]->ToString();
		str += "\n";
	}
	return str;
}

int32 GameComponent::GetPlrInx(uint32 userId)
{
	RoomPlayer* aRoomPlr = mRoom->FindPlayer(userId);
	if (aRoomPlr)
		return aRoomPlr->GetPos();
	return -1;
}