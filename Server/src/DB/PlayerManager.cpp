#include "stdafx.h"

PlayerManager::PlayerManager()
{

}

PlayerManager::~PlayerManager()
{

}

Player* PlayerManager::AddPlrByAccId(Player* aPlr)
{
	auto itr = mMapAccIdPlayer.find(aPlr->getAccId());
	if (itr != mMapAccIdPlayer.end()) return NULL;
	mMapAccIdPlayer[aPlr->getAccId()] = aPlr;
	return aPlr;
}

Player* PlayerManager::AddPlrByUserId(Player* aPlr)
{
	auto itr = mMapUserIdPlayer.find(aPlr->getUserId());
	if (itr != mMapUserIdPlayer.end()) return NULL;
	mMapUserIdPlayer[aPlr->getUserId()] = aPlr;
	return aPlr;
}

Player* PlayerManager::FindPlrByAccId(uint32 accId)
{
	auto itr = mMapAccIdPlayer.find(accId);
	if (itr != mMapAccIdPlayer.end()) return itr->second;
	return NULL;
}

Player* PlayerManager::FindPlrByUserId(uint32 userId)
{
	auto itr = mMapUserIdPlayer.find(userId);
	if (itr != mMapUserIdPlayer.end()) return itr->second;
	return NULL;
}

void PlayerManager::DelPlrByAccId(uint32 accId)
{
	auto itr = mMapAccIdPlayer.find(accId);
	if (itr != mMapAccIdPlayer.end()) {
		delete itr->second;
		mMapAccIdPlayer.erase(itr);
	}
}

void PlayerManager::DelPlrByUserId(uint32 userId)
{
	auto itr = mMapUserIdPlayer.find(userId);
	if (itr != mMapUserIdPlayer.end()) {
		delete itr->second;
		mMapUserIdPlayer.erase(itr);
	}
}

