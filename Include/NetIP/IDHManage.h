#ifndef _IDH_MANAGE_H_
#define _IDH_MANAGE_H_

#include "Manager/User.h"
#include "Manager/EventManager.h"

class IDHManage
{
public:
	enum ConfigKind
	{
		NETIP_CFG_NUM,
	};

	/// 创建大华协议的操作类实例
	/// \param [in] nID NetIPOpr对象ID
	/// \param [in] proc 消息处理后的返回消息处理
	static IDHManage *instance();

	/// 虚析构函数
	virtual ~IDHManage();

	/// 开启服务
	virtual int Start(int argc, char *argv[]) = 0;

	/// 关闭服务
	virtual int Stop() = 0;

	// 断开所有的连接
	/// \return 0 - 成功 < 0 失败
	virtual int StopAllCli(void * pStoper=NULL) = 0;
	
	/// 开启服务
	virtual int Dump(int argc, char *argv[]) = 0;

	/// 更新配置
	/// \param [in] cfgKind 配置类型
	/// \param [in] pConfig配置数据缓冲
	/// \param [in] nSize 配置大小，防止应用程序和库里头的头文件不一致
	/// \return 更新配置是否成功
	//virtual bool Config(ConfigKind cfgKind, const char *pConfig, int nSize) = 0;

	/// 报警事件产生时，需要调用该接口通知告警中心
	//virtual void LaunchAppEvent(std::string appCode, int iIndex, appEventAction appAction, const EventHandler *pEventHandle, Json::Value *pData) = 0;

	/// 强制用户退出
	/// \param [in] pUser 用户对象
	/// \return 0 - 成功 < 0 失败
	virtual int KickOffUser(IUser *pUser) = 0;

	/// 是否有网络用户在监视或者回放
	/// \return true - 有  false 无
	virtual bool IsInPlaying() = 0;

	/// 暂停网络用户监视和回放
	/// \return true - 有  false 无
	virtual bool PauseAllPlaying() = 0;

	// 恢复网络用户监视和回放	
	/// \return true - 有  false 无	
	virtual bool ResumeAllPlaying() = 0;		
};

#endif //_INETIP_MANAGE_H_

