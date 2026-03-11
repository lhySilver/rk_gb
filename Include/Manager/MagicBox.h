#ifndef __MANAGER_MAGICBOX_H__
#define __MANAGER_MAGICBOX_H__

#include "Infra/Time.h"
#include <Json/value.h>
#include <string>

class IMagicBox
{
public:
	
	/// 单件实例
	static IMagicBox *instance();

	/// 虚析构函数
	virtual ~IMagicBox();

	/// 启动魔盒
	virtual void start() = 0;

	/// 得到产品定义表格
	virtual const Json::Value &getProductDefinition() = 0;

	/// 关闭系统
	virtual void shutdown() = 0;

	/// 重启系统
	virtual void reboot() = 0;

	/// 得到系统运行的时间
	/// \return 返回系统从出厂测试到运行总共运行的分钟数
	virtual uint getRunningTime() = 0;

	/// 得到系统试用时间
	/// \param [in\out] days 系统使用天数
	/// \return 只有试用版本该接口才返回成功，正式版本该接口将返回失败
	virtual bool getTrailDays(int &days) = 0;

	/// 蜂鸣器蜂鸣
	virtual void beep(uint dwFrequence, uint dwDuration) = 0;

	/// 得到系统关机时间
	virtual void getExitTime(DateTime *time) = 0;

	virtual bool IsShowPWInfo() = 0;

	/// 开启CPU看门狗
	virtual bool WatchDogStart() = 0;

	
	/// 得到系统加密芯片信息
	/// \param [out] hwid 存放读到的加密芯片信息
	/// \return 读取加密芯片是否成功
	virtual bool getHWID(uchar *pbuf, int len)=0;

	/// 设置系统加密芯片信息
	/// \param [in] 要设置的hwid信息
	/// \return 设置hwid是否成功
	virtual bool setHWID(const uchar *pbuf, int len)=0;
};

#define g_magicBox (*IMagicBox::instance())
#endif 
