
#include "Lock.h"
#include <string.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <stdio.h>
#include <fcntl.h>

//================== CLock =======================
CLock::CLock(void)
{
    pthread_mutexattr_t   attr;
    pthread_mutexattr_init(&attr);

    /////////////////同一线程可以重复锁定
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

    pthread_mutex_init(&m_CriticalSection, &attr);

    pthread_mutexattr_destroy(&attr);
}

CLock::CLock(bool bIsRecursive/*为true对同一个线程表示可重入的，为false对同一个线程表示不可重入的*/)
{
	pthread_mutexattr_t   attr;
	pthread_mutexattr_init(&attr);

	/////////////////同一线程可以重复锁定
	pthread_mutexattr_settype(&attr, bIsRecursive ? PTHREAD_MUTEX_RECURSIVE : PTHREAD_MUTEX_TIMED_NP);

	pthread_mutex_init(&m_CriticalSection, &attr);

	pthread_mutexattr_destroy(&attr);
}

CLock::~CLock(void)
{
    pthread_mutex_destroy(&m_CriticalSection);
}

void CLock::Lock()
{
    pthread_mutex_lock(&m_CriticalSection);
}

void CLock::Unlock()
{
    pthread_mutex_unlock(&m_CriticalSection);
}


//=================== CMySemaphore ================================
CMySemaphore::CMySemaphore(tuint32 lInitCount, tuint32 lMaxCount) : m_FEventCS(false)
{
    assert(lMaxCount > 0);
    //改为等待 单调时间(只受jiffies影响，不受系统时间影响)
    //pthread_cond_init(&m_FCond, NULL);
    pthread_condattr_t cattr;
    int ret = pthread_condattr_init(&cattr);
    ret = pthread_condattr_setclock(&cattr, CLOCK_MONOTONIC);
    ret = pthread_cond_init(&m_FCond, &cattr);

    m_MaxCount = lMaxCount;
    m_semCount = lInitCount;

    UNREFERENCED_PARAMETER(ret);
}

CMySemaphore::~CMySemaphore(void)
{
    pthread_cond_destroy(&m_FCond);
}

void CMySemaphore::lock()
{
    m_FEventCS.Lock();

    while(!m_semCount)
    {
        pthread_cond_wait(&m_FCond, &m_FEventCS.m_CriticalSection);
    }

    --m_semCount;
    m_FEventCS.Unlock();
}


bool CMySemaphore::lock(tuint32 dwTimeout_MS)
{
/*
    //已有信号为0，则等待
    struct timespec ts;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    //ts.tv_sec = tv.tv_sec;	//秒

    tuint64  Timeout_ns = dwTimeout_MS;
    Timeout_ns = Timeout_ns * 1000 * 1000; ////

    tuint64  nowTime_ns = tv.tv_sec;
    nowTime_ns = nowTime_ns * 1000 * 1000 * 1000;
    nowTime_ns += (tuint64)tv.tv_usec * 1000;

    tuint64 u64Tmp = Timeout_ns + nowTime_ns;  //这里即等待 1 毫秒

    tuint64 nstosRate = 1000;
    nstosRate = nstosRate * 1000 * 1000;

    ts.tv_sec = ( u64Tmp / nstosRate );
    ts.tv_nsec = ( u64Tmp % nstosRate ); //纳秒部分
*/

    //改为等待 单调时间(只受jiffies影响，不受系统时间影响)
    struct timespec  ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    tuint32 secs = dwTimeout_MS/1000;
    tuint64 msecs = dwTimeout_MS%1000;

    tuint32 add = 0;
    msecs = msecs*1000*1000 + ts.tv_nsec;
    add = msecs / (1000*1000*1000);
    ts.tv_sec += (add + secs);
    ts.tv_nsec = msecs%(1000*1000*1000);

    m_FEventCS.Lock();
    while(!m_semCount)
    {
        if( pthread_cond_timedwait(&m_FCond, &m_FEventCS.m_CriticalSection, &ts) == ETIMEDOUT )  //如果超时，是返回 ETIMEDOUT
        {
            m_FEventCS.Unlock();
            return false;
        }
    }

    //注意：如果pthread_cond_timedwait超时到了，但是这个时候不能lock临界区，pthread_cond_timedwait并不会立即返回；
    //但是在pthread_cond_timedwait返回的时候，它仍在临界区中，且此时返回值为ETIMEDOUT.

    m_semCount--;
    m_FEventCS.Unlock();
    return true;
}

