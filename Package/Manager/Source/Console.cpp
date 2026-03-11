#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Console.h"
#include "Manager/MagicBox.h"
#include "Infra/Packet.h"
#include "Infra/Thread.h"
#include "Infra/Timer.h"
#include "Infra/Time.h"
#include "Log/DebugDef.h"
#include <Types/Defs.h>

#ifdef WIN32
	#include <io.h>
#elif defined LINUX
	#include <unistd.h>
#endif

#define TP_COM			 1

PATTERN_SINGLETON_IMPLEMENT(CConsole);


CConsole::CConsole() : CThread("Console", TP_COM), m_Timer("Console")
{
}

CConsole::~CConsole()
{
}

void CConsole::ThreadProc()
{
	char buf[512];
	int i;

	m_state = stateOff;
	while (m_bLoop)
	{
	#if 0
		fflush(stdout);
		memset(buf, 0, sizeof(buf));
		for (i = 0; i < 512; i++)
		{ 
			if (1 == fread(buf + i, 1, 1, stdin))
			{
				if(buf[i] == '\n')
				{
					break;
				}
			}
		}

		// 命令长度超出，丢弃
		if(i >= 512)
		{
			continue;
		}
		// 去掉多余的换行符
		if(i > 0 && buf[i - 1] == '\r')
		{
			i--;
		}
		buf[i] = '\0';
		onLine(buf);
		#endif
		CTime::sleep(3 * 1000);
	}
	
	
}

bool CConsole::start()
{
	Command cmd;

	ManagerInfo("CConsole::start()...\n");
	cmd.helpSting = "Try help!";
	cmd.proc =  Proc(&CConsole::onHelp,this);
	attach("help",  cmd);

	cmd.helpSting = "CPU usage!";
	cmd.proc =  Proc(&CConsole::onResource, this);
	attach("resource",  cmd);

	cmd.helpSting = "Packet usage!";
	cmd.proc =  Proc(&CConsole::onPacket, this);
	attach("packet",  cmd);

	cmd.helpSting = "Reboot the system!";
	cmd.proc =  Proc(&CConsole::onReboot, this);
	attach("reboot",  cmd);

	cmd.helpSting = "Shutdown the system!";
	cmd.proc =  Proc(&CConsole::onShutdown, this);
	attach("shutdown",  cmd);

	cmd.helpSting = "Quit!";
	cmd.proc =  Proc(&CConsole::onQuit, this);
	attach("quit",  cmd);

	cmd.helpSting = "Linux shell prompt!";
	cmd.proc =  Proc(&CConsole::onShell, this);
	attach("shell",  cmd);

	cmd.helpSting = "Dump heap status!";
	cmd.proc =  Proc(&CConsole::onHeap, this);
	attach("heap",  cmd);

	cmd.helpSting = "Set SystemTime!";
	cmd.proc =  Proc(&CConsole::onTime, this);
	attach("time",  cmd);

	cmd.helpSting = "Dump application timers!";
	cmd.proc =  Proc(&CConsole::onConsoleTimer, this);
	attach("timer",  cmd);

	cmd.helpSting = "Dump application threads!";
	cmd.proc =  Proc(&CConsole::onThread,this);
	attach("thread",  cmd);

	/// 平均5分钟打印一次系统信息
	m_Timer.Start(CTimer::Proc(&CConsole::onTimer, this), 0, 5*60*1000);
	return CreateThread();
}

bool CConsole::stop()
{
	return DestroyThread();
}

/// 将命令cmd(具体信息在cmdDesc里),注册到console中来
bool CConsole::attach(const std::string &cmd, const Command &cmdDesc)
{
	return m_commandTable.insert(CommandTable::value_type(cmd, cmdDesc)).second;
}

bool CConsole::detach(const std::string &cmd)
{
	return m_commandTable.erase(cmd) ? true : false;
}

