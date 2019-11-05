#include "stdafx.h"

RobotManager::RobotManager()
{
	mCurRobot = NULL;
	mCell = 0.5f;
	mMapData = new char[100 * 100];
	memset(mMapData, 0, 100 * 100);

	addEventListener(ID_NetSessionLeaveNotify, (EventCallbackProcess)&RobotManager::onNetSessionLeaveNotify, this);

	addEventListener(ID_NetPingNotify, (EventCallbackProcess)&RobotManager::onNetPingNotify, this);
	addEventListener(ID_NetPingGateNotify, (EventCallbackProcess)&RobotManager::onNetPingGateNotify, this);
	addEventListener(ID_NetLoginRes, (EventCallbackProcess)&RobotManager::onNetLoginRes, this);
	addEventListener(ID_NetCreateRoleRes, (EventCallbackProcess)&RobotManager::onNetCreateRoleRes, this);
	addEventListener(ID_NetSelectRoleRes, (EventCallbackProcess)&RobotManager::onNetSelectRoleRes, this);

	//addEventListener(ID_NetPropertyRes, (EventCallbackProcess)&SocketHandler::onNetPropertyRes, this);
	//addEventListener(ID_NetEntityEnterMapNotify, (EventCallbackProcess)&RobotManager::onNetEntityEnterMapNotify, this);
	//addEventListener(ID_NetEntityLeaveMapNotify, (EventCallbackProcess)&RobotManager::onNetEntityLeaveMapNotify, this);

	addEventListener(ID_NetPlayerEnterViewNotify, (EventCallbackProcess)&RobotManager::onNetPlayerEnterViewNotify, this);
	//addEventListener(ID_NetPlayerLeaveViewNotify, (EventCallbackProcess)&RobotManager::onNetPlayerLeaveViewNotify, this);

	addEventListener(ID_NetEntityPropertyNotify, (EventCallbackProcess)&RobotManager::onNetEntityPropertyNotify, this);

	addEventListener(ID_NetChatMsgNotify, (EventCallbackProcess)&RobotManager::onNetChatMsgNotify, this);

	addEventListener(ID_NetFriendListRes, (EventCallbackProcess)&RobotManager::onNetFriendListRes, this);
	addEventListener(ID_NetProductListRes, (EventCallbackProcess)&RobotManager::onNetProductListRes, this);

	addEventListener(ID_NetMailListNotify, (EventCallbackProcess)&RobotManager::onNetMailListNotify, this);

	INSTANCE(CmdDispatcher);
	INSTANCE(CmdDispatcher).addEventListener("gm", (EventCallback)&RobotManager::onGmCmd, this);
	INSTANCE(CmdDispatcher).addEventListener("login", (EventCallback)&RobotManager::onLogin, this);
	INSTANCE(CmdDispatcher).addEventListener("crole", (EventCallback)&RobotManager::onCreate, this);
	INSTANCE(CmdDispatcher).addEventListener("select", (EventCallback)&RobotManager::onSelect, this);
	INSTANCE(CmdDispatcher).addEventListener("close", (EventCallback)&RobotManager::onClose, this);
	INSTANCE(CmdDispatcher).addEventListener("sendmsg", (EventCallback)&RobotManager::onSendMsg, this);
	INSTANCE(CmdDispatcher).addEventListener("ping", (EventCallback)&RobotManager::onPing, this);
	INSTANCE(CmdDispatcher).addEventListener("pinggate", (EventCallback)&RobotManager::onPingGate, this);

}

RobotManager::~RobotManager()
{
}

Robot* RobotManager::add(Socket* socket, Robot* robot)
{
	auto itr = mMapSocketRobat.find(socket);
	if (itr != mMapSocketRobat.end())
		return NULL;

	mMapSocketRobat.insert(std::make_pair(socket, robot));
	return robot;
}

void RobotManager::removeRobat(Socket* socket)
{
	auto itr = mMapSocketRobat.find(socket);
	if (itr != mMapSocketRobat.end())
		mMapSocketRobat.erase(itr);
}

Robot* RobotManager::getRobat(Socket* socket)
{
	auto itr = mMapSocketRobat.find(socket);
	if (itr != mMapSocketRobat.end())
		return itr->second;

	return NULL;
}

