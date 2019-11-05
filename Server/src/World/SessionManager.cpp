#include "stdafx.h"

SessionManager::SessionManager()
{

}

SessionManager::~SessionManager()
{
	for ( auto itr: mMapSession)
		delete itr.second;
	mMapSession.clear();
}

Session* SessionManager::createSession(Socket* s, SessionID sessionId)
{
	Session* ssn = new Session(s);
	if (sessionId != 0)
		ssn->setSsnId(sessionId);
	else
		ssn->setSsnId(++Session::sId);

	return addSession(ssn);
}

Session* SessionManager::getSession(SessionID sessionId)
{
	auto itr = mMapSession.find(sessionId);
	if (itr != mMapSession.end())
		return itr->second;

	return NULL;
}

Session* SessionManager::addSession(Session* ssn)
{
	auto itr = mMapSession.find(ssn->getSsnId());
	if (itr != mMapSession.end())
		return NULL;
	mMapSession.insert(std::make_pair(ssn->getSsnId(), ssn));
	return ssn;
}

void SessionManager::removeSession(SessionID sessionId)
{
	auto itr = mMapSession.find(sessionId);
	if (itr != mMapSession.end())
	{
		delete itr->second;
		mMapSession.erase(itr);
	}
		
}

Session* SessionManager::addSessionBySocket(uint32 socketId, Session* ssn)
{
	auto itr = mMapSocketSession.find(socketId);
	if (itr != mMapSocketSession.end())
		return NULL;
	mMapSocketSession.insert(std::make_pair(socketId, ssn));
	return ssn;
}

Session* SessionManager::getSessionBySocket(uint32 socketId)
{
	auto itr = mMapSocketSession.find(socketId);
	if (itr == mMapSocketSession.end())
		return NULL;

	return getSession(itr->second->getSsnId());
}

void SessionManager::removeSessionBySocket(uint32 socketId)
{
	auto itr = mMapSocketSession.find(socketId);
	if (itr == mMapSocketSession.end())
		return;
	mMapSocketSession.erase(itr);
}

Session* SessionManager::addSessionsBySocket(uint32 socketId, Session* ssn)
{
	auto itr = mMapSocketSessions.find(socketId);
	if (itr != mMapSocketSessions.end())
	{
		std::set<Session*>& sset = itr->second;
		if (sset.find(ssn) == sset.end())
			sset.insert(ssn);
		else
			return NULL;
	}
	else
	{
		std::set<Session*> sset;
		sset.insert(ssn);
		mMapSocketSessions.insert(std::make_pair(socketId, sset));
	}
	return ssn;
}

std::set<Session*>* SessionManager::getSessionsBySocket(uint32 socketId)
{
	auto itr = mMapSocketSessions.find(socketId);
	if (itr != mMapSocketSessions.end())
		return &itr->second;

	return NULL;
}

void SessionManager::removeSsnsBySocket(uint32 socketId, Session* ssn)
{
	auto itr = mMapSocketSessions.find(socketId);
	if (itr != mMapSocketSessions.end())
	{
		std::set<Session*>& sset = itr->second;
		sset.erase(ssn);
	}
}

Socket* SessionManager::addSktsBySsn(SessionID sessionId, Socket* skt)
{
	auto itr = mMapSessionSockets.find(sessionId);
	if (itr != mMapSessionSockets.end())
	{
		std::set<Socket*>& sset = itr->second;
		if (sset.find(skt) == sset.end())
			sset.insert(skt);
		else
			return NULL;
	}
	else
	{
		std::set<Socket*> sset;
		sset.insert(skt);
		mMapSessionSockets.insert(std::make_pair(sessionId, sset));
	}
	return skt;
}

std::set<Socket*>* SessionManager::getSktsBySsn(SessionID sessionId)
{
	auto itr = mMapSessionSockets.find(sessionId);
	if (itr != mMapSessionSockets.end())
		return &itr->second;

	return NULL;
}

void SessionManager::removeSktsBySsn(SessionID sessionId, Socket* skt)
{
	auto itr = mMapSessionSockets.find(sessionId);
	if (itr != mMapSessionSockets.end())
	{
		std::set<Socket*>& sset = itr->second;
		sset.erase(skt);
	}
}