void CConsole::onLine(char* buf)
{
	char	*p[16];
	int i = 0;
	int n = 0;

	if (m_state != stateLine)
	{
		switch(m_state)
		{
		case stateOff:
			trace("username:");
			m_state = stateUserName;
			break;
		case stateUserName:
			// 设置为密码状态
			m_userName = buf;
			trace("password:");
			m_state = statePassword;
			break;
		case statePassword:
			bool bRet = m_user->login(m_userName, buf, "Console", "NONE");

			if (false == bRet)
			{
				trace("User not valid!\n");
				trace("user name:");
				m_state = stateUserName;
			}
			else
			{
				m_state = stateLine;
				trace("%s$ ", m_userName.c_str());
			}
			break;
		}
	}
	else//命令行状态
	{
		n = 0;
		p[0] = buf;

		if(strlen(buf) == 0) //空字符串
		{
			trace("%s$ ", m_userName.c_str());
			return;
		}

		while(buf[i])
		{
			while(buf[i] == ' ')
			{
				buf[i] = '\0';
				i++;
			}

			if(buf[i])
			{
				p[n++] = &buf[i];
			}

			while(buf[i] && buf[i] != ' ')
			{
				i++;
			}
		}

		CommandTable::iterator iter = m_commandTable.find(p[0]);
		if (iter != m_commandTable.end())
		{
			if (n == 1)
			{
				//各个模块接收到的参数是从'-'开始的
				p[1] = NULL;
			}
			(*iter).second.proc(n, p);
		}
		else
		{
			trace("'%s' is not a command.\n", p[0]);
		}

		if(m_state == stateLine)
		{
			trace("%s$ ", m_userName.c_str());
		}
	}
}

int CConsole::onHelp(int argc, char ** argv)
{
	trace("----------------------Console Commands----------------------------\n");
	for (CommandTable::iterator pi = m_commandTable.begin(); pi != m_commandTable.end(); pi++)
	{
		trace("%20s %s\n", (*pi).first.c_str(), (*pi).second.helpSting.c_str());
	}
	trace("To see details, please use \'cmd -h\'\n\n");
	return 0;
}

int CConsole::onResource(int argc, char **argv)
{
	return 0;
}

int CConsole::onPacket(int argc, char **argv)
{
	// g_PacketManager.Dump();
	// g_PacketManager.DumpNodes();
	return 0;
}

int CConsole::onReboot(int argc, char **argv)
{
	printf("\033[1;36m    CConsole::onReboot   \033[0m\n");
	IMagicBox::instance()->reboot();
	return 0;
}

int CConsole::onShutdown(int argc, char **argv)
{
	IMagicBox::instance()->shutdown();
	return 0;
}

int CConsole::onTime(int argc, char **argv)
{
	if (argc < 4)
	{
help:
		trace("time -s yyyy-mm-dd hh:mm:ss set current time\n");
		return 0;
	}
	SystemTime time;

	switch (argv[1][1])
	{
	case 's':
		if (sscanf(argv[2], "%d-%d-%d", &time.year, &time.month, &time.day) == 3
			&& sscanf(argv[3], "%d:%d:%d", &time.hour, &time.minute, &time.second) == 3)
		{
			CTime::setCurrentTime(time);
		}
		break;
	default:
		goto help;
	}
	return 0;
}

int CConsole::onQuit(int argc, char **argv)
{
	m_user->logout();
	m_state = stateOff;
	return 0;
}

int CConsole::onShell(int argc, char **argv)
{
	system("sh");
	return 0;
}

int CConsole::onHeap(int argc, char **argv)
{
	return 0;
}

int CConsole::onConsoleTimer(int argc, char **argv)
{
	CTimer::DumpThread();
	return 0;
}

int CConsole::onThread(int argc, char **argv)
{
	CThread::DumpThreads();
	return 0;
}

void CConsole::onTimer(uint arg)
{
}

/// 返回单件实例
IConsole* IConsole::instance()
{
	return CConsole::instance();
}

/// 虚析构函数
IConsole::~IConsole()
{
}
