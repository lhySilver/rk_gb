#ifndef _IIPSEARCH_H
#define _IIPSEARCH_H

#include "ExchangeAL/NetWorkExchange.h"

class IIPSearch
{
public:
	enum ConfigKind
	{
		NETCOMM_CFG,  //NetCommonConfig
		IPSEARCH_CFG_NUM,
	};

	typedef TSignal4<IIPSearch::ConfigKind, void *, int, int &> IPSearchCFGChangeSignal;
	typedef IPSearchCFGChangeSignal::SigProc IPSearchCfgChangeProc;

	typedef TSignal1<NetCommonConfig&> DevFoundSignal;
	typedef DevFoundSignal::SigProc DevFoundProc;

	/// 创建IPSearch客户端实例
	static IIPSearch *instance();

	/// 虚析构函数
	virtual ~IIPSearch();

	/// 启动IPSearch
	virtual bool start() = 0;

	/// 停止IPSearch
	virtual bool stop() = 0;

	/// 更新配置
	/// \param [in] cfgKind 配置类型
	/// \param [in] pConfig配置数据缓冲
	/// \param [in] nSize 配置大小，防止应用程序和库里头的头文件不一致
	/// \return 更新配置是否成功
	virtual bool config(ConfigKind cfgKind, const char *pConfig, int nSize) = 0;


	/// 配置修改回调
	/// \param [in] proc 回调函数
	/// \return 注册回调函数是否成功
	virtual bool attach(IPSearchCfgChangeProc proc) = 0;

	/// 注销配置修改回调
	/// \param [in] proc 回调函数 
	/// \return 注销回调函数是否成功
	virtual bool detach(IPSearchCfgChangeProc proc) = 0;

	/// 设备发现回调
	/// \param [in] proc 回调函数
	/// \return 注册回调函数是否成功
	virtual bool attach(DevFoundProc proc) = 0;

	/// 注销设备发现回调
	/// \param [in] proc 回调函数 
	/// \return 注销回调函数是否成功
	virtual bool detach(DevFoundProc proc) = 0;

	///搜索设备
	///发现一个设备回调一次设备发现回调函数
	virtual void search(void) = 0;
};

#endif //_IIPSEARCH_H


