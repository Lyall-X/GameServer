#include "Shared.hpp"

#pragma warning(push)
#pragma warning(disable : 4996)

#define LOG_BUFFER_SIZE		1024 * 4 * 2

uint32 LogSystem::ThreadProcess(Threader& therader)
{
	while (therader.Active())
	{
		processOutputer();
		Threader::sleep(1);
	}
	return 0;
}

LogSystem::LogSystem(void):
m_mutex()
{
	m_log.clear();
	m_pThread = new Threader;
	m_pThread->CreateThread((ThreadCallBack)&LogSystem::ThreadProcess, this);

	m_buffer = new char[LOG_BUFFER_SIZE];
}

LogSystem::~LogSystem(void)
{
	m_pThread->Eixt();
	SAFE_DELETE(m_pThread);
	SAFE_DELETE_ARRAY(m_buffer);
	m_log.clear();
}

void LogSystem::append(LogOutputer* outputer)
{
	m_log.push_back(outputer);
}

void LogSystem::info(const char* formt, ...)
{
	va_list va;
	va_start(va, formt);

	vsnprintf(m_buffer, LOG_BUFFER_SIZE, formt, va);
	stContent st;

	std::string now_time = "";
	DateTime::Now(now_time);
	st.context = "[";
	st.context += now_time;
	st.context += "]";
	st.context += "[INFOR]";
	st.context += m_buffer;
	st.level = logLevel_info;
	st.color = 0;
	va_end(va);
	addContent(st);
}

void LogSystem::warn(const char* formt, ...)
{
	va_list va;
	va_start(va, formt);
	vsnprintf(m_buffer, LOG_BUFFER_SIZE, formt, va);
	stContent st;

	std::string now_time = "";
	DateTime::Now(now_time);
	st.context = "[";
	st.context += now_time;
	st.context += "]";
	st.context += "[WARNI]";
	st.context += m_buffer;
	st.level = logLevel_warn;
	st.color = 0;

	va_end(va);

	addContent(st);
}

void LogSystem::error(const char* formt, ...)
{
	va_list va;
	va_start(va, formt);
	vsnprintf(m_buffer, LOG_BUFFER_SIZE, formt, va);
	stContent st;

	std::string now_time = "";
	DateTime::Now(now_time);
	st.context = "[";
	st.context += now_time;
	st.context += "]";
	st.context += "[ERROR]";
	st.context += m_buffer;
	st.level = logLevel_error;
	st.color = 0;

	va_end(va);

	addContent(st);
}

void LogSystem::processOutputer()
{
	m_mutex.lock();
	if (m_contexts.size() <= 0)
	{
		m_mutex.unlock();
		Threader::sleep(100);
	}
	else
	{
		stContent& st = m_contexts.front();
		m_mutex.unlock();

		LogOutputer* pLog = NULL;
		LogOutputerListItr itr;
		for (itr = m_log.begin();
			itr != m_log.end();
			++itr)
		{
			pLog = (*itr);
			pLog->Outputer(st.context.c_str(), st.color, st.level);
		}

		m_mutex.lock();
		m_contexts.pop();
		m_mutex.unlock();
	}
}

void LogSystem::addContent(stContent& content)
{
	m_mutex.lock();
	m_contexts.push(content);
	m_mutex.unlock();
}

void LogSystem::outHex(const void* data, int len)
{
	std::string str;
	char s[10] = {0};
	int startPos = 0;
	void* temp = (void*)data;
	str = "hex:\n";
	for (int i = 0; i < len; ++i)
	{
		sprintf_s(s, sizeof(s), "%02x", ((unsigned char*)data)[i]);
		str += s;
	}
	
	stContent st;

	std::string now_time = "";
	DateTime::Now(now_time);
	st.context = "[";
	st.context += now_time;
	st.context += "]";

	st.context += str.c_str();
	st.level = logLevel_hex;

	addContent(st);
}

bool LogSystem::assertFail( const char* pszExpression, const char* pszFile, const char* pszFunction, const int iLine )
{
	char szTempBuffer[1024] = {0};
	sprintf_s(szTempBuffer, sizeof(szTempBuffer), "Expression: %s\r\nFile: %s\r\nFunc: %s\r\nLine: %d\r\n", pszExpression, pszFile, pszFunction, iLine);
#if (defined(WIN32) || defined(WIN64))
	int iButtonResult = ::MessageBoxA(
		NULL, 
		szTempBuffer, 
		"assert", 
		MB_ABORTRETRYIGNORE);

	switch(iButtonResult)
	{
	case IDABORT:
		exit(EXIT_FAILURE);
	default:
	case IDRETRY:
		__debugbreak();
		break;
	case IDIGNORE:
		break;
	}
#else
	printf(szTempBuffer);
#endif // WIN32

	return false;
}

bool LogSystem::outMessagebox(const char* formt, const char* pszFile, const char* pszFunction, const int iLine, ...)
{
	va_list va;
	va_start(va, formt);
	char buffer[2048] = {0};
	vsnprintf(buffer, 2048, formt, va);
	va_end(va);

	return LogSystem::assertFail(buffer, pszFile, pszFunction, iLine);
}

void LogSystem::debug( int color, const char* formt, ... )
{
	va_list va;
	va_start(va, formt);
	vsnprintf(m_buffer, LOG_BUFFER_SIZE, formt, va);
	va_end(va);

	stContent st;

	std::string now_time = "";
	DateTime::Now(now_time);
	st.context = "[";
	st.context += now_time;
	st.context += "]";
	st.context += "[DEBUG]";
	st.context += m_buffer;
	st.level = logLevel_debug;
	st.color = color;

	m_mutex.lock();
	m_contexts.push(st);
	m_mutex.unlock();

}

void LogSystem::error_line( const char* formt, const char* file, const int line, const char* func, ... )
{
	va_list va;
	va_start(va, formt);
	int buf_len = vsnprintf(m_buffer, LOG_BUFFER_SIZE, formt, va);
	va_end(va);

	buf_len = sprintf_s(m_buffer, LOG_BUFFER_SIZE - buf_len, "file:%s line:%d func:%s e:%s", file, line, func, m_buffer);
	stContent st;

	std::string now_time = "";
	DateTime::Now(now_time);
	st.context = "[";
	st.context += now_time;
	st.context += "]";

	st.context += m_buffer;
	st.level = logLevel_error;
	st.color = 0;

	m_mutex.lock();
	m_contexts.push(st);
	m_mutex.unlock();

}

void LogSystem::debug_line( int color, const char* formt, const char* file, const int line, const char* func, ... )
{
	va_list va;
	va_start(va, formt);
	int buf_len = vsnprintf(m_buffer, LOG_BUFFER_SIZE, formt, va);
	va_end(va);

	buf_len = sprintf_s(m_buffer, LOG_BUFFER_SIZE - buf_len, "file:%s line:%d func:%s e:%s", file, line, func, m_buffer);
	stContent st;

	std::string now_time = "";
	DateTime::Now(now_time);
	st.context = "[";
	st.context += now_time;
	st.context += "]";

	st.context += m_buffer;
	st.level = logLevel_debug;
	st.color = color;

	m_mutex.lock();
	m_contexts.push(st);
	m_mutex.unlock();
}

#pragma warning(pop)