#include "Shared.hpp"

Threader::Threader(void):
m_thisObject(NULL),
m_callback(NULL),
m_hThread(NULL),
m_dwThreadID(0),
m_eStatus(eNone),
m_strName(""),
m_isActive(false)
{
}

Threader::~Threader(void)
{
	this->Eixt();
	m_thisObject = NULL;
}

bool Threader::CreateThread(ThreadCallBack callback, Object* thisObject, bool isRun /* = true */)
{
	m_thisObject = (Object*)thisObject;
	m_callback = callback;
	if (isRun)
		m_eStatus = eRuning;

#if (defined(WIN32) || defined(WIN64))
	m_hThread = (void*)_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *))Threader::_ThreadProcess, this, isRun ? 0 : CREATE_SUSPENDED, (unsigned int*)&m_dwThreadID);
#else
	pthread_create((pthread_t*)pThread->m_hThread, NULL, CThread::_ThreadProcess, pThread);
#endif // 
	m_isActive = true;
	return true;
}

void Threader::Eixt()
{
	if (m_eStatus == eSuspend)
	{
		this->Resume();
	}
	m_isActive = false;
	
	if (m_hThread == NULL)
		return;

#if (defined(WIN32) || defined(WIN64))
	WaitForSingleObject(m_hThread, INFINITE);
	if (m_hThread)
		CloseHandle(m_hThread);
#else
	pthread_join((pthread_t)m_hThread,NULL);
	pthread_cancel((pthread_t)m_hThread);
#endif //

	m_hThread = NULL;
	m_dwThreadID = 0;
}

#ifdef WIN64
unsigned int Threader::_ThreadProcess(void* lp)
#else
void* Threader::_ThreadProcess(void* lp)
#endif // WIN32
{
	Threader* pThread = (Threader*)lp;
	//DWORD dwReturn = (pThread->m_thisObject->*(pThread->m_callback))(*pThread);
	DWORD dwReturn = (pThread->m_thisObject->*(pThread->m_callback))(*pThread);

	//DWORD dwReturn = pThread->m_callback(pThread->m_thisObject);
	pThread->m_eStatus = eEnd;
#ifdef WIN32
	return dwReturn;
#else
	return (void*)dwReturn;
#endif
}

void Threader::Suspend()
{
	m_eStatus = eSuspend;
#ifdef WIN32
	::SuspendThread(m_hThread);
#endif // WIN32
}

void Threader::Resume()
{
	m_eStatus = eRuning;
#ifdef WIN32
	::ResumeThread(m_hThread);
#endif // WIN32
}

void Threader::SetName( const char* pszName )
{
	m_strName = pszName;
}

const char* Threader::GetName()
{
	return m_strName.c_str();
}

ThreadStatus Threader::GetStatus()
{
	return m_eStatus;
}

void Threader::sleep(unsigned int dwMilliseconds)
{
#if (defined(WIN32) || defined(WIN64))
	::Sleep((DWORD)dwMilliseconds);
#else
	usleep(dwMilliseconds * 1000);
#endif
}

bool Threader::Active()
{
	return m_isActive;
}

unsigned int Threader::GetThreadID()
{
	return m_dwThreadID;
}