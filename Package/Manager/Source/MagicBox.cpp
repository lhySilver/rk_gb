char cmos_lable[] = "XC";

#include "MagicBox.h"
#include "Infra/Time.h"
#include "Infra/File.h"
#include "Manager/ConfigManager.h"
#include "Log/Log.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "ExchangeAL/Exchange.h"
#include "ExchangeAL/ExchangeKind.h"
#include "ExchangeAL/ManagerExchange.h"
#include "AutoMaintain.h"
#include "Daylight.h"
#include "PAL/WatchDog.h"
#include "Log/DebugDef.h"

#ifdef WIN32
	#undef OEM_TYPE
	#define OEM_TYPE "PW"
	#undef VERSION_MAJ
	#define VERSION_MAJ 2
	#undef VERSION_MIN
	#define VERSION_MIN 00
	#undef VERSION_FIX
	#define VERSION_FIX "T00"
#endif 

#ifdef WIN32
	#define CONFIG_DIR			"Win32/Config"		///< 配置文件目录
	#define CUSTOM_DIR			"Win32"				///< Custom自定义目录
#else
	#define CONFIG_DIR 			"/mnt/mtd/Config"
	#define CUSTOM_DIR			"/usr/bin"		
#endif

#define RUN_STAT_CONFIG CONFIG_DIR"/SysTime"

PATTERN_SINGLETON_IMPLEMENT(CMagicBox);

CMagicBox::CMagicBox() : m_timerSaveTime("SaveTime"), m_timerBeep("Beeper"),m_timerWatch("WatchDog")
{
	memset(&m_runStat, 0, sizeof(m_runStat));
	m_dwFreq = 0;
	m_dwDur = 0;
}

CMagicBox::~CMagicBox()
{
}

