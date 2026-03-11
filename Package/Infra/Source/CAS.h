#ifndef __INFRA_CAS_H__
#define __INFRA_CAS_H__

#include "Infra/Types.h"
#include "Infra/Mutex.h"

class CAtomicCount
{
	DISABLE_COPY(CAtomicCount);
public:
	CAtomicCount();
	~CAtomicCount();
	bool AddRef();
	bool Release();
private:
	long m_value;
	CMutex m_mutex;
};
 
#endif