/////////////////增加一个信号
void CMySemaphore::unlock(void)
{
    m_FEventCS.Lock();
    ++m_semCount;
    pthread_cond_signal(&m_FCond);
    m_FEventCS.Unlock();
}

void CMySemaphore::unlock(tint32 lReleaseCount, tint32* lpPreviousCount)
{
    m_FEventCS.Lock();

    if (lpPreviousCount)
        *lpPreviousCount = m_semCount;

    m_semCount += lReleaseCount;

    pthread_cond_signal(&m_FCond);
    m_FEventCS.Unlock();
}

bool CMySemaphore::trylock()
{
    m_FEventCS.Lock();
    if(!m_semCount)
    {
        m_FEventCS.Unlock();
        return false;
    }
    else
    {
        --m_semCount;
        m_FEventCS.Unlock();
        return true;
    }
}

#if 0
//===================== CMyEvent ===========================
CMyEvent::CMyEvent(bool bManualReset) : m_bManualReset(bManualReset)
{
    memset(&m_FEvent, 0, sizeof(m_FEvent));
	bool bInitiallyOwn=false;
    if( sem_init(&m_FEvent, 0, bInitiallyOwn ? 1 : 0) == -1)
    {
        perror("sem_init ");
    }
}

CMyEvent::~CMyEvent()
{
    sem_destroy(&m_FEvent);
}

bool CMyEvent::SetEvent()
{
    m_FEventCS.Lock();
    try
    {
        int nValue = 0;
        sem_getvalue(&m_FEvent, &nValue);

        if(!nValue)
        {
            sem_post(&m_FEvent);
        }
    }
    catch(...)
    {

    }
    m_FEventCS.Unlock();
    return true;
}

bool CMyEvent::ResetEvent()
{
    m_FEventCS.Lock();

    do
    {

    }while(!sem_trywait(&m_FEvent));

    m_FEventCS.Unlock();
	return true;
}

tuint32 CMyEvent::WaitEvent(tuint32 timeout)
{
    tint32  ret = 0;
    tuint32 Result = WAIT_TIMEOUT;

    if(timeout == INFINITE)
    {
        while( (!!(ret = sem_wait(&m_FEvent))) && (EINTR == errno));
    }
    else
    {
        struct timespec ts;
        struct timeval tv;
        gettimeofday(&tv, NULL);


//        ts.tv_sec = tv.tv_sec;	//秒
//        tuint64 u64Tmp = ((tuint64)(tv.tv_usec  + timeout*1000)) * 1000ULL;  //这里即等待 1 毫秒
//        ts.tv_sec += ( u64Tmp / ((tuint64)(1000 * 1000 * 1000)) );
//        ts.tv_nsec = ( u64Tmp % ((tuint64)(1000 * 1000 * 1000)) ); //纳秒部分

        ts.tv_sec = tv.tv_sec + timeout/1000;
        ts.tv_nsec = tv.tv_usec*1000UL + (timeout %1000)*1000000UL;
        if(ts.tv_nsec >= 1000000000L)
        {
            ts.tv_nsec -= 1000000000L;
            ts.tv_sec++;
        }


        while((!!(ret = sem_timedwait(&m_FEvent, &ts))) && (EINTR == errno));

    }

    if (!ret)
    {
      Result = WAIT_OBJECT_0;
      //只有在收到事件的情况下才能需要手动重置（即另外再在外面做ResetEvent）

      m_FEventCS.Lock();

      if (m_bManualReset)
      {

        try
        {
          // the event might have been signaled between the sem_wait above and now so we reset it again
          do
          {
          }while (!sem_trywait(&m_FEvent)); //nothing

          sem_post(&m_FEvent);
        }
        catch (...)
        {
          Result = WAIT_FAILED;
        }

      }
      m_FEventCS.Unlock();
    }
    else
    {
        Result = WAIT_TIMEOUT;
    }

    return Result;
}

#endif

CMyEvent::CMyEvent(bool bManualReset) : m_bManualReset(bManualReset)
{
    //改为等待 单调时间(只受jiffies影响，不受系统时间影响)
    //pthread_cond_init(&m_FCond, NULL);
    pthread_condattr_t cattr;
    int ret = pthread_condattr_init(&cattr);
    ret = pthread_condattr_setclock(&cattr, CLOCK_MONOTONIC);
    ret = pthread_cond_init(&m_FCond, &cattr);

    //m_MaxCount = 1;
    m_semCount = 0;

    UNREFERENCED_PARAMETER(ret);
}

