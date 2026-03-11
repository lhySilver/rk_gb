#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ConfigManager.h"
#include "Log/Log.h"
#include "Infra/File.h"
#include "Manager/Console.h"
#include "ExchangeAL/ExchangeKind.h"
#include "ExchangeAL/Exchange.h"
#include "Log/DebugDef.h"
#include "PathDefinition.h"
#include <unistd.h>
#include <sys/stat.h>


// 单个配置文件最大长度暂时固定，以后再动态调节。如果配置长度超过configStreamSize，写配置会变慢。
const int configStreamSize = (128 * 1024);

std::string CConfigManager::sm_firstFileDir	= "";
std::string CConfigManager::sm_secondFileDir = "";
std::string CConfigManager::sm_customFileDir = "";

PATTERN_SINGLETON_IMPLEMENT(CConfigManager);

void CConfigManager::mkdirConfigPath(const char *dir)
{
	if( (NULL == dir) || (0 == strlen(dir)) )
		return;
	
	std::string dir_str = dir;
	if( '/' != dir[strlen(dir) -1 ] )		
		dir_str += "/";
	
	std::string dir_tmp;
	for(int i = 0; '\0' != dir_str[i]; i++)
	{
		dir_tmp += dir[i];
		if( '/' == dir_str[i])
		{
			//<shang>判断配置文件存储目录是否存在，不存在则创建
			if(access(dir_tmp.c_str(), F_OK) == -1)
			{
				ManagerInfo("%s is not exsit, create now\n", dir_tmp.c_str());
				mkdir(dir_tmp.c_str(), 0777);
			}
		}
	}
}

void CConfigManager::config(const char *firstFileDir, const char *secondFileDir, const char *customFileDir)
{
	mkdirConfigPath(firstFileDir);
	mkdirConfigPath(secondFileDir);
	mkdirConfigPath(customFileDir);
	
	sm_firstFileDir = firstFileDir;
	sm_secondFileDir = secondFileDir;
	sm_customFileDir = customFileDir;
}

CConfigManager::CConfigManager() : m_cfgTimer("TConfigManager"), m_mutex(MUTEX_RECURSIVE)
{
}

CConfigManager::~CConfigManager()
{

}

void CConfigManager::start()
{
	CConfigReader reader;

	ManagerInfo("CConfigManager::start()...\n");
	m_stream.reserve(configStreamSize);

	CFileFind finder;
	std::string firstFilePath = sm_firstFileDir;

	firstFilePath += "/*";
	if (finder.findFile(firstFilePath.c_str()))
	{
		do {                                    // read and parse config file in "$sm_firstFileDir/*"
			if (finder.getFileName() == "."
				|| finder.getFileName() == ".."
				|| finder.isDirectory()
				|| strstr(finder.getFileName().c_str(), "."))	// filter file whose filename contains "." (which is "*.second")
			{
				continue;
			}
			bool bToRead2ndFile = true;
			if(readConfig(finder.getFilePath().c_str(), m_stream))
			{
				bToRead2ndFile = !reader.parse(m_stream, m_configAll[finder.getFileName().c_str()]);
			}
			else
			{
				ManagerErr("readConfig file:%s failed\n", finder.getFilePath().c_str());
			}

			if (bToRead2ndFile) // parse the first config file failed, then read the second config file and parse
			{
				ManagerWraning("CConfigManager::start() first config file %s parsing failed.\n", finder.getFilePath().c_str());
				std::string secondFilePath = finder.getFilePath();

				secondFilePath += ".second";
				m_stream = "";
				if(readConfig(secondFilePath.c_str(), m_stream))
				{
					bToRead2ndFile = !reader.parse(m_stream, m_configAll[finder.getFileName().c_str()]);
				}
				else
				{
					ManagerErr("readConfig file:%s failed\n", secondFilePath.c_str());
				}
			}

			if (bToRead2ndFile) // failed to have parsed the second config file
			{
				ManagerErr("CConfigManager::start() second config file %s.second parsing failed too.\n", finder.getFilePath().c_str());
			}
		} while (finder.findNextFile());
		finder.close();

		// 读取用户自定义配置文件
		firstFilePath = sm_customFileDir;
		firstFilePath += "/*";
		if (finder.findFile(firstFilePath.c_str()))
		{
			do {															// read and parse config file in "$sm_customFileDir/*"
				if (strstr(finder.getFileName().c_str(), ".custom"))
				{
					std::string customFilePath = sm_customFileDir;

					customFilePath += "/";
					customFilePath += finder.getFileName();
					if (readConfig(customFilePath.c_str(), m_stream))   	// read custom config file to m_stream
					{
						char name[128];

						strcpy(name, finder.getFileName().c_str());
						*strstr(name, ".custom") = '\0';
						if (!reader.parse(m_stream, m_configCustom[name]))  // parse m_stream to m_configCustom[name]
						{
							ManagerErr("parse custom config:%s failed!\n", customFilePath.c_str());
						}
					}
					else
					{
						ManagerErr("read custom config:%s failed!\n", customFilePath.c_str());
					}
				}
			} while (finder.findNextFile());
			finder.close();
		}
	}

	IEventManager::instance()->attach(IEventManager::Proc(&CConfigManager::onAppEvent, this));
}

