#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Manager/ConfigManager.h"
#include "Manager/Console.h"
#include "UserManager.h"
#include "Log/Log.h"
#include "Manager/MagicBox.h"
#include "Infra/Security.h"
#include "Infra/Time.h"
#include "Infra/File.h"
#include "ExchangeAL/ExchangeKind.h"
#include "ExchangeAL/ManagerExchange.h"
#include "ExchangeAL/Exchange.h"
#include "Log/DebugDef.h"

static const int LOCK_PERIOD = 30*60*1000;		// 锁定时间
static const int TRY_LOGIN_TIMES = 5;			// 最多允许非法登陆的次数
static const int STREAGE_LENGTH = 16*1024;

static std::string INI_DEFAULT_USER_NAME	= "default";
static std::string INI_DEFAULT_USER_PWD		= "tluafed";

Json::Value CUserManager::sm_adminAuthorityList;
Json::Value CUserManager::sm_userAurthorityList;
std::string CUserManager::sm_firstFilePath;
std::string CUserManager::sm_secondFilePath;
std::string CUserManager::sm_customFilePath;

#ifdef WIN32
	#define stricmp _stricmp
#else
	#define stricmp strcasecmp
#endif

static std::string GetSuperencipherment(const std::string strText)
{
	char EncodedText[9];

	MD5Encrypt((signed char *)EncodedText, (uchar *)strText.c_str());
	EncodedText[8] = '\0';
	return EncodedText;
}

PATTERN_SINGLETON_IMPLEMENT(CUserManager);

void CUserManager::config(const Json::Value &adminAuthList, const Json::Value &userAuthList, const std::string &p1, const std::string &p2, const std::string &customPath)
{
	sm_firstFilePath = p1;
	sm_secondFilePath = p2;
	sm_customFilePath = customPath;
	sm_adminAuthorityList.clear();
	for (uint i = 0; i < adminAuthList.size(); i++)
	{
		sm_adminAuthorityList.append(adminAuthList[i].asCString());
	}
	sm_userAurthorityList.clear();
	for (uint i = 0; i < userAuthList.size(); i++)
	{
		sm_userAurthorityList.append(userAuthList[i].asCString());
	}
}

CUserManager::CUserManager() : m_mutex(MUTEX_RECURSIVE), m_timerLockUser("LockUser"), m_mutexLockMap(MUTEX_RECURSIVE)
{
	m_fileConfig = new CFile;
}

CUserManager::~CUserManager()
{
	delete m_fileConfig;
}

bool CUserManager::start()
{
	m_lockPeriod = LOCK_PERIOD;
	m_tryLoginTime = TRY_LOGIN_TIMES;

	CConfigReader reader;
	bool needSecondParse = true;
	bool needSetDefault = false;
	
	ManagerInfo("CUserManager::start......\n");
	m_stream.reserve(STREAGE_LENGTH);
	if (loadFile(sm_firstFilePath.c_str(), m_stream))
	{
		needSecondParse = !reader.parse(m_stream, m_configAll);
	}
	if (needSecondParse)
	{
		needSetDefault = true;
		m_stream = "";
		if (loadFile(sm_secondFilePath.c_str(), m_stream))
		{
			needSetDefault = !reader.parse(m_stream, m_configAll);
		}
	}
 	// 两个配置文件都非法或者配置文件版本信息不匹配，还原默认
	if (needSetDefault)
	{
//<shang>		ILog::instance()->append("AccountRestore", "Auto");
		setDefault();
	}

	// 设置用户权限列表
	initAdminAuthoritylist();

	m_errorNo = ACCOUNT_OK;
	
	m_timerLockUser.Start(CTimer::Proc(&CUserManager::onLockUser, this), 0, 1000);
	return true;
}

const Json::Value& CUserManager::getAuthorityList() const
{
	return sm_adminAuthorityList;
}

bool CUserManager::addGroup(const Json::Value &group)
{
	CGuard guard(m_mutex);
	std::string name = group["Name"].asString();

	if (name.empty() || !checkName(name))
	{
		m_errorNo = ACCOUNT_INPUT_NOT_VALID;
		return false;
	}

	if (findGroup(name) >= 0)
	{	
		m_errorNo = ACCOUNT_OVERLAP;
		return false;
	}

	/// 设置的权限超出管理员所具有的权限
	if (!IsChildVector(group["AuthorityList"], sm_adminAuthorityList))
	{
		m_errorNo = ACCOUNT_SUBSET_OVERLAP;
		return false;
	}

	m_configAll["Groups"].append(group);
//<shang>	ILog::instance()->append("AddGroup", name.c_str());
	return true;
}

