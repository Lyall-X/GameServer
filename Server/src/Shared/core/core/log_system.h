#pragma once

class LogOutputer;
class Threader;
class ThreadProcessor;

class LogSystem : public Object
{
public:
	enum
	{
		csl_color_white,		// °×;
		csl_color_b_w_f_g,		// °×µ×ÂÌ×Ö;	
		csl_color_b_w_f_r,		// °×µ×ÂÌ×Ö;
		csl_color_b_w_f_b,		// °×µ×À¶×Ö;
		csl_color_red,			// ºì×Ö;
		csl_color_green,		// ÂÌ×Ö;
		csl_color_blue,			// À¶×Ö;
		csl_color_yellow,		// »Æ×Ö;
		csl_color_green_blue,	// ÂÌÀ¶;
		csl_color_red_blue,		// ºì À¶×Ö;
	};
	struct stContent
	{
		int				level;
		std::string		context;
		int				color;	// 0 1 2 3;
	};

	friend class LogThreadProcessor;
public:
	LogSystem(void);
	virtual ~LogSystem(void);
public:
	void append(LogOutputer* outputer);
	void info(const char* formt, ...);
	void warn(const char* formt, ...);
	void error(const char* formt, ...);
	void debug(int color, const char* formt, ...);
	void error_line(const char* formt, const char* file, const int line, const char* func, ...);
	void debug_line(int color, const char* formt, const char* file, const int line, const char* func, ...);
	void outHex(const void* data, int len);
public:
	static bool assertFail(const char* pszExpression, const char* pszFile, const char* pszFunction, const int iLine);
	static bool outMessagebox(const char* formt, const char* pszFile, const char* pszFunction, const int iLine, ...);
protected:
	uint32 ThreadProcess(Threader& therader);
private:
	void processOutputer();
	void addContent(stContent& content);
private:
	typedef std::vector<LogOutputer*>					LogOutputerList;
	typedef LogOutputerList::iterator					LogOutputerListItr;
private:

	LogOutputerList										m_log;
	Threader*											m_pThread;
	Mutex												m_mutex;
	typedef std::queue<stContent>						contextQueue;
	contextQueue										m_contexts;
	char*												m_buffer;
};

#ifdef _DEBUG

#define ASSERTFAIL(e)						LogSystem::assertFail(#e, __FILE__, __FUNCTION__, __LINE__)
#define IF_TRUE(e)							if (e ? 1: (ASSERTFAIL(e), 0))
#define IF_FALSE(e)							if (e ? (ASSERTFAIL(e), 1): 0)
#define DEBUG_MESSAGE_BOX(formt, ...)		LogSystem::outMessagebox(formt, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define DEBUG_ASSERT(e)						(e ? 1: (ASSERTFAIL(e), 0))

#else

#define ASSERTFAIL(e)
#define IF_TRUE(e)							if (e)
#define IF_FALSE(e)							if (e)
#define DEBUG_MESSAGE_BOX(formt, ...)
#define DEBUG_ASSERT(e)

#endif // _DEBUG