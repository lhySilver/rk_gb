#ifndef __DEFAULT_CONFIG_H__
#define __DEFAULT_CONFIG_H__

#include "Common.h"

/// 默认配置事务类，设置配置的默认配置
/// 目前暂时统一处理，等后续各个模块的耦合性降低以后将由各个模块内部自己管理2008-12-10
class CDefaultConfig
{
	PATTERN_SINGLETON_DECLARE(CDefaultConfig);

public:

	/// 设置默认配置
	int Start();
	
private:

	/// 设置网络相关配置的默认配置,包括通用网络和网络服务
	void NetWork();

	void Camera();
		
	// sd 卡是否需要格式化
	void SdCardFromat();
	// 升级状态 是否要提示升级成功
	void UpdateFlag();
    
    void AiOnOff();
    void PrivateOnOff();
	void LightStatus();
	void setResumePosition();
	void setFlightWarn();
	void setUsageMode();
	void setOnvif();
	void setRebootCount();
};

#define g_defaultConfig (*CDefaultConfig::instance())

#endif