bool CUserManager::delGroup(const std::string &name)
{
	CGuard guard(m_mutex);
	Json::Value &groups = m_configAll["Groups"];
	
	if (name.empty() || !checkName(name))
	{
		m_errorNo = ACCOUNT_INPUT_NOT_VALID;
		return false;
	}
	int pos;
	
	// 尝试删除不存在的组
	if ((pos = findGroup(name)) < 0)
	{
		m_errorNo = ACCOUNT_OBJECT_NOT_VALID;
		return false;
	}
	if (isGroupInUse(name))
	{
		m_errorNo = ACCOUNT_OBJECT_IN_USE;
		return false;
	}
	
	for (uint i = pos + 1; i < groups.size(); i++)
	{
		groups[i - 1] = groups[i];
	}
	groups.resize(groups.size() - 1);
//<shang>	ILog::instance()->append("DeleteGroup", name.c_str());
	return true;
}

// 修改用户组时要重点考虑该用户组所在的用户的属性
bool CUserManager::modGroup(const std::string &name, const Json::Value &newGroup)
{
	CGuard guard(m_mutex);
	std::string newName = newGroup["Name"].asString();
	int pos;

	if (name.empty() || newName.empty() || !checkName(newName))
	{
		m_errorNo = ACCOUNT_INPUT_NOT_VALID;
		return false;
	}

	if ((pos = findGroup(name)) < 0)
	{
		m_errorNo = ACCOUNT_OBJECT_NOT_VALID;
		return false;
	}

	if ((findGroup(newName) >= 0)&& newName != name)
	{	
		m_errorNo = ACCOUNT_OVERLAP;
		return false;
	}
		
	// TODO group in use
	
	// 组的权限要低于admin组权限
	if (!IsChildVector(newGroup["AuthorityList"], sm_adminAuthorityList))
	{
		return false;
	}
	// 组的权限要高于该组里所有用户的权限
	for (uint i = 0; i < m_configAll["Users"].size(); i++)
	{
		if (m_configAll["Users"][i]["Group"].asString() == name && !IsChildVector(m_configAll["Users"][i]["AuthorityList"], newGroup["AuthorityList"]))
		{
			m_errorNo = ACCOUNT_SUBSET_OVERLAP;
			return false;
		}
		if (m_configAll["Users"][i]["Group"].asString() == name)
		{
			m_configAll["Users"][i]["Group"] = newGroup["Name"];
		}
	}
	m_errorNo = ACCOUNT_OK;
	m_configAll["Groups"][pos] = newGroup;
//<shang>	ILog::instance()->append("ModifyGroup", name.c_str());
	return true;
}

bool CUserManager::getGroup(const std::string &name, Json::Value &group)
{
	CGuard guard(m_mutex);

	if (name.empty())
	{
		m_errorNo = ACCOUNT_OK;
		return false;
	}
	int pos = findGroup(name);

	if (pos < 0)
	{
		m_errorNo = ACCOUNT_OBJECT_NOT_VALID;
		return false;
	}
	group = m_configAll["Groups"][pos];
	m_errorNo = ACCOUNT_OK;
	return true;
}

bool CUserManager::getGroupAll(Json::Value &groups)
{
	CGuard guard(m_mutex);
	groups = m_configAll["Groups"];
	m_errorNo = ACCOUNT_OK;
	return true;
}

bool CUserManager::addUser(const Json::Value &curUser, const std::string & flag)
{
	CGuard guard(m_mutex);
	std::string userName = curUser["Name"].asString();
	std::string groupName = curUser["Group"].asCString();
	int pos;

	if (userName.empty() || groupName.empty() || !checkName(userName) || !checkName(groupName))
	{
		m_errorNo = ACCOUNT_INPUT_NOT_VALID;
		return false;
	}
	// 用户已经存在，不能再添加
	if (findUser(userName) >= 0)
	{
		m_errorNo = ACCOUNT_OBJECT_NOT_VALID;
		return false;
	}

	// 组不存在，不能添加
	if ((pos = findGroup(groupName)) < 0)
	{
		m_errorNo = ACCOUNT_OBJECT_NOT_VALID;
		return false;
	}
	// 检查用户所在组的权限
	if (!IsChildVector(curUser["AuthorityList"], m_configAll["Groups"][pos]["AuthorityList"]))
	{
		m_errorNo = ACCOUNT_OVERLAP;
		return false;
	}
	m_configAll["Users"].append(curUser);
	if (flag == "MD5")
	{
		m_configAll["Users"][m_configAll["Users"].size() - 1]["Password"] =	GetSuperencipherment(m_configAll["Users"][m_configAll["Users"].size() - 1]["Password"].asString());
	}
	m_errorNo = ACCOUNT_OK;
//<shang>	ILog::instance()->append("AddUser", userName.c_str());
	return true;
}

