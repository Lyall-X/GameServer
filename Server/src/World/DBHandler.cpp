#include "stdafx.h"

DBHandler::DBHandler()
{

}

DBHandler::~DBHandler()
{

}

void DBHandler::doRegister()
{
	RegDBEvent(ID_NetProductListRes, &DBHandler::onNetProductListRes, this);
	RegDBEvent(ID_NetLoginRes, &DBHandler::onNetNetLoginRes, this);
	RegDBEvent(ID_NetCreateRoleRes, &DBHandler::onNetCreateRoleRes, this);
	RegDBEvent(ID_NetSelectRoleRes, &DBHandler::onNetSelectRoleRes, this);
	RegDBEvent(ID_NetQueryRoleRes, &DBHandler::onNetQueryRoleRes, this);
	RegDBEvent(ID_NetSellProductRes, &DBHandler::onNetSellProductRes, this);
}

int32 DBHandler::onNetProductListRes(Socket* sck, NetProductListRes* res)
{
	for (int32 i = 0 ; i < res->productInfos.size(); ++i)
	{
		ProductInfo& info = res->productInfos[i];

		Product* product = new Product;
		product->mInsId = info.productInsId;
		product->mProductId = info.productId;
		product->mUserId = info.sellUserId;
		product->mBuyUserId = info.buyUserId;
		product->mShelvesTime = info.shelvesTime;
		product->mUnshelvesTime = info.unShelvesTime;
		if (sShop.AddProduct(product) == NULL) {
			delete product;
			product = NULL;
		}
	}

	return 1;
}

int32 DBHandler::onNetNetLoginRes(Session* ssn, NetLoginRes* res)
{
	uint32 accId = res->accInfo.id;
	if (res->result != NResultSuccess)
	{
		ssn->sendPacketToWorld(*res);
		return 0;
	}

	Player* player = sWorld.FindPlrByAccId(accId);
	do 
	{
		if (player) {
			Session* oldSession = player->getSession();
			if (oldSession)
			{
				player->sendRespnoseMsg(MC_LoginTheNumberInTheRemote);
				Dictionary dict;
				sApp.onLeaveWorld(player, dict);

				NetLoginRes res;
				res.result = 3;
				oldSession->sendPacketToWorld(res);
				player->unbindSession();

				sApp.doSessionLeaveWorld(oldSession);
			}
			player->SetOnline(true);
			player->bindSession(ssn);
			break;
		}

		player = new Player;
		player->SetOnline(true);
		player->bindSession(ssn);
		player->setAccId(res->accInfo.id);
		player->setUser(res->accInfo.user);
		sWorld.addPlayerByAccId(player);
	} while (false);
	player->setHost(ssn->getHost());
	res->guid = player->getGuid();
	player->sendPacket(*res);
	return 0;
}

int32 DBHandler::onNetCreateRoleRes(Player* aPlr, NetCreateRoleRes* res)
{
	//////////////////////////////////////////////////////////////////////////
	/// 角色创建成功 ///
	aPlr->sendPacket(*res);
	return 0;
}

int32 DBHandler::onNetSelectRoleRes(Player* aPlr, NetSelectRoleRes* res)
{
	if (res->result == NResultFail) {
		aPlr->sendPacket(*res);
		return 0;
	}

	aPlr->sendPacket(*res);

	DBRoleInfo& info = res->roleInfo;
	aPlr->setUserId(info.id);
	aPlr->setName(info.name);
	aPlr->SetOnlineTimer(DateTime::Now());
	aPlr->SetOnlineTime(info.onlinetotaltime);
	sWorld.addPlayerToUserId(aPlr);
	sWorld.addPlayerByName(aPlr);

	Dictionary dict;
	if (info.datas.wpos() <= 0) // 没有角色创建角色
		aPlr->DoCreateCharacter(dict, info);
	else
		info.datas >> dict;
	dict.Add("datastr", info.datastr);
	sApp.onLoad(aPlr, dict);
	sApp.onEnterWorld(aPlr, dict);
	return 0;
}

int32 DBHandler::onNetQueryRoleRes(Player* aPlr, NetQueryRoleRes* res)
{
	aPlr->sendPacket(*res);
	return 0;
}

int32 DBHandler::onNetSellProductRes(Player* aPlr, NetSellProductRes* res)
{
	if (res->result == NResultFail)
	{
		aPlr->sendPacket(*res);
		return 0;
	}
	Product* product = new Product;
	product->mInsId = res->productInfo.productInsId;
	product->mProductId = res->productInfo.productId;
	product->mUserId = res->productInfo.sellUserId;
	product->mBuyUserId = res->productInfo.buyUserId;
	product->mShelvesTime = res->productInfo.shelvesTime;
	product->mUnshelvesTime = res->productInfo.unShelvesTime;
	if (sShop.AddProduct(product) == NULL) {
		delete product;
		product = NULL;
	}
	sShop.OnSellProduct(aPlr, product);
	aPlr->sendPacket(*res);
	return 0;
}
