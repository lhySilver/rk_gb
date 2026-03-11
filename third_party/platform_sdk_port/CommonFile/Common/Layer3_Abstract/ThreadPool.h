
#ifndef THREAD_POOL_CLASS
#define THREAD_POOL_CLASS
#ifdef WIN32
#pragma warning( disable : 4786) // remove irritating STL warnings
#endif

#include "base_type_define.h"
#include "ShareSDK.h"
#include "MySingleLock.h"
#include <list>
#include <map>

#define POOL_SIZE		  2
#define POOL_MAXSIZE	  100
using namespace std;

// info about functions which require servicing will be saved using this struct.
typedef struct tagFunctionData
{
	LPTHREAD_ROUTINE lpStartAddress;
	LPVOID pData;
} _FunctionData;

// // info about threads in the pool will be saved using this struct.
typedef struct tagThreadData
{
	bool	bFree;
	CMyEvent	*WaitHandle;
	HANDLE	hThread;
    tuint_ptr	dwThreadId;
} _ThreadData;

// info about all threads belonging to this pool will be stored in this map
typedef map<tuint_ptr, _ThreadData, less<tuint_ptr>, allocator<_ThreadData> > ThreadMap;

// all functions passed in by clients will be initially stored in this list.
typedef list<_FunctionData, allocator<_FunctionData> > FunctionList;

// this decides whether a function is added to the front or back of the list.
enum ThreadPriority
{
	High,
	Low
};

class CThreadPool
{
private:
	bool CreateThread(bool bSuspend);
	static void _ThreadProc(LPVOID);
	
	bool	m_bCreate;
	FunctionList m_functionList;
	ThreadMap m_threads;

	int		m_nPoolSize;		// ³õÊ¼³ß´ç
	int		m_nPoolMaxSize;		// ×î´ó³ß´ç
    //CMyEvent	m_hNotifyShutdown; // notifies threads that a new function
    bool m_bRunning;
							   // is added
	CLock m_cs;

    bool	GetThreadProc(tuint_ptr dwThreadId, LPTHREAD_ROUTINE&, LPVOID*);
	
    void	FinishNotify(tuint_ptr dwThreadId);
    void	BusyNotify(tuint_ptr dwThreadId);
	
    CMyEvent *	GetWaitHandle(tuint_ptr dwThreadId);

public:
	void SetPoolMaxSize(int nMaxSize);
	CThreadPool(int nPoolSize = POOL_SIZE, int nPoolMaxSize = POOL_MAXSIZE, bool bCreateNow = true);
	virtual ~CThreadPool();
	bool	Create();	// creates the thread pool
	void	Destroy();	// destroy the thread pool
	
	int		GetPoolSize();
	void	SetPoolSize(int);
	
	void	Run(LPTHREAD_ROUTINE pFunc, LPVOID pData,ThreadPriority priority = Low);
};
//------------------------------------------------------------------------------
#endif
