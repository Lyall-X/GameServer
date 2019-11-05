#pragma once

class LogConsoleOutpter;
class LogWinOutputer;
class LogFileOutputer;
class LogSystem;

class log
{
public:
	static log& instance() { static log s_ins; return s_ins; }
public:
	log();
	~log();
public:
	LogSystem& getLog() { return m_log; }
protected:
	LogConsoleOutpter*			m_console;
	LogWinOutputer*				m_win;
	LogFileOutputer*			m_file;
	LogSystem					m_log;
};

#define LOG_INFO(formt, ...)					log::instance().getLog().info(formt, __VA_ARGS__)
#define LOG_WARN(formt, ...)					log::instance().getLog().warn(formt, __VA_ARGS__)
#define LOG_ERROR(formt, ...)					log::instance().getLog().error(formt, __VA_ARGS__)
#define LOG_HEX(data, len)						log::instance().getLog().outHex(data, len);
#define LOG_DEBUG(color, formt, ...)			log::instance().getLog().debug(color, formt, __VA_ARGS__)

#define LOG_ERROR_LINE(formt, ...)				log::instance().getLog().error_line(formt, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

#define DEBUG_TACK()							log::instance().getLog().error("ERROR: %s %s %d", __FILE__, __FUNCTION__, __LINE__)

#ifdef _DEBUG

#define DEBUG_INFO(formt, ...)					log::instance().getLog().info(formt, __VA_ARGS__)
#define DEBUG_WARN(formt, ...)					log::instance().getLog().warn(formt, __VA_ARGS__)
#define DEBUG_ERROR(formt, ...)					log::instance().getLog().error(formt, __VA_ARGS__)
#define DEBUG_HEX(data, len)					log::instance().getLog().outHex(data, len);
#define DEBUG_DEBUG(color, formt, ...)			log::instance().getLog().debug(color, formt, __VA_ARGS__);

#else

#define DEBUG_INFO(log, formt, ...)			//CLog::findLog(log)->Debug(__FILE__, __FUNCTION__, __LINE__, formt, __VA_ARGS__)
#define DEBUG_WARN(log, formt, ...)			//CLog::findLog(log)->Debug(__FILE__, __FUNCTION__, __LINE__, formt, __VA_ARGS__)
#define DEBUG_ERROR(log, formt, ...)		//CLog::findLog(log)->Debug(__FILE__, __FUNCTION__, __LINE__, formt, __VA_ARGS__)
#define DEBUG_HEX(data, len)
#define DEBUG_DEBUG(color, formt, ...)

//#define DEBUG_TACK

#endif // _DEBUG