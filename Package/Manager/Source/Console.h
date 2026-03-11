#ifndef __INTER_MANAGER_CONSOLE_H__
#define __INTER_MANAGER_CONSOLE_H__

#include "Infra/Timer.h"
#include "Manager/User.h"
#include "Manager/Console.h"
#include "Infra/Thread.h"
#include "Infra/Function.h"
#include "Infra/Singleton.h"
#include "Infra/Wrapper.h"
#include <map>
#include <string>

class CConsole : public IConsole, public CThread
{
public:
	/// 控制台状态
	enum ConsoleState 
	{
		stateOff = 0,	//控制台关闭状态
		stateUserName,	//用户名输入状态
		statePassword, 	//密码输入状态
		stateLine,		//命令行状态
	};

	enum 
	{
		MaxHistoryCommand = 64,	// 最多历史命令数
	};

	/// 控制台命令表
	typedef std::map<std::string, Command> CommandTable;

	/// 单件模式
	PATTERN_SINGLETON_DECLARE(CConsole);

public:

	/// 线程执行体
	void ThreadProc();

	/// 开启控制台
	bool start();

	/// 停止控制台
	bool stop();

	/// 注册控制台命令
	bool attach(const std::string &cmd, const Command &cmdDesc);

	/// 撤销控制台命令
	bool detach(const std::string &cmd);

private:

	int onHelp(int argc, char ** argv);
	int onResource(int argc, char **argv);
	int onPacket(int argc, char **argv);
	int onReboot(int argc, char **argv);
	int onShutdown(int argc, char **argv);
	int onTime(int argc, char **argv);
	int onQuit(int argc, char **argv);
	int onShell(int argc, char **argv);
	int onHeap(int argc, char **argv);
	int onConsoleTimer(int argc, char **argv);
	int onThread(int argc, char **argv);
	void onTimer(uint arg);
	
private:

	void onLine(char* buf);
private:
	CommandTable	m_commandTable;
	TWrapper<IUser> m_user;
	int 			m_iParams;		//命令行参数数量
	char			m_dbParamTab[16][32];
	CTimer			m_Timer;
	bool			m_showContinue;
	int				m_state;
	int				m_wordPosition;
	char			m_buffer[512];
	std::string		m_userName;
	std::string		m_historyCommand[MaxHistoryCommand];
	int				m_nHistory;				// 历史命令数量
	int				m_nCurrentHistory;		// 当前历史命令
};

#endif// __CONSOLE_H__

