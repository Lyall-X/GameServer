#include "Shared.hpp"

Mutex::Mutex(void)
{
#if (defined(WIN32) || defined(WIN64))
	m_pMutex = new CRITICAL_SECTION;
	InitializeCriticalSection((CRITICAL_SECTION*)m_pMutex);
#else
	pthread_mutex_init(&m_mutex, NULL);
#endif
}

Mutex::~Mutex(void)
{
#if (defined(WIN32) || defined(WIN64))
	DeleteCriticalSection((CRITICAL_SECTION*)m_pMutex);
	delete (CRITICAL_SECTION*)m_pMutex;
	m_pMutex = NULL;
#else
	pthread_mutex_destroy(&m_mutex);
#endif
}

void Mutex::lock()
{
#if (defined(WIN32) || defined(WIN64))
	EnterCriticalSection((CRITICAL_SECTION*)m_pMutex);
#else
	pthread_mutex_lock(&m_mutex);
#endif
}

void Mutex::unlock()
{
#if (defined(WIN32) || defined(WIN64))
	LeaveCriticalSection((CRITICAL_SECTION*)m_pMutex);
#else
	pthread_mutex_unlock(&m_mutex);
#endif
}

MutexGuard::MutexGuard(Mutex& mutex) :
m_mutex(m_mutex)
{
	m_mutex.lock();
}

MutexGuard::~MutexGuard()
{
	m_mutex.unlock();
}
