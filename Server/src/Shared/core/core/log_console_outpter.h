#pragma once

class LogConsoleOutpter : public LogOutputer
{
public:
	LogConsoleOutpter(void);
	~LogConsoleOutpter(void);
public:
	virtual void Outputer(const char* content, int color, int level);
private:
	void*				m_hStdHandle;
};

