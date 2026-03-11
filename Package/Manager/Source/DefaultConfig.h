#ifndef __DEFAULT_CONFIG_H__
#define __DEFAULT_CONFIG_H__

#include "Infra/Singleton.h"

/// 默认配置事务类，设置配置的默认配置
/// 目前暂时统一处理，等后续各个模块的耦合性降低以后将由各个模块内部自己管理2008-12-10
class CManagerDefaultConfig
{
	PATTERN_SINGLETON_DECLARE(CManagerDefaultConfig);

public:

	void setDst();

	void setAutoMaintain();

};

#define g_defaultConfig (*CManagerDefaultConfig::instance())

#endif
