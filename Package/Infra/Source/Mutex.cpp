///
///	\class C
///	\brief
///									 \n
///						     Digital Video Recoder xp\n
///	      (c) Copyright 1992-2007, ZheJiang Dahua Technology Stock Co.Ltd.\n
///								All Rights Reserved\n
///
///	FILE NAME		:  Mutex.cpp\n
///	Description		:\n
///	\b Revisions	:
///		- 2008-12-18			yuan_shiyong		Create
///

#include <stdio.h>
#include <stdlib.h>
#include <Infra/Mutex.h>
//#include <Infra/APIMutex.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include <errno.h>


////////////////////////////////////////////////////////////////////////////

typedef void *			HANDLE;


//#include "DVRDEF.H"

//#include "dvr_inc.h"
//#include "dvr_debug.h"
//#include "PAL/Mutex.h"
//#include <Infra/Mutex.h>

#define ERR_PRINT printf

//#ifdef REC_MUTEX
#if 1
//#warning "Use recursive mutex!"



#define TS_LOCK(lock)	pthread_cleanup_push((void(*)(void*))pthread_mutex_unlock, (void*)lock );	pthread_mutex_lock( lock )

#define TS_UNLOCK(lock)	pthread_cleanup_pop( 1 )


/************************************************************************
 * thread-safe malloc
 ************************************************************************/
void *ts_malloc(size_t s)
{
	void *			addr;
	
	static pthread_mutex_t	malloc_lock = PTHREAD_MUTEX_INITIALIZER;
	
	TS_LOCK(&malloc_lock);
	addr = malloc(s);
	TS_UNLOCK(&malloc_lock);
	
	return(addr);

} /* end ts_malloc */
    
/************************************************************************
 * thread-safe free
 ************************************************************************/
void ts_free(void *addr)
{
	static pthread_mutex_t	free_lock = PTHREAD_MUTEX_INITIALIZER;
	
	TS_LOCK(&free_lock);
	free(addr);
	TS_UNLOCK(&free_lock);
 
} /* end ts_free */




int MutexCreate(HANDLE *phMutex,int nType)
{
	int ret;	
	pthread_mutex_t *lock;
	pthread_mutexattr_t attr;


	lock = (pthread_mutex_t *)ts_malloc(sizeof(pthread_mutex_t));
	if (lock == NULL)
	{
		ERR_PRINT("No Mem\n");
		return -ENOMEM;
	}
	
	switch (nType) 
	{
		case MUTEX_FAST:
		{
			ret = pthread_mutex_init(lock, NULL);
			if( ret != 0 )
			{
				ts_free(lock);
				errno = ret;
				ERR_PRINT("Pthread Mutex Init: %s\n", strerror(ret));
				*phMutex = (HANDLE)(NULL);
				return -ret;
			}
			break;
		}
		case MUTEX_RECURSIVE:
		{

    		if ((ret = pthread_mutexattr_init(&attr)) != 0)
    		{
     			ts_free(lock);
    			ERR_PRINT("Pthread_mutexattr_init Failed\n");	
    			return -ENOMEM;
    		}
     		
		    if ((ret = pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE_NP)) != 0)
		    {
		    	ts_free(lock);
		    	ERR_PRINT("Can't Set Recursive Type\n");
		    	*phMutex = (HANDLE)(NULL);
		    	return -ret;
		    }
		    
		    if ((ret = pthread_mutex_init(lock, &attr)) != 0)
		    {
		    	ERR_PRINT("Can't Create Recursive Mutex\n");
		    	*phMutex = (HANDLE)(NULL);
		    	return -ret;
		    }
		}
		break;
		default:
		{
			ret = pthread_mutex_init(lock, NULL);
			if( ret != 0 )
			{
				ts_free(lock);
				errno = ret;
				ERR_PRINT("Pthread Mutex Init: %s\n", strerror(ret));
				*phMutex = (HANDLE)(NULL);
				return -ret;
			}
			break;
		}
			
	}
	
	*phMutex = (HANDLE)lock;

	return 0;
}

#else
//#warning "Not Use recursive mutex!"

/************************************************************************
 * mutex create
 *
 * ret :	= 0		success
 * 		< 0		false
 ************************************************************************/
int MutexCreate(HANDLE* phmutex,int nType)
{
	int			ret;
	pthread_mutex_t *	lock;
	
	lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (lock)
	{
		ret = pthread_mutex_init(lock, NULL);
		if (ret != 0)
		{
			free(lock);
			errno	= ret;
			ERR_PRINT("Pthread Mutex Init\n");
			*phmutex	= (HANDLE)(NULL);
			return -ret;
		}
	
		*phmutex = (HANDLE)lock;
	
		return 0;
	}
	
	ERR_PRINT(("MutexCreate No Mem\n"));
	return -1;
} /* end MutexCreate */
#endif

/************************************************************************
 * mutex destory
 *
 * ret :	= 0		success
 * 		< 0		false
 ************************************************************************/
int MutexDestory(HANDLE hmutex)
{
	pthread_mutex_destroy((pthread_mutex_t *)hmutex);

	free((void*)hmutex);
	return 0;
} /* end MutexDestory */

/************************************************************************
 * mutex enter
 *
 * ret :	= 0		success
 * 		< 0		false
 ************************************************************************/