// 保存文件
void CConfigManager::saveFile()
{
	static CMutex fileMutex;
	CGuard guard(m_mutex);
	CGuard l_cGuard(fileMutex);

#ifdef WIN32
	Json::StyledWriter writer(m_stream);
#else
	#ifdef _DEBUG
		CConfigWriter writer(m_stream);
	#else
		CConfigWriter writer;
	#endif
#endif

	ManagerInfo("do saveFile\n");

	for (std::map<std::string, bool>::iterator pi = m_changed.begin(); pi != m_changed.end(); pi++)
	{
		ManagerInfo("saveFile--->%s\n", pi->first.c_str());
		if (pi->second)
		{
			pi->second = false;
			m_stream = "";
			m_stream = writer.write(m_configAll[pi->first.c_str()]);
			std::string firstFilePath = sm_firstFileDir + "/" + pi->first;
			std::string secondFilePath = sm_secondFileDir + "/" + pi->first + ".second";

			ManagerInfo("saveFile:%s\n", firstFilePath.c_str());
			remove(secondFilePath.c_str());
			rename(firstFilePath.c_str(), secondFilePath.c_str());

			if ((m_fileConfig = gzopen(firstFilePath.c_str(), "wb")) != (gzFile)NULL)
			{
				if((int)m_stream.size() != gzwrite(m_fileConfig, (char*)m_stream.c_str(), m_stream.size()))
				{
					ManagerErr("write config file failed!\n");
				}
				gzflush(m_fileConfig, Z_FINISH);
				gzclose(m_fileConfig);
			}
		}
	}
	
}

bool CConfigManager::readConfig(const char* chPath, std::string& input)
{
	m_fileConfig = gzopen(chPath, "rb");
	if(!m_fileConfig)
		return false;

	const int size = 32*1024;
	char* buf = new char[size + 1];

	input = "";

	while (1)
	{
		int nLen = gzread(m_fileConfig, buf, size);
		if(nLen <= 0 )
			break;
		buf[nLen] = 0;
		input += buf;
	}
	input += '\0';
	gzclose(m_fileConfig);
	//input = buf;
	delete []buf;

	return true;
}

void CConfigManager::onTimer(uint arg)
{
	ManagerInfo("CConfigManager::onTimer:saveFile\n");
	saveFile();
}

void CConfigManager::onAppEvent(std::string code, int index, appEventAction action, const EventHandler *param, Json::Value* data)
{
	// 关机，重启事件
	if (code == "Shutdown"
		|| code == "Reboot")
	{
		ManagerInfo("CConfigManager::onAppEvent:code = %s\n", code.c_str());
		saveFile();
	}
}

void CConfigManager::deleteFile(const char* user /* = NULL */)
{
	// TODO 删除配置文件
}

