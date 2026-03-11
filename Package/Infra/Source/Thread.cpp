#include <stdio.h>
#include <string.h>
#ifdef WIN32
	#include <Windows.h>
#elif defined(__GNUC__)
	#include <algorithm>
    #include <errno.h>
	#include <unistd.h>
    #include <sys/syscall.h>   /* For SYS_xxx definitions */
	#define	SCHED_FIFO_SUPPORT	0	/* 1=support SCHED_FIFO		*/
	#define TSK_DEF_STACK_SIZE		16384
    #define gettid() syscall(SYS_gettid)
#else
	#error "unsupport platform!"
#endif

#include "Infra/Thread.h"
#include "Infra/Time.h"
#include "MsgQue.h"
#include "Thread.h"

#define trace printf

//////////////////////////////////////////////////////////////////////////
CThreadImp::CThreadImp(const char*pName, int nPriority, int nMsgQueSize /* = 0 */, uint dwStackSize /* = 0 */) : m_cSemaphore(1), m_desSemaphore(0)
{
}

//////////////////////////////////////////////////////////////////////////

void ThreadBody(void *pdat) 
{
	CThread *pThread = (CThread *)pdat;

/************************************************************************
	线程通用回调函数：
	1、获得线程ID；
	2、执行特定对象的线程回调函数；
	3、当特定对象的线程回调函数执行完毕，把该线程从线程管理队列中删除；
	4、释放同步信号量；
	5、把线程句柄置为空；
************************************************************************/
	pThread->m_threadImp->m_ID = CThread::GetCurrentID();
	g_ThreadManager.AddThread(pThread);
	//trace("ThreadBody Enter name = %s, id = %d, handle = 0x%x, stack = 0x%0x\n", pThread->m_Name, pThread->m_threadImp->m_ID, (int)pThread->m_threadImp->m_hThread, &pThread);
	pThread->ThreadProc();
	//trace("ThreadBody Leave name = %s, id = %d, handle = 0x%x\n", pThread->m_Name, pThread->m_threadImp->m_ID, (int)pThread->m_threadImp->m_hThread);
	pThread->ClearMessage();
	pThread->m_threadImp->m_cSemaphore.Post();
	g_ThreadManager.RemoveThread(pThread);

	if (pThread->m_threadImp->m_bWaitThreadExit)
	{
		pThread->m_threadImp->m_desSemaphore.Post();
	}
#ifdef WIN32
	ExitThread(0);
#elif defined(__GNUC__)
	pthread_exit(NULL);
#endif
}

CThread::CThread(const char*pName, int nPriority, int nMsgQueSize /* = 0 */, uint dwStackSize /* = 0 */)
{
	m_threadImp = new CThreadImp(pName, nPriority, nMsgQueSize, dwStackSize);

/************************************************************************
	线程对象初始化：
	1、信号量初始化；
	2、优先级变量；
	3、堆栈大小变量；
	4、线程名；
	5、线程句柄；
	6、消息队列对象指针；
************************************************************************/
	m_threadImp->m_nPriority = nPriority;
	m_threadImp->m_dwStackSize = dwStackSize;
	if(pName)
	{
		m_threadImp->m_threadName = pName;
	}
	else
	{
		m_threadImp->m_threadName = "Nonamed";
	}
	m_threadImp->m_hThread = NULL;
	m_threadImp->m_ID = -1;
	m_bLoop = false;
	if(nMsgQueSize)
	{
		m_threadImp->m_pMsgQue = new CMsgQueImp(nMsgQueSize);
	}
	else
	{
		m_threadImp->m_pMsgQue = NULL;
	}
	m_threadImp->m_expectedTime = 0;

	m_threadImp->m_bWaitThreadExit = false;
}

CThread::~CThread()
{
	if (m_threadImp->m_hThread)
	{
		DestroyThread();
	}
	
	if(m_threadImp->m_pMsgQue)
	{
		delete m_threadImp->m_pMsgQue;
	}
	delete m_threadImp;
}

int CThread::SetThreadName(const char*pName)
{
	if(pName)
	{
		m_threadImp->m_threadName = pName;
	}
	else
	{
		m_threadImp->m_threadName = "Nonamed";
	}

	return 0;
}