int MutexEnter(HANDLE hmutex)
{
  return -pthread_mutex_lock((pthread_mutex_t *)hmutex);
} /* end MutexEnter */

/************************************************************************
 * Mutex leave
 *
 * ret :	= 0		success
 * 		< 0		false
 ************************************************************************/
int MutexLeave(HANDLE hmutex)
{
  return -pthread_mutex_unlock((pthread_mutex_t *)hmutex);
} /* end MutexLeave */


/************************************************************************
 * mutex create
 *
 * ret :	= 0		success
 * 		< 0		false
 ************************************************************************/
int RWMutexCreate(HANDLE* phmutex)
{
	int			ret;
	pthread_rwlock_t *lock;
	
	lock = (pthread_rwlock_t *)malloc(sizeof(pthread_rwlock_t));
	if (lock)
	{
		ret = pthread_rwlock_init(lock, NULL);
		if (ret != 0)
		{
			free(lock);
			errno	= ret;
			ERR_PRINT("Pthread Mutex Init\n");
			*phmutex	= (HANDLE)(NULL);
			return -ret;
		}
	
		*phmutex = (HANDLE)lock;
	
		return 0;
	}
	
	return -1;
} /* end MutexCreate */

/************************************************************************
 * mutex destory
 *
 * ret :	= 0		success
 * 		< 0		false
 ************************************************************************/
int RWMutexDestory(HANDLE hmutex)
{
	pthread_rwlock_destroy((pthread_rwlock_t *)hmutex);

	free((void*)hmutex);
	return 0;
} /* end MutexDestory */

/************************************************************************
 * mutex enter
 *
 * ret :	= 0		success
 * 		< 0		false
 ************************************************************************/
int WriteMutexEnter(HANDLE hmutex)
{
  return -pthread_rwlock_wrlock((pthread_rwlock_t *)hmutex);
} /* end MutexEnter */

int ReadMutexEnter(HANDLE hmutex)
{
  return -pthread_rwlock_rdlock((pthread_rwlock_t *)hmutex);
} /* end MutexEnter */

/************************************************************************
 * Mutex leave
 *
 * ret :	= 0		success
 * 		< 0		false
 ************************************************************************/
int WriteMutexLeave(HANDLE hmutex)
{
  return -pthread_rwlock_unlock((pthread_rwlock_t *)hmutex);
} /* end MutexLeave */

int ReadMutexLeave(HANDLE hmutex)
{
  return -pthread_rwlock_unlock((pthread_rwlock_t *)hmutex);
} /* end MutexLeave */




////////////////////////////////////////////////////////////////////////////


CMutex::CMutex(int nType)
{
	m_hMutex = 0;
	MutexCreate(&m_hMutex, nType);
	assert(m_hMutex);
}

CMutex::~CMutex()
{
	MutexDestory(m_hMutex);
}

bool CMutex::Enter()
{
	assert(m_hMutex);

	return MutexEnter(m_hMutex) == 0 ? true : false;
	//return true;
}

bool CMutex::Leave()
{
	assert(m_hMutex);

	return MutexLeave(m_hMutex) == 0 ? true : false;
	//return true;
}

// ------- zzb 2013.8.20 add begin --------------------
bool CMutex::TryEnter()		// zzb 2013.8.16 add
{
	assert(m_hMutex);
	return pthread_mutex_trylock((pthread_mutex_t*)m_hMutex) == 0 ? true : false;
	//return true;
}

CReadWriteMutex::CReadWriteMutex()
{
	m_hrwMutex = 0;
	RWMutexCreate(&m_hrwMutex);
	assert(m_hrwMutex);
}

CReadWriteMutex::~CReadWriteMutex()
{
	RWMutexDestory(m_hrwMutex);
}

bool CReadWriteMutex::LeaveRead()
{
	assert(m_hrwMutex);
	return ReadMutexLeave(m_hrwMutex) == 0 ? true : false;
	//return true;
}

bool CReadWriteMutex::EnterRead()
{
	assert(m_hrwMutex);
	return ReadMutexEnter(m_hrwMutex) == 0 ? true : false;
	//return true;
}

bool CReadWriteMutex::EnterWrite()
{
	assert(m_hrwMutex);
	return WriteMutexEnter(m_hrwMutex) == 0 ? true : false;
	//return true;
}

bool CReadWriteMutex::LeaveWrite()
{
	assert(m_hrwMutex);
	return WriteMutexLeave(m_hrwMutex) == 0 ? true : false;
	//return true;
}

////// ----------------- add for jzz begin -------------------------
int ReadMutexTryEnter(HANDLE hMutex)
{
	return -pthread_rwlock_tryrdlock((pthread_rwlock_t*)hMutex);
}

int WriteMutexTryEnter(HANDLE hMutex)
{
	return -pthread_rwlock_trywrlock((pthread_rwlock_t*)hMutex);
}
////// ----------------- add for jzz end -------------------------

bool CReadWriteMutex::TryEnterRead()
{
	assert(m_hrwMutex);
	return ReadMutexTryEnter(m_hrwMutex) == 0 ? true : false;
	//return true;
}

bool CReadWriteMutex::TryEnterWrite()
{
	assert(m_hrwMutex);
	return WriteMutexTryEnter(m_hrwMutex) == 0 ? true : false;
	//return true;
}

