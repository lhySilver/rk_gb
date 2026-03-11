#ifndef __XM_DEBUG_H__
#define __XM_DEBUG_H__
#include "Infra/Singleton.h"
#include "Infra/Mutex.h"
#include "Log/IDebug.h"

class CXMDebug : public IDebug
{
	PATTERN_SINGLETON_DECLARE(CXMDebug);
public:
	bool RegisterDebug(std::string modeName, int defLevel = IDebug::DL_ERROR);
	void UnRegisterDebug(std::string modeName);
	void Print(std::string modeName, int level, const char *format, ...);

private:
	std::map<std::string, int> m_DebugItem;
	
	int onConsole(int argc, char **argv);
	int strToLevel(std::string strLevel);
	std::string levelToStr(int level);
	void showHelp();
	void dumpModeLevel();
	CReadWriteMutex m_rwLock;

};

#endif