void RobotManager::update(float32 time, float32 delay)
{
	//if (INSTANCE(SocketHandler).mCreate == false)
	//{
	//	if (mCurRobot == NULL)
	//	{
	//		INSTANCE(SocketHandler).createRobot();
	//	}
	//}
	INSTANCE(CmdDispatcher).update(time, delay);
	for (auto itr : mMapSocketRobat)
	{
		itr.second->update(time, delay);
	}
}

int RobotManager::onNetSessionLeaveNotify(Robot* robot, NetSessionLeaveNotify* nfy)
{
	LOG_ERROR(__FUNCTION__);
	return 0;
}

int RobotManager::onNetPingNotify(Robot* robot, NetPingNotify* nfy)
{
	std::string str;
	DateTime::Now(str);
	LOG_INFO("ping [%d]", GetTickCount() - nfy->time);

	return 0;
}

int RobotManager::onNetPingGateNotify(Robot* robot, NetPingNotify* nfy)
{
	std::string str;
	DateTime::Now(str);
	LOG_INFO("pinggate [%d]", GetTickCount() - nfy->time);
	return 0;
}

int RobotManager::onNetLoginRes(Robot* robot, NetLoginRes* res)
{
	if (res->result == NResultSuccess)
	{
		NetPingNotify nfy;
		nfy.time = GetTickCount();
		robot->sendPacket(nfy);

		robot->mGuid = res->guid;
		mCurRobot->mAccountId = res->accInfo.id;
		LOG_DEBUG(LogSystem::csl_color_green, "user:%s accId:%d guid:%llu", res->accInfo.user.c_str(), res->accInfo.id, res->guid);
		if (res->roleInfos.size() == 0)
		{
			NetCreateRoleReq req;
			req.accId = res->accInfo.id;
			req.name = res->accInfo.user;
			robot->sendPacket(req);
			return 0;
		}

		for (uint32 i = 0; i < res->roleInfos.size(); ++i)
		{
			DBRoleInfo& info = res->roleInfos[i];
			LOG_DEBUG(LogSystem::csl_color_green, "userId:%d user:%s", info.id, info.name.c_str());
			if (i == 0)
			{
				NetSelectRoleReq req;
				req.accId = res->accInfo.id;
				req.userId = info.id;
				robot->sendPacket(req);
				return 0;
			}
		}

		/*
		NetFirst nFirst;
		uint32 length = 1024 * 7;
		char* datas = new char[length];
		nFirst.sbytes.WriteBytes(datas, length);
		robot->sendPacket(nFirst);

		NetPingNotify nfy;
		nfy.time = GetTickCount();
		robot->sendPacket(nfy);
		robot->mGuid = res->guid;
		robot->mAccountId = res->accountInfo.id;
		robot->mIsLogin = true;

		NetGmMsg msg;
		msg.name = "test";
		msg.gmParams.push_back("p1");
		msg.gmParams.push_back("p2");
		msg.gmParams.push_back("p3");
		robot->sendPacket(msg);
		*/

		//NetChatMsgNotify msgNfy;
		//char szBuffer[256] = { 0 };
		//sprintf_s(szBuffer, 256, "login ok");
		//msgNfy.chatMsg = szBuffer;
		//msgNfy.channelType = 1;
		//robot->sendPacket(msgNfy);

	}
	else
	{
		printf("login fail [%s]\n", INSTANCE(ConfigManager).getMsg(res->result).c_str());
	}
	return 0;
}

int RobotManager::onNetCreateRoleRes(Robot* robot, NetCreateRoleRes* res)
{
	if (res->result == NResultFail)
	{
		LOG_ERROR("创建角色失败");
		return 0;
	}

	NetSelectRoleReq req;
	req.accId = res->roleInfo.accountId;
	req.userId = res->roleInfo.id;
	robot->sendPacket(req);
	LOG_DEBUG(LogSystem::csl_color_green, "create userId:%d user:%s guid:%llu", res->roleInfo.id, res->roleInfo.name.c_str(), robot->mGuid);
	return 0;
}

