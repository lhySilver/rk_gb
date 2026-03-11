#ifndef _IARSP_H
#define _IARSP_H

#include "ExchangeAL/NetWorkExchange.h"

class IArspCli
{
public:
	enum ConfigKind
	{
		ARSP_CFG = 0, //NetARSPConfigAll
		NETCOMM_CFG,  //NetCommonConfig
		UPNP_CFG,     //NetUPNPConfig
		MOBILE_CFG,	  //NetMoblieConfig
		NETPLATFORM_CFG,  //NetplatformConfig
		WIFI_CFG,
		ARSP_CFG_NUM,
	};

	typedef enum _ARSP_TYPE
	{
		ARSP_TALIDE,
		ARSP_DNS,
		ARSP_JINGDING,
		ARSP_NANRUI,
		ARSP_NUM
	}ARSP_TYPE;

	typedef struct 
	{
		char sDeviceID[32];
		char sDeviceIP[16];
		int iWebPort;
		int iMediaPort;
		int iMobilePort;
		int iUPNPWebPort;
		int iUPNPMediaPort;
		int iUPNPMobilePort;
		int iControlPort;
	}ARSPDns;

	typedef struct RETIDE_DATA
	{
		char sIP[16];
		char sID[32];
		int iHttpPort;
		int iTcpPort;
	}RetideData;

	/// 创建DDNS客户端实例
	static IArspCli *instance();

	/// 虚析构函数
	virtual ~IArspCli();

	/// 启动DDNS
	virtual bool start() = 0;

	/// 更新配置
	/// \param [in] cfgKind 配置类型
	/// \param [in] pConfig配置数据缓冲
	/// \param [in] nSize 配置大小，防止应用程序和库里头的头文件不一致
	/// \return 更新配置是否成功
	virtual bool config(ConfigKind cfgKind, const char *pConfig, int nSize) = 0;

	/// 获取注册信息
	/// \param [in] strType ARSP协议类型
	/// \param [out] 注册信息,不同的协议对应不同的注册信息
	/// \return true:成功 false: 失败
	virtual bool getArspInfo(const std::string &strType, char *pBuf, int iLen) = 0;
};

#endif //_IUPNP_H


