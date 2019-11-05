#include "Shared.hpp"
#include "RedisProxy.h"
#include "hiredis.h"
#include "async.h"

const char* RedisEvent::CONNECT = "onConnect";

RedisProxy::RedisProxy():
mHost(""),
mPort(0)
{
	mThread = new Threader;
	mThread->CreateThread((ThreadCallBack)&RedisProxy::onThreadProcess, this);
}

RedisProxy::~RedisProxy()
{
	Destroy();
}

void RedisProxy::Destroy()
{
	if (mContext)
		redisFree((redisContext*)mContext);
	mContext = NULL;
	mThread->Eixt();
	SAFE_DELETE(mThread);
}


bool RedisProxy::Connect(const std::string& host, short port)
{
	mContext = redisConnect(host.c_str(), port);
	redisContext* context = (redisContext*)mContext;
	if (context->err) {
		printf("%s %d", context->errstr, context->err);
		mContext = NULL;
		return false;
	}

	mHost = host;
	mPort = port;
	return true;		
}

bool RedisProxy::AsyncConnect(const std::string& host, short port)
{
	mHost = host;
	mPort = port;
	return true;
}

std::string RedisProxy::get(const std::string& key)
{
	redisReply* reply = (redisReply*)redisCommand((redisContext*)mContext, "GET %s", key.c_str());
	std::string str = reply->str;
	freeReplyObject(reply);
	return str;
}

std::string RedisProxy::get(int32 Id, const std::string& key)
{
	redisReply* reply = (redisReply*)redisCommand((redisContext*)mContext, "GET %s_%d", key.c_str(), Id);
	std::string str = reply->str;
	freeReplyObject(reply);
	return str;
}

std::string RedisProxy::get(const std::string& table, int32 Id, const std::string& key)
{
	redisReply* reply = (redisReply*)redisCommand((redisContext*)mContext, "GET %s_%d_%s", key.c_str(), Id, table.c_str());
	std::string str = reply->str;
	freeReplyObject(reply);
	return str;
}

void RedisProxy::set(const std::string& key, const std::string& value)
{
	redisReply* reply = (redisReply*)redisCommand((redisContext*)mContext, "SET %s %s", key.c_str(), value.c_str());

	if (!(reply->type == REDIS_REPLY_STATUS && (strcmp(reply->str, "OK") == 0 || strcmp(reply->str, "ok") == 0)))
	{
		printf("Failed to execute command key[%s] value[%s]\n", key.c_str(), value.c_str());
		freeReplyObject(reply);
		return;
	}
	freeReplyObject(reply);
}

void RedisProxy::set(int32 Id, const std::string& key, const std::string& value)
{
	redisReply* reply = (redisReply*)redisCommand((redisContext*)mContext, "SET %s_%d %s", key.c_str(), Id, value.c_str());

	if (!(reply->type == REDIS_REPLY_STATUS && (strcmp(reply->str, "OK") == 0 || strcmp(reply->str, "ok") == 0)))
	{
		printf("Failed to execute command key[%s_%d] value[%s]\n", key.c_str(), Id, value.c_str());
		freeReplyObject(reply);
		return;
	}
	freeReplyObject(reply);
}

void RedisProxy::set(const std::string& table, int32 Id, const std::string& key, const std::string& value)
{
	redisReply* reply = (redisReply*)redisCommand((redisContext*)mContext, "SET %s_%d_%s %s", key.c_str(), Id, table.c_str(), value.c_str());

	if (!(reply->type == REDIS_REPLY_STATUS && (strcmp(reply->str, "OK") == 0 || strcmp(reply->str, "ok") == 0)))
	{
		printf("Failed to execute command key[%s_%d_%s] value[%s]\n", key.c_str(), Id, table.c_str(), value.c_str());
		freeReplyObject(reply);
		return;
	}
	freeReplyObject(reply);
}

std::vector<std::string> RedisProxy::sendCommand(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	std::vector<std::string> backstr = sendCommand(format, ap);
	va_end(ap);
	return backstr;
}

