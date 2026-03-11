#ifdef WIN32
#	include <Windows.h>
#	define INCREMENT_AMOUNT	1
#	define LONG_MAX      2147483647L   /* maximum (signed) long value */
#else
#	include <semaphore.h>
#endif

#include "Infra/Semaphore.h"
#include <stdlib.h>
#include <assert.h>
#include <time.h>

CSemaphore::CSemaphore(int dwInitialCount)
:m_hSemphore(0)
{
#ifdef WIN32
	m_hSemphore = CreateSemaphore(NULL, dwInitialCount, LONG_MAX, NULL);
#else
	int ret;

	m_hSemphore = new sem_t;
	ret = sem_init((sem_t*)m_hSemphore, 0, dwInitialCount);
	assert(ret != -1);
#endif
}

CSemaphore::~CSemaphore()
{
	assert(m_hSemphore);
#ifdef WIN32
	CloseHandle(m_hSemphore);
#else
	int ret;

	ret = sem_destroy((sem_t *)m_hSemphore);
	assert(ret == 0);
	delete (sem_t *)m_hSemphore;
#endif
}

bool CSemaphore::Pend(uint dwWait)
{
	assert(m_hSemphore);
#ifdef WIN32
	return WaitForSingleObject(m_hSemphore, dwWait) == 0 ? true : false;
#else
	if (dwWait == 0xFFFFFFFF)
	{
		return sem_wait((sem_t *)m_hSemphore) == 0 ? true : false;
	}
	else
	{
		struct timespec ts;

		ts.tv_sec=time(NULL)+dwWait / 1000;
		ts.tv_nsec=dwWait % 1000 * 1000;
		return sem_timedwait((sem_t *)m_hSemphore, &ts) == 0 ? true : false;
	}
#endif
	return true;
}

bool CSemaphore::Post()
{
	assert(m_hSemphore);
#ifdef WIN32
	LONG cnt = 0;
	return ReleaseSemaphore(m_hSemphore, INCREMENT_AMOUNT, &cnt) ? true : false; 

#else
	return sem_post((sem_t *)m_hSemphore) == 0 ? true : false;
#endif
}
