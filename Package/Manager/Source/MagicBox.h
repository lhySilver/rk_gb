#ifndef __INTER_MANAGER_MAGICBOX_H__
#define __INTER_MANAGER_MAGICBOX_H__

#include "Manager/MagicBox.h"
#include "Infra/Types.h"
#include "Infra/Timer.h"
#include "Infra/Singleton.h"
#include "PAL/System.h"
#include "Manager/EventManager.h"



typedef enum cmos_offs_t {		/* 0x08 - 0x3f 56个字节可用*/
	CMOS_RTC			= 0x00,			//RTC内部使用		[0x00 - 0x07]
	CMOS_LABAL			= 0x08,			//幻数(固定为XC)	[0x08 - 0x09]
	CMOS_IP				= 0x0A,			//静态IP			[0x0A - 0x0D]
	CMOS_MASK			= 0x0E,			//静态掩码			[0x0E - 0x11]
	CMOS_GATE			= 0x12,			//静态网关			[0x12 - 0x15]
	CMOS_DHCP			= 0x16,			//DHCP标志			[0x16 - 0x16]
	CMOS_PPPOE_ENABLE	= 0x17,			//PPPOE使能标志		[0x17 - 0x17]
	CMOS_PPPOE_NAME		= 0x18,			//PPPOE用户名		[0x18 - 0x27]
	CMOS_PPPOE_PWD		= 0x28,			//PPPOE密码			[0x28 - 0x2F]
	CMOS_FLAGS			= 0x30,			//					[0x30 - 0x31]
	CMOS_RESERVED		= 0x32,			//保留				[0x32 - 0x32]
	CMOS_EXIT_TIME		= 0x33,			//程序退出时间		[0x33 - 0x36]
	CMOS_EXIT_STATE		= 0x37,			//程序退出状态		[0x37 - 0x37]
	CMOS_MANUAL_REC		= 0x38,			//手动录象标志		[0x38 - 0x3F]
	CMOS_AREA_END		= 0x40,
} CMOS_OFFS;

class CMagicBox : public IMagicBox
{
	PATTERN_SINGLETON_DECLARE(CMagicBox);

public:
	enum MagicVals
	{
		magicMemoryLen = 128,	///< 黑匣子长度为128字节
	};
	enum CMOS_FLAGS_INDEX
	{
		CMOS_REMOTE_LOCK,				//遥控锁定
		CMOS_DST_TUNE,					//夏令时调整
	};

	struct RunTime
	{
		DateTime dateTime;		// 当前
		uint runMinutes;		// 运行时间
		uint trailMinutes;		// 试用时间
	};

	/// 启动魔盒
	void start();

	/// 得到产品定义表格
	const Json::Value &getProductDefinition();

	/// 关闭系统
	void shutdown();

	/// 重启系统
	void reboot();

	/// 得到系统加密芯片信息
	/// \param [out] hwid 存放读到的加密芯片信息
	/// \return 读取加密芯片是否成功
	bool getHWID(uchar *pbuf, int len);

	/// 设置系统加密芯片信息
	/// \param [in] 要设置的hwid信息
	/// \return 设置hwid是否成功
	bool setHWID(const uchar *pbuf, int len);

	/// 得到系统运行的时间
	/// \return 返回系统从出厂测试到运行总共运行的分钟数
	uint getRunningTime();

	/// 得到系统使用时间
	/// \param [in\out] days 系统使用天数
	/// \return 只有试用版本该接口才返回成功，正式版本该接口将返回失败
	bool getTrailDays(int &days);

	/// 蜂鸣器蜂鸣
	void beep(uint dwFrequence, uint dwDuration);

	bool IsShowPWInfo();

	/// 开启CPU看门狗
	bool WatchDogStart();

	int Read (CMOS_OFFS offs, void *pdat, int len);
	int Write(CMOS_OFFS offs, void *pdat, int len);
	int readFlag (CMOS_FLAGS_INDEX index);
	int writeFlag (CMOS_FLAGS_INDEX index, int enable);
	void getExitTime(DateTime *time);
	
	int getRemoteFlag();

	/// 得到夏令时标示
	int getDstFlag();
private:

	/// 事件回调函数
	void onAppEvent(std::string code, int index, appEventAction action, const EventHandler *param, Json::Value* data);
	void onConfigLocation(const Json::Value &table, int &ret);
	void onConfigDeviceDesc(const Json::Value &table, int &ret);
	void onConsole();
	void cleanup(const char *note = "Shutdown");
	void setExitState(uchar state);
	uchar getExitState();
	void onSaveTime(uint arg);
	void onTimerBeep(uint arg);
	void setExitTime(DateTime *time);
	void onWatch(Param arg);
private:
	SYSTEM_CAPS_EX m_systemCapsExtent;
	Json::Value m_productTable;
	Json::Value m_productAbility;
	Json::Value m_deviceDesc;
	CTimer m_timerSaveTime;
	RunTime m_runStat;
	std::string m_serialNo;
	CTimer m_timerBeep;
	int m_dwFreq;
	int m_dwDur;
	CMutex m_mutex;
	CTimer m_timerWatch;
};

#endif 