int RobotManager::onNetSelectRoleRes(Robot* robot, NetSelectRoleRes* res)
{
	if (res->result == NResultFail)
	{
		LOG_ERROR("创建选择角色失败");
		return 0;
	}

	LOG_DEBUG(LogSystem::csl_color_green, "select userId:%d user:%s guid:%llu", res->roleInfo.id, res->roleInfo.name.c_str(), robot->mGuid);
	return 0;
}

int RobotManager::onNetEntityEnterMapNotify(Robot* robot, NetEntityEnterMapNotify* nfy)
{
	//LOG_INFO("entity %s instanceId:%d enter mapId:%d", nfy->name.c_str(), nfy->instanceId, nfy->mapId);
	return 0;
}

int RobotManager::onNetEntityLeaveMapNotify(Robot* robot, NetEntityLeaveMapNotify* nfy)
{
	//LOG_INFO("player instanceId:%d leave mapId:%d", nfy->instanceId, nfy->mapId);
	return 0;
}

int RobotManager::onNetPlayerEnterViewNotify(Robot* robot, NetPlayerEnterViewNotify* nfy)
{
	if (nfy->guid == robot->mGuid)
	{
		LOG_INFO("entity %s instanceId:%d enter %s view", nfy->name.c_str(), nfy->guid, robot->user.c_str());

		robot->mPosX = nfy->pos.x;
		robot->mPosY = nfy->pos.y;
		robot->mInMap = true;
		Vector2 pos;
		pos = Utils::Point2ToVector2(nfy->pos.x, nfy->pos.y, mCell, mCell);
		robot->mPos = pos;
		robot->mSendPos.x = robot->mPosX;
		robot->mSendPos.y = robot->mPosY;
		robot->mSpeed = nfy->speed;
	}


	return 0;
}

int RobotManager::onNetPlayerLeaveViewNotify(Robot* robot, NetPlayerLeaveViewNotify* nfy)
{
	LOG_INFO("player instanceId:%d leave %s view", nfy->guid, robot->user.c_str());
	return 0;
}

int RobotManager::onNetEntityPropertyNotify(Robot* robot, NetEntityPropertyNotify* nfy)
{
	Dictionary dict;
	nfy->property >> dict;

	LOG_DEBUG(LogSystem::csl_color_green, "Guid[%d][%s]", nfy->guid, robot->user.c_str());
	for (auto itr : dict.MapVariant())
	{
		const Variant& var = itr.first;
		const Variant& second = itr.second;

		if (var.type() == Variant::TypeString)
			LOG_INFO("type:[%s] value:[%s]", var.valueString(), second.toString().c_str());
		else
			LOG_INFO("type:[%s] value:[%s]", var.toString().c_str(), second.toString().c_str());
		//LOG_INFO("type:[%s] value:[%s]", Property::GetPropertyName(itr.first.valueInt32()).c_str(), itr.second.toString().c_str());
	}
	LOG_DEBUG(LogSystem::csl_color_green, "Guid");
	return 0;
}

int RobotManager::onNetChatMsgNotify(Robot* robot, NetChatMsgNotify* nfy)
{
	switch (nfy->channelType)
	{
	case EC_WORLD:
		LOG_DEBUG(LogSystem::csl_color_green, "世界频道:%s", nfy->chatMsg.c_str());
		break;
	case EC_SYSTEM:
		LOG_DEBUG(LogSystem::csl_color_green, "系统频道:%s", nfy->chatMsg.c_str());
		break;
	case EC_ROOM:
		LOG_DEBUG(LogSystem::csl_color_green, "房间频道:%s", nfy->chatMsg.c_str());
		break;
	case EC_TARGET:
		LOG_DEBUG(LogSystem::csl_color_green, "[%s]%s", nfy->from.c_str(), nfy->chatMsg.c_str());
		break;
	default:
		LOG_DEBUG(LogSystem::csl_color_green, "self[%s][%s]:%s", robot->user.c_str(), nfy->from.c_str(), nfy->chatMsg.c_str());
		break;
	}
	return 0;
}

