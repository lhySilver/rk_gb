
#include "ThreadPool.h"
#include "ShareSDK.h"
#include "PFHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////
void CThreadPool::_ThreadProc(LPVOID pParam)
{
	CThreadPool*			pool;
	LPTHREAD_ROUTINE		proc;
	LPVOID					data;
    tuint_ptr				dwThreadId = CPFHelper::GetThreadID();
    //CMyEvent				*hWaits[2];

//	ASSERT(pParam != NULL);
	if(NULL == pParam)
	{
		ABNORMAL_OUTPUT("线程池的线程参数错误导致退出");
		return;
	}

    pool = static_cast<CThreadPool*>(pParam);
    CMyEvent *pMyEvent = pool->GetWaitHandle(dwThreadId);
    //hWaits[0] = pool->GetWaitHandle(dwThreadId);
    //hWaits[1] = &pool->m_hNotifyShutdown;
/*
    if (hWaits[0]==NULL || hWaits[1]==NULL)
    {
        return;
    }
*/
	loop_here:

    //tuint32 dwWait = WaitForMultipleObjects(2,(const CMyEvent**)hWaits, FALSE, INFINITE);
    pMyEvent->WaitEvent(INFINITE);
    //if(dwWait - WAIT_OBJECT_0 == 0)
    if (pool->m_bRunning)
	{
		// a new function was added, go and get it
		if(pool->GetThreadProc(dwThreadId, proc, &data))
		{
			pool->BusyNotify(dwThreadId);
			
			proc(data);

			pool->FinishNotify(dwThreadId); // tell the pool, i am now free
		}
		else
		{
			///////////////////没有任务需要处理  也要设置空闲状态
			pool->FinishNotify(dwThreadId); // tell the pool, i am now free
		}

//		tuint32   dwShutdownWaitRet = WaitForSingleObject(hWaits[1], 0);
//		if(dwShutdownWaitRet != WAIT_OBJECT_0)
		if (pool->m_bRunning)
		{
			goto loop_here;			
		}
	}	
	NORMAL_OUTPUT("线程池的线程由于关闭事件有信号而退出！");
}

//------------------------------------------------------------------------------
/* Parameters	: Pool size, indicates the number of threads that will be 
				  available in the queue.
*******************************************************************************/
CThreadPool::CThreadPool(int nPoolSize, int nPoolMaxSize, bool bCreateNow)// : m_hNotifyShutdown(TRUE)
{
	m_nPoolSize = nPoolSize;
	m_nPoolMaxSize = nPoolMaxSize;
	m_bCreate=false;
    m_bRunning = true;
	if(bCreateNow)
	{
		if(!Create())
		{
			ABNORMAL_OUTPUT("初始化的时候线程池的创建线程失败");
			throw 1;
		}
	}
}
//------------------------------------------------------------------------------

/* Description	: Use this method to create the thread pool. The constructor of
				  this class by default will create the pool. Make sure you 
				  do not call this method without first calling the Destroy() 
				  method to release the existing pool.
   Returns		: true if everything went fine else returns false.
  *****************************************************************************/
bool CThreadPool::Create()
{
	// create the threads
    m_bRunning = true;

	for(int nIndex = 0; nIndex < m_nPoolSize; nIndex++)
	{
		if(!CreateThread(true))
		{
			break;
		}
	}
	m_bCreate=true;
	return true;
}
//------------------------------------------------------------------------------

CThreadPool::~CThreadPool()
{
	Destroy();
}
//------------------------------------------------------------------------------

/* Description	: Use this method to destory the thread pool. The destructor of
				  this class will destory the pool anyway. Make sure you 
				  this method before calling a Create() when an existing pool is 
				  already existing.
   Returns		: void
  *****************************************************************************/
void CThreadPool::Destroy()
{
	// tell all threads to shutdown.
	if(m_bCreate)
	{
        //m_hNotifyShutdown.SetEvent();
        m_bRunning = false;

		// lets give the thread one second atleast to terminate
        //CPFHelper::Sleep(100);
			
		// walk through the events and threads and close them all
		for(ThreadMap::iterator iter = m_threads.begin(); iter != m_threads.end(); iter++)
		{
            _ThreadData &ThreadData = (*iter).second;
            ThreadData.WaitHandle->SetEvent();
			DESTROY_THREAD(ThreadData.hThread);
			//delete ThreadData.WaitHandle;
		} 

		// empty all collections
		m_functionList.clear();

		{
			CMySingleLock singlock(m_cs);
			for(ThreadMap::iterator iter = m_threads.begin(); iter != m_threads.end(); iter++)
			{
				_ThreadData &ThreadData = (*iter).second;
				delete ThreadData.WaitHandle;
				ThreadData.WaitHandle = NULL;
			}
			m_threads.clear();
		}
				 
		
        //m_hNotifyShutdown.ResetEvent();
	}
}
//------------------------------------------------------------------------------

int CThreadPool::GetPoolSize()
{
	return m_nPoolSize;
}
//------------------------------------------------------------------------------

/* Parameters	: nSize - number of threads in the pool.   
   ****************************************************************************/
void CThreadPool::SetPoolSize(int nSize)
{
//	ASSERT(nSize > 0);
	if(nSize <= 0)
	{
		return;
	}

	m_nPoolSize = nSize;
}

//------------------------------------------------------------------------------

/* Parameters	: hThread - Handle of the thread that is invoking this function.
   Return		: A ThreadProc function pointer. This function pointer will be 
			      executed by the actual calling thread.
				  NULL is returned if no functions list is empty.
																			  */
