#include "LogManager.h"
#include "Manager/Console.h"
#include "Infra/Guard.h"
#include "Infra/Time.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

int CLog::sm_maxLogItem = 2048;					// 最大的日志条数
std::string CLog::sm_logFilePath;				// 日志存放的路径

PATTERN_SINGLETON_IMPLEMENT(CLog);

void CLog::config(int maxLogItem, const char *logFilePath)
{
	sm_maxLogItem  = maxLogItem;
	sm_logFilePath = logFilePath;
}

CLog::CLog() : m_logMutex(MUTEX_RECURSIVE), m_saveFileTimer("LogSaveFile"),m_sigEventHandler(4)
{
	CGuard guard(m_logMutex);
	assert(sizeof(Item) == itemLength);	
	
	trace("Log Manager starting....!\n");
	m_logFile = new CFile;
	char buffer[itemLength + 1];

	if (m_logFile->Open(sm_logFilePath.c_str(), CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate))
	{
		int year, month, day, hour, minute, second;

		while (m_logFile->Gets(buffer, itemLength))
		{
			// 校验
			if (sscanf(buffer, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) != 6)
			{
				trace("LogItem:%s format error!\n", buffer);
				continue;
			}
			m_logBuffer.push_back(buffer);
			if (m_logBuffer.size() > static_cast<size_t>(sm_maxLogItem))
			{
				m_logBuffer.pop_front();
			}
		}
	}
	// 断电的时候最多会丢失30S的日志
	m_saveFileTimer.Start(CTimer::Proc(&CLog::onTimer, this), 0, 30*1000);

	//IEventManager::instance()->attach(IEventManager::Proc(&CLog::onAppEvent, this));
}

CLog::~CLog()
{
	m_logFile->Close();
	delete m_logFile;
}

void CLog::append(const char* type, const char* datafmt, ...)
{
	CGuard guard(m_logMutex);
	Item logItem;
	char buffer[itemLength + 1];

	assert(strlen(type) < itemTypeLength);
	
	memset((void *)&logItem, 0, sizeof(Item));
	GetCurDateTime(&logItem.time);
	strcpy(logItem.type, type);
	if (datafmt)
	{
		va_list args;
		va_start(args, datafmt);
		vsnprintf(logItem.data, itemDataLength, datafmt, args);
		va_end(args);
	}
	strcpy(logItem.user, "System");
	sprintf(buffer, "%d-%d-%d %d:%d:%d[%s],[%s],[%s]\n", 
			logItem.time.year, logItem.time.month, logItem.time.day,
			logItem.time.hour, logItem.time.minute, logItem.time.second,
			logItem.user, logItem.type, logItem.data);
	m_logBuffer.push_back(buffer);
	if (m_logBuffer.size() > static_cast<size_t>(sm_maxLogItem))
	{
		//trace("log item %s poped!\n", m_logBuffer[0].c_str());
		m_logBuffer.pop_front();
	}
	m_bufdirty = true;
	m_sigEventHandler(logItem.type, logItem.data, logItem.time);
}	

void CLog::clear(const char *user)
{
	CGuard guard(m_logMutex);

	m_logBuffer.clear();
	append("ClearLog");
}

uint CLog::size()
{
	CGuard guard(m_logMutex);
	
	return m_logBuffer.size();
}

