#include "stdafx.h"

Application::Application():
dbServer(NULL)
{
	mRun = true;
}
Application::~Application()
{

}


bool StartRedisServer()
{
	STARTUPINFOA si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = TRUE; //TRUE表示显示创建的进程的窗口  
	char szBuffer[MAX_PATH] = { 0 };
	sprintf_s(szBuffer, MAX_PATH, "%s %s", "redis-64.3.0.503\\redis-server.exe", "redis-64.3.0.503\\redis.windows.conf");
	if (!CreateProcessA(NULL, szBuffer, NULL, NULL, false, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		int error = GetLastError();
		LOG_ERROR("redis start error id %d", error);
		return false;
	}

	return true;
}

bool Application::Initialize()
{
	StartRedisServer();
	INSTANCE(ConfigManager);
	INSTANCE(SessionManager);
	INSTANCE(DBProxy);
	INSTANCE(ConfigManager).reloadConfig();

	ServerConfig& cf = INSTANCE(ConfigManager).getConfig("DB");
	dbServer = INSTANCE(Network).listen(cf.Port);
	if (!dbServer) {
		LOG_ERROR("DB listen Port:%d fial", cf.Port);
		return false;
	}

	LOG_DEBUG(LogSystem::csl_color_green, "DB listen Port:%d success", cf.Port);

	dbServer->addEventListener(SocketEvent::ACCEPT, (EventCallback)&Application::onDBAccept, this);
	dbServer->addEventListener(SocketEvent::RECV, (EventCallback)&Application::onDBRecv, this);
	dbServer->addEventListener(SocketEvent::EXIT, (EventCallback)&Application::onDBExit, this);

	INSTANCE(SessionHandler);

	ServerConfig& cfMysql = INSTANCE(ConfigManager).getConfig("Mysql");
	dbConnector = INSTANCE(DBProxy).connect(
		cfMysql.Host.c_str(),
		cfMysql.User.c_str(),
		cfMysql.Password.c_str(),
		cfMysql.DbName.c_str(),
		cfMysql.Port);

	
	if (dbConnector)
		LOG_DEBUG(LogSystem::csl_color_green, "Database connect %s %s %d success", cfMysql.DbName.c_str(), cfMysql.Host.c_str(), cfMysql.Port);
	else
	{
		LOG_ERROR("Database connect fail...");
		return false;
	}

	sDBAngent.Connect(cfMysql.Host.c_str(),
		cfMysql.User.c_str(),
		cfMysql.Password.c_str(),
		cfMysql.DbName.c_str(),
		cfMysql.Port, 10);

	for (uint32 i = 0; i < 10; ++i)
	{
		DBInsertRequest* request = (DBInsertRequest*)sDBAngent.DispatchRequest(new DBInsertRequest(new DBAccount));
		request->addEventListener(DBEvent::DBEVENT_CMD, (EventCallback)&Application::onMysql, this);
		DBAccount* insertDB = (DBAccount*)request->record;
		request->record = insertDB;
		insertDB->user = Shared::uint32tostr(i);
		insertDB->createTime = time(NULL);
	}

	DBQueryRequest* qrequest = (DBQueryRequest*)sDBAngent.DispatchRequest(new DBQueryRequest(new DBAccount));
	DBAccount* qaccount = (DBAccount*)qrequest->record;
	qaccount->user = "1";
	qrequest->record = qaccount;

	//qrequest->compareRecord = "user";
	qrequest->addEventListener(DBEvent::DBEVENT_CMD, (EventCallback)&Application::onMysql, this);

	sRedisProxy.addEventListener(RedisEvent::CONNECT, (EventCallback)&Application::RedisConnect, this);
	ServerConfig& cfg = INSTANCE(ConfigManager).getConfig("Redis");
	IF_FALSE(!sRedisProxy.AsyncConnect(cfg.Host, cfg.Port))
		return false;
	return true;
}

bool Application::Update()
{
	sDBAngent.Update();
	INSTANCE(Network).update(0.f, 0.f);
	INSTANCE(DBProxy).update(0.f, 0.f);
	sRedisProxy.loop();
	return true;
}

bool Application::Destroy()
{
	sRedisProxy.Destroy();
	return true;
}

void Application::sendPacketToTarget(Packet& packet, Socket* socket)
{
	static char input[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(input, PACKET_MAX_LENGTH);
	int32 packetCount = 0;
	in << (int8)Snd_Sck;
	int32 pos = in.wpos();
	in << packetCount;
	int32 offset = in.wpos();
	in << packet;
	packetCount = in.wpos() - offset;
	packetCount = Shared::htonl(packetCount);
	in.push(pos, &packetCount, sizeof(int32));
	socket->sendBuffer(in.datas(), in.wpos());
}

int32 Application::onDBAccept(SocketEvent& e)
{
	LOG_DEBUG(LogSystem::csl_color_green, "world server accept success");
	return 0;
}

int32 Application::onDBRecv(SocketEvent& e)
{
	BinaryStream out(e.data, e.count);
	uint64 sessionId = 0;
	uint32 accId = 0;
	int32 packetCount = 0;
	int32 msgId = 0;
	int8 sndTarget = 0;
	CHECK_RETURN(out >> sndTarget, 0);

	switch (sndTarget)
	{
	case Snd_Ssn:
		CHECK_RETURN(out >> sessionId, 0);
		break;
	case Snd_Plr:
		CHECK_RETURN(out >> accId, 0);
		break;
	case Snd_Sck:
		break;
	}

	CHECK_RETURN(out >> packetCount, 0);
	int32 rpos = out.rpos();
	CHECK_RETURN(out >> msgId, 0);
	out.rpos(rpos);
	switch (sndTarget)
	{
	case Snd_Ssn: {
		Session* ssn = sSsnMgr.getSession(sessionId);
		do
		{
			if (ssn == NULL && msgId == ID_NetSessionEnterNotify)
			{
				ssn = sSsnMgr.newSession(e.socket, sessionId);
				sSsnMgr.addSessionsBySocket(e.socket->getSocketId(), ssn);
			}

			if (ssn == NULL) break;

			Packet* pack = sPacketMgr.Alloc(msgId);
			if (pack == NULL) break;
			out >> (*pack);

			if (msgId == ID_NetSessionEnterNotify || msgId == ID_NetSessionLeaveNotify || msgId == ID_NetLoginReq)
				dbServer->Dispatch(pack->getMsgId(), ssn, pack);

			sPacketMgr.Free(pack);
			return 0;

		} while (false);

		if (ssn == NULL) return 0;

		NetSessionLeaveNotify nfy;
		ssn->sendPacketToWorld(nfy);
		LOG_DEBUG(LogSystem::csl_color_red, "ssnId %0.16llx packet error leave world", ssn->getSsnId());
		sSsnMgr.removeSessionsBySocket(e.socket->getSocketId(), ssn);
	}
		break;
	case Snd_Sck: {
		Packet* pack = sPacketMgr.Alloc(msgId);
		if (pack == NULL) break;
		out >> (*pack);
		dbServer->Dispatch(pack->getMsgId(), e.socket, pack);
		sPacketMgr.Free(pack);
	}
		break;
	case Snd_Plr: {
		Player* aPlr = sPlrMgr.FindPlrByAccId(accId);
		if (aPlr == NULL) break;
		
		Packet* pack = sPacketMgr.Alloc(msgId);
		if (pack == NULL) break;
		out >> (*pack);
		dbServer->Dispatch(pack->getMsgId(), aPlr, pack);
		sPacketMgr.Free(pack);
	}
		break;
	}
	
	return 0;
}

int32 Application::onDBExit(SocketEvent& e)
{
	LOG_DEBUG(LogSystem::csl_color_red, "world server exit db");
	
	std::set<Session*>* setSession = INSTANCE(SessionManager).getSessionsBySocket(e.socket->getSocketId());
	if (setSession)
	{
		std::set<Session*> copySetSession = (*setSession);
		for (auto ssn : copySetSession)
		{
			INSTANCE(SessionManager).removeSessionsBySocket(e.socket->getSocketId(), ssn);
		}
	}
	return 0;
}

int32 Application::RedisConnect(RedisEvent& e)
{
	ServerConfig& cfg = INSTANCE(ConfigManager).getConfig("Redis");
	char szBuffer[64] = { 0 };
	sprintf_s(szBuffer, 64, "auth %s", cfg.Password.c_str());
	sRedisProxy.sendCmd(szBuffer, (EventCallback)&Application::RedisAuth, this);
	return 0;
}

int32 Application::RedisAuth(RedisEvent& e)
{
	ServerConfig& cfg = INSTANCE(ConfigManager).getConfig("Redis");
	LOG_DEBUG(LogSystem::csl_color_green, "redis auth success  %s %d", cfg.Host.c_str(), cfg.Port);
	return 0;
}

int32 Application::onMysql(DBEvent& e)
{
	DBRequest* request = (DBRequest*)e.targetDispatcher;
	if (request->err != "")
		LOG_ERROR("cmdstr:%s err:%s", request->cmd.c_str(), request->err.c_str());
	else
		LOG_DEBUG(LogSystem::csl_color_green, "cmdstr:%s", request->cmd.c_str());

	return 0;
}
