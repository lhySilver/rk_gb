#ifdef _USE_DHCP_MODULE

#ifndef _IDHCP_CLI_H
#define _IDHCP_CLI_H

#include "Infra/Signals.h"
#include "ExchangeAL/NetWorkExchange.h"

class IDhcpCli
{
public:
	enum ConfigKind
	{
		DNS_CFG = 0, //NetDNSConfig
		NETCOMM_CFG,  //NetCommonConfig
		WIFI_CFG,
		DHCP_CFG_NUM,
	};

	enum EthKind
	{
		DNS_ETH = 0,
		NETCOMM_ETH,
		WIFI_ETH,
	};
	typedef TSignal4<IDhcpCli::ConfigKind, void *, int, int &> DHCPCFGChangeSignal;
	typedef DHCPCFGChangeSignal::SigProc DHCPCfgChangeProc;


	/// 创建DNS客户端实例
	static IDhcpCli *instance();

	/// 虚析构函数
	virtual ~IDhcpCli();

	/// 启动DDNS
	virtual bool start() = 0;

	/// 更新配置
	/// \param [in] cfgKind 配置类型
	/// \param [in] pConfig配置数据缓冲
	/// \param [in] nSize 配置大小，防止应用程序和库里头的头文件不一致
	/// \return 更新配置是否成功
	virtual bool config(ConfigKind cfgKind, const char *pConfig, int nSize) = 0;

	/// 配置修改回调
	/// \param [in] proc 回调函数
	/// \return 注册回调函数是否成功
	virtual bool attach(DHCPCfgChangeProc proc) = 0;

	/// 注销配置修改回调
	/// \param [in] proc 回调函数 
	/// \return 注销回调函数是否成功
	virtual bool detach(DHCPCfgChangeProc proc) = 0;
};

#endif //_IDHCP_CLI_H
#endif //_USE_DHCP_MODULE

