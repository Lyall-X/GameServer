#pragma once


enum RoomPlayerState :  char
{
	RPS_None = 0,			// 正常
	RPS_Ready,				// 准备
	RPS_Game,				// 游戏中
	RPS_Observed,			// 观战中
};

class RoomPlayer : public Object
{
public:
	Player*			mPlayer = NULL;
	uint32			mUserId = 0;
	Guid			mInsId = 0;
	std::string		mName;
	uint8			mState = RPS_None;
	uint8			mPos = 0;
	RoomPlayerState GetState() { return (RoomPlayerState)mState; }
	void SetState(RoomPlayerState rps) { mState = rps; }
	void SetPos(uint8 pos) { mPos = pos; }

	uint8 GetPos() { return mPos; }
public:
	bool operator << (RoomPlayerInfo& info);
	bool operator >> (RoomPlayerInfo& info);
};

class Room : public Object
{
	friend class RoomModule;
public:
	Room();
	~Room();
	void sendPacketToAll(Packet& packet);
	void RemovePlayer(uint32 userId);
	void SetMaster(RoomPlayer* master) { mMaster = master; }
	void SetPassword(const std::string& password) { mPassword = password; }
	void SetName(const std::string& name) { mName = name; }
	void SetGameInsId(uint32 insId) { mGameInsId = insId; }

	bool operator >> (RoomInfo& info);
	bool DoLeave(Player* aPlr);
	bool DoLeave(uint32 userId);
	bool IsFull();
	bool IsCanStart();
	bool DoAllStart();
	bool IsCanAdd(Player* aPlr);

	uint32 GetInsId() { return mId; }
	uint32 GetMaxCount() { return mMaxCount; }
	uint32 GetRoomPlayerCount() { return (uint32)mRoomPlayers.size(); }
	uint32 GetGameInsId() { return mGameInsId; }

	RoomPlayer* DoEnter(Player* aPlr, bool isMaster = false);
	RoomPlayer* FindPlayer(uint32 userId);
	RoomPlayer* AddPlayer(RoomPlayer* roomPlr);
	RoomPlayer* GetRoomPlayer(uint32 idx);
	RoomPlayer* GetMaster() { return mMaster; }
	Player* GetMasterPlayer() { return mMaster ? mMaster->mPlayer : NULL; }
	const std::string& GetPassword() { return mPassword; }
	const std::string& GetName() { return mName; }
protected:
	uint8 GetEnterPos();
protected:
	void OnCreate(uint32 userId);
	void OnClose();
	void OnEnter(uint32 userId);
	void OnLeave(uint32 userId);
	void OnChangeMaster(uint32 oldUserId, uint32 newUserId);
	void OnChangeState(uint32 userId, uint8 oldState, uint8 state);
protected:
	uint32									mId;
	std::string								mScript;
	RoomPlayer*								mMaster;
	std::string								mName;
	std::string								mPassword;
	uint32									mMaxCount;			// 房间最多人数
	std::map<uint8, RoomPlayer*>			mRoomPlayers;
	uint32									mGameInsId;			// 当前游戏ID
private:
};