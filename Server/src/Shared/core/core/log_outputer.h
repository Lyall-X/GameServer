#pragma once

enum logLevel
{
	logLevel_info,
	logLevel_warn,
	logLevel_error,
	logLevel_debug,
	logLevel_hex,
};

class LogOutputer
{
public:
	LogOutputer(void);
	~LogOutputer(void);
public:
	virtual void Outputer(const char* content, int color = 0, int level = logLevel_info) = 0;
};

