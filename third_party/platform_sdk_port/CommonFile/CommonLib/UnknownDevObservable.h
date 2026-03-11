#ifndef __UNKNOWN_DEV_OBSERVABLE__
#define __UNKNOWN_DEV_OBSERVABLE__

//////////包含头文件 
#include <list>
#include "Lock.h"


class CUnknownDevObserver
{
public:
	CUnknownDevObserver(){};
	~CUnknownDevObserver(){};
	virtual void	AddUnknownDev(const char* devName,const char * devSN)=0;
};

#endif //__UNKNOWN_DEV_OBSERVABLE__