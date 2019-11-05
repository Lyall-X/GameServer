#pragma once

class SocketListener;
class SocketEvent;
class RedisEvent;
class DBConnector;

class Application : public Object
{
public:
	Application();
	~Application();

	bool Initialize();
	bool Update();
	bool Destroy();

	SocketListener* getDBServer() { return dbServer; }
	DBConnector* getDBConnector() { return dbConnector; }
	bool GetRun() { return mRun; }
	void sendPacketToTarget(Packet& packet, Socket* socket);
protected:
	int32 onDBAccept(SocketEvent& e);
	int32 onDBRecv(SocketEvent& e);
	int32 onDBExit(SocketEvent& e);
protected:
	int32 RedisConnect(RedisEvent& e);
	int32 RedisAuth(RedisEvent& e);
	int32 onMysql(DBEvent& e);
protected:
	SocketListener* dbServer;
	DBConnector* dbConnector;
	bool mRun;
private:
};

#define sDBConnector (*(sApp.getDBConnector()))
#define sDBSvr (*(sApp.getDBServer()))