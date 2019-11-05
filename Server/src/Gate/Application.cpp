#include "stdafx.h"

Application::Application():
gateServer(NULL),
worldServer(NULL)
{
	mTime = 0.0;
	mDelay = 0.0;
	mFPSTimer = 0.0;
	mFPS = 0;
	mGateIdx = 0;
}
Application::~Application()
{

}

bool Application::Initialize()
{
	INSTANCE(ConfigManager);
	INSTANCE(SessionManager);
	INSTANCE(ConfigManager).reloadConfig();

	ServerConfig& cf = INSTANCE(ConfigManager).getConfig("Gate");
	gateServer = INSTANCE(Network).listen(cf.Port);
	if (!gateServer) {
		LOG_ERROR("Gate listen Port:%d fial", cf.Port);
		return false;
	}

	char szBuffer[128];
	sprintf_s(szBuffer, 128, "%s%d", cf.Host.c_str(), cf.Port);
	mGateIdx = Shared::BKDRHash(szBuffer);

	LOG_DEBUG(LogSystem::csl_color_green, "Gate listen Port:%d Idx:%d success", cf.Port, mGateIdx);

	gateServer->addEventListener(SocketEvent::ACCEPT, (EventCallback)&Application::onGateAccept, this);
	gateServer->addEventListener(SocketEvent::RECV, (EventCallback)&Application::onGateRecv, this);
	gateServer->addEventListener(SocketEvent::EXIT, (EventCallback)&Application::onGateExit, this);


	return true;
}

bool Application::Update()
{
	mDelay = DateTime::GetNowAppUS() - mTime;
	mTotalTime += mDelay;
	mFPSTimer += mDelay;
	mTime = DateTime::GetNowAppUS();
	mFPS++;
	if (mFPSTimer >= 1.0)
	{
		static char szBuffer[256] = { 0 };
		sprintf_s(szBuffer, 256, "Gate:%d FPS:%d Player:%d", mGateIdx,mFPS, gateServer->getSocketCount());
		Shared::setConsoleTitle(szBuffer);
		//LOG_INFO("FPS:%d", mFPS);
		mFPS = 0;
		mFPSTimer = 0.0;
	}

	INSTANCE(Network).update(0.f, 0.f);
	if (!worldServer)
	{
		ServerConfig& cf = INSTANCE(ConfigManager).getConfig("World");
		worldServer = INSTANCE(Network).connect(cf.Host.c_str(), cf.Port);

		if (worldServer)
		{
			LOG_DEBUG(LogSystem::csl_color_green, "connect World Port:%d ...", cf.Port);
			worldServer->addEventListener(SocketEvent::CONNECT, (EventCallback)&Application::onWorldConnect, this);
			worldServer->addEventListener(SocketEvent::RECV, (EventCallback)&Application::onWorldRecv, this);
			worldServer->addEventListener(SocketEvent::EXIT, (EventCallback)&Application::onWorldExit, this);
			worldServer->addEventListener(SocketEvent::EXCEPTION, (EventCallback)&Application::onWorldException, this);
		}
		else
		{
			LOG_DEBUG(LogSystem::csl_color_green, "connect World Port:%d fail...", cf.Port);
		}
	}


	return true;
}

bool Application::Destroy()
{
	return true;
}

bool Application::sendPacketToWorld(Packet& packet, Session* tar)
{
	if (!worldServer)
		return false;
	if (tar)
		tar->sendPacketToTarget(packet, worldServer->getSocket());
	return true;
}

bool Application::sendBufferToWorld(int8* data, int32 count, Session* tar)
{
	if (!worldServer)
		return false;
	if (tar)
		tar->sendBufferToTarget(data, count, worldServer->getSocket());
	return true;
}

int Application::onGateAccept(SocketEvent& e)
{
	if (worldServer == NULL) {
		INSTANCE(Network).closesocket(e.socket->getSocketId());
		return 0;
	}	
	Session* ssn = INSTANCE(SessionManager).createSession(e.socket, MakeSsnId());
	if (!ssn)
		return 0;
	ssn = INSTANCE(SessionManager).addSessionBySocket(e.socket->getSocketId(), ssn);
	if (!ssn)
		return 0;

	NetSessionEnterNotify nfy;
	nfy.host = e.socket->getIP();
	LOG_INFO("ssnId %0.16llx [%s] accept", ssn->getSsnId(), e.socket->getIP());
	sendPacketToWorld(nfy, ssn);
	return 0;
}

