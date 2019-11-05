#include "stdafx.h"

#define REGISTER_EVENT(msg, cb, obj) INSTANCE(Application).getDBServer()->addEventListener(msg, (EventCallbackProcess)cb, obj)
SessionHandler::SessionHandler()
{
	REGISTER_EVENT(ID_NetProductListReq, &SessionHandler::onNetProductListReq, this);
	REGISTER_EVENT(ID_NetSessionEnterNotify, &SessionHandler::onNetSessionEnterNotify, this);
	REGISTER_EVENT(ID_NetSessionLeaveNotify, &SessionHandler::onNetSessionLeaveNotify, this);
	REGISTER_EVENT(ID_NetPlayerSaveNotify, &SessionHandler::onNetPlayerSaveNotify, this);

	REGISTER_EVENT(ID_NetLoginReq, &SessionHandler::onNetLoginReq, this);
	REGISTER_EVENT(ID_NetCreateRoleReq, &SessionHandler::onNetCreateRoleReq, this);
	REGISTER_EVENT(ID_NetSelectRoleReq, &SessionHandler::onNetSelectRoleReq, this);
	REGISTER_EVENT(ID_NetQueryRoleReq, &SessionHandler::onNetQueryRoleReq, this);

	REGISTER_EVENT(ID_NetSellProductReq, &SessionHandler::onNetSellProductReq, this);
}

SessionHandler::~SessionHandler()
{

}

int32 SessionHandler::onNetProductListReq(Socket* sck, NetProductListReq* req)
{
	NetProductListRes res;
	DBQueryResult* result = new DBQueryResult;
	char* err = sDBConnector.doQuery(DBProduct::getDescriptor().tableName, result);
	if (err)
	{
		LOG_ERROR(err);
		return 0;
	}
	DBProduct* dbProducts = swapQueryResult<DBProduct>(result);
	for (uint32 i = 0; i < result->length; ++i)
	{
		DBProduct& dbProduct = dbProducts[i];
		ProductInfo info;
		info.productInsId = dbProduct.id;
		info.productId = dbProduct.productId;
		info.sellUserId = dbProduct.userId;
		info.buyUserId = dbProduct.buyUserId;
		info.shelvesTime = dbProduct.shelvesTime;
		info.unShelvesTime = dbProduct.unShelvesTime;
		res.productInfos.push_back(info);
	}
	sApp.sendPacketToTarget(res, sck);

	SAFE_DELETE_ARRAY(dbProducts);
	releaseResult(result);
	result = NULL;
	return 0;
}

int32 SessionHandler::onNetSessionEnterNotify(Session* ssn, NetSessionEnterNotify* nfy)
{
	LOG_INFO("ssnId %0.16llx enter world", ssn->getSsnId());
	return 0;
}

int32 SessionHandler::onNetSessionLeaveNotify(Session* ssn, NetSessionLeaveNotify* nfy)
{
	LOG_INFO("ssnId %0.16llx leave world", ssn->getSsnId());

	if (ssn->getPlayer()) {
		ssn->getPlayer()->unbindSsn();
	}
	INSTANCE(SessionManager).removeSessionsBySocket(ssn->getSocketId(), ssn);
	return 0;
}

int32 SessionHandler::onNetLoginReq(Session* ssn, NetLoginReq* req)
{
	NetLoginRes res;
	if (DoLogin(req, res) == 0)
	{
		res.result = NResultFail;
		ssn->sendPacketToWorld(res);
		return 0;
	}
	if (DoQueryRole(req, res) == 0)
	{
		res.result = NResultFail;
		ssn->sendPacketToWorld(res);
		return 0;
	}
	res.result = NResultSuccess;

	Player* aPlr = sPlrMgr.FindPlrByAccId(res.accInfo.id);
	if (aPlr == NULL)
	{
		aPlr = new Player;
		aPlr->setAccId(res.accInfo.id);
		sPlrMgr.AddPlrByAccId(aPlr);
	}
	aPlr->bindSsn(ssn);
	ssn->sendPacketToWorld(res);
	LOG_DEBUG(LogSystem::csl_color_red_blue, "[%s] login success", req->user.c_str());
	return 0;
}

int32 SessionHandler::onNetCreateRoleReq(Player* aPlr, NetCreateRoleReq* req)
{
	NetCreateRoleRes res;
	if (DoCreateRole(req, res) == 0)
	{
		aPlr->sendPacket(res);
		return 0;
	}

	aPlr->sendPacket(res);
	LOG_DEBUG(LogSystem::csl_color_red_blue, "accId[%d] insert role[%s]", req->accId, res.roleInfo.name.c_str());
	return 1;
}

