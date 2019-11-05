#pragma once

class Mutex
{
public:
	Mutex(void);
	virtual ~Mutex(void);
	void lock();
	void unlock();
private:
	void*					m_pMutex;
//
//#if (defined(WIN32) || defined(WIN64))
//#else//
//	pthread_mutex_t			m_mutex;//
//#endif
};

class MutexGuard
{
public:
	MutexGuard(Mutex& mutex);
	~MutexGuard();
protected:
	Mutex&			m_mutex;
};