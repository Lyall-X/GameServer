#pragma once

class SessionManager
{
public:
	SessionManager();
	~SessionManager();
	Session* createSession(Socket* s, SessionID sessionId = 0);
	Session* getSession(SessionID sessionId);
	Session* addSession(Session* ssn);
	void removeSession(SessionID sessionId);
	Session* addSessionBySocket(uint32 socketId, Session* ssn);
	Session* getSessionBySocket(uint32 socketId);
	void removeSessionBySocket(uint32 socketId);
	
	Session* addSessionsBySocket(uint32 socketId, Session* ssn);
	std::set<Session*>* getSessionsBySocket(uint32 socketId);
	void removeSsnsBySocket(uint32 socketId, Session* ssn);
	std::map<uint32, std::set<Session*>>& getMapSocketSessions() { return mMapSocketSessions; }
	Socket* addSktsBySsn(SessionID sessionId, Socket* skt);
	std::set<Socket*>* getSktsBySsn(SessionID sessionId);
	void removeSktsBySsn(SessionID sessionId, Socket* skt);
protected:
	std::map<SessionID, Session*>					mMapSession;
	std::map<uint32, Session*>						mMapSocketSession;
	std::map<uint32, std::set<Session*>>			mMapSocketSessions;
	std::map<SessionID, std::set<Socket*>>				mMapSessionSockets;
};