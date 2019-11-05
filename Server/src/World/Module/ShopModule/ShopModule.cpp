#include "stdafx.h"

IMPLEMENT_CLASS(ShopModule, Module);
ShopModule::ShopModule()
{

}

ShopModule::~ShopModule()
{

}

Product* ShopModule::AddProduct(Product* product)
{
	if (FindProduct(product->mInsId)) return NULL;
	mMapProduct[product->mInsId] = product;
	return product;
}

Product* ShopModule::FindProduct(uint32 insId)
{
	auto itr = mMapProduct.find(insId);
	if (itr != mMapProduct.end())
		return itr->second;
	return NULL;
}

void ShopModule::DelProduct(uint32 insId)
{
	auto itr = mMapProduct.find(insId);
	if (itr != mMapProduct.end()) {
		delete itr->second;
		mMapProduct.erase(itr);
	}
}

void ShopModule::DoAddProduct(Player* aPlr, uint32 productId)
{
	NetSellProductReq req;
	req.productId = productId;
	Session* ssn = aPlr->getSession();
	if (ssn) {
		ssn->sendPacketToDB(req);
	}
}

void ShopModule::DoFindProductList(Player* aPlr, uint32 idx, uint32 count)
{
	NetProductListRes res;
	uint32 index = 0;
	for (auto& itr : mMapProduct)
	{
		if (idx == index && count > 0) {
			Product* product = itr.second;
			ProductInfo info;
			info.productInsId = product->mInsId;
			info.productId = product->mProductId;
			info.sellUserId = product->mUserId;
			info.buyUserId = product->mBuyUserId;
			info.shelvesTime = product->mShelvesTime;
			info.unShelvesTime = product->mUnshelvesTime;
			res.productInfos.push_back(info);
			count--;				
		}
		index++;
	}
	aPlr->sendPacket(res);
}

void ShopModule::OnSellProduct(Player* aPlr, Product* product)
{
	LuaEngine::Call("shop", "OnSellProduct", aPlr->getUserId(), product->mInsId);
}

bool ShopModule::Initialize()
{
	return true;
}

bool ShopModule::Update(float time, float delay)
{
	return true;
}

bool ShopModule::Destroy()
{
	return true;
}

bool ShopModule::onEnterWorld(Player* player, Dictionary& dict)
{
	return true;
}

bool ShopModule::onLeaveWorld(Player* player, Dictionary& dict)
{
	return true;

}

bool ShopModule::onLoad(Player* player, Dictionary& bytes)
{
	return true;
}

bool ShopModule::onSave(Player* player, Dictionary& bytes)
{
	return true;
}