bool CConfigManager::getConfig(const char* path, Json::Value& table)
{	
	return getConfigInter(path, table);
}

int CConfigManager::setConfig(const char* path, const Json::Value& table, const char* user, int applyOptions)
{
	CGuard guard(m_mutex);
	char notifyPath[260];
	char subPath[260];
	const char *str = strstr(path, ".[");

	ManagerInfo("CConfigManager::setConfig path:%s\n", path);
	
	if (str)
	{
		// 单通道，分解
		memset(notifyPath, 0, sizeof(notifyPath));
		strncpy(notifyPath, path, str - path);
		memset(subPath, 0, sizeof(subPath));
		strcpy(subPath, str);
		// 全部通道
		if (!strcmp(subPath, ".[ff]"))
		{
			Json::Value table2;
			{
				getConfigInter(notifyPath, table2);
				for (Json::Value::UInt i = 0; i < table2.size(); i++)
				{
					table2[i] = table;
				}
				return setConfigInter(notifyPath, table2, user, applyOptions);
			}
		}
		else
		{
			Json::Value table2;
			{
				return setConfigInter(path, table, user, applyOptions);
			}
		}
	}
	else
	{
		{
			return setConfigInter(path, table, user , applyOptions);
		}
	}
}

int CConfigManager::setConfigInter(const char* path, const Json::Value& table, const char* user , int applyOptions )
{
    trace("setConfigInter(), path=%s\n", path);
	CGuard guard(m_mutex);
	int ret = applyOptions;
	char filePath[260];
	char notifyPath[260];
	char subPath[260];
	const char *str = strstr(path, ".[");

	if (str)
	{
		// 单通道，分解
		memset(notifyPath, 0, sizeof(notifyPath));
		strcpy(notifyPath, ".");
		strncpy(notifyPath + 1, path, str - path);
		memset(subPath, 0, sizeof(subPath));
		strcpy(subPath, str);
	}
	else
	{
		// 全通道
		memset(notifyPath, 0, sizeof(notifyPath));
		strcpy(notifyPath, ".");
		strcat(notifyPath, path);					// .Ability.DeviceDesc
		memset(subPath, 0, sizeof(subPath));
		strcpy(subPath, ".");
	} 
    
	ManagerInfo("CConfigManager::setConfigInter(%s, %s)\n", notifyPath + 1, subPath + 1);
	
	Json::Path resolvConfigPath(notifyPath);
	Json::Path subResolvConfigPath(subPath);

	Json::Value &mainConfig = resolvConfigPath.make(m_configAll);	// mainConfig = m_configAll.Ability.DeviceDesc
	Json::Value &leafConfig = subResolvConfigPath.make(mainConfig);	// leafConfig = mainConfig
	Json::Value rawTable;		// used to backup old config

	if (leafConfig == table)
	{
		tracepoint();
		return ret;
	}
	if (str)
	{
		rawTable = leafConfig;
		leafConfig = table;
	}
	
	SignalMap::iterator pi = m_verifyMap.find(notifyPath + 1);
	if(pi != m_verifyMap.end())
	{
		m_mutex.Leave();
		(*pi).second->operator ()(str ? mainConfig : table, ret);
		m_mutex.Enter();

		if((ret & applyValiateFailed))
		{
			ManagerErr("config [%s] verify failed with ret:%#x!\n", notifyPath, ret);
			// 新配置非法，将老配置还原回去
			if (str)
			{
				leafConfig = rawTable;
			}
			return ret; // 校验失败
		}
	}

	pi = m_applyMap.find(notifyPath + 1);
	if(pi != m_applyMap.end())
	{
		(*pi).second->operator ()(str ? mainConfig : table, ret);

		if((ret & applyValiateFailed))
		{
			ManagerErr("config %s verify failed with ret:%x!\n", notifyPath, ret);
			// 新配置非法，将老配置还原回去
			if (str)
			{
				leafConfig = rawTable;
			}
			return ret; // 校验失败
		}
	}
	if (!str)
	{
		leafConfig = table;
	}

	if (strstr(path, "."))
	{
		memset(filePath, 0, sizeof(filePath));
		strncpy(filePath, path, strstr(path, ".") - path);
	}
	else
	{
		strcpy(filePath, path);
	}
	m_changed[filePath] = true;		/// name代表对应文件名需要更新

	HookSignalMap::iterator vpi = m_hookMap.find(notifyPath + 1);
	if (vpi != m_hookMap.end())
	{
		int ret = 0;
		(*vpi).second->operator ()(mainConfig, ret);
	}
	//保存文件,优先处理延迟保存
	trace("setConfigInter()-%dL: ret = %#x\n", __LINE__, ret);
	if (ret & applyDelaySave)       // 0x100
	{
		// 延迟5S
//		m_cfgTimer.Start(CTimer::Proc(&CConfigManager::onTimer, this), 0, 0);
		m_cfgTimer.Start(CTimer::Proc(&CConfigManager::onTimer, this), 0, 5000);
	}
	else
	{
		if (!(ret & applyNotSave))  // 0x80
		{
			saveFile();
		}
	}

//<shang>	ILog::instance()->append("SaveConfig", strstr(notifyPath + 1, ".") ? strstr(notifyPath + 1, ".") + 1 : notifyPath + 1);
	return ret;
}