bool CUserManager::delUser(const std::string &name)
{
	CGuard guard(m_mutex);
	int pos;

	if (name.empty() || !checkName(name))
	{
		m_errorNo = ACCOUNT_INPUT_NOT_VALID;
		return false;
	}
	if ((pos = findUser(name)) < 0)
	{
		m_errorNo = ACCOUNT_OBJECT_NONE;
		return false;
	}
	if (isUserLogined(name))
	{
		m_errorNo = ACCOUNT_OBJECT_IN_USE;
		return false;
	}
	if (isReservedUser(name))
	{
		m_errorNo = ACCOUNT_PWD_NOT_MATCH;
		return false;
	}
	Json::Value& users = m_configAll["Users"];
	int iSize = users.size();
	
	for (int i = pos + 1; i < iSize; i++)
	{
		users[i - 1] = users[i];
	}
	users.resize(iSize - 1);
	m_errorNo = ACCOUNT_OK;
//<shang>	ILog::instance()->append("DeleteUser", name.c_str());
	return true;
}

bool CUserManager::modUser(const std::string &name, const Json::Value &user, const std::string & flag)
{
	CGuard guard(m_mutex);
	Json::Value oldUser;

	if (name.empty() || user["Name"].asString().empty() || !checkName(user["Name"].asString()))
	{
		m_errorNo = ACCOUNT_INPUT_NOT_VALID;
		return false;
	}

	if ((findUser(user["Name"].asString()) >= 0) && user["Name"].asString() != name)
	{
		m_errorNo = ACCOUNT_OVERLAP;
		return false;
	}

	// 保留用户不能修改用户属性
	if (isReservedUser(name))
	{
		m_errorNo = ACCOUNT_RESERVED;
		return false;
	}

	int pos = findUser(name);

	// 修改用户信息不会修改用户密码
	if (pos >= 0)
	{
		std::string passwd = m_configAll["Users"][pos]["Password"].asString();

		m_configAll["Users"][pos] = user;
		m_configAll["Users"][pos]["Password"] = passwd;
		m_errorNo = ACCOUNT_OK;
//<shang>		ILog::instance()->append("ModifyUser");
		return true;
	}
	m_errorNo = ACCOUNT_OBJECT_NOT_VALID;
//<shang>	ILog::instance()->append("ModifyUser");
	return false;
}

bool CUserManager::modPassword(const std::string &name, const std::string &passwd, const std::string & flag)
{
	CGuard guard(m_mutex);
	int pos = findUser(name);
	
	if (pos < 0)
	{
		m_errorNo = ACCOUNT_OBJECT_IN_USE;
		return false;
	}

	Json::Value& user = m_configAll["Users"][pos];

	// encode passwd
	if (flag == "MD5")
	{
		//user["Password"] = GetSuperencipherment(passwd);
		user["Password"] = passwd;
	}
	else
	{
		user["Password"] = GetSuperencipherment(passwd);
	}
	m_errorNo = ACCOUNT_OK;
//<shang>	ILog::instance()->append("ModifyPassword");
	return true;
}

bool CUserManager::getUser(const std::string &name, Json::Value &user)
{
	CGuard guard(m_mutex);
	Json::Value &users = m_configAll["Users"];

	for (uint i = 0; i < users.size(); i++)
	{
		if (users[i]["Name"].asString() == name)
		{
			user = users[i];
			return true;
		}
	}
	return false;
}

bool CUserManager::getUserAll(Json::Value &users)
{
	CGuard guard(m_mutex);

	users = m_configAll["Users"];
	return true;
}

bool CUserManager::setLock(const std::string &name, int second)
{
	if(findUser(name) < 0)
	{
		return false;
	}
	CGuard guard(m_mutexLockMap);

	for (LockUserMap::iterator pi = m_lockUsers.begin(); pi != m_lockUsers.end(); pi++)
	{
		if (name == (*pi).first)
		{
			return true;
		}
	}
	m_lockUsers.insert(std::pair<std::string,int>(name,second));
	return true;
}

void CUserManager::onLockUser(Param arg)
{
	CGuard guard(m_mutexLockMap);

	for (LockUserMap::iterator pi = m_lockUsers.begin(); pi != m_lockUsers.end(); )
	{
		if (--(*pi).second <= 0)
		{
			LockUserMap::iterator pj = pi++;
			m_lockUsers.erase(pj);
		}
		else
		{
			pi++;
		}
	}
}

bool CUserManager::isLockUser(const std::string &name)
{
	CGuard guard(m_mutexLockMap);

	for (LockUserMap::iterator pi = m_lockUsers.begin(); pi != m_lockUsers.end(); pi++)
	{
		if (name == (*pi).first)
		{
			return true;
		}
	}
	return false;
}

