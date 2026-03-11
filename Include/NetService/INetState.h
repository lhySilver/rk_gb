#ifndef _INET_STATE_H
#define _INET_STATE_H

class INetState
{
public:
	enum ConfigKind
	{
		NETSTATE_CONFLICT_CFG = 0, //网络冲突配置
		NETSTATE_ABORT_CFG,        //网络中断
		NETSTATE_3GNET_CFG,
		NETSTATE_WIFINET_CFG,
		NETSTATE_CFG_NUM,
	};

	enum NetType
	{
		NET_WIRED = 0,//有线网络
		NET_WLAN,//WIFI网络
		NET_3G,//3G网络
		NET_NR
	};
	typedef TSignal2<INetState::NetType, bool>     SIG_NET_STATE;	//第一个参数 是网络类型第二个参数是网络状态1为连接成功，0为异常
	typedef SIG_NET_STATE::SigProc  SIG_NET_STATE_PROC;

	/// 创建DDNS客户端实例
	static INetState *instance();

	/// 虚析构函数
	virtual ~INetState();

	/// 启动DDNS
	virtual bool start() = 0;

	/// 更新配置
	/// \param [in] cfgKind 配置类型
	/// \param [in] pConfig配置数据缓冲
	/// \param [in] nSize 配置大小，防止应用程序和库里头的头文件不一致
	/// \return 更新配置是否成功
	virtual bool config(ConfigKind cfgKind, const char *pConfig, int nSize) = 0;

	/// 注册网络状态变化回调
	/// \param [in] proc 回调函数
	/// \return 注册回调函数是否成功
	virtual bool attach(SIG_NET_STATE_PROC pProc) = 0;

	/// 注销网络状态变化回调
	/// \param [in] proc 回调函数 
	/// \return 注销回调函数是否成功
	virtual bool detach(SIG_NET_STATE_PROC pProc) = 0;

};

#endif //_INET_STATE_H