bool CConfigManager::getConfigInter(const char* path, Json::Value& table)
{
	CGuard guard(m_mutex);
	char name[260];
	int i;

	int ret = applyOK;

	for (i = 0; path[i] && path[i] != '.'; i++)
	{
		name[i] = path[i];
	}
	name[i] = '\0';

	Json::Path resolvConfigPath(path[i] == '\0' ? "." : path + i);
	Json::Path resolvDefaultPath(path[i] == '\0' ? "." : path + i);
	Json::Value &leafConfig = resolvConfigPath.make(m_configAll[name]);

	Json::Value &leftDefaultConfig = resolvDefaultPath.make(m_configDefault[name]);
	ValidMap::iterator pi = m_validMap.find(path);

	if(pi == m_validMap.end()) // 还没有校验
	{
		if(leftDefaultConfig.type() == Json::nullValue)
		{
			ManagerWraning("CConfigManager::getConfig '%s', but default config is not set yet!\n", name);
		}
		// 配置中没有的值取默认值
		replaceConfig(leafConfig, leftDefaultConfig, true);

		// 如果有注册了的校验函数，调用校验函数进行校验
		SignalMap::iterator vpi = m_verifyMap.find(path);

		if(vpi != m_verifyMap.end())
		{
			(*vpi).second->operator ()(leafConfig, ret);

			if(ret & applyValiateFailed) // 校验失败
			{
				leafConfig = leftDefaultConfig;
			}
		}

		m_validMap.insert(ValidMap::value_type(path, path)); // 已经校验
	}
	HookSignalMap::iterator vpi = m_hookMap.find(path);
	if (vpi != m_hookMap.end())
	{
		int ret = 1;
		(*vpi).second->operator ()(leafConfig, ret);
	}
	table = leafConfig;
	return table.type() != Json::nullValue;
}

bool CConfigManager::getDefaultInter(const char* path, Json::Value& table)
{
	CGuard guard(m_mutex);
	char name[260];
	int i;

	for (i = 0; path[i] && path[i] != '.'; i++)
	{
		name[i] = path[i];
	}
	name[i] = '\0';

	if (path[i])
	{
		Json::Path resolvPath(path + i);
		tracepoint();
		table = resolvPath.make(m_configDefault[name]);
	}
	else
	{
		tracepoint();
		table = m_configDefault[name];
	}
	return true;
}

const Json::Value& CConfigManager::resolveConfig(const char *path)
{
	char name[260];
	int i;

	for (i = 0; path[i] && path[i] != '.'; i++)
	{
		name[i] = path[i];
	}
	name[i] = '\0';
//	trace("CConfigManager::resolveConfig(%s, %s)\n", name, path);
	if (path[i])
	{
		Json::Path resolvPath(path + i);

		return resolvPath.make(m_configAll[name]);
	}
	return m_configAll[name];
}

