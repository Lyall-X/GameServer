#pragma once

class Threader;

typedef uint32 (Object::*ThreadCallBack) (Threader& therader);
enum ThreadStatus
{
	eNone,
	eRuning,
	eSuspend,
	eEnd,
};

class Threader
{
public:
	Threader(void);
	virtual ~Threader(void);
	bool CreateThread(ThreadCallBack callback, Object* thisObject, bool isRun = true);
public:
	void Eixt();
	void Suspend();
	void Resume();
	void SetName(const char* name);
	const char* GetName();
	ThreadStatus GetStatus();
	bool Active();
	unsigned int GetThreadID();
public:
	static void sleep(unsigned int dwMilliseconds);
private:
#ifdef WIN64
	static unsigned int __stdcall _ThreadProcess(void* lp);
#else
	static void* _ThreadProcess(void* lp);
#endif // WIN32
public:
	Object*					m_thisObject;
	ThreadCallBack			m_callback;
	void*					m_hThread;
	unsigned int			m_dwThreadID;
	ThreadStatus			m_eStatus;
	std::string				m_strName;
	bool					m_isActive;
private:

};