void CMagicBox::start()
{

#if 0
	int i;
	char zero = 0;
	char magic[2] = {0};

	ManagerInfo("CMagicBox::start()...\n");
	//Check cmos lable
	CMOSRead(CMOS_LABAL, magic, 2);
	if (strncmp(magic, cmos_lable,2) != 0) {//load default
		ManagerDebug("CMagicBox::Initialize() clear...\n");
		FOR (i, CMOS_AREA_END) {
			CMOSWrite((CMOS_OFFS)i, &zero, 1);
		}
		CMOSWrite(CMOS_LABAL, cmos_lable, 2);
	}
	CFile file;
	char *buffer = (char *)file.Load(CUSTOM_DIR"/ProductDefinition");
	// 读取产品定义表

	if (buffer)
	{
		CConfigReader reader;
		std::string stream = buffer;
		if (!reader.parse(stream, m_productTable))
		{
			ManagerErr("ProductDefinition:[%s] parse failed!\n", stream.c_str());
		}
		file.UnLoad();
	}
	else
	{
		m_productTable["Hardware"] = "*";
		m_productTable["Vendor"] = "*";
	}
	if (!m_productTable.isMember("PackSize"))
	{
		m_productTable["PackSize"] = 20480;
	}

	Json::Value table;
	int ret = 0;

	IConfigManager::instance()->attach(getConfigName(CFG_LOCATION), IConfigManager::Proc(&CMagicBox::onConfigLocation, this));
	IConfigManager::instance()->attach(getConfigName(CFG_ABILITY_DEVDESC), IConfigManager::Proc(&CMagicBox::onConfigDeviceDesc, this));
	IConfigManager::instance()->getConfig(getConfigName(CFG_LOCATION), table);
	IConfigManager::instance()->getConfig(getConfigName(CFG_ABILITY_DEVDESC), m_deviceDesc);
	memset(&m_systemCapsExtent, 0xff, sizeof(m_systemCapsExtent));
	SystemGetCapsEx(&m_systemCapsExtent);

	onConfigLocation(table, ret);

	IEventManager::instance()->attach(IEventManager::Proc(&CMagicBox::onAppEvent, this));
	m_timerSaveTime.Start(CTimer::Proc(&CMagicBox::onSaveTime, this), 1000, 1000);
	//记录启动日志
	SystemTime time;
	DateTime exittime;
	uchar exitstate;

	//打印退出状态
	CMagicBox::instance()->getExitTime(&exittime);
	exitstate = CMagicBox::instance()->getExitState();
	TimeDate2Sys(&time, &exittime);

	// 异常日志修改为SaveSystemState
	if (exitstate)
	{
		ILog::instance()->append("SaveSystemState", "%04d-%2d-%02d %02d:%02d:%02d", 
			time.year, time.month, time.day,
			time.hour, time.minute, time.second);
	}
	else //正常启动增加这个日志，异常情况下不记录该日志
	{
		ILog::instance()->append("Reboot", "%04d-%2d-%02d %02d:%02d:%02d", 
			time.year, time.month, time.day,
			time.hour, time.minute, time.second);
	}
	
	//设置退出状态
	setExitState(1);
	//打印退出状态
	ManagerDebug("Shut Down at %d-%d-%d %02d:%02d:%02d with state[%d], SerialNo:%s\n",
		exittime.year + 2000, exittime.month, exittime.day, exittime.hour, exittime.minute, exittime.second, exitstate,
		m_serialNo.c_str());

	CAutoMaintain::instance()->Start(); 	// 启动自动维护功能
	CDaylight::instance()->start();			// 启动夏令时功能
#else

#if 0
	int i;
	char zero = 0;
	char magic[2] = {0};

	ManagerInfo("CMagicBox::start()...\n");
	
	{
		m_productTable["Hardware"] = "*";
		m_productTable["Vendor"] = "*";
	}
	if (!m_productTable.isMember("PackSize"))
	{
		m_productTable["PackSize"] = 20480;
	}

	Json::Value table;
	int ret = 0;

	IConfigManager::instance()->attach(getConfigName(CFG_LOCATION), IConfigManager::Proc(&CMagicBox::onConfigLocation, this));
	IConfigManager::instance()->attach(getConfigName(CFG_ABILITY_DEVDESC), IConfigManager::Proc(&CMagicBox::onConfigDeviceDesc, this));
	IConfigManager::instance()->getConfig(getConfigName(CFG_LOCATION), table);
	IConfigManager::instance()->getConfig(getConfigName(CFG_ABILITY_DEVDESC), m_deviceDesc);
	memset(&m_systemCapsExtent, 0xff, sizeof(m_systemCapsExtent));
	//SystemGetCapsEx(&m_systemCapsExtent);

	onConfigLocation(table, ret);

	IEventManager::instance()->attach(IEventManager::Proc(&CMagicBox::onAppEvent, this));
	m_timerSaveTime.Start(CTimer::Proc(&CMagicBox::onSaveTime, this), 1000, 1000);
	//记录启动日志
	SystemTime time;
	DateTime exittime;
	uchar exitstate;

	//打印退出状态
	CMagicBox::instance()->getExitTime(&exittime);
	exitstate = CMagicBox::instance()->getExitState();
	TimeDate2Sys(&time, &exittime);

	// 异常日志修改为SaveSystemState
	if (exitstate)
	{
		ILog::instance()->append("SaveSystemState", "%04d-%2d-%02d %02d:%02d:%02d", 
			time.year, time.month, time.day,
			time.hour, time.minute, time.second);
	}
	else //正常启动增加这个日志，异常情况下不记录该日志
	{
		ILog::instance()->append("Reboot", "%04d-%2d-%02d %02d:%02d:%02d", 
			time.year, time.month, time.day,
			time.hour, time.minute, time.second);
	}

	//设置退出状态
	setExitState(1);
	//打印退出状态
	ManagerDebug("Shut Down at %d-%d-%d %02d:%02d:%02d with state[%d], SerialNo:%s\n",
		exittime.year + 2000, exittime.month, exittime.day, exittime.hour, exittime.minute, exittime.second, exitstate,
		m_serialNo.c_str());
#endif

//	CAutoMaintain::instance()->Start(); 	// 启动自动维护功能
//	CDaylight::instance()->start(); 		// 启动夏令时功能

#endif
}

int CMagicBox::Read(CMOS_OFFS offs, void *pdat, int len)
{
	return 0;
	//return CMOSRead(offs, pdat, len);
}

int CMagicBox::Write(CMOS_OFFS offs, void *pdat, int len)
{
	return 0;
	//return CMOSWrite(offs, pdat, len);
}

