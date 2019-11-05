#pragma once


class Session;
class Player : public Entity
{
public:
	void sendPacket(Packet& packet);

	void setSession(Session* s) { mSsn = s; }
	void setAccId(uint32 accId) { mAccId = accId; }
	void setUserId(uint32 userId) { mUserId = userId; }
	void setSocket(Socket* s) { mSocket = s; }

	Session* getSession() { return mSsn; }
	uint32 getAccId() { return mAccId; }
	uint32 getUserId() { return mUserId; }
	Socket* getSocket() { return mSocket; }

	void bindSsn(Session* ssn);
	void unbindSsn();
protected:

	uint32		mAccId = 0;
	uint32		mUserId = 0;
	Session*	mSsn = NULL;
	Socket*		mSocket = NULL;
	//DBAccount*					mAccount;
	//std::vector<DBUser*>		mLstUsers;
};