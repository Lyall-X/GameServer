#pragma once

class ShopHandler : public Object
{
public:
	ShopHandler();
	~ShopHandler();
protected:
	int32 onNetSellProductReq(Player* aPlr, NetSellProductReq* req);
private:
};