int CMagicBox::readFlag(CMOS_FLAGS_INDEX index)
{
	ushort flag;
	Read(CMOS_FLAGS, &flag, 2);
	if(flag & BITMSK(index)){
		return 1;
	}else{
		return 0;
	}
}

int CMagicBox::writeFlag(CMOS_FLAGS_INDEX index, int enable)
{
	ushort flag;
	Read(CMOS_FLAGS, &flag, 2);
	if((flag & BITMSK(index)) && !enable){
		flag &= ~BITMSK(index);
	}else if(!(flag & BITMSK(index)) && enable){
		flag |= BITMSK(index);
	}
	return Write(CMOS_FLAGS, &flag, 2);
}

void CMagicBox::setExitTime(DateTime *time)
{
	//Write(CMOS_EXIT_TIME, time, sizeof(DateTime));
}

void CMagicBox::getExitTime(DateTime *time)
{
	//Read(CMOS_EXIT_TIME, time, sizeof(DateTime));
}

void CMagicBox::setExitState(uchar state)
{
	//Write(CMOS_EXIT_STATE, &state, 1);
}

uchar CMagicBox::getExitState()
{
	uchar state;
	Read(CMOS_EXIT_STATE, &state, 1);
	return state;
}

void CMagicBox::onConsole()
{
#if 0
	int i = 0;
	char data = 0;

	FOR (i, CMOS_AREA_END) {
		CMOSRead((CMOS_OFFS)i, &data, 1);
		if (0 == i % 16)
		{
			trace("\n");
		}
		trace("%02x ", data);
	}
	trace("\n");
#endif
}

uint CMagicBox::getRunningTime()
{
	return m_runStat.runMinutes;
}

bool CMagicBox::getTrailDays(int &days)
{
	return false;
}

void CMagicBox::beep(uint dwFreq, uint dwDur)
{
	//SystemBeep(dwFreq, dwDur);
	return;
}
bool CMagicBox::IsShowPWInfo()
{
#if 0
	if (CryptoCheck() < 0)
	{
		Json::Value table;
		int penaltyDay;
		IConfigManager::instance()->getConfig(getConfigName(CFG_ABILITY_DEVDESC), table);
		penaltyDay = table["PenaltyPaDay"].asInt();
		if (0 == penaltyDay)
		{
			table["PenaltyPaDay"] = 15; //默认15天为惩罚期
			IConfigManager::instance()->setConfig(getConfigName(CFG_ABILITY_DEVDESC), table);
			m_runStat.runMinutes = 0;
			ManagerInfo("start penalty after 15 days\n");
			return false;
		}
		else if (m_runStat.runMinutes > (uint)(penaltyDay*24*60)) //到了惩罚期
		{
			ManagerInfo("start penalty now: penaltyDay: %d, trail: %d\n", penaltyDay, m_runStat.runMinutes / 24 / 60);
			return true;
		}
		else
		{
			ManagerInfo("start penalty after %d days\n", penaltyDay - m_runStat.runMinutes /24/60);
			return false;
		}
	}
	else
	{
		return false;
	}
#endif
	return true;
}

static ConfigPair s_dateFormat[] = 
{
	{"YYMMDD", CTime::DF_YYMMDD},
	{"MMDDYY", CTime::DF_MMDDYY},
	{"DDMMYY", CTime::DF_DDMMYY},
	{NULL,	},
};

static ConfigPair s_dateSeparator[] = 
{
	{".", CTime::DS_DOT},
	{"-", CTime::DS_DASH},
	{"/", CTime::DS_SLASH},
	{NULL,	},
};

static ConfigPair s_timeFormat[] = 
{
	{"12", CTime::TF_12},
	{"24", CTime::TF_24},
	{NULL,	},
};

void CMagicBox::onConfigLocation(const Json::Value &table, int &ret)
{
	int iDateFormat = s_dateFormat[getIndex(s_dateFormat, table["DateFormat"].asCString())].value;
	int iTimeFormat = s_timeFormat[getIndex(s_timeFormat, table["TimeFormat"].asCString())].value;
	int iDateSeparator	= s_dateSeparator[getIndex(s_dateSeparator, table["DateSeparator"].asCString())].value;

	CTime::setTimeFormat(iDateFormat, iTimeFormat, iDateSeparator);
}

