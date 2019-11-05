#pragma once

typedef std::queue<DBRequest*> QueueRequest;

class DBAngent : public Object
{
public:
	DBAngent();
	~DBAngent();
	bool Connect(cstring& host, cstring& user, cstring& password, cstring& name, int16 port, uint32 count);
	DBRequest* DispatchRequest(DBRequest* request);
	void Update();
protected:
	uint32 OnThreaderCallback(Threader& theader);
	void* GetCurrentMysql();
protected:
	std::vector<void*>		mMysqls;
	uint32					mMysqlIdx = 0;
	std::string				mHost;
	std::string				mUser;
	std::string				mPassword;
	std::string				mName;
	int16					mPort;
	Threader*				mThreader;
	Mutex					mMutex;
	QueueRequest			mQueueRequest;
	QueueRequest			mFinishQueueRequest;
};