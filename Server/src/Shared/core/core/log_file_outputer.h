#pragma once


class LogFileOutputer : public LogOutputer
{
public:
	LogFileOutputer();
	~LogFileOutputer(void);
public:
	virtual void Outputer(const char* content, int color, int level);
protected:
	int32							mIdx;
	std::string						mFirstPath;
	char							mBuffer[256];
};

