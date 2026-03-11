#ifndef _INTP_H
#define _INTP_H

class INtpCli
{
public:
	enum ConfigKind
	{
		NTP_CFG = 0, //NetNTPConfig 配置
		NTP_LOCATION_CFG,
		NTP_CFG_NUM,
	};

	/// 创建DDNS客户端实例
	static INtpCli *instance();

	/// 虚析构函数
	virtual ~INtpCli();

	/// 启动DDNS
	virtual bool start() = 0;

	/// 更新配置
	/// \param [in] cfgKind 配置类型
	/// \param [in] pConfig配置数据缓冲
	/// \param [in] nSize 配置大小，防止应用程序和库里头的头文件不一致
	/// \return 更新配置是否成功
	virtual bool config(ConfigKind cfgKind, const char *pConfig, int nSize) = 0;
};

#endif //_INTP_H