bool CThread::CreateThread()
{
/************************************************************************
	创建线程：
	1、设置信号量和线程回调函数中的循环标志；
	2、调用创建线程API；
	3、如果线程创建成功，把该线程对象加入到线程管理队列，并返回TURE，
	   否则返回false；
************************************************************************/

	m_threadImp->m_cSemaphore.Pend();

	m_bLoop = true;
#ifdef WIN32
	ulong	id;

	m_threadImp->m_hThread = ::CreateThread(NULL, m_threadImp->m_dwStackSize, (LPTHREAD_START_ROUTINE)ThreadBody, this, 0, &id);
	assert(m_threadImp->m_hThread);
	if(!m_threadImp->m_hThread)
	{
		m_bLoop = false;
	}
	return (m_threadImp->m_hThread != 0);


#elif defined(__GNUC__)
	int			policy;
	int			priority_min;
	int			priority_max;
	uint		min_stack_size = TSK_DEF_STACK_SIZE;
	pthread_attr_t		attr;
    struct sched_param sp;

	for (;;)
	{
		if ((m_threadImp->m_nPriority > 0) && (m_threadImp->m_nPriority < 30))
		{
			//policy = SCHED_FIFO;		// 1
			//policy = SCHED_RR;		// 2
			policy = SCHED_OTHER;		// 0
		}
        else if(m_threadImp->m_nPriority == 0)
        {
            policy = SCHED_RR;
        }
		else
		{
			policy = SCHED_OTHER;
		}

		if ((priority_min = sched_get_priority_min(policy)) == -1)
		{
			break;
		}

		if ((priority_max = sched_get_priority_max(policy)) == -1)
		{
			break;
		}

		if (m_threadImp->m_nPriority > priority_max)	m_threadImp->m_nPriority = priority_max;
		if (m_threadImp->m_nPriority < priority_min)	m_threadImp->m_nPriority = priority_min;

		if (policy != SCHED_OTHER)
		{
			m_threadImp->m_nPriority = priority_max - m_threadImp->m_nPriority;
		}
		else
		{
			m_threadImp->m_nPriority = 50;
		}

		if( m_threadImp->m_dwStackSize < min_stack_size )
		{
			m_threadImp->m_dwStackSize = min_stack_size;
		}

		if (pthread_attr_init(&attr) != 0)
		{
			break;
		}

		if (pthread_create((pthread_t *)&m_threadImp->m_hThread, NULL,
			(void* (*)(void *))ThreadBody, this) != 0)
		{
			break;
		}

		if (pthread_detach((pthread_t)(m_threadImp->m_hThread)) != 0)
		{
			break;
		}

        #if 1
        if (policy != SCHED_OTHER)
        {
            sp.sched_priority = m_threadImp->m_nPriority;
            trace("sched set %d, %d\n", sp.sched_priority, policy);
            if (pthread_setschedparam((pthread_t)(m_threadImp->m_hThread),
                policy, &sp) != 0)
            {
                trace("set pthread_setschedparam fail, %s\n", strerror(errno));
                break;
            }
        }
        
        #else
        if (pthread_setschedparam((pthread_t)(m_threadImp->m_hThread),
            policy, (struct sched_param *)(&m_threadImp->m_nPriority)) != 0)
        {
            trace("set pthread_setschedparam fail, %s\n", strerror(errno));
            break;
        }
        #endif

#if (SCHED_FIFO_SUPPORT)
		if (policy != SCHED_OTHER)
		{
			if (pthread_setschedparam((pthread_t)(m_threadImp->m_hThread),
				policy, (struct sched_param *)(&m_threadImp->m_nPriority)) != 0)
			{
				break;
			}
		}
#endif
		return true;
	}

	return false;
#else
	return false;
#endif
}

bool CThread::DestroyThread(bool bWaited)
{
/************************************************************************
	结束线程：
	1、直接把用于控制主循环的变量置为false，让它退出主循环；
************************************************************************/
	if (!IsThreadOver())
	{
		m_bLoop = false;
		SendMessage(msgThreadOver);//如果发送出错，说明队列以满，此时m_bLoop可以保证线程循环退出

		// 不是自己关自己的时候才采用阻塞式
		if (m_threadImp->m_ID != GetCurrentID())
		{
			m_threadImp->m_bWaitThreadExit = bWaited;
			if (m_threadImp->m_bWaitThreadExit)
			{
				m_threadImp->m_desSemaphore.Pend();
			}
		}
	}
	return true;
}

bool CThread::IsThreadOver()
{
/************************************************************************
	判断线程回调函数是否已经停止：
	1、如果已经停止，则返回true，否则返回false；
************************************************************************/
	return !m_bLoop;
	
}

int CThread::GetThreadID()
{
	return m_threadImp->m_ID;
}

