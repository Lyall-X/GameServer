#pragma once

class CmdExecute;
class CmdEvent : public Event
{
public:
	const char* CMDEVENT;
public:
	CmdExecute* cmdExecute;
};

class CmdExecute
{
public:
	std::string cmd;
	std::vector<std::string> params;
};

class CmdDispatcher : public EventDispatcher
{
	friend class CmdThreadProcessor;
public:
	CmdDispatcher();
	~CmdDispatcher();
	void update(float32 time, float32 delay);
	void Destroy();
protected:
	uint32 onThreadProcess(Threader& threader);
protected:
	Threader*											m_pThread;
	Mutex												m_mutex;
	std::queue<CmdExecute*>								mQueueCmd;
};