#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "XMDebug.h"
#include "Manager/Console.h"
#include <stdarg.h>
#include "Infra/Guard.h"

#ifdef WIN32
#define strcasecmp stricmp
#endif
IDebug *IDebug::instance()
{
    return CXMDebug::instance();
}

PATTERN_SINGLETON_IMPLEMENT(CXMDebug);

CXMDebug::CXMDebug()
{
}

CXMDebug::~CXMDebug()
{
}

bool CXMDebug::RegisterDebug(std::string modeName, int defLevel/* = DL_ERROR*/)
{
    CReadWriteGuard wGuard(m_rwLock);
    std::map<std::string, int>::iterator iter;
    iter = m_DebugItem.find(modeName);
    if(iter == m_DebugItem.end())
    {
        m_DebugItem[modeName]=defLevel;
        return true;
    }
    return false;
}

void CXMDebug::UnRegisterDebug(std::string modeName)
{
    CReadWriteGuard wGuard(m_rwLock);
    std::map<std::string, int>::iterator iter;
    iter = m_DebugItem.find(modeName);
    if(iter != m_DebugItem.end())
    {
        m_DebugItem.erase(iter);
    }
}

void CXMDebug::Print(std::string modeName, int level,const char * format,...)
{
    CReadWriteGuard wGuard(m_rwLock, CReadWriteGuard::guardRead);
    
    std::map<std::string, int>::iterator iter;
    iter = m_DebugItem.find(modeName);
    if(iter != m_DebugItem.end())
    {
        if(iter->second != DL_OFF && level <= iter->second)
        {
            printf("[%s]: ", iter->first.c_str());
            va_list list;
            va_start(list, format);
            vprintf(format, list);
            va_end(list);
        }
    }
}

int CXMDebug::strToLevel(std::string strLevel)
{
    int level = -1;
    if(!strcasecmp(strLevel.c_str(), "off"))
    {
        level = DL_OFF;
    }
    else if(!strcasecmp(strLevel.c_str(), "info"))
    {
        level = DL_INFO;
    }
    else if(!strcasecmp(strLevel.c_str(), "error"))
    {
        level = DL_ERROR;
    }
    else if(!strcasecmp(strLevel.c_str(), "warning"))
    {
        level = DL_WARNING;
    }
    else if(!strcasecmp(strLevel.c_str(), "debug"))
    {
        level = DL_DEBUG;
    }
    else if(!strcasecmp(strLevel.c_str(), "all"))
    {
        level = DL_ALL;
    }
    return level;
        
}

std::string CXMDebug::levelToStr(int level)
{
    switch(level)
    {
        case DL_OFF:
            return "off";
            break;
        case DL_INFO:
            return "info";
            break;
        case DL_ERROR:
            return "error";
            break;
        case DL_DEBUG:
            return "debug";
            break;
        case DL_WARNING:
            return "warning";
            break;
        case DL_ALL:
            return "all";
            break;
        default:
            return "none";
            break;
    }
}
void CXMDebug::showHelp()
{
    trace("debug -i           -- set debug level\n");
    trace("debug mode level   -- set debug level\n");
    trace("mode: ");
    
    std::map<std::string, int>::iterator iter;
    for(iter = m_DebugItem.begin(); iter != m_DebugItem.end(); iter++)
    {
        trace("%s ", iter->first.c_str());
    }
    trace("\n");
    trace("level: off info error warning debug all\n");
}
void CXMDebug::dumpModeLevel()
{
    CReadWriteGuard wGuard(m_rwLock, CReadWriteGuard::guardRead);
    
    std::map<std::string, int>::iterator iter;
    trace("            mode  level\n");
    trace("-------------------------------------------------\n");
    for(iter = m_DebugItem.begin(); iter != m_DebugItem.end(); iter++)
    {
        trace("%16s  %s\n", iter->first.c_str(), levelToStr(iter->second).c_str());        
    }
}

int CXMDebug::onConsole(int argc, char **argv)
{
    bool help = true;
    if(argc == 2)
    {
        if(!strcmp(argv[1], "-i"))
        {
            dumpModeLevel();
            help = false;
        }
        else if(!strcmp(argv[1], "-t"))
        {
            Print("APP", DL_INFO, "DL_INFO\n");
            Print("APP", DL_ERROR, "DL_ERROR\n");
            Print("APP", DL_WARNING, "DL_WARNING\n");
            Print("APP", DL_DEBUG, "DL_DEBUG\n");
            Print("APP", DL_ALL, "DL_ALL\n");
            help = false;
        }
    }
    else if(argc == 3)
    {
        std::string mode = argv[1];
        std::string strLevel = argv[2];
        m_rwLock.EnterWrite();
 
        if(!strcasecmp(mode.c_str(), "all"))
        {
            int level = strToLevel(strLevel);
            if(level != -1)
            {
                std::map<std::string, int>::iterator iter;
                for(iter = m_DebugItem.begin(); iter != m_DebugItem.end(); iter++)
                {
                    iter->second = level;
                    help = false;
                }
            }
        }
        else
        {
            std::map<std::string, int>::iterator iter;
            iter = m_DebugItem.find(mode);
            if(iter != m_DebugItem.end())
            {
                int level = strToLevel(strLevel);
                if(level != -1)
                {
                    iter->second = level;
                    help = false;
                }
            }
        }
        
        m_rwLock.LeaveWrite();
    }

    if(help)
    {
        showHelp();
    }

	return 0;
}