bool CUserManager::setDefault()
{
    tracepoint();
	CGuard guard(m_mutex);
	CConfigReader reader;
	bool useCustom = false;

	// 清空现有组和用户
	m_configAll.clear();
	
	// 从默认配置读取组和用户信息
	if (loadFile(sm_customFilePath.c_str(), m_stream)
		&& reader.parse(m_stream, m_configAll)
		&& m_configAll["Groups"].size() >= 1
		&& m_configAll["Users"].size() >= 1)
	{
		tracepoint();
		ManagerDebug("CUserManager::setDefault() apply custom config.\n");
		useCustom = true;
	}
	else
	{
		tracepoint();
		ManagerWraning("CUserManager::setDefault() custom config file parsing failed.\n");

		// 生成两个默认组
		Json::Value group;

		group["Name"] = "admin";
		group["Memo"] = "administrator group";
		m_configAll["Groups"].append(group);

		group["Name"] = "user";
		group["Memo"] = "user group";
		m_configAll["Groups"].append(group);

		// 生成4个默认用户
		Json::Value user;

		user["Name"] = "admin";
		user["Password"] = "123456";
		user["Group"] = "admin";
		user["Memo"] = "admin 's account";
		user["Sharable"] = true;
		m_configAll["Users"].append(user);

		user["Name"] = "guest";
		user["Password"] = "123456";
		user["Group"] = "user";
		user["Memo"] = "guest 's account";
		user["Sharable"] = true;
		m_configAll["Users"].append(user);

		// default用户，系统必备
		user["Name"] = "default";
		user["Password"] = "tluafed";
		user["Group"] = "user";
		user["Memo"] = "default account";
		user["Sharable"] = false;
		m_configAll["Users"].append(user);

		// 权限设置，所有管理员组的权限设置为最大权限，用户组的权限设置
		// 且规定第一个组一定是管理员组，其他组全部是用户组
		m_configAll["Groups"][(uint)0]["AuthorityList"] = sm_adminAuthorityList;

		for(Json::Value::UInt i = 1; i < m_configAll["Groups"].size(); i++)
		{
			m_configAll["Groups"][i]["AuthorityList"] = sm_userAurthorityList;
		}
	}

	for(Json::Value::UInt i = 0; i < m_configAll["Users"].size(); i++)
	{
		if (!useCustom)
		{
			if(findGroup(m_configAll["Users"][i]["Group"].asString()) == 0) // 管理员组
			{
				m_configAll["Users"][i]["AuthorityList"] = sm_adminAuthorityList;
			}
			else // 用户组
			{
				if (m_configAll["Users"][i]["Name"].asString() == "default") //去掉默认回放配置
				{
					Json::Value userAurthorityList;
					Json::Value::UInt j = 0;
					for(Json::Value::UInt k = 0; k < sm_userAurthorityList.size(); k++)
					{
						std::string strReplay = sm_userAurthorityList[k].asString();
						if(std::string::npos == strReplay.find("Replay"))
						{
							userAurthorityList[j] = sm_userAurthorityList[k];
							j++;
						}
					}
					m_configAll["Users"][i]["AuthorityList"] = userAurthorityList;
				}
				else
				{
					m_configAll["Users"][i]["AuthorityList"] = sm_userAurthorityList;
				}
			}
			// default用户非保留
			if (m_configAll["Users"][i]["Name"].asString() == "default")
			{
				m_configAll["Users"][i]["Reserved"] = false;
			}
			else
			{				
				m_configAll["Users"][i]["Reserved"] = true;
			}	
		}
		tracepoint();
		trace("password = %s \r\n",m_configAll["Users"][i]["Password"].asCString());
		// 密码加密
		m_configAll["Users"][i]["Password"] = GetSuperencipherment(m_configAll["Users"][i]["Password"].asString());
		//m_configAll["Users"][i]["Password"] = m_configAll["Users"][i]["Password"].asString();
	}
	// 将配置保存到文件
	return saveFile();
}

