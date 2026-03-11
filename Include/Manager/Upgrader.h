#ifndef __MANAGER_UPGRADER_H__
#define __MANAGER_UPGRADER_H__

#include "Infra/Types.h"

/// \brief 升级管理器
/// 
class IUpgrader
{
public:

	enum UpgradeState
	{
		stateOK,
		stateNoEnoughMemory,
		stateInvalidFomat,
		stateUpdatePartFail,
	};

	/// 构造函数，创建升级对管理对象
	static IUpgrader *instance();

	/// 虚析构函数
	virtual ~IUpgrader();
	
	/// 升级初始化
	virtual bool init() = 0;

	/// 追加升级数据和setFilePath只能使用其中一个
	virtual bool putBuffer(uchar *buf, int len) = 0;

	/// 开始应用升级数据
	virtual bool start(bool sync = false) = 0;

	/// 中止升级
	virtual bool abort() = 0;

	/// 当前是否已经开始升级
	virtual bool started() const = 0;

	virtual bool setFilePath(const char* path) = 0;
};

#endif
