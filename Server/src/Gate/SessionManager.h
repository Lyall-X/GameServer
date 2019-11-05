#pragma once

class Session;
class Entity;
class Player;
class Socket;
class Packet;
class SessionManager
{
public:
	SessionManager();
	~SessionManager();
	Session* createSession(Socket* s, uint64 sessionId = 0);
	Session* getSession(uint64 sessionId);
	Session* addSession(Session* ssn);
	void removeSession(uint64 sessionId);
	Session* addSessionBySocket(uint32 socketId, Session* ssn);
	Session* getSessionBySocket(uint32 socketId);
	void removeSessionBySocket(uint32 socketId);

	Entity* getEntity(uint32 unitId);
	Entity* addEntity(Entity* unit);
	void removeEntity(uint32 unitId);
	Player* getPlayer(uint32 playerId);
	Player* addPlayer(Player* player);
	void removePlayer(uint32 playerId);
	
	Session* addSessionsBySocket(uint32 socketId, Session* ssn);
	std::set<Session*>* getSessionsBySocket(uint32 socketId);
	void removeSessionsBySocket(uint32 socketId, Session* ssn);

	Socket* addSktsBySsn(uint64 sessionId, Socket* skt);
	std::set<Socket*>* getSktsBySsn(uint64 sessionId);
	void removeSktsBySsn(uint64 sessionId, Socket* skt);
	std::map<uint64, Session*>& getMapSession() { return mMapSession; }
protected:
	std::map<uint64, Session*>					mMapSession;
	std::map<uint32, Entity*>					mMapEntity;
	std::map<uint32, Player*>					mMapPlayer;
	std::map<uint32, Session*>					mMapSocketSession;
	std::map<uint32, std::set<Session*>>		mMapSocketSessions;
	std::map<uint64, std::set<Socket*>>			mMapSessionSockets;
};