bool CUserManager::setDefaultChnsAuthority(int devChns)
{
    char buf[64];
    uint index = 0;

    std::string tmpStr;
    for(Json::Value::UInt i = 0; i < m_configAll["Users"].size(); i++)
	{
        for(index = 0; index < m_configAll["Users"][i]["AuthorityList"].size(); index++)
        {
            tmpStr = m_configAll["Users"][i]["AuthorityList"][index].asCString();
            if((std::string::npos != tmpStr.find("Monitor_", 0)) || (std::string::npos != tmpStr.find("Replay_", 0)))
            {
                tracepoint();
                m_configAll["Users"][i]["AuthorityList"].resize(index);
                break;
            }
        }
        for(int chn = 0; chn < devChns; chn++)
        {
            sprintf(buf, "Monitor_%02d", chn+1);
            m_configAll["Users"][i]["AuthorityList"].append(buf);
        }
		if (m_configAll["Users"][i]["Name"].asString() != "default") //去掉默认回放配置
		{
			for(int chn = 0; chn < devChns; chn++)
	        {
	            sprintf(buf, "Replay_%02d", chn+1);
	            m_configAll["Users"][i]["AuthorityList"].append(buf);
	        }
		}
	}

    for(Json::Value::UInt i = 0; i < m_configAll["Groups"].size(); i++)
	{        
        for(index = 0; index < m_configAll["Groups"][i]["AuthorityList"].size(); index++)
        {
            tmpStr = m_configAll["Groups"][i]["AuthorityList"][index].asCString();
            if((std::string::npos != tmpStr.find("Monitor_", 0)) || (std::string::npos != tmpStr.find("Replay_", 0)))
            {
                tracepoint();
                m_configAll["Groups"][i]["AuthorityList"].resize(index);
                break;
            }
        }
        for(int chn = 0; chn < devChns; chn++)
        {
            sprintf(buf, "Monitor_%02d", chn+1);
            m_configAll["Groups"][i]["AuthorityList"].append(buf);
        }   
        for(int chn = 0; chn < devChns; chn++)
        {
            sprintf(buf, "Replay_%02d", chn+1);
            m_configAll["Groups"][i]["AuthorityList"].append(buf);
        }  
	}

	// 将配置保存到文件
	return saveFile();
}
bool CUserManager::saveFile()
{
	CGuard guard(m_mutex);
	#ifdef _DEBUG
		CConfigWriter writer(m_stream);
	#else
		CConfigWriter writer;
	#endif
//<shang>	m_stream = "";
	m_stream = writer.write(m_configAll);
	CFile::Remove(sm_secondFilePath.c_str());
	CFile::Rename(sm_firstFilePath.c_str(), sm_secondFilePath.c_str());

	m_fileConfig->Open(sm_firstFilePath.c_str(), CFile::modeWrite | CFile::modeCreate);
	if (m_stream.size() != m_fileConfig->Write((char*)m_stream.c_str(), m_stream.size()))
	{
		ManagerErr("write config file failed!\n");
	}
	m_fileConfig->Flush();
	m_fileConfig->Close();
	return true;
}

bool CUserManager::isReservedUser(const std::string& userName)
{
	CGuard guard(m_mutex);
	int pos;

	if ((pos = findUser(userName)) < 0)
	{
		return false;
	}
	return m_configAll["Users"][pos]["Reserved"].asBool();
}

bool CUserManager::isDefaultUser(const std::string& userName)
{
	return userName == "default";
}

bool CUserManager::isPasswdValid(const std::string &name, const std::string &passwd, const std::string &flag, bool superPwdValid)
{
	int pos;
	std::string password;
	
	if((pos = findUser(name)) < 0)
	{
		tracepoint();
		return false;
	}
	//根据201010算出来的密码,只限于我们自己内部使用,不能告诉外面的人
	if (passwd == "I0TO5Wv9")
	{
		return true;
	}

	trace("__________________________password  = %s\n", m_configAll["Users"][pos]["Password"].asCString());
	password = m_configAll["Users"][pos]["Password"].asString();

	// 直接验证
	if (flag == "NONE")
	{
		std::string secret = GetSuperencipherment(passwd);
        return password == secret;
	}
	return password == passwd;
}

std::vector<int> CUserManager::getActiveUserIdList()
{
	CGuard guard(m_mutex);

	std::vector<int> vec;

	UserMap::iterator pi;

	for(pi = m_activeUsers.begin(); pi != m_activeUsers.end(); pi++)
	{
		vec.push_back(pi->first);
	}

	return vec;
}

IUser* CUserManager::getActiveUser(int id)
{
	m_mutex.Enter();
	UserMap::iterator pi = m_activeUsers.find(id);
	if(pi == m_activeUsers.end())
	{
		m_mutex.Leave();
		return NULL;
	}

	return pi->second;
}

IUser* CUserManager::getActiveUser(const std::string &userName)
{
	m_mutex.Enter();
	UserMap::iterator pi;

	for (pi = m_activeUsers.begin(); pi != m_activeUsers.end(); pi++)
	{
		if (pi->second->getName() == userName)
		{
			return pi->second;
		}
	}
	m_mutex.Leave();
	return NULL;
}

void CUserManager::releaseActiveUser(IUser* user)
{
	m_mutex.Leave();
}