bool CConfigManager::getDefault(const char* path, Json::Value& table)
{	
	return getDefaultInter(path, table);
}

void CConfigManager::setDefault(const char* path, const Json::Value& table, bool bProduceConfig)
{
	CGuard guard(m_mutex);
	char name[260];
	int i;
	Json::Value tableCustom;

	ManagerInfo("CConfigManager::setDefault path:%s\n", path);

	for (i = 0; path[i] && path[i] != '.'; i++)
	{
		name[i] = path[i];
	}
	name[i] = '\0';
	ManagerInfo("CConfigManager::setDefault name:%s\n", name);
	
	if(bProduceConfig)
	{
		if (path[i])
		{
			Json::Path resolvPath(path + i);
			Json::Path resolvCustomPath(path + i);
			resolvPath.make(m_configDefault[name]) = table;
			tableCustom = table;
		}
		else
		{
			m_configDefault[name] = table;
			tableCustom = table;
		}
	}
	else
	{
		if (path[i])
		{
			Json::Path resolvPath(path + i);
			Json::Path resolvCustomPath(path + i);
			resolvPath.make(m_configDefault[name]) = table;

			tableCustom = table;
			replaceConfig(tableCustom, resolvCustomPath.make(m_configCustom[name]));
			//printJsonValue(table);
			//m_configAll = table;
		}
		else
		{
			m_configDefault[name] = table;
			tableCustom = table;
			replaceConfig(tableCustom, m_configCustom[name]);
		}
	}
	SignalMap::iterator pi = m_verifyMap.find(path);
	if(pi != m_verifyMap.end())
	{
		int ret = 0;

		m_mutex.Leave();
		(*pi).second->operator ()(tableCustom, ret);
		m_mutex.Enter();
//		tracepoint();
		if((ret & applyValiateFailed))
		{
			ManagerErr("custom config %s verify failed with ret:%x!\n", path, ret);
			// 新配置非法，将老配置还原回去
			return; // 校验失败
		}
	}
	if (path[i])
	{
//		tracepoint();
		Json::Path resolvPath(path + i);
		resolvPath.make(m_configDefault[name]) = tableCustom;
	}
	else
	{
//		tracepoint();
		m_configDefault[name] = tableCustom;
	}

/*
    Json::FastWriter writer;
	std::string strConfig = writer.write(m_configDefault);
	ManagerErr("m_configDefault.length : %d\n", strConfig.length());
*/
}

void CConfigManager::replaceConfig(Json::Value& dest, const Json::Value& src, bool fillEmptyOnly)
{
	switch ( src.type() )
	{
	case Json::nullValue:
		break;
	case Json::intValue:
	case Json::uintValue:
	case Json::realValue:
	case Json::stringValue:
	case Json::booleanValue:
		if(!fillEmptyOnly || dest.type() == Json::nullValue)
		{
			dest = src;
		}
		break;
	case Json::arrayValue:
		{
			int size = src.size();

			for ( int index =0; index < size; ++index )
			{
				replaceConfig(dest[index], src[index], fillEmptyOnly);
			}
		}
		break;
	case Json::objectValue:
		{
			Json::Value::Members members( src.getMemberNames() );
			for ( Json::Value::Members::iterator it = members.begin();
				it != members.end();
				++it )
			{
				const std::string &name = *it;

				replaceConfig(dest[name], src[name], fillEmptyOnly);
			}
		}
		break;
	}
}

int CConfigManager::attach(const char* name, Proc proc)
{
	CGuard guard(m_mutex);

	SignalMap::iterator pi = m_applyMap.find(name);

	if(pi == m_applyMap.end())
	{
		Signal *sig = new Signal(128);
		pi = m_applyMap.insert(SignalMap::value_type(name, sig)).first;
	}

	return (*pi).second->Attach(proc);
}

int CConfigManager::detach(const char* name, Proc proc)
{
	CGuard guard(m_mutex);

	SignalMap::iterator pi = m_applyMap.find(name);

	if(pi == m_applyMap.end())
	{
		return 0;
	}

	return (*pi).second->Detach(proc);
}

