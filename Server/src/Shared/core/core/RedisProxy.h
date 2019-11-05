#pragma once

struct RedisRequest
{
	std::string cmd;
	Object* thisObject;
	EventCallback callback;
	std::vector<std::string> parstr;
};

class RedisEvent : public Event
{
public:
	static const char* CONNECT;
public:
	std::vector<std::string> cmdstr;
	std::vector<std::string> backstr;
	std::vector<std::string> parstr;
};

class RedisProxy : public EventDispatcher
{
public:
	RedisProxy();
	~RedisProxy();
public:
	bool Connect(const std::string& host, short port);
	bool AsyncConnect(const std::string& host, short port);
	void sendCmd(const std::string& cmd, EventCallback callback, Object* thisObject);
	void sendCmd(const std::string& cmd, EventCallback callback, Object* thisObject, std::vector<std::string>& parstr);

	std::string get(const std::string& key);
	std::string get(int32 Id, const std::string& key);
	std::string get(const std::string& table, int32 Id, const std::string& key);
	void set(const std::string& key, const std::string& value);
	void set(int32 Id, const std::string& key, const std::string& value);
	void set(const std::string& table, int32 Id, const std::string& key, const std::string& value);
	std::vector<std::string> sendCommand(const char* format, ...);
	std::vector<std::string> sendCommand(const char *format, va_list ap);
	std::string getError();
	void loop();
	void Destroy();
protected:
	uint32 onThreadProcess(Threader& threader);
public:
	void*								mContext;
	void*								mReply;
	Mutex								mMutex;
	std::list<RedisRequest>				mRedisRequest;
	std::list<RedisEvent*>				mRedisResponse;
	Threader*							mThread;
	std::string							mHost;
	uint16								mPort;
};