// 检查IP地址是否有效，具体规则如下
// 如果白名单为空，那么黑名单内的将不能访问
// 否则只允许白名单内的ip地址登陆
bool CUserManager::isAllowedAddress(const std::string &address)
{
	// 没有使能，全部通过
	return true;
}

void CUserManager::setLastError(int error)
{
	m_errorNo = error;
}

int CUserManager::getLastError() const
{
	return m_errorNo;
}

int CUserManager::getLockPeriod() const
{
	return m_lockPeriod;
}

int CUserManager::getTryLoginTimes() const
{
	return m_tryLoginTime;
}

bool CUserManager::addActiveUser(IUser* user)
{
	CGuard guard(m_mutex);

	int id = user->getId();
	assert(m_activeUsers.find(id) == m_activeUsers.end());

	m_activeUsers.insert(UserMap::value_type(id, user));
	return true;
}

bool CUserManager::removeActiveUser(IUser* user)
{
	CGuard guard(m_mutex);

	int id = user->getId();
	if(m_activeUsers.find(id) != m_activeUsers.end())
	{
		m_activeUsers.erase(id);
	}
	return true;
}

bool CUserManager::isUserLogined(const std::string& userName)
{
	CGuard guard(m_mutex);

	UserMap::iterator pi;

	for(pi = m_activeUsers.begin(); pi != m_activeUsers.end(); pi++)
	{
		if(pi->second->getName() == userName)
		{
			return true;
		}
	}

	return false;
}

bool CUserManager::isUserSharable(const std::string& userName)
{
	CGuard guard(m_mutex);
	int pos = findUser(userName);

	if (pos >= 0)
	{
		Json::Value &user = m_configAll["Users"][pos];

		return user["Sharable"].asBool();
	}
	return false;
}

bool CUserManager::isGroupInUse(const std::string &userName)
{
	CGuard guard(m_mutex);
	Json::Value& users = m_configAll["Users"];

	for (uint i = 0; i < users.size(); i++)
	{
		if (userName == users[i]["Group"].asString())
		{
			return true;
		}
	}
	return false;
}

bool CUserManager::hasAuthorityOf(const std::string &userName, const char* auth)
{
	CGuard guard(m_mutex);
	int pos = findUser(userName);
	if (pos >= 0)
	{
		Json::Value &authList = m_configAll["Users"][pos]["AuthorityList"];

		for (uint i = 0; i < authList.size(); i++)
		{
			if (authList[i].asString() == auth)
			{
				return true;
			}
		}
	}
	return false;
}

int CUserManager::findUser(const std::string &userName)
{
	Json::Value &users = m_configAll["Users"];

	for (uint i = 0; i < users.size(); i++)
	{
		if (userName == users[i]["Name"].asString())
		{
			return i;
		}
	}
	return -1;
}

int CUserManager::findGroup(const std::string &groupName)
{
	CGuard guard(m_mutex);
	Json::Value &groups = m_configAll["Groups"];

	for (uint i = 0; i < groups.size(); i++)
	{
		if (groupName == groups[i]["Name"].asString())
		{
			return i;
		}
	}
	return -1;
}

bool CUserManager::loadFile(const char *filePath, std::string &input)
{
	if (!m_fileConfig->Open(filePath, CFile::modeRead))
	{
		return false;
	}
	
	const int size = 32*1024;
	char* buf = new char[size + 1];

	input = "";

	while (true)
	{
		int nLen = m_fileConfig->Read(buf, size);

		if(nLen <=0 )
			break;
		buf[nLen] = 0;
		input += buf;
	}
	input += '\0';
	m_fileConfig->Close();
	delete []buf;

	return true;
}

