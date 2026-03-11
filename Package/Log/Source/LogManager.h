#ifndef __INTER_MANAGER_LOG_H__
#define __INTER_MANAGER_LOG_H__

#include "Log/Log.h"
#include "Infra/Mutex.h"
#include "Infra/Timer.h"
#include "Infra/Singleton.h"
//#include "Manager/EventManager.h"
#include "Infra/File.h"
#include <string>
#include <map>
#include <deque>

class CFile;

///
/// 日志管理部分，用于记录系统运行过程中的一些关键事件方便系统的维护和调试，
/// 目前日志全部采用字符串的形式存储方便然日后扩展。
/// 日志内容部分和日志文件头部分分开存放，方便扩展
/// 
class CLog : public ILog
{
public:

	
	PATTERN_SINGLETON_DECLARE(CLog);

public:

	/// \param [in] maxLogItem 最大日志条数
	/// \param [in] logFilePath 日志文件路径
	static void config(int maxLogItem, const char *logFilePath);
	
	/// 记录日志，同时加上日志记录的时间。
	/// \param  type 日志类型，如"SaveConfig"
	/// \param  data 日志数据具体格式有datafmt传入，详细见《JsonConfigFormat.doc》
	void append(const char* type, const char* datafmt = NULL, ...);

	/// 清除日志，同时自动产生一条清除日志的日志记录。
	/// \param  user 日志产生时对用的用户名。
	void clear(const char *user);

	/// 得到日志总条数。
	uint size();

	/// 读取日志项结构
	/// \param pos 日志项数，[0, size()-1]
	/// \param item 日志项。
	/// \return 读取日志是否成功
	bool get(uint pos, Item& item);

	/// 注册回调, 产生日志时将会调用该回调
	bool attach(Proc proc);

	/// 注销回调
	bool detach(Proc proc);

	/// 保存文件
	bool saveFile();

private:

	/// 控制台
	int onConsole(int argc, char **argv);

	/// 定时器回调函数
	void onTimer(uint arg);


	//void onAppEvent(std::string code, int index, appEventAction action, const EventHandler *param, Json::Value* data = NULL);
private:
	typedef std::deque<std::string> LogDeque;
	CMutex	m_logMutex;
	CFile *m_logFile;
	LogDeque m_logBuffer;			// 日志缓冲
	bool	m_bufdirty;				// 缓冲脏标识
	CTimer	m_saveFileTimer;
	static int sm_maxLogItem;		// 最大的日志条数
	static std::string sm_logFilePath;	// 日志存放的路径

	SIG_LOG m_sigEventHandler;
};

#endif //__LOG_H__
