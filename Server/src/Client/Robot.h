#pragma once

class Robot : public Object
{
public:
	Robot(Socket* socket);
	~Robot();

	void doLogin(const std::string& user, const std::string& pwd);

	void sendPacket(Packet& packet);

	int32 onTimerSyncPos(TimerEvent& e);
public:
	void update(float32 time, float32 delay);
protected:
	void getPoint();
public:
	Socket* mSocket;
	Guid	mGuid;
	int32   mAccountId;
	bool	mIsLogin;
	bool	mInMap;
	std::string user;
	std::string password;
	int32 mPosX;
	int32 mPosY;
	std::list<Point2>		mPoints;
	Astar	mAstar;

	Vector2 mPos;
	Vector2 mTar;
	Vector2 mDir;
	bool mIsMove;
	uint8 mDirPosition;
	float mSpeed;
	bool mStartFind;
	float mFindTime;
	float mFindTotalTime;
	Point2 mSendPos;
	Timer mTimer;
};

