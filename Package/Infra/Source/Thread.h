#ifndef __IMPLEMENT_THREAD_H__
#define __IMPLEMENT_THREAD_H__

#include "MsgQue.h"

class Threadlet;

class PooledThread : public CThread
{
	friend class CThreadManager;
	friend class Threadlet;

public:
	PooledThread();
	~PooledThread();
	void ThreadProc();

private:
	IThreadLet::Proc m_asynFunc;
	uint m_param;
	CSemaphore m_semaphore;
	PooledThread* m_nextPooled;
	Threadlet* m_caller;
};

class Threadlet : public IThreadLet
{
	friend class PooledThread;

public:
	Threadlet();
	~Threadlet();
	bool run(const char *name, Proc asynProc, uint param, uint timeout);
	bool isRunning();
	bool stopRun(bool bWaited = false );    // zzb 2013.9.30 modify
	bool sendMessage (uint msg, uint wpa = 0, uint lpa = 0, uint priority = 0);
	bool recvMessage (CMsgQue::MSG *pMsg, bool wait = true);
	void quitMessage ();
	void clearMessage();
	bool isTimeout();
	void setLoop(bool loop);
	bool getLoop();
	int getThreadID();
private:
	PooledThread* m_thread;
	static CMutex m_mutex;	///< 保证Threadlet和PooledThread的关联关系
};

class CThreadManager
{
	friend class CThread;
	friend class PooledThread;
	friend class Threadlet;
	friend void ThreadBody(void *pdat);

	PATTERN_SINGLETON_DECLARE(CThreadManager);

public:
	typedef std::map<PooledThread*, PooledThread*> PooledThreadMap;
	typedef std::map<CThread*, CThread*> ThreadMap;

	void RegisterMainThread(int id);
	bool HasThread(CThread *pThread);
	void DumpThreads();
	bool GetTimeOutThread(std::string  &threadName);

private:
	bool AddThread(CThread *pThread);
	bool RemoveThread(CThread *pThread);
	PooledThread * GetPooledThread();
	void ReleasePooledThread(PooledThread* pThread);

private:
	PooledThreadMap m_PooledTheadMap;
	ThreadMap	m_ThreadMap;
	CMutex m_Mutex;
	int m_mainThreadID;
};

class CThreadImp
{
public:
	CThreadImp(const char*pName, int nPriority, int nMsgQueSize  = 0 , uint dwStackSize  = 0 );

	volatile bool	m_bWaitThreadExit;
	int		m_nPriority;
	uint	m_dwStackSize;
	void	*m_hThread;
	int		m_ID;
	std::string	m_threadName;
	CMsgQueImp* m_pMsgQue;
	CSemaphore m_cSemaphore;	// 该信号量用来防止同一个对象的线程同时被创建多次；
	uint	m_expectedTime;		//预计执行结束时的时间，0表示不预计
	CSemaphore m_desSemaphore;
};

#define g_ThreadManager (*CThreadManager::instance())

#endif