CMyEvent::~CMyEvent()
{
    pthread_cond_destroy(&m_FCond);
}

bool CMyEvent::SetEvent()
{
    m_FEventCS.Lock();
    if (!m_semCount)
    {
        ++m_semCount;
    }
    pthread_cond_signal(&m_FCond);
    m_FEventCS.Unlock();

    return true;
}

bool CMyEvent::ResetEvent()
{
    m_FEventCS.Lock();
    if (m_semCount)
    {
        --m_semCount;
    }
    m_FEventCS.Unlock();

    return true;
}

tuint32 CMyEvent::WaitEvent(tuint32 dwTimeout_MS)
{
    //改为等待 单调时间(只受jiffies影响，不受系统时间影响)
    struct timespec  ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    tuint32 secs = dwTimeout_MS/1000;
    tuint64 msecs = dwTimeout_MS%1000;

    tuint32 add = 0;
    msecs = msecs*1000*1000 + ts.tv_nsec;
    add = msecs / (1000*1000*1000);
    ts.tv_sec += (add + secs);
    ts.tv_nsec = msecs%(1000*1000*1000);

    m_FEventCS.Lock();
    while(!m_semCount)
    {
        if( pthread_cond_timedwait(&m_FCond, &m_FEventCS.m_CriticalSection, &ts) == ETIMEDOUT )  //如果超时，是返回 ETIMEDOUT
        {
            m_FEventCS.Unlock();
            return WAIT_TIMEOUT;
        }
    }

    //注意：如果pthread_cond_timedwait超时到了，但是这个时候不能lock临界区，pthread_cond_timedwait并不会立即返回；
    //但是在pthread_cond_timedwait返回的时候，它仍在临界区中，且此时返回值为ETIMEDOUT.

    //只有在收到事件的情况下才能需要手动重置（即另外再在外面做ResetEvent）
    if (!m_bManualReset)
    {
        m_semCount--;
    }
    m_FEventCS.Unlock();

    return WAIT_OBJECT_0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
tuint32 WaitForMultipleObjects(tint32 nCount, const CMyEvent** lpEvents, bool bWaitAll, tuint32 dwMilliseconds)
{
    int i = -1;
    bool *pSignaleds = new bool[nCount]; //如果对应事件有信号，即标记为 true

    tuint32 dwRet = WAIT_TIMEOUT, dwTmpRet;
    tuint32 dwSignaled_Count; //有信号的事件个数
    tuint32 dwMilliseconds_Escaped = 0; //经过的毫秒数
    memset(pSignaleds, false, sizeof(bool)*nCount);

    while (true)
    {
        dwSignaled_Count = 0;
        for (i=0; i<(int)nCount; i++)
        {
            if (!pSignaleds[i])
            {
                dwMilliseconds_Escaped+=10;
                dwTmpRet = ((CMyEvent *)lpEvents[i])->WaitEvent(10);
                if (dwTmpRet == WAIT_OBJECT_0)
                {
                    pSignaleds[i] = true;
                    dwSignaled_Count++;
                    break;
                }
            }
            else
                dwSignaled_Count++;
        }

        if (bWaitAll)
        {
            if (dwSignaled_Count==nCount)
            { //如果是等待全部事件，并且全部事件都有信号则返回
                dwRet = WAIT_OBJECT_0;
                break;
            }
        }
        else
        {
            if (i<(int)nCount)
            {  //如果只等待部分事件，那么某个事件有信号了，直接返回
                dwRet = WAIT_OBJECT_0 + i;
                break;
            }
        }
        if (dwMilliseconds_Escaped > dwMilliseconds)
        {	//超时
            break;
        }
    }
    delete[] pSignaleds;
    return dwRet;
}


CNameLock::CNameLock(const char *szName)
{
	m_pSemLock = sem_open(szName, 0);
	if (!m_pSemLock)
	{
		m_pSemLock = sem_open(szName, O_RDWR|O_CREAT, 0666, 1);
	}
}

CNameLock::~CNameLock(void)
{
	sem_close(m_pSemLock);
}

void CNameLock::Lock(void)
{
    sem_wait(m_pSemLock);
}

void CNameLock::Unlock(void)
{
	sem_post(m_pSemLock);
}
