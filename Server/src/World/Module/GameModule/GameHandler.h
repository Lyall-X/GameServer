#pragma once

class GameHandler : public Object
{
public:
	GameHandler();
protected:
	int32 onNetGameOperateSeeReq(Player* aPlr, NetGameOperateSeeReq* req);
	int32 onNetGameOperateGiveupReq(Player* aPlr, NetGameOperateGiveupReq* req);
	int32 onNetGameOperateChipinReq(Player* aPlr, NetGameOperateChipinReq* req);
	int32 onNetGameOperateCallReq(Player* aPlr, NetGameOperateCallReq* req);
	int32 onNetGameOperateCompareReq(Player* aPlr, NetGameOperateCompareReq* req);
private:
};

