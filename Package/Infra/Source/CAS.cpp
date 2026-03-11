#include "CAS.h"
#ifdef WIN32
	#include <Windows.h>
#endif

CAtomicCount::CAtomicCount() 
{
	m_value = 0;
}

CAtomicCount::~CAtomicCount() 
{
}
  
bool CAtomicCount::AddRef() 
{
#ifdef WIN32
	long v = ::InterlockedIncrement(reinterpret_cast<long *>(&m_value));
	return static_cast<unsigned long>(v) != 0;
#else
	m_mutex.Enter();
	m_value++;
	m_mutex.Leave();
#endif
	return true;
}
  
bool CAtomicCount::Release() 
{
#ifdef WIN32
	long v = ::InterlockedDecrement(reinterpret_cast<long *>(&m_value));
	return static_cast<unsigned long>(v) == 0;
#else
	m_mutex.Enter();
	m_value--;
	m_mutex.Leave();
#endif
	return true;
}