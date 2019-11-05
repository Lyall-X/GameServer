#include "Shared.hpp"

using std::string;
using std::vector;
using std::sregex_iterator;
using std::regex;
using std::sub_match;
using std::cin;
using std::cout;
using std::endl;

void SplitParams(std::string& cmdline, std::string& cmd, std::vector<std::string>& list) {
	// splits the commandline into cmd & paramlist
	std::regex splitPattern("(\")(.*)(\\1)|([^,;\\s]+)");
	std::match_results<string::const_iterator> results;
	sregex_iterator itor(cmdline.begin(), cmdline.end(), splitPattern);
	sregex_iterator itor_end;
	string param;
	bool isParam = false;
	cmd = cmdline;
	/* 由于c++ regex库是DFA模型, 不支持前向断言预判, 只能使用子串匹配模式 */
	for (; itor != itor_end; itor++) {
		// index 2 means find quoted string 
		const sub_match<string::const_iterator> &submatch = (*itor)[2];
		param = (submatch.matched) ? submatch.str() : itor->str(0);
		if (isParam)
			list.push_back(param);
		else
		{
			cmd = param;
			isParam = true;
		}
	}
}


uint32 CmdDispatcher::onThreadProcess(Threader& threader)
{
	while (threader.Active())
	{
		Threader::sleep(1);
		std::string cmdline;
		getline(std::cin, cmdline, '\n');
		CmdExecute* cmdExecute = new CmdExecute;
		SplitParams(cmdline, cmdExecute->cmd, cmdExecute->params);
		m_mutex.lock();
		mQueueCmd.push(cmdExecute);
		m_mutex.unlock();
	}
	return 0;
}

CmdDispatcher::CmdDispatcher()
{
	m_pThread = new Threader;
	m_pThread->CreateThread((ThreadCallBack)&CmdDispatcher::onThreadProcess, this);
}


CmdDispatcher::~CmdDispatcher()
{
	Destroy();
}

void CmdDispatcher::Destroy()
{
	while (mQueueCmd.size() > 0)
	{
		m_mutex.lock();
		delete mQueueCmd.front();
		mQueueCmd.pop();
		m_mutex.unlock();
	}

	m_pThread->Eixt();
	SAFE_DELETE(m_pThread);
}

void CmdDispatcher::update(float32 time, float32 delay)
{
	CmdExecute* cmd = NULL;
	m_mutex.lock();
	if (mQueueCmd.size() <= 0)
	{
		m_mutex.unlock();
		return;
	}

	cmd = mQueueCmd.front();
	mQueueCmd.pop();
	m_mutex.unlock();
	
	CmdEvent e;
	e.event = cmd->cmd;
	e.cmdExecute = cmd;
	if (cmd->cmd == "clear") {
		system("cls");
	}
	this->Dispatch(e);
	delete cmd;
}
