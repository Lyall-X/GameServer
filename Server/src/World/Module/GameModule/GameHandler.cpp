#include "stdafx.h"

GameHandler::GameHandler()
{
	RegWorldEvent(ID_NetGameOperateSeeReq, &GameHandler::onNetGameOperateSeeReq, this);
	RegWorldEvent(ID_NetGameOperateChipinReq, &GameHandler::onNetGameOperateChipinReq, this);
	RegWorldEvent(ID_NetGameOperateCallReq, &GameHandler::onNetGameOperateCallReq, this);
	RegWorldEvent(ID_NetGameOperateCompareReq, &GameHandler::onNetGameOperateCompareReq, this);
}

int32 GameHandler::onNetGameOperateSeeReq(Player* aPlr, NetGameOperateSeeReq* req)
{
	sGame.DoOperateSee(aPlr);
	return 0;
}

int32 GameHandler::onNetGameOperateGiveupReq(Player* aPlr, NetGameOperateGiveupReq* req)
{
	sGame.DoOperateGiveup(aPlr);
	return 0;
}

int32 GameHandler::onNetGameOperateChipinReq(Player* aPlr, NetGameOperateChipinReq* req)
{
	sGame.DoOperateChipinReq(aPlr, req->gold);
	return 0;
}

int32 GameHandler::onNetGameOperateCallReq(Player* aPlr, NetGameOperateCallReq* req)
{
	sGame.DoOperateCallReq(aPlr);
	return 0;
}

int32 GameHandler::onNetGameOperateCompareReq(Player* aPlr, NetGameOperateCompareReq* req)
{
	sGame.DoOperateCompareReq(aPlr, req->tarUserId);
	return 0;
}

