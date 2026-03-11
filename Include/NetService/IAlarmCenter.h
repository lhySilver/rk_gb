#ifndef _IALARMCENTER_H
#define _IALARMCENTER_H

#include "ExchangeAL/NetWorkExchange.h"

class IAlarmCenter
{
public:
	enum ConfigKind
	{
		ALARMSERVER_CFG = 0, //NetAlarmServerConfigAll 配置
		ALARMCENTER_CFG_NUM,
	};

	/// 创建FTP客户端实例
	static IAlarmCenter *instance();

	/// 虚析构函数
	virtual ~IAlarmCenter();

	/// 启动DDNS
	virtual bool start() = 0;

	/// 更新配置
	/// \param [in] cfgKind配置类型
	/// \param [in] pConfig配置数据缓冲
	/// \param [in] nSize 配置大小，防止应用程序和库里头的头文件不一致
	/// \return 更新配置是否成功
	virtual bool config(ConfigKind cfgKind, const char *pConfig, int nSize) = 0;

	/// 报警事件产生时，需要调用该接口通知告警中心
	virtual void LaunchAppEvent(std::string appCode, int iIndex, appEventAction appAction, const EventHandler *pEventHandle, Json::Value *pData) = 0;

	/// 日志事件产生时，需要调用该接口通知告警中心
	virtual void LaunchLogEvent(std::string strType, std::string strDescrip, DateTime stTime) = 0;
};

#endif //_IALARMCENTER_H


