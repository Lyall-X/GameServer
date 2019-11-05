#pragma once

class Application : public EventDispatcher
{
public:
	Application();
	~Application();

	bool Initialize();
	bool Update();
	bool Destroy();
	SocketListener* getWorldServer() { return worldServer; }
	SocketClient* getDBServer() { return dbServer; }
	Module* getModule(const std::string& name);
	Module* addModule(Module* module);
	void removeModule(const std::string& name, bool free = true);
	virtual bool onEnterWorld(Player* player, Dictionary& dict);
	virtual bool onLeaveWorld(Player* player, Dictionary& dict);
	virtual bool onLoad(Player* player, Dictionary& bytes);
	virtual bool onSave(Player* player, Dictionary& bytes);
	bool GetRun() { return mRun; }
public:
	////void sendPacketToDB(Packet& packet, Session* ssn);
	void sendPacketToDB(Packet& packet, Player* aPlr);
	void sendPacketToDB(Packet& packet);
	void doSessionLeaveWorld(Session* ssn);
	void doPlayerLeaveWorld(Player* aPlr);
	void doPlayerSave(Player* plr, Dictionary& bytes);
protected:
	int onWorldAccept(SocketEvent& e);
	int onWorldRecv(SocketEvent& e);
	int onWorldExit(SocketEvent& e);

	int onDBConnect(SocketEvent& e);
	int onDBRecv(SocketEvent& e);
	int onDBExit(SocketEvent& e);
	int onDBException(SocketEvent& e);
protected:
	// 没有Player对象时发来的消息
	bool checkSessionMessage(uint32 msgId);
	void addSessionMessage(uint32 msgId);
protected:
	int32 onRefresh(CmdEvent& e);
	int32 onRefreshLua(CmdEvent& e);
	void OnInitialize();
public:
	int32 onClose(CmdEvent& e);
	int32 onAddFrd(CmdEvent& e);
	int32 onTest(CmdEvent& e);
	int32 onDump(CmdEvent& e);
protected:
	int32 onRedisConnect(RedisEvent& e);
	int32 onRedisAuth(RedisEvent& e);
	int32 RedisCallback1(RedisEvent& e);
protected:
	SocketListener* worldServer;
	SocketClient* dbServer;
	std::map<std::string, Module*> mMapModule;
	float64 mTime;
	float64 mDelay;
	float64 mTotalTime;
	int32 mFPS;
	float64 mFPSTimer;
	std::set<uint32>			mSetSessionMessage;
	bool						mRun;
private:
};

#define GetModule(class_name) ((class_name*)INSTANCE(Application).getModule(#class_name))
#define GetWorldServer() (INSTANCE(Application).getWorldServer())
#define GetDBServer() (INSTANCE(Application).getDBServer())