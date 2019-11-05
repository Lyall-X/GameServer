#pragma once

class LogWinOutputer : public LogOutputer
{
public:
	LogWinOutputer(void) {}
	~LogWinOutputer(void) {}
public:
	virtual void Outputer(const char* content, int color, int level);
};

