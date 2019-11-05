#include "stdafx.h"

SessionHandler::SessionHandler()
{
	RegWorldEvent(ID_NetSessionEnterNotify, &SessionHandler::onNetSessionEnterNotify, this);
	RegWorldEvent(ID_NetSessionLeaveNotify, &SessionHandler::onNetSessionLeaveNotify, this);
	RegWorldEvent(ID_NetLoginReq, &SessionHandler::onNetLoingReq, this);
	RegWorldEvent(ID_NetPingNotify, &SessionHandler::onNetPingNotify, this);
}

SessionHandler::~SessionHandler()
{

}

int SessionHandler::onNetSessionEnterNotify(Session* ssn, NetSessionEnterNotify* nfy)
{
	LOG_INFO("ssnId %0.16llx [%s] enter world", ssn->getSsnId(), nfy->host.c_str());
	ssn->setHost(nfy->host);
	ssn->sendPacketToDB(*nfy);
	return 0;
}

int SessionHandler::onNetSessionLeaveNotify(Session* ssn, NetSessionLeaveNotify* nfy)
{
	sApp.doSessionLeaveWorld(ssn);
	return 0;
}

int SessionHandler::onNetLoingReq(Session* ssn, NetLoginReq* req)
{
	Player* plr = ssn->getPlayer();
	if (plr)
	{
		plr->sendRespnoseMsg(MC_LoginYouHaveLanded);
		return 0;
	}

	ssn->sendPacketToDB(*req);
	return 0;
}

int SessionHandler::onNetPingNotify(Session* ssn, NetPingNotify* nfy)
{
	ssn->sendPacketToWorld(*nfy);
	return 0;
}

