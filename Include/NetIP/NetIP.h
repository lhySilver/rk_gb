#ifndef __NETIP_STARTUP_H__
#define __NETIP_STARTUP_H__

#include "Infra/Singleton.h"

/// \brief NetIP组件启动封装
///
class CNetIP
{
	PATTERN_SINGLETON_DECLARE(CNetIP);
	
public:

	bool start();
};
#endif