int CConfigManager::attachVerify(const char* name, Proc proc)
{
	CGuard guard(m_mutex);

	SignalMap::iterator pi = m_verifyMap.find(name);

	if(pi == m_verifyMap.end())
	{
		Signal *sig = new Signal(128);
		pi = m_verifyMap.insert(SignalMap::value_type(name, sig)).first;
	}

	return (*pi).second->Attach(proc);
}


int CConfigManager::detachVerify(const char* name, Proc proc)
{
	CGuard guard(m_mutex);

	SignalMap::iterator pi = m_verifyMap.find(name);

	if(pi == m_verifyMap.end())
	{
		return 0;
	}

	return (*pi).second->Detach(proc);
}

int CConfigManager::hooks(const char *name, HookProc proc)
{
	CGuard guard(m_mutex);

	HookSignalMap::iterator pi = m_hookMap.find(name);

	if(pi == m_hookMap.end())
	{
		HookSignal *sig = new HookSignal(128);
		pi = m_hookMap.insert(HookSignalMap::value_type(name, sig)).first;
	}

	return (*pi).second->Attach(proc);
}

// 还原到上一次正确的配置
void CConfigManager::restore()
{
}

void CConfigManager::makeNewConfig(const Json::Value &defaultConfig, const Json::Value &nowConfig, Json::Value &newConfig)
{
	Json::Value subConfig;

	switch(defaultConfig.type())
	{
	case Json::nullValue:
		return;
	case Json::intValue:
	case Json::uintValue:
	case Json::realValue:
	case Json::stringValue:
	case Json::booleanValue:
		if(defaultConfig != nowConfig)
		{
			newConfig = nowConfig;
		}
		return;
	case Json::arrayValue:
		int size;

		size = defaultConfig.size();
		for(int i = 0; i < size; i++)
		{
			subConfig.clear();
			makeNewConfig(defaultConfig[i], nowConfig[i], subConfig);
			if(subConfig.type() != Json::nullValue)
			{
				newConfig[i] = subConfig;
			}
		}
		return;
	case Json::objectValue:
		Json::Value::Members defaultMembers(defaultConfig.getMemberNames());
		Json::Value::Members::iterator it;

		for(it = defaultMembers.begin(); it != defaultMembers.end(); it++)
		{
			const std::string &name = *it;

			subConfig.clear();
			makeNewConfig(defaultConfig[name], nowConfig[name], subConfig);
			if(subConfig.type() != Json::nullValue)
			{
				newConfig[name] = subConfig;
			}
		}
		return;
	}
}