int RobotManager::onNetFriendListRes(Robot* robot, NetFriendListRes* res)
{
	std::map<uint32, std::string> groups;
	for (uint32 i = 0; i < res->gruopIds.size(); ++i) {
		groups[res->gruopIds[i]] = res->gruopNames[i];
	}

	LOG_DEBUG(LogSystem::csl_color_green, "好友个数:%d", res->friendInfos.size());
	for (uint32 i = 0 ; i < res->friendInfos.size(); ++i)
	{
		FriendInfo& info = res->friendInfos[i];
		LOG_DEBUG(LogSystem::csl_color_green, "\ngroupId:%s\nuserId:%d\nname:%s\n%s\n", groups[info.groupId].c_str(), info.userId, info.name.c_str(), info.state == 1 ? "在线" : "离线");
	}
	return 0;
}

int RobotManager::onNetProductListRes(Robot* robot, NetProductListRes* res)
{
	LOG_DEBUG(LogSystem::csl_color_green, "商品个数:%d", res->productInfos.size());
	for (uint32 i = 0; i < res->productInfos.size(); ++i)
	{
		ProductInfo& info = res->productInfos[i];
		LOG_DEBUG(LogSystem::csl_color_green, "\ninsId:%d\npductId:%d\nsellUid:%d\n", info.productInsId, info.productId, info.sellUserId);
	}
	return 0;
}

int RobotManager::onNetMailListNotify(Robot* robot, NetMailListNotify* nfy)
{
	char szBuffer[256] = {};
	sprintf_s(szBuffer, 256, "mail count:%d", nfy->mailInfos.size());
	for (uint32 i = 0; i < nfy->mailInfos.size(); ++i)
	{
		int32 len = strlen(szBuffer);
		MailInfo& info = nfy->mailInfos[i];
		sprintf_s(szBuffer + len, 256 - len, "\ntitle:%s\ncontent:%s", info.title.c_str(), info.content.c_str());
	}
	LOG_DEBUG(LogSystem::csl_color_green, szBuffer);
	return  0;
}

int32 RobotManager::onGmCmd(CmdEvent& e)
{
	std::vector<std::string>& params = e.cmdExecute->params;
	NetGmMsg msg;
	msg.name = params.size() >= 1 ? params[0] : "";
	for (uint32 i = 1; i < e.cmdExecute->params.size(); ++i)
	{
		msg.gmParams.push_back(e.cmdExecute->params[i]);
	}
	if (mCurRobot == NULL) return 0;
	mCurRobot->sendPacket(msg);
	return 0;
}

int32 RobotManager::onLogin(CmdEvent& e)
{
	std::string user = e.cmdExecute->params[0];
	INSTANCE(SocketHandler).PushUser(user);
	INSTANCE(SocketHandler).createRobot(user, "");
	return 0;
}

int32 RobotManager::onCreate(CmdEvent& e)
{
	if (mCurRobot == NULL) return 0;

	NetCreateRoleReq req;
	req.accId = mCurRobot->mAccountId;
	req.name = e.cmdExecute->params[0];
	mCurRobot->sendPacket(req);
	return 0;
}

int32 RobotManager::onSelect(CmdEvent& e)
{
	if (mCurRobot == NULL) return 0;

	NetSelectRoleReq req;
	req.accId = mCurRobot->mAccountId;
	
	req.userId = Shared::strtoint32(e.cmdExecute->params[0]);
	mCurRobot->sendPacket(req);
	return 0;
}

int32 RobotManager::onClose(CmdEvent& e)
{
	if (mCurRobot == NULL) return 0;
	INSTANCE(Network).closesocket(mCurRobot->mSocket->getSocketId());
	return 0;
}

int32 RobotManager::onSendMsg(CmdEvent& e)
{
	if (mCurRobot == NULL) return 0;
	NetChatMsgNotify nfy;
	nfy.channelType = Shared::strtoint32(e.cmdExecute->params[0]);
	nfy.chatMsg = e.cmdExecute->params[1];
	nfy.to = e.cmdExecute->params[2];
	mCurRobot->sendPacket(nfy);
	return 0;
}

int32 RobotManager::onPing(CmdEvent& e)
{
	if (mCurRobot == NULL) return 0;
	NetPingNotify nfy;
	nfy.time = GetTickCount();
	mCurRobot->sendPacket(nfy);
	return 0;
}

int32 RobotManager::onPingGate(CmdEvent& e)
{
	if (mCurRobot == NULL) return 0;
	NetPingGateNotify nfy;
	nfy.time = GetTickCount();
	mCurRobot->sendPacket(nfy);
	return 0;
}
