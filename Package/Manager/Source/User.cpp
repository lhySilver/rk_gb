#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "User.h"
#include "Manager/UserManager.h"
#include "Log/Log.h"
#include "Manager/MagicBox.h"
#include "ExchangeAL/ManagerExchange.h"
#include "Log/DebugDef.h"

static void AddressConvert(const uint iIp, char *pStr)
{
	unsigned char i;
	unsigned char cIp[4];

	for (i=0; i<4; i++)
		cIp[i] = (unsigned char)( (iIp>>(i*8)) & (0xff) );

	sprintf(pStr, "%d.%d.%d.%d", cIp[0], cIp[1], cIp[2], cIp[3]);
}

int CUser::sm_maxUserId = 1;	// 最大的用户ID，当用户登陆的时候会自动给用户复上一个唯一的ID

CUser *CUser::getLocalUser()
{
	static CUser s_user;
	return &s_user;
}

CUser::CUser()
{	
	clear();
	m_userManager = IUserManager::instance();
}

CUser::~CUser()
{
	logout();
}

bool CUser::valid()
{
	return m_valid;
}

bool CUser::login(const std::string &name, const std::string &passwd, const std::string &loginType, const std::string &flag, const std::string &address)
{
	Json::Value table;
	trace("login(%s, ******, %s, address:%s)\n", name.c_str(), loginType.c_str(), address.c_str());
	ManagerDebug("login(%s, ******, %s, address:%s)\n", name.c_str(), loginType.c_str(), address.c_str());
	if (m_userManager->getUser(name, table))
	{
		if (!address.empty() && !m_userManager->isAllowedAddress(address))
		{
			m_errorNo = ACCOUNT_USER_IN_BLACKLIST;
			ManagerErr("user:%s in blacklist\n", name.c_str());
			return false;
		}
		if (m_locked)
		{
			// 锁定超时，自动解锁
			if (CTime::getCurrentMicroSecond() - m_lockTime > (ulong)m_userManager->getLockPeriod())
			{
				m_tryLockNum = 0;
				m_locked = false;
			}
			else
			{
				m_errorNo = ACCOUNT_USER_LOCKED;
				return false;
			}
		}
		
		if (IUserManager::instance()->isLockUser(name))
		{
			ManagerErr("User: %s is locked\n", name.c_str());
			m_errorNo = ACCOUNT_USER_LOCKED;
			return false;
		}

		// 判断用户名是否有效
		if (m_userManager->findUser(name) < 0)
		{
			tracepoint();
			m_errorNo = ACCOUNT_USER_NOT_VALID;
			ManagerErr("user:%s not valid\n", name.c_str());
			return false;
		}
	
		// 用户已经登陆，检查复用权限
		if (m_userManager->isUserLogined(name) && !m_userManager->isUserSharable(name))
		{
			tracepoint();
			if (m_userManager->isUserSharable(name))
			{
				m_tryLockNum++;

				// 次数超出，锁定该用户
				if (m_tryLockNum > m_userManager->getTryLoginTimes())
				{
					m_locked = true;
					m_lockTime = CTime::getCurrentMicroSecond();
				}
			}
			m_errorNo = ACCOUNT_USER_HAS_USED;
			ManagerErr("user:%s has used\n", name.c_str());
			return false;
		}

		// 检查是否到期，到期了只能用升级工具升级
		int trailDays;
		if (IMagicBox::instance()->getTrailDays(trailDays))
		{
			tracepoint();
			ManagerDebug("RunningTime:%d day, Trail:%d day\n", IMagicBox::instance()->getRunningTime()/(24*60), trailDays);
			if (IMagicBox::instance()->getRunningTime()/(24*60) >= (uint)trailDays)
			{
				ManagerDebug("outof trail range!\n");
				IMagicBox::instance()->beep(1000, 1000);
				if (!strstr(loginType.c_str(), "Upgrade"))
				{	
					m_errorNo = ACCOUNT_USER_LOCKED;
					return false;
				}
			}
		}

		// 检查用户密码
		if (!m_userManager->isPasswdValid(name, passwd, flag))
		{
			tracepoint();
			m_errorNo = ACCOUNT_PASSWORD_NOT_VALID;
			m_tryLockNum++;
			if (m_tryLockNum > m_userManager->getTryLoginTimes())
			{
				m_locked = true;
				m_lockTime = CTime::getCurrentMicroSecond();
			}
			
			ManagerErr("user:%s password invalid\n", name.c_str());
			return false;
		}
		// 先将原先的用户注销
		if (m_valid)
		{
			logout();
		}
		uint ipaddr;
		char sIpaddr[32];

		// 成功登陆
		m_valid = true;
		m_userId = sm_maxUserId++;
		m_userName = name;	
		m_userPasswd = passwd;	
		m_iloginType = loginType;
		m_passwordFlag = flag;

		memset(sIpaddr, 0, sizeof(sIpaddr));
		sscanf(address.c_str(), "0x%x", &ipaddr);
		AddressConvert(ipaddr, sIpaddr);
		m_loginAddress = sIpaddr;
		m_userManager->addActiveUser(this);
//<shang>		ILog::instance()->append("LogIn", "%s,%s", m_userName.c_str(), m_iloginType.c_str());
		return true;
	}
	m_errorNo = ACCOUNT_USER_NOT_VALID;
	ManagerErr("user:%s account invalid\n", name.c_str());
	return false;
}

bool CUser::logout()
{
	if (!m_valid)
	{
		return false;
	}
	m_userManager->removeActiveUser(this);
//<shang>	ILog::instance()->append("LogOut", "%s,%s", m_userName.c_str(), m_iloginType.c_str());
	clear();
	return true;
}

const std::string &CUser::getName() const
{
	return m_userName;
}

const std::string &CUser::getType() const
{
	return m_iloginType;
}

const std::string &CUser::GetAddress() const
{
	return m_loginAddress;
}

int CUser::getId() const
{
	return m_userId;
}

bool CUser::isValidAuthority(const char *auth) const
{	
	int trailDays;
	
	if (IMagicBox::instance()->getTrailDays(trailDays))
	{
		if (IMagicBox::instance()->getRunningTime()/(24*60) >= (uint)trailDays)
		{
			return false;
		}
	}
	return m_userManager->hasAuthorityOf(m_userName, auth);
}

void CUser::clear()
{
	m_userName = "";
	m_iloginType = "";
	m_lockTime = 0;
	m_tryLockNum = 0;
	m_errorNo = ACCOUNT_OK;
	m_valid = false;
	m_locked = false;
}

int CUser::getErrorNo()
{
	return m_errorNo;
}

/// 默认用户登陆
bool CUser::loginDefault()
{
	return login(m_userManager->getDefaultUser(), m_userManager->getDefaultPassword(), "GUI", "NONE");
}

IUser *IUser::getLocalUser()
{
	return CUser::getLocalUser();
}

/// 创建新的用户
IUser *IUser::create()
{
	return new CUser();
}