void CMagicBox::onConfigDeviceDesc(const Json::Value &table, int &ret)
{
	if (m_deviceDesc["TrailDay"].asInt() != table["TrailDay"].asInt())
	{
		ManagerDebug("TrailDays Changed Old:%d, New:%d\n", m_deviceDesc["TrailDay"].asInt(), table["TrailDay"].asInt());
		m_runStat.trailMinutes = 0;
	}
}

void CMagicBox::onAppEvent(std::string code, int index, appEventAction action, const EventHandler *param, Json::Value* data)
{
	printf("\033[1;36m  CMagicBox --------> onAppEvent          \033[0m\n");
	if (code == "Shutdown" 
		|| code == "Reboot")
	{
		setExitState(0);
		m_timerSaveTime.Stop(true);
		onSaveTime(true);
	}
}

const Json::Value &CMagicBox::getProductDefinition()
{
	return m_productTable;
}

void CMagicBox::cleanup(const char *note)
{
	SystemTime time;

	CTime::getCurrentTime(time);
	// 异常关机的日志在系统重启时添加
/*//<shang>	
	ILog::instance()->append("ShutDown", "%04d-%2d-%02d %02d:%02d:%02d [Normal]", 
		time.year, time.month, time.day,
		time.hour, time.minute, time.second);
*/
	IEventManager::instance()->notify(note);
}

void CMagicBox::shutdown()
{
	ManagerInfo("Alloca is shutting down...\n");
	cleanup("ShutDown");
	SystemShutdown();
}

void CMagicBox::reboot()
{
	printf("\033[1;36m   CMagicBox::reboot()   \033[0m\n");
	ManagerInfo("Alloca is rebooting...\n");
	cleanup("Reboot");
	SystemReboot();
}

bool CMagicBox::getHWID(uchar *pbuf, int len)
{
	int ret;
	bool bRes;
	unsigned char *hwid = pbuf;

	//if (len != magicMemoryLen)
	//{
	//	return false;
	//}
	ret = CryptoCreate();
	if(ret < 0)
	{
		return false;
	}
	usleep(100*1000);
	ret = CryptoRead(0, &(hwid[0]), 32);
	if(ret < 0)
	{
		bRes = false;
		goto exit;
	}
	usleep(100*1000);
	ret = CryptoRead(32, &(hwid[32]), 32);
	if(ret < 0)
	{
		bRes = false;
		goto exit;
	}
	usleep(100*1000);
	ret = CryptoRead(64, &(hwid[64]), 32);
	if(ret < 0)
	{
		bRes = false;
		goto exit;
	}
	usleep(100*1000);
	ret = CryptoRead(96, &(hwid[96]), 32);
	if(ret < 0)
	{
		bRes = false;
		goto exit;
	}

	bRes = true;

exit:
	CryptoDestory();

	return bRes;
}

bool CMagicBox::setHWID(const uchar *pbuf, int len)
{
	int ret;
	bool bRes;
	const uchar *hwid;

	//if (len != magicMemoryLen)
	//{
	//	return false;
	//}
	hwid = pbuf;
	ret = CryptoCreate();
	if(ret < 0)
	{
		return false;
	}
	usleep(100*1000);
	ret = CryptoWrite(0, &(hwid[0]), 32);
	if(ret < 0)
	{
		bRes = false;
		goto exit;
	}
	usleep(100*1000);
	ret = CryptoWrite(32, &(hwid[32]), 32);
	if(ret < 0)
	{
		bRes = false;
		goto exit;
	}
	usleep(100*1000);
	ret = CryptoWrite(64, &(hwid[64]), 32);
	if(ret < 0)
	{
		bRes = false;
		goto exit;
	}
	usleep(100*1000);
	ret = CryptoWrite(96, &(hwid[96]), 32);
	if(ret < 0)
	{
		bRes = false;
		goto exit;
	}
	
	bRes = true;

exit:
	CryptoDestory();

	return bRes;
}