int Application::onGateRecv(SocketEvent& e)
{
	if (worldServer == NULL) {
		INSTANCE(Network).closesocket(e.socket->getSocketId());
		return 0;
	}

	Session* ssn = INSTANCE(SessionManager).getSessionBySocket(e.socket->getSocketId());
	if (ssn == NULL)
	{
		INSTANCE(Network).closesocket(e.socket->getSocketId());
		return 0;
	}
	/*
	*********** 解密 ***********
	*/
	//AES aes(sKey);
	//aes.InvCipher((unsigned char*)e.data, e.count);
	Shared::XOR((char*)e.data, e.count, sKey);
	sendBufferToWorld((int8*)e.data, e.count, ssn);
	/*
	*********** 解密 ***********
	*/
#ifdef _DEBUG
	BinaryStream out(e.data, e.count);
	int32 msgId = 0;
	int32 rpos = out.wpos();
	CHECK_RETURN(out >> msgId, 0);
	out.rpos(rpos);
	if (msgId == ID_NetPingGateNotify) {
		char* datastr = (char*)e.data;
		int32 count = e.count;
		char* dataptr = new char[count];
		memcpy(dataptr, datastr, count);
		Shared::XOR(dataptr, count, sKey);
		ssn->sendBuffer(dataptr, count);
		delete[] dataptr;
	}

	DEBUG_DEBUG(LogSystem::csl_color_green_blue, "ssnId:%0.16llx c to s %s size:%d", ssn->getSsnId(), INSTANCE(PacketManager).GetName(msgId).c_str(), e.count);
#endif // _DEBUG

	return 0;
}

int Application::onGateExit(SocketEvent& e)
{
	Session* ssn = INSTANCE(SessionManager).getSessionBySocket(e.socket->getSocketId());
	if (!ssn)
		return 0;

	LOG_INFO("ssnId %0.16llx leave", ssn->getSsnId());

	NetSessionLeaveNotify nfy;
	sendPacketToWorld(nfy, ssn);

	INSTANCE(SessionManager).removeSessionBySocket(e.socket->getSocketId());
	INSTANCE(SessionManager).removeSession(ssn->getSsnId());
	return 0;
}

int Application::onWorldConnect(SocketEvent& e)
{
	LOG_DEBUG(LogSystem::csl_color_green, "world connect success");
	return 0;
}

int Application::onWorldRecv(SocketEvent& e)
{
	BinaryStream out(e.data, e.count);
	uint64 sessionId = 0;
	int32 packetCount = 0;
	int32 msgId = 0;
	CHECK_RETURN(out >> sessionId, 0);
	CHECK_RETURN(out >> packetCount, 0);
	int32 rpos = out.rpos();
	CHECK_RETURN(out >> msgId, 0);
	out.rpos(rpos);

	Session* ssn = INSTANCE(SessionManager).getSession(sessionId);
	do
	{
		if (ssn == NULL)
			break;

#ifdef _DEBUG
		DEBUG_DEBUG(LogSystem::csl_color_yellow, "ssnId:%0.16llx s to c %s size:%d", ssn->getSsnId(), INSTANCE(PacketManager).GetName(msgId).c_str(), packetCount);
#endif // DEBUG

		/*
		*********** 加密 ***********
		*/
		//AES aes(sKey);
		//aes.Cipher((unsigned char*)e.data + rpos, packetCount);
		Shared::XOR((int8*)e.data + rpos, packetCount, sKey);
		ssn->sendBuffer((int8*)e.data + rpos, packetCount);
		/*
		*********** 加密 ***********
		*/

		if (msgId == ID_NetSessionLeaveNotify)
			break;
		return 0;

	} while (false);

	if (!ssn)
		return 0;

	NetSessionLeaveNotify nfy;
	sendPacketToWorld(nfy, ssn);
	INSTANCE(Network).closesocket(ssn->getSocketId());
	return 0;
}

int Application::onWorldExit(SocketEvent& e)
{
	worldServer = NULL;
	std::map<uint64, Session*>  mapSession = INSTANCE(SessionManager).getMapSession();
	for (auto& itr : mapSession)
	{
		Session* ssn = itr.second;
		INSTANCE(Network).closesocket(ssn->getSocketId());
	}

	LOG_DEBUG(LogSystem::csl_color_red, "world exit");
	return 0;
}

int Application::onWorldException(SocketEvent& e)
{
	LOG_DEBUG(LogSystem::csl_color_red, "world not exist");
	worldServer = NULL;
	return 0;
}

uint64 Application::MakeSsnId()
{
	uint64 ssnId = ((uint64)mGateIdx << 32) | ++Session::sId;
	return ssnId;
}