bool CThreadPool::GetThreadProc(tuint_ptr dwThreadId, LPTHREAD_ROUTINE& Proc,LPVOID* Data)
{
	// get the first function info in the function list
	CMySingleLock singlock(m_cs);
	
	FunctionList::iterator	iter = m_functionList.begin();

	if(iter != m_functionList.end())
	{
		_FunctionData	FunctionData = (*iter);

		Proc = FunctionData.lpStartAddress;
		
		*Data		= FunctionData.pData;

		// 从任务队列中删除
		m_functionList.pop_front(); // remove the function from the list
	
		return true;	
	}
	else
	{
		return false;
	}
}
//------------------------------------------------------------------------------

/* Parameters	: hThread - Handle of the thread that is invoking this function.
   Description	: When ever a thread finishes executing the user function, it 
				  should notify the pool that it has finished executing.      
																			  */
void CThreadPool::FinishNotify(tuint_ptr dwThreadId)
{
	ThreadMap::iterator iter;
	
	CMySingleLock singlock(m_cs);
	iter = m_threads.find(dwThreadId);

	if(iter == m_threads.end())	// if search found no elements
	{			
		ASSERT(FALSE);
		return;
	}
	else
	{	
		if(!m_functionList.empty())
		{
			// there are some more functions that need servicing, lets do that.
			// By not doing anything here we are letting the thread go back and
			// check the function list and pick up a function and execute it.
			return;
		}		
		else
		{
			m_threads[dwThreadId].bFree = true;
			// back to sleep, there is nothing that needs servicing.		
			m_threads[dwThreadId].WaitHandle->ResetEvent();
		}
	}	
}
//------------------------------------------------------------------------------

void CThreadPool::BusyNotify(tuint_ptr dwThreadId)
{
	ThreadMap::iterator iter;
	
	CMySingleLock singlock(m_cs);

	iter = m_threads.find(dwThreadId);

	if(iter == m_threads.end())	// if search found no elements
	{
		ASSERT(FALSE);
	}
	else
	{		
		m_threads[dwThreadId].bFree = false;		
	}	
}
//------------------------------------------------------------------------------

/* Parameters	: pFunc - function pointer of type ThreadProc
				  pData - An LPVOID pointer
   Decription	: This function is to be called by clients which want to make 
				  use of the thread pool.
  *****************************************************************************/
void CThreadPool::Run(LPTHREAD_ROUTINE pFunc, LPVOID pData, ThreadPriority priority)
{
	_FunctionData funcdata;

	funcdata.lpStartAddress = pFunc;
	funcdata.pData			= pData;

	// add it to the list
	m_cs.Lock();
	if(priority == Low)
	{
		m_functionList.push_back(funcdata);
	}
	else
	{
		m_functionList.push_front(funcdata);
	}
	m_cs.Unlock();

	// See if any threads are free
	ThreadMap::iterator iter;
	_ThreadData ThreadData;

	m_cs.Lock();
	bool bStarted = false;
	for(iter = m_threads.begin(); iter != m_threads.end(); iter++)
	{
		ThreadData = (*iter).second;
		
		if(ThreadData.bFree)
		{
			// here is a free thread, put it to work
			m_threads[ThreadData.dwThreadId].bFree = false;			
			ThreadData.WaitHandle->SetEvent(); 
			// this thread will now call GetThreadProc() and pick up the next
			// function in the list.
			bStarted = true;
			break;
		}
	}

	if(!bStarted)
	{
		CreateThread(false);
	}

	m_cs.Unlock();
}

//------------------------------------------------------------------------------

/* Parameters	: ThreadId - the id of the thread for which the wait handle is 
							 being requested.
   Returns		: NULL if no mathcing thread id is present.
				  The HANDLE which can be used by WaitForXXXObject API.
  *****************************************************************************/
CMyEvent *CThreadPool::GetWaitHandle(tuint_ptr dwThreadId)
{
	ThreadMap::iterator iter;
	
	CMySingleLock singlock(m_cs);
	iter = m_threads.find(dwThreadId);
	
	if(iter == m_threads.end())	// if search found no elements
	{
		return NULL;
	}
	else
	{		
		return m_threads[dwThreadId].WaitHandle;
	}	
}
//------------------------------------------------------------------------------

void CThreadPool::SetPoolMaxSize(int nMaxSize)
{
	m_nPoolMaxSize = nMaxSize;
}

// 创建线程，调用时需用临界区保护
// bSuspend:是否暂时挂起

bool CThreadPool::CreateThread(bool bSuspend)
{
	if(m_threads.size() >= (size_t)m_nPoolMaxSize)
	{
		//CRunLog::AddLog(_T("线程池的线程参数错误导致退出"));
		return false;
	}

    HANDLE	hThread = CREATE_RUNONE_THREAD(CThreadPool::_ThreadProc, this, TRUE, "##Thread Pool Thread##");
	if(hThread)
	{
        tuint_ptr	dwThreadId=SHARESDK_GetThreadID(hThread);
		TVT_LOG_INFO("create Thread:CThreadPool:" << CPFHelper::GetThreadID(hThread) );
		// add the entry to the map of threads
		_ThreadData ThreadData;
		ThreadData.bFree		= bSuspend;
		ThreadData.WaitHandle	= new CMyEvent(TRUE);
		if(!bSuspend) ThreadData.WaitHandle->SetEvent();//如果不刮起，启动就要有信号
		ThreadData.hThread		= hThread;
		ThreadData.dwThreadId	= dwThreadId;
		
		m_threads.insert(ThreadMap::value_type(dwThreadId, ThreadData));		
		
		///////////////////////////////////////
		SHARESDK_ResumeThread(hThread); 
	}
	else
	{
		NORMAL_OUTPUT("线程池的线程创建失败！");
		return false;
	}

	return true;
}
