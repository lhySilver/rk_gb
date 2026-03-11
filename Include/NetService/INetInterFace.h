#ifndef _INETTERFACE_H
#define _INETTERFACE_H

#include "Infra/Singleton.h"
#include "Infra/Thread.h"
#include "PAL/Net.h"

class INetInterFace
{
public:
	struct NetFunction
	{
		bool vNetFunction[32]; //见NetSupport
	};

	enum ConfigKind
	{
		NETCOMM_CFG = 0, //NetCommonConfig
		ABILITY_CFG,     //NetFunction
		NETITF_CFG_NUM,
	};

	enum NetSupport
	{
		SUPPORT_WIFI = 0,
		SUPPORT_3G, 
		NETITF_SUPPORT_NUM,
	};

	/// 创建NetInterFace实例
	static INetInterFace *instance();

	/// 虚析构函数
	virtual ~INetInterFace();

	/// 更新配置
	/// \param [in] cfgKind 配置类型
	/// \param [in] pConfig配置数据缓冲
	/// \param [in] nSize 配置大小，防止应用程序和库里头的头文件不一致
	/// \return 更新配置是否成功
	virtual bool config(ConfigKind cfgKind, const char *pConfig, int nSize) = 0;

	virtual int	GetHostName(char *pHostName, int iNameLen) = 0;
	virtual int	SetHostName(char *pHostName, int iNameLen) = 0;
	virtual int	GetLocalIp(const char *pEthName, char *pIpAddr, int iIpLen, char *pMask, int iMaskLen) = 0;
	virtual int	SetLocalIp(const char *pEthName, const char *pIp, int iIpLen, const char *pMask, int iMaskLen) = 0;
	virtual int	GetGateway(const char *pEthName, char *pGateway, int iGwLen) = 0;
	virtual int	SetGateway(const char *pEthName,  const char *pGateway, const int iGwLen) = 0;
	virtual int	GetMacAddr(const char *pEthName, char *pEthMac, int iEthMacLen) = 0;
	virtual int	SetMacAddr(const char *pEthName, char *pEthMac, int iEthMacLen) = 0;
	virtual int	GetActiveIP(char *pIpAddr, int iIpLen) = 0;
	virtual int SendARP(const char *pEthName) = 0;
	virtual int IsPPPOE() = 0;
	virtual bool IsSupportMoblie() = 0;

	/// 获取无线路由列表
	virtual WLAN_DEVICE* GetWifiDevList(int &nNumber) = 0;

	/// 是否成功连接WIFI
	virtual bool IsWifiConnet() = 0;

	/// 获取WIFI网卡名
	virtual bool GetWLanIfname(char *pIpAddr, int iIpLen) = 0;
};
#endif