int32 SessionHandler::onNetSelectRoleReq(Player* aPlr, NetSelectRoleReq* req)
{
	NetSelectRoleRes res;
	if (DoSelectRole(req, res) == 0)
	{
		aPlr->sendPacket(res);
		return 0;
	}

	aPlr->setUserId(res.roleInfo.id);
	sPlrMgr.AddPlrByUserId(aPlr);
	aPlr->sendPacket(res);
	LOG_DEBUG(LogSystem::csl_color_red_blue, "accId[%d] userId[%d] selete role[%s]", res.roleInfo.accountId, res.roleInfo.id, res.roleInfo.name.c_str());
	return 1;
}

int32 SessionHandler::onNetQueryRoleReq(Player* aPlr, NetQueryRoleReq* req)
{
	NetQueryRoleRes res;
	DBUser role;
	role.accountId = req->accountId;
	uint32 queryCount = 0;
	const int32 maxCount = sMaxRoleCount;
	DBUser retRoles[maxCount];
	std::vector<DBRecord*> result_records;
	result_records.push_back((DBRecord*)&retRoles[0]);
	result_records.push_back((DBRecord*)&retRoles[1]);
	result_records.push_back((DBRecord*)&retRoles[2]);
	const int8* err = sDBConnector.doQuery(role, result_records, queryCount, "accountId", "", maxCount);
	do {
		if (err)
		{
			LOG_ERROR(err);
			aPlr->sendPacket(res);
			return 0;
		}

		if (queryCount > 0)
		{
			for (uint32 i = 0; i < queryCount; ++i)
			{
				DBRoleInfo info;
				info.id = role.id;
				info.accountId = role.id;
				info.name = role.name;
				DBUser& dbUser = retRoles[i];
				info.datas.write(dbUser.datas.datas(), dbUser.datas.wpos());
				info.createTime = dbUser.createTime;
				info.onlinetotaltime = dbUser.onlinetotaltime;
				res.roleInfos.push_back(info);
			}
			LOG_DEBUG(LogSystem::csl_color_red_blue, "[%s] query role count[%d]", req->user.c_str(), queryCount);
		}
	} while (false);
	aPlr->sendPacket(res);
	return 1;
}

int32 SessionHandler::onNetPlayerSaveNotify(Player* aPlr, NetPlayerSaveNotify* nfy)
{
	for (uint32 i = 0; i < nfy->roleInfos.size() && i < sMaxRoleCount; ++i)
	{
		DBRoleInfo& info = nfy->roleInfos[i];
		DBUser dbRole;
		dbRole.id = info.id;
		dbRole.datas.write(info.datas.datas(), info.datas.wpos());
		dbRole.onlinetotaltime = info.onlinetotaltime;
		dbRole.datastr = info.datastr;
		dbRole.name = info.name;
		uint32 updateRows = 0;
		const int8* err = sApp.getDBConnector()->doUpdate(dbRole, "id", updateRows, "name, property, onlinetotaltime, datastr");
		if (!err) return 0;
		LOG_ERROR(err);
	}
	return 0;
}

int32 SessionHandler::onNetSellProductReq(Player* aPlr, NetSellProductReq* req)
{
	DBProduct dbProduct;
	dbProduct.productId = req->productId;
	dbProduct.userId = aPlr->getUserId();
	dbProduct.shelvesTime = DateTime::Now();
	NetSellProductRes res;
	char* err =	sDBConnector.doInsert(dbProduct);
	if (err) {
		res.result = NResultFail;
		LOG_ERROR(err);
		aPlr->sendPacket(res);
		return 0;
	}
	res.result = NResultSuccess;
	res.productInfo.productInsId = dbProduct.id;
	res.productInfo.productId = dbProduct.productId;
	res.productInfo.sellUserId = dbProduct.userId;
	res.productInfo.shelvesTime = dbProduct.shelvesTime;

	aPlr->sendPacket(res);
	return 0;
}