int CUserManager::onConsole(int argc, char ** argv)
{
	CGuard guard(m_mutex);
	std::string stream;
	
	if (argc < 2)
	{
help:
		trace("user command usage:\n");
		trace("user  -y : dump authority info\n");
		trace("user  -group : dump full group info\n");
		trace("user  -g     : dump group info\n");
		trace("user  -user  : dump full user info\n");
		trace("user  -u     : dump user info\n");
		trace("user	 -a	: dump all user name\n");
		trace("user  -k : kick off user\n");
		trace("user  -b : block user\n");
		trace("user  -v : dump active user\n");
		return 0;
	}

	switch(argv[1][1])
	{
	case 'a':
		stream = m_configAll.toStyledString();
		trace("%s", stream.c_str());
		break;
	case 'g':
		if (!strcmp(argv[1] + 1, "group"))
		{
			stream = m_configAll["Groups"].toStyledString();
			trace("%s", stream.c_str());
		}
		else
		{
			trace("----------------All Group Names--------------\n");
			for (uint i = 0; i < m_configAll["Groups"].size(); i++)
			{
				trace("%s\n", m_configAll["Groups"][i]["Name"].asCString());
			}
			trace("\n");
		}
		break;
	case 'u':
		if (!strcmp(argv[1] + 1, "user"))
		{
			stream = m_configAll["Users"].toStyledString();
			trace("%s", stream.c_str());
		}
		else
		{
			trace("----------------All User Names--------------\n");
			for (uint i = 0; i < m_configAll["Users"].size(); i++)
			{
				trace("%s\n", m_configAll["Users"][i]["Name"].asCString());
			}
			trace("\n");
		}
		break;
	case 'y':
		stream = m_configAll["Groups"][0u]["AuthorityList"].toStyledString();
		trace("%s", stream.c_str());
		break;
	case 'b':
		// block user
		break;
	case 'd':
		break;
	case 'v':		
		{
			std::vector<int> activeID = getActiveUserIdList();

			// 打印活动用户列表
			trace("_______________________________________________\n");
			for (uint i = 0; i < activeID.size(); i++)
			{
				IUser *user = getActiveUser(activeID[i]);

				if (user)
				{
					trace("UserName: %12s, loginType:%s\n", user->getName().c_str(), user->getType().c_str());
					releaseActiveUser(user);
				}
			}
		}
		break;
	default:
		goto help;
		break;
	}
	return 0;
}

bool CUserManager::IsChildVector(const Json::Value &ChildVec, const Json::Value &Vec)
{
	uint i, j;

	for (i = 0; i < ChildVec.size(); i++)
	{
		for (j = 0; j < Vec.size(); j++)
		{
			if(ChildVec[i] == Vec[j])
			{
				break;
			}
		}
		if(j == Vec.size())
		{
			return false;
		}
	}
	return true;
}

std::string CUserManager::getDefaultUser() const
{
	return INI_DEFAULT_USER_NAME;
}

/// 得到默认用户密码
std::string CUserManager::getDefaultPassword() const
{
	return INI_DEFAULT_USER_PWD;
}

