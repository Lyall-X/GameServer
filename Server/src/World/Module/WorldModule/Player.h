#pragma once

class Session;

enum UserStatus : uint8
{
	UStatus_None,
	UStatus_Online,
	UStatus_Offline,
};

class Player : public Entity
{
	friend class Application;
public:
	Player();
	~Player();
	virtual bool Initialize();
	virtual bool Update(float time, float delay);
	virtual bool Destroy();
	void setSession(Session* s) { ssn = s; }
	void setAccId(uint32 plrId) { accountId = plrId; }
	void setUserId(uint32 rId) { userId = rId; }
	void bindSession(Session* ssn);
	void unbindSession();
	void setUser(const std::string& u) { user = u; }
	void SetStatus(UserStatus s) { mUserStatus = s; }
	void setHost(const std::string& host) { mHost = host; }
	bool GetOnline() { return mOnline; }
	void SetOnline(bool isOnline) { mOnline = isOnline; }

	uint32 getAccId() { return accountId; }
	uint32 getUserId() { return userId; }

	const std::string& getUser() { return user; }
	const std::string& getHost() { return mHost; }
	UserStatus GetStatus() { return mUserStatus; }
	Json::Value& GetJson();
	Session* getSession() { return ssn; }
	
public:
	void DoCreateCharacter(Dictionary& dict, DBRoleInfo& dbRoleInfo);
public:
	virtual void sendPacket(Packet& packet);
	virtual void sendBuffer(void* data, int32 count);
	virtual void sendPacketToView(Packet& packet);
	virtual void sendPacketToMap(Packet& packet);
	virtual void sendPacketToWorld(Packet& packet);
	virtual void sendPacketToTeam(Packet& packet);
	virtual void sendPacketToRoom(Packet& packet);
	virtual void sendPacketToTarget(Packet& packet, Entity* tar);
	virtual void sendRespnoseMsg(int32 msgId, std::vector<std::string>* msgParams = NULL);
	virtual void sendPacketToMsg(const std::string& msg);
	virtual void sendPacketToMsg(EnumChannel ec, const std::string& msg);
	virtual bool CanDestroy();
public:
	bool onLoad(Dictionary& dict);
	bool onSaveBegin(Dictionary& dict);
	bool onSave(Dictionary& dict);
	bool onSaveEnd(Dictionary& dict);
protected:
	bool onLoadJson(Dictionary& dict);
	bool onLoadProperty(Dictionary& dict);
	bool onSavejson(Dictionary& dict);
	bool onLoadJson(Json::Value& root);
	bool onSavejson(Json::Value& root);
	bool onSaveProperty(Dictionary& dict);
protected:
	virtual bool onEnterMap();
	virtual bool onLeaveMap();

	// tar enter this;
	virtual bool onEnterView(Entity* tar);
	virtual bool onLeaveView(Entity* tar);

	virtual bool onEnterPlayerView(Player* plr);
	virtual bool onLeavePlayerView(Player* plr);
protected:
	int32 onTimerSaveDB(TimerEvent& e);
protected:
	void OnCreate();
	void OnDispose();
	void OnEnter();
	void OnLeave();
public:
	void SetOnlineTimer(uint32 t) { mOnlineTimer = t; }
	void SetOfflineTimer(uint32 t) { mOfflineTimer = t; }
	void SetOnlineTime(uint32 t) { mOnlineTime = t; }
	void setDownloadBytes(uint64 bytes) { mDownloadBytes = bytes; }
	void setUploadBytes(uint64 bytes) { mUploadBytes = bytes; }
	void setTotalDownloadBytes(uint64 bytes) { mTotalDownloadBytes = bytes; }
	void setTotalUploadBytes(uint64 bytes) { mTotalUploadBytes = bytes; }

	uint32 GetOnlineTimer() { return mOnlineTimer; }
	uint32 GetOfflineTimer() { return mOfflineTimer; }
	uint32 GetOnlineTime() { return mOnlineTime; }

	uint64 getDownloadBytes() { return mDownloadBytes; }
	uint64 getUploadBytes() { return mUploadBytes; }
	uint64 getTotalDownloadBytes() { return mTotalDownloadBytes; }
	uint64 getTotalUploadBytes() { return mTotalUploadBytes; }
	void AddOldName(cstring& oldName);

protected:
	uint32				accountId;
	uint32				userId;
	std::string			user;
	std::string			password;
	Session*			ssn;
	UserStatus			mUserStatus;
	Json::Value			mJson;
	bool				mOnline;
	std::string			mHost;
protected:
	uint32				mOnlineTimer = 0;
	uint32				mOfflineTimer = 0;
	uint32				mOnlineTime = 0; // 在线总时长
	uint64				mDownloadBytes = 0;
	uint64				mUploadBytes = 0;
	uint64				mTotalDownloadBytes = 0;
	uint64				mTotalUploadBytes = 0;
	std::vector<std::string> mOldNames;
};