#include "stdafx.h"

ShopHandler::ShopHandler()
{
	RegWorldEvent(ID_NetSellProductReq, &ShopHandler::onNetSellProductReq, this);
}

ShopHandler::~ShopHandler()
{

}

int32 ShopHandler::onNetSellProductReq(Player* aPlr, NetSellProductReq* req)
{
	sShop.DoAddProduct(aPlr, req->productId);
	return 0;
}

