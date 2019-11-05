#include "stdafx.h"

Robot::Robot(Socket* socket):
mSocket(socket),
mIsLogin(false),
mIsMove(false),
mSpeed(0.f),
mInMap(false),
mStartFind(false),
mFindTime(0.f),
mFindTotalTime(0.f)
{
	mAstar.create(100, 100, INSTANCE(RobotManager).mMapData);
	mTimer.addEventListener((EventCallback)&Robot::onTimerSyncPos, this, 1.f);
}

Robot::~Robot()
{
}

void Robot::doLogin(const std::string& user, const std::string& pwd)
{
	this->user = user;
	this->password = pwd;

	NetLoginReq req;
	req.user = user;
	req.password = password;

	//LOG_DEBUG(LogSystem::csl_color_green, "%s login request", user.c_str());
	sendPacket(req);
}

void Robot::sendPacket(Packet& packet)
{
	if (!mSocket)
		return;

	static char sPacketBuffer[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(sPacketBuffer, PACKET_MAX_LENGTH);
	in << packet;
	//AES aes(sKey);
	//aes.Cipher(in.datas(), in.wpos());
	Shared::XOR((char*)in.datas(), in.wpos(), sKey);
	char* datas = new char[in.wpos()];
	memcpy(datas, in.datas(), in.wpos());
	AES aes1(sKey);
	aes1.InvCipher(datas, in.wpos());
	mSocket->sendBuffer(in.datas(), in.wpos());
	//mSocket->sendPacket(packet);
}

int32 Robot::onTimerSyncPos(TimerEvent& e)
{
	if (mSendPos.x != mPosX || mSendPos.y - mPosY)
	{
		NetEntityMoveNotify nfy;
		nfy.guid = mGuid;
		nfy.x = mPosX;
		nfy.y = mPosY;
		nfy.startDir = mDirPosition;
		//sendPacket(nfy);
		mSendPos.x = mPosX;
		mSendPos.y = mPosY;
		//LOG_INFO("[%s] moveto x:%d y:%d point count:%d ", user.c_str(), mPosX, mPosY, mPoints.size());
	}
	return 0;
}

void Robot::update(float32 time, float32 delay)
{
	return;
	mTimer.update(time, delay);
	if (!mIsLogin || !mInMap)
		return;

	if (mIsMove && mPoints.size() <= 0)
	{
		mIsMove = false;
	}
	if (!mStartFind)
	{
		if (mPoints.size() <= 0)
		{
			mStartFind = true;
			mFindTime = 0.f;
			mFindTotalTime = 5 + rand() % 10;
		}
	}

	if (mStartFind)
	{
		mFindTime += delay;

		if (mFindTime >= mFindTotalTime)
		{
			float32 cell = INSTANCE(RobotManager).mCell;
			//Point2 pt = Utils::Vector2ToPoint2(mPos, cell, cell);
			Point2 pt;
			pt.x = rand() % 100;
			pt.y = rand() % 100;
			if (mAstar.find(mPosX, mPosY, pt.x, pt.y))
			{
				Astar::points(mPoints, mAstar.end_node());
				if (mPoints.size() > 0)
					mPoints.erase(mPoints.begin());

				if (mPoints.size() > 0)
				{
					NetEntityMoveToNotify nfy;
					nfy.guid = mGuid;
					nfy.x = pt.x;
					nfy.y = pt.y;
					sendPacket(nfy);

					//LOG_INFO("[%s] [x:%d y:%d] move to %d %d", user.c_str(), mPosX, mPosY, pt.x, pt.y);
				}
			}

			mFindTime = 0.f;
			mStartFind = false;
		}
	}

	if (!mIsMove)
		getPoint();

	if (mIsMove)
	{
		Vector2 pos = mPos;
		float32 dis = pos.distance(mTar);

		Vector2 pos1 = pos + mDir * mSpeed * delay;
		float32 dis1 = pos1.distance(mTar);
		if (dis1 < 0.5f || dis < dis1)
		{
			mIsMove = false;
			getPoint();
		}
		else
		{
			mPos = pos1;
			//LOG_INFO("setPos x:%0.2f y:%0.2f point count:%d", mPos.x, mPos.y, mPoints.size());
		}
	}
}

void Robot::getPoint()
{
	if (mPoints.size() <= 0)
		return;
	Point2 pt = mPoints.front();
	mPoints.pop_front();
	mIsMove = true;
	

	float32 cell = INSTANCE(RobotManager).mCell;
	mTar = Utils::Point2ToVector2(pt, cell, cell);

	mDir = mTar - mPos;
	mDir.normalise();
	mDirPosition = Utils::CalculateDir(mDir);

	mPosX = pt.x;
	mPosY = pt.y;
}