void CMagicBox::onSaveTime(uint arg)
{
#if 0
	SystemTime sysTime;
	SystemTime curTime;
	static int s_count = 0;

	// 第一次保存时间通过读取CMOS中的关机时间来判断系统CMOS是否没电，没电时从Flash读，并且只读一次
	if (s_count == 0)
	{
		DateTime time;

		// 关机时间取走之后才能写该文件
		Read(CMOS_EXIT_TIME, &time, sizeof(DateTime));
		TimeDate2Sys(&sysTime, &time);
		CTime::getCurrentTime(curTime);
		// 通过判断关机时间是否为2000- 0-00 00:00:00来断定CMOS是否没电了
		// 没电了直接从Flash里取(2分钟更新一次),然后用这个时间设置一下系统时间
		ManagerDebug("CMOS ExitTime:%04d-%02d-%02d %02d:%02d:%02d\n",
			sysTime.year, sysTime.month, sysTime.day, 
			sysTime.hour, sysTime.minute, sysTime.second);
		ManagerDebug("CurrentTime:%04d-%02d-%02d %02d:%02d:%02d\n",
			curTime.year, curTime.month, curTime.day,
			curTime.hour, curTime.minute, curTime.second);
		{
			CFile file;
			SystemTime flashSysTime;

			memset(&m_runStat, 0, sizeof(m_runStat));
			if (file.Open(RUN_STAT_CONFIG, CFile::modeRead))
			{
				file.Read(&m_runStat, sizeof(m_runStat));
				file.Close();
				TimeDate2Sys(&flashSysTime, &m_runStat.dateTime);
				//if (IsZeroDateTime(time) || !(TimeCompare(&curTime, &sysTime) > 0))
				if (IsZeroDateTime(time))
				{
					ManagerDebug("CMOS NO PWOER$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
					CTime flashTime(flashSysTime);
					ManagerDebug("RecoverFlash Time:%04d-%02d-%02d %02d:%02d:%02d\n", 
						flashSysTime.year, flashSysTime.month, flashSysTime.day, 
						flashSysTime.hour, flashSysTime.minute, flashSysTime.second);
					ILog::instance()->append("RecoverTime", flashTime.format().c_str());
					CTime::setCurrentTime(flashSysTime);
				}
			}
		}
	}
	// 每秒钟保存当前时间到CMOS，用于取关机时间
	CTime::getCurrentTime(sysTime);
	TimeSys2Date(&m_runStat.dateTime, &sysTime);

	if(sysTime.year < 2000 || m_runStat.dateTime.year > 37)
	{
		m_runStat.dateTime.year = 0;
		TimeDate2Sys(&sysTime,&m_runStat.dateTime);
		CTime::setCurrentTime(sysTime);
	}
	setExitTime(&m_runStat.dateTime);

	// 每隔2分钟保存一次系统时间到Flash
	if (s_count++ % 120 == 0 || arg)
	{
		CFile file;
		CFile::Remove(RUN_STAT_CONFIG);
		if (file.Open(RUN_STAT_CONFIG, CFile::modeCreate|CFile::modeReadWrite))
		{
			m_runStat.runMinutes += 2;
			m_runStat.trailMinutes += 2;
			ManagerDebug("Save SysTime to Flash:%04d-%02d-%02d %02d:%02d:%02d, Time:%d Min, Trail:%d Min\n",
				m_runStat.dateTime.year + 2000, m_runStat.dateTime.month, m_runStat.dateTime.day, 
				m_runStat.dateTime.hour, m_runStat.dateTime.minute, m_runStat.dateTime.second,
				m_runStat.runMinutes,
				m_runStat.trailMinutes);
			file.Write(&m_runStat, sizeof(m_runStat));
			file.Close();
		}
	}
#endif
}
void CMagicBox::onTimerBeep(uint arg)
{
	CGuard guard(m_mutex);

	SystemBeep(m_dwFreq, m_dwDur);
}

bool CMagicBox::WatchDogStart()
{
	//WatchdogCreate();
	//m_timerWatch.Start(CTimer::Proc(&CMagicBox::onWatch, this), 0, 5000);
	return true;
}

void CMagicBox::onWatch(Param arg)
{
	//int uRet = 40; 
	//WatchdogWrite(&uRet, 4); 
}

/// 单件实例
IMagicBox *IMagicBox::instance()
{
	return CMagicBox::instance();
}

/// 虚析构函数
IMagicBox::~IMagicBox()
{
}
