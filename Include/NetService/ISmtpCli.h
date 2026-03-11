#ifndef _ISMTP_CLI_H
#define _ISMTP_CLI_H

class ISmtpCli
{
public:
	enum ConfigKind
	{
		SMTP_CFG = 0, //NetEmailConfig 配置
		SMTP_CFG_NUM,
	};

	/// 创建FTP客户端实例
	static ISmtpCli *instance();

	/// 虚析构函数
	virtual ~ISmtpCli();

	/// 启动SMTP
	virtual bool start() = 0;
	virtual bool stop() = 0;

	/// 更新配置
	/// \param [in] cfgKind 配置类型
	/// \param [in] pConfig配置数据缓冲
	/// \param [in] nSize 配置大小，防止应用程序和库里头的头文件不一致
	/// \return 更新配置是否成功
	virtual bool config(ConfigKind cfgKind, const char *pConfig, int nSize) = 0;

	/// 报警事件产生时，需要调用该接口通知告警中心
	virtual void LaunchAppEvent(std::string appCode, int iIndex, appEventAction appAction, const EventHandler *pEventHandle, Json::Value *pData) = 0;

	virtual bool SetChannelName(int ichannel, std::string strName) = 0;
};

#endif //_ISMTP_CLI_H