/// 控制台调试接口
int CConfigManager::onConsole(int argc, char **argv)
{
	if (argc < 2)
	{
help:
		trace("cfg -d [name] [index] display the config of name!\n");
		trace("cfg -a        display all config name!\n");
		trace("cfg -p        dispaly config file path!\n");
		trace("cfg -m        dispaly map status!\n");
		trace("cfg -c [path] convert config file to styled file!\n");
 		trace("cfg -s [path] style dump config file!\n");
		trace("cfg -n [name] [path] style dump config file!\n");
		trace("cfg -g [config] [dir] save the differences betwen current configs and default configs.\n");
		trace("                         config is the config name,like fVideo, * means all config;\n");
		trace("                         dir is the directory where to save the result\n");
		return 0;
	}
	switch (argv[1][1])
	{
	case 'n':
		{
			Json::Value table;

			if (argc >= 4 && getConfig(argv[2], table))
			{
				std::string stream = table.toStyledString();
				CFile file;

				trace("\n%s\n", stream.c_str());
				if (file.Open(argv[3], CFile::modeReadWrite|CFile::modeNoTruncate|CFile::modeCreate))
				{
					file.Write(stream.c_str(), stream.size());
					file.Close();
				}
				else
				{
					trace("open %s failed!\n", argv[3]);
				}
			}
		}
		break;
	case 'd':
		{
			Json::Value table;

			if (argc >= 3 && getConfig(argv[2], table))
			{
				std::string stream;
				int index = 0;

				if (argc >= 4)
				{
					index = atoi(argv[3]);
					if ((size_t)index >= table.size())
					{
						index = table.size() - 1;
					}
					if (table.type() == Json::arrayValue)
					{
						stream = table[index].toStyledString();
					}
					else
					{
						stream = table.toStyledString();
					}
				}
				else
				{
					stream = table.toStyledString();
				}
				trace("%s:%s\n", argv[2], stream.c_str());
			}
		}
		break;
	case 'a':
		{
			CGuard guard(m_mutex);
			int i = 0;

			Json::Value::Members membersAll(m_configAll.getMemberNames());
			trace("--------------------------------All Config members------------------------------\n");
			for (Json::Value::Members::iterator it = membersAll.begin(); it != membersAll.end(); ++it)
			{
				const std::string &name = *it;
				trace("%18s ", name.c_str());
				if (++i % 4 == 0)
				{
					trace("\n");
				}
			}
			trace("\n");
			Json::Value::Members membersDefault(m_configDefault.getMemberNames());
			trace("\n--------------------------------Default Config members------------------------------\n");
			for (Json::Value::Members::iterator it = membersDefault.begin(); it != membersDefault.end(); ++it)
			{
				const std::string &name = *it;
				trace("%18s ", name.c_str());
				if (++i % 4 == 0)
				{
					trace("\n");
				}
			}
			trace("\n");
			Json::Value::Members membersCustom(m_configCustom.getMemberNames());
			trace("\n--------------------------------Custom Config members------------------------------\n");
			for (Json::Value::Members::iterator it = membersCustom.begin(); it != membersCustom.end(); ++it)
			{
				const std::string &name = *it;
				trace("%18s ", name.c_str());
				if (++i % 4 == 0)
				{
					trace("\n");
				}
			}
			trace("\n");
		}
		break;
	case 'p':
		{
			trace("\nFirst Config File Path:%s\n", sm_firstFileDir.c_str());
			trace("Second Config File Path:%s\n", sm_secondFileDir.c_str());
			trace("Custom Config File Path:%s\n", sm_customFileDir.c_str());
		}
		break;
	case 'm':
		{
			int i;

			trace("\n--------------------------------Apply Map members------------------------------\n");
			i = 0;
			for (SignalMap::iterator pi = m_applyMap.begin(); pi != m_applyMap.end(); pi++)
			{
				trace("%18s", (*pi).first.c_str());
				if (++i % 4 == 0)
				{
					trace("\n");
				}
			}
			trace("\n--------------------------------Verify Map members------------------------------\n");
			i = 0;
			for (SignalMap::iterator pi = m_verifyMap.begin(); pi != m_verifyMap.end(); pi++)
			{
				trace("%18s", (*pi).first.c_str());
				if (++i % 4 == 0)
				{
					trace("\n");
				}
			}

			trace("\n--------------------------------Valid Map members------------------------------\n");
			i = 0;
			for (ValidMap::iterator pi = m_validMap.begin(); pi != m_validMap.end(); pi++)
			{
				trace("%18s", (*pi).first.c_str());
				if (++i % 4 == 0)
				{
					trace("\n");
				}
			}
			trace("\n");
		}
		break;
	case 's':
		{
			if (argc >= 2)
			{
				CGuard guard(m_mutex);
				CConfigReader reader;
				std::string sStreamIn;
				std::string sStreamOut;
				Json::Value tableAll;

				if (readConfig(sm_firstFileDir.c_str(), sStreamIn)
					&& reader.parse(sStreamIn, tableAll))
				{
					#ifdef _DEBUG
						CConfigWriter writer(sStreamOut);
					#else
						CConfigWriter writer;
					#endif
					sStreamOut = writer.write(tableAll);

					CFile destFile;

					if (destFile.Open(argv[2], CFile::modeCreate | CFile::modeReadWrite))
					{
						destFile.Write((void *)sStreamOut.c_str(), sStreamOut.size());
						destFile.Close();
						trace("Config Dump to:%s...\n", argv[2]);
					}
				}
			}
		}
		break;

	case 'c':
		{
			CGuard guard(m_mutex);
			CConfigReader reader;
			std::string sStreamIn;
			std::string sStreamOut;
			Json::Value tableAll;

			if (readConfig(argv[2], sStreamIn)
				&& reader.parse(sStreamIn, tableAll))
			{
				#ifdef _DEBUG
					CConfigWriter writer(sStreamOut);
				#else
					CConfigWriter writer;
				#endif
				sStreamOut = writer.write(tableAll);

				CFile destFile;
				char filePath[260];

				sprintf(filePath, "%s.styled", argv[2]);
				if (destFile.Open(filePath, CFile::modeCreate | CFile::modeReadWrite))
				{
					destFile.Write((void *)sStreamOut.c_str(), sStreamOut.size());
					destFile.Close();
				}
			}
		}
		break;
	case 'g':
		{
			if(argc < 4)
			{
				goto help;
			}
			Json::Value defaultTable;  //默认配置
			Json::Value nowTable;      //当前正使用的配置
			Json::Value newTable;      //上面两个配置的不同部分
			const char* configFile[] = {
				         "AVEnc", "Record", "Detect", "Alarm", "ChannelTitle"
						 ,"Storage", "NetWork", "Uart", "fVideo"
						 ,"General", "Ability", "Media", "NetPlat"
			};
			int start, end;
			char path[70], dir[50];
			FILE* configfd;
			std::string buf;
			char config[20];
			int i;
			int len, count;

			if(argv[2][0] == '*')
			{
				start = 0;
				end = sizeof(configFile)/sizeof(configFile[0]) - 1;
			}
			else
			{
				len = strlen(argv[2]);
				memcpy(config, argv[2], len);
				config[len] = '\0';
				start = -1;
				for(i = 0; i < (int)(sizeof(configFile)/sizeof(configFile[0])); i++)
				{
					if(strcmp(configFile[i], config) == 0)
					{
						start = end = i;
						break;
					}
				}
				if(start == -1)
				{
					printf("error:can't find config file %s\n", config);
					goto help;
				}
			}
			len = strlen(argv[3]);
			memcpy(dir, argv[3], len);
			if(dir[len-1] != '/')
			{
				dir[len] = '/';
				len++;
			}
			dir[len] = '\0';

			for(i = start; i <= end; i++)
			{
				defaultTable.clear();
				nowTable.clear();
				g_configManager.getDefault(configFile[i], defaultTable);
				g_configManager.getConfig(configFile[i], nowTable);

				newTable.clear();
				makeNewConfig(defaultTable, nowTable, newTable);

				if(newTable.type() != Json::nullValue)
				{
					strcpy(path, dir);
					strcat(path, configFile[i]);
					strcat(path, ".custom");
					//Now path likes as /home/xxx/fVideo.custom
					configfd = fopen(path, "w");
					if(configfd == NULL)
					{
						printf("error:invalid path %s\n", path);
						goto help;
					}
					#ifdef _DEBUG
						CConfigWriter writer(buf);
					#else
						CConfigWriter writer;
					#endif
					writer.write(newTable);
					//printf("\n%s\n", buf.c_str());
					count = fwrite(buf.c_str(), sizeof(char), buf.size(), configfd);
					fclose(configfd);
					if(count <= 0)
					{
						printf("error:failed to write change into file\n");
					}
					else
					{
						printf("\n%s: Write change to file %s OK!\n", configFile[i], path);
					}
				}
				else
				{
					printf("\n%s: No change!\n", configFile[i]);
				}
			}
			break;
		}
	default:
		goto help;
	}
	return 0;
}

/// 获取实例句柄
IConfigManager *IConfigManager::instance()
{
	return CConfigManager::instance();
}

/// 虚析构函数
IConfigManager::~IConfigManager()
{
}

void IConfigManager::config(const char *firstDir, const char *secondDir, const char *customDir)
{
	CConfigManager::config(firstDir, secondDir, customDir);
}