// 根据文件长度，开始位置，结束位置进行计算
bool CLog::get(uint pos, Item& item)
{
	CGuard guard(m_logMutex);
retry:
	if (pos >= size())
	{
		return false;
	}
	SystemTime time;

	memset(&time, 0, sizeof(SystemTime));
	memset(&item, 0, sizeof(item));
	if (sscanf(m_logBuffer[pos].c_str(), "%d-%d-%d %d:%d:%d", 
		&time.year, &time.month, &time.day, 
		&time.hour, &time.minute, &time.second) == 6)
	{
		item.time.year = time.year;
		item.time.month = time.month;
		item.time.day = time.day;
		item.time.hour = time.hour;
		item.time.minute = time.minute;
		item.time.second = time.second;
		const char *begin = strstr(m_logBuffer[pos].c_str(), "[");
		const char *end = strstr(m_logBuffer[pos].c_str(), "]");

		// 取用户名
		if (begin && end)
		{
			int len = end - begin - 1;

			if ((size_t)(end - begin - 1) >= sizeof(item.user))
			{
				len = sizeof(item.user);
			}
			strncpy(item.user, begin + 1, len);
			// 取日志类型
			begin = strstr(end, "[");
			if (begin)
			{
				end = strstr(begin, "]");
				if (end)
				{
					// 可能断电引起日志文件格式不对
					if ((size_t)(end - begin - 1) >= sizeof(item.type))
					{
						pos++;
						goto retry;
					}
					strncpy(item.type, begin + 1, end - begin - 1);
				}
			}
			if (end)
			{
				// 取日志数据
				begin = strstr(end, "[");
				if (begin)
				{
					end = strstr(begin, "]");
					if (end)
					{
						int len = end - begin - 1;

						if ((size_t)(end - begin - 1) >= sizeof(item.data))
						{
							len = sizeof(item.data);
						}
						strncpy(item.data, begin + 1, len);
					}
				}
			}
		
			return true;
		}
	}
	trace("log item:%s parse failed!\n", m_logBuffer[pos].c_str());
	return true;
}
void CLog::onTimer(uint arg)
{
	saveFile();
}

bool CLog::saveFile()
{
	CGuard guard(m_logMutex);

	if (m_bufdirty)
	{
		if (m_logFile->Open(sm_logFilePath.c_str(), CFile::modeCreate | CFile::modeReadWrite))
		{
			for (LogDeque::iterator pi = m_logBuffer.begin(); pi != m_logBuffer.end(); pi++)
			{
				m_logFile->Puts((*pi).c_str());
			}
		}
		m_logFile->Close();
		m_bufdirty = false;
		return true;
	}
	return false;
}

/*
void CLog::onAppEvent(std::string code, int index, appEventAction action, const EventHandler *param, Json::Value* data)
{
	// 关机，重启事件
	if (code == "Shutdown" 
		|| code == "Reboot")
	{
		saveFile();
	}
}
*/
bool CLog::attach(Proc proc)
{
	return (m_sigEventHandler.Attach(proc) >= 0);
}

bool CLog::detach(Proc proc)
{
	return (m_sigEventHandler.Detach(proc) >= 0);
}

int CLog::onConsole(int argc, char **argv)
{
	if (argc < 2)
	{
help:
		trace("log		-d				dump all log!\n");
		trace("log		-a [n] [type]	append log!\n");
		trace("log		-c				clear all log!\n");
		trace("log		-s				dump stat info!\n");
		return 0;
	}
	switch (argv[1][1])
	{
	case 'd':
		{
			CGuard guard(m_logMutex);
			int logSize = size();

			trace("------------%d log item-------------\n", logSize);
			for (int i = 0; i < logSize; i++)
			{
				trace("%8d:%s", i + 1, m_logBuffer[i].c_str());	
			}
			trace("\n");
		}
		break;
	case 'a':
		if (argc >= 4)
		{
			int n;

			sscanf(argv[2], "%d", &n);
			for (int i = 0; i < n; i++)
			{
				append(argv[3]);
			}
		}
		break;
	case 'c':
		clear(NULL);
		break;
	case 's':
		trace("------------------------------------\n");
		trace("             logPath:%s\n", sm_logFilePath.c_str());
		trace("        max log item:%d\n", sm_maxLogItem);
		trace("            log size:%d\n", size());
		break;
	default:
		goto help;
		break;
	}
	return 0;
}

ILog* ILog::instance()
{
	return CLog::instance();
}

void ILog::config(int maxLogItem, const char *logFilePath)
{
	CLog::config(maxLogItem, logFilePath);
}

/// 虚析构函数
ILog::~ILog()
{
}
