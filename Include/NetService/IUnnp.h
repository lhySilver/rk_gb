#ifndef _IUPNP_H
#define _IUPNP_H

class IUpnpCli
{
public:
	enum ConfigKind
	{
		UPNP_CFG = 0, //NetUPNPConfig
		NETCOMM_CFG, //NetCommonConfig
		MOBILE_CFG, //NetMoblieConfig
		UPNP_CFG_NUM,
	};

	typedef TSignal4<IUpnpCli::ConfigKind, void *, int, int &> UPNPCFGChangeSignal;
	typedef UPNPCFGChangeSignal::SigProc UPNPCfgChangeProc;

	/// 创建DDNS客户端实例
	static IUpnpCli *instance();

	/// 虚析构函数
	virtual ~IUpnpCli();

	/// 启动DDNS
	virtual bool start() = 0;

	/// 更新配置
	/// \param [in] cfgKind 配置类型
	/// \param [in] pConfig配置数据缓冲
	/// \param [in] nSize 配置大小，防止应用程序和库里头的头文件不一致
	/// \return 更新配置是否成功
	virtual bool config(ConfigKind cfgKind, const char *pConfig, int nSize) = 0;

	/// 获取映射端口
	/// \param [in] iImPort 原端口
	/// \param [in] strProto 协议: TCP UDP 
	/// \return 0: 不成功 > 0: 映射后的端口
	virtual int getExport(int iImPort, std::string strProto) = 0;

	/// UPNPCfgChangeProc 配置修改回调
	/// \param [in] proc 回调函数
	/// \return 注册回调函数是否成功
	virtual bool attach(UPNPCfgChangeProc proc) = 0;

	/// 注销UPNPCfgChangeProc 配置修改回调
	/// \param [in] proc 回调函数 
	/// \return 注销回调函数是否成功
	virtual bool detach(UPNPCfgChangeProc proc) = 0;
	
	/// 增加一个需要映射的端口
	/// \param [in] iImPort  需要映射的源端口
	/// \return 成功返回true，失败false
	///获取映射后的端口调用接口getExport
	virtual bool addMapPort(int iImPort) = 0;

	///删除一个需要映射的端口
	/// \param [in] iImPort  需要映射的源端口
	/// \return 成功返回true，失败false
	virtual bool delMapPort(int iImPort) = 0;
};

#endif //_IUPNP_H