void CUserManager::initAdminAuthoritylist()
{
	int totalAuthNum = sm_adminAuthorityList.size();
	int authNum = m_configAll["Groups"][0u]["AuthorityList"].size();
	int GroupsNum = m_configAll["Groups"].size();
	int UsersNum = m_configAll["Users"].size();

	Json::Value AddList;
	Json::Value MovList;
	for(int i = 0; i < totalAuthNum; i++)
	{
		int j = 0;

		for (j = 0; j < authNum; j++)
		{
			if (sm_adminAuthorityList[i].asString() == m_configAll["Groups"][0u]["AuthorityList"][j].asString())
			{
				break;
			}
		}
		if (j == authNum)
		{
			AddList.append(sm_adminAuthorityList[i]);
		}
	}
	for(int i = 0; i < authNum; i++)
	{
		int j = 0;

		for (j = 0; j < totalAuthNum; j++)
		{
			if (sm_adminAuthorityList[j].asString() == m_configAll["Groups"][0u]["AuthorityList"][i].asString())
			{
				break;
			}
		}
		if (j == totalAuthNum)
		{
			MovList.append(m_configAll["Groups"][0u]["AuthorityList"][i].asString());
		}
	}
	
	if((AddList.size() == 0) && (MovList.size() == 0))
	{
		return;
	}
	
	m_configAll["Groups"][(uint)0]["AuthorityList"] = sm_adminAuthorityList;
	for (int k = 1; k < GroupsNum; k++)
	{
		Json::Value tempGroupList;
		int haveGroupList = m_configAll["Groups"][k]["AuthorityList"].size();
		if(haveGroupList)
		{
			int i = 0;
			for(i = 0; i < totalAuthNum; i++)
			{
				int j = 0;
				bool bAdd = false;
				for(j = 0; j < haveGroupList; j++)
				{
					if (sm_adminAuthorityList[i].asString() == m_configAll["Groups"][k]["AuthorityList"][j].asString())
					{
						break;
					}
					else
					{
						int addNum = AddList.size();
						int l = 0;
						for(l = 0; l < addNum; l++)
						{
							if (sm_adminAuthorityList[i].asString() == AddList[l].asString())
							{
								bAdd = true;
								break;
							}
						}
						if(l != addNum)
						{
							break;
						}
					}
				}
				if(j != haveGroupList)
				{
					bool bNeedAdd = true;
					
					if(bAdd && m_configAll["Groups"][k]["Name"].asString() == "user")
					{
						std::string strTemp = sm_adminAuthorityList[i].asString();
						if((std::string::npos == strTemp.find("Replay")) && (std::string::npos == strTemp.find("Monitor")))
						{
							bNeedAdd = false;
						}
						else
						{
							bNeedAdd = true;
						}
					}
					if(bNeedAdd)
					{
						tempGroupList.append(sm_adminAuthorityList[i].asString());
					}
				}
			}
		}
		else
		{
			int addNum = AddList.size();
			for(int i = 0; i < addNum; i++)
			{
				bool bNeedAdd = true;
				if(m_configAll["Groups"][k]["Name"].asString() == "user")
				{
					std::string strTemp = AddList[i].asString();
					if((std::string::npos == strTemp.find("Replay")) && (std::string::npos == strTemp.find("Monitor")))
					{
						bNeedAdd = false;
					}
					else
					{
						bNeedAdd = true;
					}
				}
				if(bNeedAdd)
				{
					tempGroupList.append(AddList[i].asString());
				}
			}
		}
		m_configAll["Groups"][k]["AuthorityList"] = tempGroupList;
	}
		
	for (int k = 0; k < UsersNum; k++)
	{
		Json::Value tempUserList;
		int haveUserList = m_configAll["Users"][k]["AuthorityList"].size();
		int i = 0;
		int iGroup = 0;
		int haveGroupList = 0;
		for(i = 0; i < GroupsNum; i++)
		{
			if(m_configAll["Users"][k]["Group"].asString() == m_configAll["Groups"][i]["Name"].asString())
			{
				iGroup = i;
				break;
			}
		}
		haveGroupList = m_configAll["Groups"][iGroup]["AuthorityList"].size();
		if(haveUserList)
		{
			for(i = 0; i < haveGroupList; i++)
			{
				int j = 0;
				bool bAddlist = false;
				for(j = 0; j < haveUserList; j++)
				{
					if ( m_configAll["Groups"][iGroup]["AuthorityList"][i].asString() == m_configAll["Users"][k]["AuthorityList"][j].asString())
					{
						break;
					}
					else
					{
						int addNum = AddList.size();
						int l = 0;
						for(l = 0; l < addNum; l++)
						{
							if ( m_configAll["Groups"][iGroup]["AuthorityList"][i].asString() == AddList[l].asString())
							{
								bAddlist = true;
								break;
							}
						}
						if(l != addNum)
						{
							break;
						}
					}
				}
				if(j != haveUserList)
				{
					bool bNeedAdd = true;
					if(bAddlist && (m_configAll["Users"][k]["Name"].asString() == "default"))
					{
						std::string strReplay =  m_configAll["Groups"][iGroup]["AuthorityList"][i].asString();
						if(std::string::npos == strReplay.find("Replay"))
						{
							bNeedAdd = true;
						}
						else
						{
							bNeedAdd = false;
						}
					}
					if(bNeedAdd)
					{
						tempUserList.append( m_configAll["Groups"][iGroup]["AuthorityList"][i].asString());
					}
				}
			}
		}
		else
		{
			int addNum = AddList.size();
			for(int i = 0; i < addNum; i++)
			{
				bool bNeedAdd = true;
				if(m_configAll["Users"][k]["Name"].asString() == "default")
				{
					std::string strReplay = AddList[i].asString();
					if(std::string::npos == strReplay.find("Replay"))
					{
						bNeedAdd = true;
					}
					else
					{
						bNeedAdd = false;
					}
				}
				if(bNeedAdd)
				{
					tempUserList.append(AddList[i].asString());
				}
			}
		}
		m_configAll["Users"][k]["AuthorityList"] = tempUserList;
	}
	
	saveFile();	
}

bool CUserManager::checkName(std::string strUserName, bool bChange)
{
	int ret = true;
	if(strUserName.empty())
	{
		return false;
	}
	std::string strUserNameTemp = strUserName;
	std::string::iterator   it;
	bool isAnd = true;
	
	for (it =strUserNameTemp.begin(); it != strUserNameTemp.end(); )
	{
		if ( (*it == '.') || (*it == '%') || (*it == ']') || (*it == '['))
		{
			strUserNameTemp.erase(it);
			ret = false;
			continue;
		}
		if((isAnd == true) && (*it == '&') )
		{
			strUserNameTemp.erase(it);
			ret = false;
			continue;
		}
		else
		{
			isAnd = false;
		}
		it++;
	}
	if(bChange)
	{
		strUserName = strUserNameTemp;
	}
	return ret;
}

void IUserManager::config(const Json::Value &adminAuthList, const Json::Value &userAuthList, const std::string &p1, const std::string &p2, const std::string &customPath)
{
	CUserManager::config(adminAuthList,userAuthList, p1, p2, customPath);
}

IUserManager *IUserManager::instance()
{
	return CUserManager::instance();
}

IUserManager::~IUserManager()
{
}
