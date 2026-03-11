#ifndef _IDATACENTER_H
#define _IDATACENTER_H

#include "ExchangeAL/NetWorkExchange.h"

#define MAX_OSD_LEN 40
#define MAX_OSD_LINE 6

typedef struct
{
	int OSDLine;
	char buffer[MAX_OSD_LINE][MAX_OSD_LEN];
}POS_OSD_DATA;

typedef struct
{
	POS_OSD_DATA PosOSD[N_SYS_CH];
}POS_OSD_DATA_ALL;

class IDataCenter
{
public:
	enum ConfigKind
	{
		POS_CFG = 0, //POS机 配置
		DATACENTER_CFG_NUM,
	};

	enum ActionKind
	{
		DATACENTER_ACTION_NUM,
	};

	typedef TFunction3<int, int, void *, int> Proc;

	/// 创建FTP客户端实例
	static IDataCenter *instance();

	/// 虚析构函数
	virtual ~IDataCenter();

	/// 启动DDNS
	virtual bool start() = 0;

	/// 更新配置
	/// \param [in] cfgKind配置类型
	/// \param [in] pConfig配置数据缓冲
	/// \param [in] nSize 配置大小，防止应用程序和库里头的头文件不一致
	/// \return 更新配置是否成功
	virtual bool config(ConfigKind cfgKind, const char *pConfig, int nSize) = 0;

	/// 设置回调
	/// \param [in] proc 回调函数
	virtual int setCallBack(Proc proc) = 0;

	/// 操作
	/// \param [in] actKind操作类型
	/// \param [in] pConfig配置数据缓冲
	/// \param [in] nSize 配置大小，防止应用程序和库里头的头文件不一致
	/// \return 操作是否成功
	virtual int doAction(ActionKind cfgKind, const char *pConfig, int nSize) = 0;
};

#endif //_IALARMCENTER_H


