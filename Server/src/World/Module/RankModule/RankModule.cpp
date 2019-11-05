#include "stdafx.h"

IMPLEMENT_CLASS(RankModule, Module)

RankModule::RankModule()
{

}

RankModule::~RankModule()
{

}

bool RankModule::Initialize()
{
	return true;
}

bool RankModule::Update(float time, float delay)
{
	return true;
}

bool RankModule::Destroy()
{
	return true;
}

bool RankModule::onEnterWorld(Player* player, Dictionary& dict)
{
	return true;
}

bool RankModule::onLeaveWorld(Player* player, Dictionary& dict)
{
	return true;

}

bool RankModule::onLoad(Player* player, Dictionary& bytes)
{
	return true;
}

bool RankModule::onSave(Player* player, Dictionary& bytes)
{
	return true;
}

bool RankModule::onLoadEnd(Player* player, Dictionary& bytes)
{
	return true;
}

bool RankModule::onSaveEnd(Player* player, Dictionary& bytes)
{
	return true;
}

bool RankModule::RankAdd(uint8 rankType, uint32 userId, uint64 value)
{
	char szBuffer[256] = {};
	switch (rankType)
	{
	case RT_Gold:
		sprintf_s(szBuffer, 256, "zadd %s %llu %d", sGoldRank, value, userId);
		break;
	case RT_Diamond:
		sprintf_s(szBuffer, 256, "zadd %s %llu %d", sDiamondRank, value, userId);
		break;
	}
	sRedisProxy.sendCmd(szBuffer, NULL, NULL);
	return true;
}

bool RankModule::RankFind(uint8 rankType, uint32 start, uint32 count)
{
	char szBuffer[256] = {};
	switch (rankType)
	{
	case RT_Gold:
		sprintf_s(szBuffer, 256, "ZREVRANGE %s %d %d WITHSCORES", sGoldRank, start, start + count);
		break;
	case RT_Diamond:
		sprintf_s(szBuffer, 256, "ZREVRANGE %s %d %d WITHSCORES", sDiamondRank, start, start + count);
		break;
	}
	sRedisProxy.sendCmd(szBuffer, (EventCallback)&RankModule::onRankFind, this);
	return true;
}

int32 RankModule::onRankFind(RedisEvent& e)
{
	return 0;
}
