#ifndef __IMPLEMENT_MSGQUE_H__
#define __IMPLEMENT_MSGQUE_H__

#include "Infra/Mutex.h"
#include "Infra/Semaphore.h"
#include "Infra/PoolAllocator.h"
#include "Infra/Types.h"
#include <list>

// Windows内已定义SendMessage宏，从而导致链接程序实际编译的时候并不是
// SendMessage函数的问题
#ifdef SendMessage
	#undef SendMessage
#endif

class CMsgQueImp
{
public:

	CMsgQueImp(int size = 1024);
	virtual ~CMsgQueImp();

	bool SendMessage (uint msg, uint wpa = 0, uint lpa = 0, uint priority = 0);
	bool RecvMessage (CMsgQue::MSG *pMsg, bool wait = true);
	void QuitMessage ();
	void ClearMessage();
	int GetMessageCount();
	int GetMessageSize();
	void SetMessageSize(int size);

private:
	typedef std::list<CMsgQue::MSG, pool_allocator<CMsgQue::MSG> > MSGQUEUE;

private:

	MSGQUEUE m_Queue;
	bool m_bMsgFlg;
	CMutex m_Mutex;
	CSemaphore m_Semaphore;
	int m_nMsg;
	int m_nMaxMsg;
};

#endif