bool CThread::SendMessage(uint msg, uint wpa /* = 0 */, uint lpa /* = 0 */, uint priority /* = 0 */)
{
/************************************************************************
	发送消息：
	1、判断本线程是否有消息队列；
	2、如果有，则调用消息队列对象的SendMessage函数发送消息，
	   否则直接返回false；
************************************************************************/
	if(m_threadImp->m_pMsgQue)
	{
		return m_threadImp->m_pMsgQue->SendMessage(msg, wpa, lpa, priority);
	}
	return false;
}

bool CThread::RecvMessage(CMsgQue::MSG *pMsg, bool wait /* = true */)
{
/************************************************************************
	接收消息：
	1、判断本线程是否有消息队列；
	2、如果有，则调用消息队列对象的RecvMessage函数接收消息，
	   否则直接返回false；
************************************************************************/
	if(m_threadImp->m_pMsgQue)
	{
		return m_threadImp->m_pMsgQue->RecvMessage(pMsg, wait);
	}
	return false;
}

void CThread::QuitMessage()
{
/************************************************************************
	停止消息：
	1、判断本线程是否有消息队列；
	2、如果有，则调用消息队列对象的QuitMessage函数停止消息，
	   否则直接返回false；
************************************************************************/
	if(m_threadImp->m_pMsgQue)
	{
		m_threadImp->m_pMsgQue->QuitMessage();
	}
}

void CThread::ClearMessage()
{
	if(m_threadImp->m_pMsgQue)
	{
		m_threadImp->m_pMsgQue->ClearMessage();
	}
}

uint CThread::GetMessageCount()
{
	if(m_threadImp->m_pMsgQue)
	{
		return m_threadImp->m_pMsgQue->GetMessageCount();
	}
	return 0;
}

void CThread::SetTimeout(uint milliSeconds)
{
	if(milliSeconds == 0) // 清空预期时间
	{
		m_threadImp->m_expectedTime = 0;
	}
	else
	{
		m_threadImp->m_expectedTime = CTime::getCurrentMicroSecond() + milliSeconds;
		if(m_threadImp->m_expectedTime < milliSeconds) // 计数溢出，暂时不设置预期时时间
		{
			m_threadImp->m_expectedTime = 0;
		}
	}
}

int CThread::GetCurrentID()
{
#ifdef WIN32
	return (int)GetCurrentThreadId();
#elif defined(__GNUC__)
	return gettid();//getpid();
#else
	assert("unknow platform" && 0);
#endif
}

void CThread::DumpThreads()
{
	g_ThreadManager.DumpThreads();
}

bool CThread::IsTimeout()
{
	return (m_threadImp->m_expectedTime != 0 && m_threadImp->m_expectedTime < CTime::getCurrentMicroSecond());
}

#define TP_DEFAULT		64

////////////////////////////////////////////////////////////////////////////////////
// PooledThread
////////////////////////////////////////////////////////////////////////////////////
PooledThread::PooledThread() : CThread("Pooled", TP_DEFAULT)
{
}

PooledThread::~PooledThread()
{
}

void PooledThread::ThreadProc()
{
	while(m_bLoop)
	{
		m_semaphore.Pend();
		m_asynFunc(m_param); 
		SetThreadName("Pooled");
		SetTimeout(0);
		Threadlet::m_mutex.Enter();
		if(m_caller)
		{
			m_caller->m_thread = NULL;
			m_caller = NULL;
		}
		Threadlet::m_mutex.Leave();
		g_ThreadManager.ReleasePooledThread(this);
	}
}

////////////////////////////////////////////////////////////////////////////////////
// Threadlet
////////////////////////////////////////////////////////////////////////////////////
CMutex Threadlet::m_mutex;

Threadlet::Threadlet()
{
	m_thread = NULL;
}

Threadlet::~Threadlet()
{
	CGuard guard(m_mutex);

	if(m_thread)
	{
		m_thread->m_caller = NULL;
	}
}

bool Threadlet::run(const char *name, Proc asynProc, uint param, uint timeout)
{
	CGuard guard(m_mutex);

	if(m_thread) //正在运行
	{
		return false;
	}
	m_thread = g_ThreadManager.GetPooledThread();
	m_thread->SetThreadName(name);
	m_thread->SetTimeout(timeout);
	m_thread->m_asynFunc = asynProc;
	m_thread->m_param = param;
	m_thread->m_caller = this;
	m_thread->m_semaphore.Post();
	return true;
}

bool Threadlet::isRunning()
{
	return (m_thread != NULL);
}

bool Threadlet::stopRun(bool wait)
{
	if (m_thread)
	{
		return m_thread->DestroyThread(wait);
	}
	return true;
}

