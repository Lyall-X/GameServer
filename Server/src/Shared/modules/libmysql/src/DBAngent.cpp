#include "mysql.hpp"
#include <mysql.h>


DBAngent::DBAngent()
{
	mThreader = new Threader;
	mThreader->CreateThread((ThreadCallBack)(&DBAngent::OnThreaderCallback), this);
}


DBAngent::~DBAngent()
{
	SAFE_DELETE(mThreader);
}

bool DBAngent::Connect(cstring& host, cstring& user, cstring& password, cstring& name, int16 port, uint32 count)
{
	mHost = host;
	mUser = user;
	mPassword = password;
	mName = name;
	mPort = port;

	for (uint32 i = 0; i < count; ++i)
	{
		MYSQL* mysql = new MYSQL;
		mMysqls.push_back(mysql);
		mysql_init(mysql);
		mysql_options(mysql, MYSQL_SET_CHARSET_NAME, "gb2312");
		my_bool reconnect = true;
		mysql_options(mysql, MYSQL_OPT_RECONNECT, &reconnect);
		if (!mysql_real_connect(mysql, mHost.c_str(), mUser.c_str(), mPassword.c_str(), mName.c_str(), mPort, NULL, 0))
			return false;
	}

	return true;
}

DBRequest* DBAngent::DispatchRequest(DBRequest* request)
{
	mMutex.lock();
	mQueueRequest.push(request);
	mMutex.unlock();
	return request;
}

void DBAngent::Update()
{
	uint32 queueSize = 0;
	mMutex.lock();
	queueSize = mFinishQueueRequest.size();
	mMutex.unlock();
	if (queueSize == 0)
		return;


	DBRequest* request = NULL;
	mMutex.lock();
	request = mFinishQueueRequest.front();
	mFinishQueueRequest.pop();
	mMutex.unlock();

	DBEvent event;
	event.event = DBEvent::DBEVENT_CMD;
	event.request = request;
	request->Dispatch(event);
	delete request;
	event.request = NULL;
}

uint32 DBAngent::OnThreaderCallback(Threader& theader)
{
	while (theader.Active())
	{
		Threader::sleep(1);
		bool queueSize = 0;
		mMutex.lock();
		queueSize = mQueueRequest.size();
		mMutex.unlock();
		if (queueSize == 0)
			continue;

		DBRequest* request = NULL;
		mMutex.lock();
		request = mQueueRequest.front();
		mQueueRequest.pop();
		mMutex.unlock();
		if (request == NULL)
			continue;
		request->mysql = GetCurrentMysql();
		if (request->mysql == NULL)
			continue;
		request->Execute();
		
		mMutex.lock();
		mFinishQueueRequest.push(request);
		mMutex.unlock();
	}
	return true;
}

void* DBAngent::GetCurrentMysql()
{
	if (mMysqlIdx >= mMysqls.size())
		return NULL;
	void* mysql = mMysqls[mMysqlIdx];
	mMysqlIdx++;
	if (mMysqlIdx >= mMysqls.size())
		mMysqlIdx = 0;
	return mysql;
}
