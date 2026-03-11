#ifndef LOCK_H
#define LOCK_H

#include <semaphore.h>
#include <pthread.h>
#include "base_type_define.h"

class CLock
{
    friend class CMySemaphore;
    friend class CMyEvent;
public:
	CLock(void);
	~CLock(void);
	void Lock();
	void Unlock();

protected:
	explicit CLock(bool bIsRecursive/*为true对同一个线程表示可重入的，为false对同一个线程表示不可重入的*/);

private:
    pthread_mutex_t   m_CriticalSection;
};

/////////////////////////////////////////
class CMySemaphore
{
public:
    CMySemaphore(tuint32 lInitCount, tuint32 lMaxCount = 0x7FFFFFFF);
    ~CMySemaphore(void);

    void lock();///////
    bool lock(tuint32 dwTimeout_MS); //减少一个信号，如果已有信号为零，则等待

    void unlock(void);    //增加一个信号
    void unlock(tint32 lReleaseCount,tint32 * lpPreviousCount = NULL);

    bool trylock();
private:
    pthread_cond_t m_FCond;
    CLock          m_FEventCS;
    tuint32 m_MaxCount;
    volatile tuint32 m_semCount;
};


class CMyEvent
{
public:
	CMyEvent(bool bManualReset = FALSE);
	~CMyEvent();

public:
	bool SetEvent();
	bool ResetEvent();
    tuint32 WaitEvent(tuint32 dwTimeout_MS);

private:
    const bool m_bManualReset;
    CLock            m_FEventCS;
    //sem_t           m_FEvent;

    pthread_cond_t m_FCond;
    volatile tuint32 m_semCount;
};

inline tuint32 WaitForSingleObject(CMyEvent *hEvent, tuint32 dwMillseconds)
{
    return hEvent->WaitEvent(dwMillseconds);
}

tuint32 WaitForMultipleObjects(tint32 nCount, const CMyEvent** lpEvents, bool bWaitAll, tuint32 dwMilliseconds);

//命名锁
class CNameLock
{
public:
	CNameLock(const char *szName);
	~CNameLock(void);

	void Lock(void);
	void Unlock(void);

private:
	sem_t *m_pSemLock;
};

/**************************************************************************************************************************/
#endif // LOCK_H