bool Threadlet::sendMessage (uint msg, uint wpa, uint lpa, uint priority)
{
	if (m_thread)
	{
		return m_thread->SendMessage(msg, wpa, lpa, priority);
	}
	return true;
}

bool Threadlet::recvMessage (CMsgQue::MSG *pMsg, bool wait)
{
	if (m_thread)
	{
		return m_thread->RecvMessage(pMsg, wait);
	}
	return true;
}

void Threadlet::quitMessage ()
{
	if (m_thread)
	{
		m_thread->QuitMessage();
	}
}

void Threadlet::clearMessage()
{
	if (m_thread)
	{
		m_thread->ClearMessage();
	}
}

bool Threadlet::isTimeout()
{
	if (m_thread)
	{
		m_thread->IsTimeout();
	}
	return true;
}

void Threadlet::setLoop(bool loop)
{
	if (m_thread)
	{
		m_thread->m_bLoop = loop;
	}
}

bool Threadlet::getLoop()
{
	if (m_thread)
	{
		return m_thread->m_bLoop;
	}
	return false;
}

int Threadlet::getThreadID()
{
	if (m_thread)
	{
		return m_thread->GetThreadID();
	}
	return -1;
}

IThreadLet *IThreadLet::create()
{
	return new Threadlet;
}

/// 析构函数
IThreadLet::~IThreadLet()
{
}

////////////////////////////////////////////////////////////////////////////////////
// CThreadManager
////////////////////////////////////////////////////////////////////////////////////

PATTERN_SINGLETON_IMPLEMENT(CThreadManager);

CThreadManager::CThreadManager() : m_Mutex(MUTEX_RECURSIVE)
{
	trace("CThreadManager::CThreadManager()\n");
}

CThreadManager::~CThreadManager()
{
}

void CThreadManager::RegisterMainThread(int id)
{
	m_mainThreadID = id;
}

bool CThreadManager::AddThread(CThread *pThread)
{
	CGuard guard(m_Mutex);
	std::pair<ThreadMap::iterator, bool> result = m_ThreadMap.insert(ThreadMap::value_type(pThread, pThread));
	
	if (!result.second)
	{
		trace("AddThread failed!\n");
		return false;
	}
	return true;
}

bool CThreadManager::RemoveThread(CThread *pThread)
{
	CGuard guard(m_Mutex);
	ThreadMap::iterator pi = m_ThreadMap.find(pThread);

	if (pi != m_ThreadMap.end())
	{
		m_ThreadMap.erase(pi);
	}
	return true;
}

bool CThreadManager::HasThread(CThread *pThread)
{
	CGuard guard(m_Mutex);
	ThreadMap::iterator pi = m_ThreadMap.find(pThread);

	if (pi != m_ThreadMap.end())
	{
		return true;
	}
	return false;
}

PooledThread * CThreadManager::GetPooledThread()
{
	CGuard guard(m_Mutex);
	PooledThread *p;
	PooledThreadMap::iterator pi = m_PooledTheadMap.begin();

	if(pi != m_PooledTheadMap.end()) 
	{
		p = (*pi).second;
		m_PooledTheadMap.erase(pi);
	}
	else 
	{
		p = new PooledThread();
		p->CreateThread();
	}

	return p;
}

void CThreadManager::ReleasePooledThread(PooledThread* pThread)
{
	CGuard guard(m_Mutex);
	assert(pThread);
	PooledThreadMap::iterator pi = m_PooledTheadMap.find(pThread);

	std::pair<PooledThreadMap::iterator, bool> result = m_PooledTheadMap.insert(PooledThreadMap::value_type(pThread, pThread));
	if (!result.second)
	{
		trace("ReleasePooledThread failed!\n");
	}
}

void CThreadManager::DumpThreads()
{
	CGuard guard(m_Mutex);

	trace("Threads:\n");
	trace("               Name            PID  Prior State\n");
	trace("_______________________________________________________\n");
	trace("%24s   %8d  %3d  %s\n", "Main", m_mainThreadID, TP_DEFAULT, "Normal");
	for (ThreadMap::iterator pi = m_ThreadMap.begin(); pi != m_ThreadMap.end(); pi++)
	{
		CThread *p = (*pi).second;
		trace("%24s   %8d  %3d  %s\n", p->m_threadImp->m_threadName.c_str(), p->m_threadImp->m_ID, p->m_threadImp->m_nPriority, p->IsTimeout() ? "Timeout" : "Normal");
	}
}

