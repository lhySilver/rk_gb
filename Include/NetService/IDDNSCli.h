#ifndef _IDDNS_CLI_H
#define _IDDNS_CLI_H

class IDdnsCli
{
public:
	enum ConfigKind
	{
		DDNS_CFG = 0, //DDNSTypeConfigAll
		DDNS_CFG_NUM,
	};

	enum ActionKind
	{
		DDNS_REGISTER,
		DDNS_GET_INFO,
		DDNS_NEW_USER_REG,
	};

	/// 创建DDNS客户端实例
	static IDdnsCli *instance();

	/// 虚析构函数
	virtual ~IDdnsCli();

	/// 启动DDNS
	virtual bool start() = 0;

	/// 配置修改重新更新
	virtual bool reUpdate() = 0;

	/// 更新配置
	/// \param [in] cfgKind 配置类型
	/// \param [in] pConfig配置数据缓冲
	/// \param [in] nSize 配置大小，防止应用程序和库里头的头文件不一致
	/// \return 更新配置是否成功
	virtual bool config(ConfigKind cfgKind, const char *pConfig, int nSize) = 0;
	
	/// 执行操作
	/// \param [in] actKind 操作类型
	/// \param [in] pActInfo 操作配置数据缓冲
	/// \param [in] nSize 操作配置大小
	/// \return 操作命令是否执行成功
	virtual int	doAction(ActionKind actKind, const char* pActInfo, int nSize) = 0;
};

#endif //_IDDNS_CLI_H

