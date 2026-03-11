#include "Manager/Console.h"
#include "Manager/ConfigManager.h"
#include "Manager/MagicBox.h"
#include "Manager/Upgrader.h"
#include "AutoMaintain.h"
#include "ExchangeAL/ExchangeKind.h"
#include "Infra/Time.h"
#include "MagicBox.h"
#include "DefaultConfig.h"
#include "Log/DebugDef.h"

PATTERN_SINGLETON_IMPLEMENT(CAutoMaintain);

CAutoMaintain::CAutoMaintain() : m_timerAuto("AutoMaintain")
{
	CManagerDefaultConfig::instance()->setAutoMaintain();

	m_pfnMaintain[MAINTAIN_REBOOT] = MAINTAINPROC(&CAutoMaintain::OnReboot, this);
	m_pfnMaintain[MAINTAIN_DELETE_FILES] = 0;
	CTime::getCurrentTime(m_LastTime[MAINTAIN_REBOOT]);
	m_timeMsCount = CTime::getCurrentMicroSecond();
}

CAutoMaintain::~CAutoMaintain()
{
	IConfigManager::instance()->detach(getConfigName(CFG_AUTOMAINTAIN), IConfigManager::Proc(&CAutoMaintain::OnConfigAutoMaintain, this));
}

void CAutoMaintain::Start()
{
	return ;//ale
	Json::Value table;
	int ret = 0;

	ManagerInfo("CAutoMaintain::Start()...\n");
	IConfigManager::instance()->attach(getConfigName(CFG_AUTOMAINTAIN), IConfigManager::Proc(&CAutoMaintain::OnConfigAutoMaintain, this));
	IConfigManager::instance()->getConfig(getConfigName(CFG_AUTOMAINTAIN), table);

	TExchangeAL<AutoMaintainConfig>::getConfig(table, m_ConfigAutoMaintain);
	OnConfigAutoMaintain(table,ret);
}

void CAutoMaintain::OnConfigAutoMaintain(const Json::Value& tableNew, int& ret)
{
	CGuard guard(m_mutexCout);

	TExchangeAL<AutoMaintainConfig>::getConfig(tableNew, m_ConfigAutoMaintain);
	m_timerAuto.Start(CTimer::Proc(&CAutoMaintain::OnTimer, this), 0, 10 * 1000);
}

void CAutoMaintain::OnTimer(uint par)
{
	return;			// zzb 2013.10.21 comment
	int i;

	for (i = 0; i < MAINTAIN_TYPES; i++)
	{
		if (m_pfnMaintain[i].empty())
		{
			continue;
		}
		if (IsMaintainNow(i))
		{
			m_pfnMaintain[i](par);
		}
	}
	uint oldCount = m_timeMsCount;

	m_timeMsCount = CTime::getCurrentMicroSecond();
	if (oldCount > m_timeMsCount)
	{
		ManagerDebug("CTime::getCurrentMicroSecond() OverFlow!\n");
		OnReboot(0);
	}
}

bool CAutoMaintain::IsMaintainNow(int iMaintainType)
{
	bool bRet = false;
	SystemTime stCurTime, stMaintainTime;
	CGuard guard(m_mutexCout);

	// 输入参数检查
	if (iMaintainType < 0 || iMaintainType >= MAINTAIN_TYPES)
	{
		ManagerErr("iMaintainType error!\n");
		return false;
	}

	CTime::getCurrentTime(stCurTime);
	if(iMaintainType == MAINTAIN_REBOOT)
	{
		stMaintainTime = m_LastTime[iMaintainType];
		if(m_ConfigAutoMaintain.iAutoRebootDay == 1 //每天重启
			|| (m_ConfigAutoMaintain.iAutoRebootDay > 1 && 0 != m_ConfigAutoMaintain.iAutoRebootHour &&stMaintainTime.wday % 7 == m_ConfigAutoMaintain.iAutoRebootDay - 2)		//每周重启,非零点重启
			|| (m_ConfigAutoMaintain.iAutoRebootDay > 1 && 0 == m_ConfigAutoMaintain.iAutoRebootHour && 2 != m_ConfigAutoMaintain.iAutoRebootDay && stMaintainTime.wday % 7 == m_ConfigAutoMaintain.iAutoRebootDay - 3)  //设置为每周0点重启时，日期正好相差1天，所以要减去3，非周末
			|| (2 == m_ConfigAutoMaintain.iAutoRebootDay && 0 == m_ConfigAutoMaintain.iAutoRebootHour && stMaintainTime.wday % 7 == m_ConfigAutoMaintain.iAutoRebootDay + 4))  //如果设置为每周日的话，相差一周时间，所以要＋4	
		{
			stMaintainTime.hour = m_ConfigAutoMaintain.iAutoRebootHour;
			stMaintainTime.minute = 0;
			stMaintainTime.second = 0;

			if (0 == stMaintainTime.hour)
			{
				//说明是零点,转到下一天判断
				TimeAdd(&stMaintainTime, &stMaintainTime, 24 * 60 * 60);
			}

			int Seconds = Time2Second(&stCurTime, &stMaintainTime);
			if (TimeCompare(&m_LastTime[iMaintainType], &stMaintainTime) < 0 //时间跨越监视点, 触发自动维护服务
				&& Seconds <= 15
				&& Seconds >= 0)// 处理还没到的时间和手动修改系统时间的情况
				{
					bRet = true;
				}
		}
	}
	
	m_LastTime[iMaintainType] = stCurTime;
	return bRet;
}

void CAutoMaintain::OnReboot(uint par)
{
	printf("\033[1;36m  CAutoMaintain -----> OnReboot   \033[0m\n");
	SystemTime stCurTime;

	CTime::getCurrentTime(stCurTime);
	ManagerInfo("The reboot is %d-%d-%d  %d:%d:%d\n", 
		stCurTime.year, stCurTime.month, stCurTime.day, stCurTime.hour, stCurTime.minute, stCurTime.second);
	// 当前正在升级，忽略掉重启命令
	/*<shang>
	if (IUpgrader::instance()->started())
	{
		ManagerInfo("reboot cmd ignored\n");
		return;
	}
	*/
	CMagicBox::instance()->reboot();
}