std::vector<std::string> RedisProxy::sendCommand(const char *format, va_list ap)
{
	std::vector<std::string> backstr;
	redisReply* reply = (redisReply*)redisvCommand((redisContext*)mContext, format, ap);
	if (reply == NULL)
	{
		backstr.push_back("sendCommand error");
		return backstr;
	}

	switch (reply->type)
	{
	case REDIS_REPLY_INTEGER:
		backstr.push_back(Shared::int64tostr(reply->integer));
		break;
	case REDIS_REPLY_ARRAY:
		for (uint32 i = 0; i < reply->elements; ++i)
		{
			redisReply* redisRy = reply->element[i];
			if (redisRy == NULL) continue;
			backstr.push_back(redisRy->str ? redisRy->str : "");
		}
		break;
	case REDIS_REPLY_STRING:
		if (reply->str)
			backstr.push_back(reply->str);
		break;
	case REDIS_REPLY_STATUS:
		break;
	case REDIS_REPLY_ERROR:
		printf("Failed to execute command key[%s] err[%s]\n", format, reply->str);
		break;
	default:
		printf("Failed to execute command key[%s]\n", format);
		break;
	}
	
	freeReplyObject(reply);
	return backstr;
}

void RedisProxy::sendCmd(const std::string& cmd, EventCallback callback, Object* thisObject)
{
	RedisRequest request;
	request.cmd = cmd;
	request.thisObject = thisObject;
	request.callback = callback;

	mMutex.lock();
	mRedisRequest.push_back(request);
	mMutex.unlock();

	addEventListener(cmd, callback, thisObject);
}

void RedisProxy::sendCmd(const std::string& cmd, EventCallback callback, Object* thisObject, std::vector<std::string>& parstr)
{
	RedisRequest request;
	request.cmd = cmd;
	request.thisObject = thisObject;
	request.callback = callback;
	request.parstr = parstr;

	mMutex.lock();
	mRedisRequest.push_back(request);
	mMutex.unlock();

	addEventListener(cmd, callback, thisObject);
}

std::string RedisProxy::getError()
{
	if (!mContext)
		return "";

	return ((redisContext*)mContext)->errstr;
}

void RedisProxy::loop()
{
	if (mRedisResponse.size() <= 0) return;

	mMutex.lock();
	RedisEvent* redisEvent = mRedisResponse.front();
	mRedisResponse.pop_front();
	mMutex.unlock();
	Dispatch(*redisEvent);
	delete redisEvent;
}

uint32 RedisProxy::onThreadProcess(Threader& threader)
{
	while (threader.Active())
	{
		Shared::Sleep(1);
		if (mContext == NULL)
		{
			if (mHost == "" || mPort == 0) continue;
			int timeout = 1000;
			struct timeval tv;
			tv.tv_sec = timeout / 1000;
			tv.tv_usec = timeout * 1000;
			mContext = redisConnectWithTimeout(mHost.c_str(), mPort, tv);
			redisContext* context = (redisContext*)mContext;
			if (context->err) {
				LOG_ERROR("error:[%s] [%s] [%d]",context->errstr, mHost.c_str(), mPort);
				mContext = NULL;
				continue;
			}
			
			RedisEvent* redisEvent = new RedisEvent;
			redisEvent->event = RedisEvent::CONNECT;
			mMutex.lock();
			mRedisResponse.push_back(redisEvent);
			mMutex.unlock();
		}
		if (mContext == NULL) continue;
		if (mRedisRequest.size() <= 0) continue;

		mMutex.lock();
		RedisRequest request = mRedisRequest.front();
		mRedisRequest.pop_front();
		mMutex.unlock();
		RedisEvent* redisEvent = new RedisEvent;
		redisEvent->event = request.cmd;
		redisEvent->backstr = sendCommand(request.cmd.c_str());
		redisEvent->parstr = request.parstr;
		Shared::split(request.cmd, redisEvent->cmdstr, " ");
		mMutex.lock();
		mRedisResponse.push_back(redisEvent);
		mMutex.unlock();
	}
	return 0;
}