int SessionHandler::DoLogin(NetLoginReq* req, NetLoginRes& res)
{
	DBAccount account;
	account.user = req->user;
	uint32 queryCount = 0;
	const int8* err = sDBConnector.doQuery(account, account, queryCount, "user", "");
	do
	{
		if (err)
		{
			LOG_ERROR(err);
			return 0;
		}
		if (queryCount == 0)
		{
			account.user = req->user;
			account.createTime = DateTime::Now();
			err = sDBConnector.doInsert(account, "", "user");
			if (err) {
				res.result = 1;
				LOG_ERROR(err);
				return 0;
			}
		}
	} while (false);

	res.accInfo.id = account.id;
	res.accInfo.user = account.user;
	res.accInfo.password.write(account.password.datas(), account.password.wpos());
	res.accInfo.name = account.name;
	res.accInfo.email = account.email;
	res.accInfo.phone = account.phone;
	return 1;
}

int SessionHandler::DoQueryRole(NetLoginReq* req, NetLoginRes& res)
{
	DBUser role;
	role.accountId = res.accInfo.id;
	uint32 queryCount = 0;
	const int32 maxCount = sMaxRoleCount;
	DBUser retRoles[maxCount];

	std::vector<DBRecord*> result_records;
	for (uint32 i = 0; i < maxCount; ++i)
		result_records.push_back((DBRecord*)&retRoles[i]);

	//uint32 t0 = (uint32)DateTime::GetNowAppUS();
	const int8* err = sDBConnector.doQuery(role, result_records, queryCount, "accountId", "", maxCount);
	do {
		if (err)
		{
			LOG_ERROR(err);
			return 0;
		}

		if (queryCount > 0)
		{
			for (uint32 i = 0; i < queryCount; ++i)
			{
				DBRoleInfo info;
				DBUser& dbUser = retRoles[i];
				info.id = dbUser.id;
				info.accountId = dbUser.accountId;
				info.name = dbUser.name;
				info.createTime = dbUser.createTime;
				info.onlinetotaltime = dbUser.onlinetotaltime;
				info.datas.write(dbUser.datas.datas(), dbUser.datas.wpos());
				//info.datastr = dbUser.datastr;
				res.roleInfos.push_back(info);
			}
			LOG_DEBUG(LogSystem::csl_color_red_blue, "[%s] query role count[%d]", req->user.c_str(), queryCount);
		}
	} while (false);
	return 1;
}

int SessionHandler::DoCreateRole(NetCreateRoleReq* req, NetCreateRoleRes& res)
{
	res.accId = req->accId;
	{
		DBUser queryRole;
		queryRole.accountId = req->accId;
		uint32 queryCount = 0;
		const int32 maxCount = sMaxRoleCount;
		DBUser retRoles[maxCount];

		std::vector<DBRecord*> result_records;
		for (uint32 i = 0; i < maxCount; ++i)
			result_records.push_back((DBRecord*)&retRoles[i]);

		char* err = sDBConnector.doQuery(queryRole, result_records, queryCount, "accountId", "", maxCount);
		if (queryCount >= sMaxRoleCount)
		{
			res.result = NResultFail;
			return 0;
		}
	}

	{
		DBUser role;
		role.accountId = req->accId;
		role.name = req->name;
		role.createTime = DateTime::Now();
		char* err = sDBConnector.doInsert(role, "", "name");
		if (err)
		{
			LOG_ERROR(err);
			res.result = NResultFail;

			return 0;
		}
		res.result = NResultSuccess;
		DBRoleInfo& info = res.roleInfo;
		info.id = role.id;
		info.accountId = role.id;
		info.name = role.name;
		info.datas.write(role.datas.datas(), role.datas.wpos());
	}
	return 1;
}

int SessionHandler::DoSelectRole(NetSelectRoleReq* req, NetSelectRoleRes& res)
{
	res.accId = req->accId;
	DBUser role;
	role.id = req->userId;
	uint32 queryCount = 0;
	const int32 maxCount = 1;
	const int8* err = sDBConnector.doQuery(role, role, queryCount, "id", "", maxCount);
	do {
		if (err)
		{
			LOG_ERROR(err);
			res.result = NResultFail;
			return 0;
		}

		if (queryCount > 0)
		{
			DBRoleInfo& info = res.roleInfo;
			info.id = role.id;
			info.accountId = role.accountId;
			info.name = role.name;
			info.createTime = role.createTime;
			info.onlinetotaltime = role.onlinetotaltime;
			info.datas.write(role.datas.datas(), role.datas.wpos());
			info.datastr = role.datastr;
			res.result = 0;
		}
	} while (false);
	res.result = NResultSuccess;
	return 1;
}

