#include "Infra/MsgQue.h"
#include "Infra/Guard.h"
#include "Infra/Time.h"
#include "MsgQue.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////////

CMsgQue::CMsgQue(int size)
{
	m_MsgQueImp = new CMsgQueImp(size);
}

CMsgQue::~CMsgQue()
{
	delete static_cast<CMsgQueImp *>(m_MsgQueImp);
}

bool CMsgQue::SendMessage (uint msg, uint wpa, uint lpa, uint priority)
{
	return (static_cast<CMsgQueImp *>(m_MsgQueImp))->SendMessage(msg, wpa, lpa, priority);
}

bool CMsgQue::RecvMessage (MSG *pMsg, bool wait)
{
	return (static_cast<CMsgQueImp *>(m_MsgQueImp))->RecvMessage(pMsg, wait);
}

void CMsgQue::QuitMessage ()
{
	static_cast<CMsgQueImp *>(m_MsgQueImp)->QuitMessage();
}

void CMsgQue::ClearMessage()
{
	static_cast<CMsgQueImp *>(m_MsgQueImp)->ClearMessage();
}

int CMsgQue::GetMessageCount()
{
	return static_cast<CMsgQueImp *>(m_MsgQueImp)->GetMessageCount();
}

int CMsgQue::GetMessageSize()
{
	return static_cast<CMsgQueImp *>(m_MsgQueImp)->GetMessageSize();
}

void CMsgQue::SetMessageSize(int size)
{
	static_cast<CMsgQueImp *>(m_MsgQueImp)->SetMessageSize(size);
}

//////////////////////////////////////////////////////////////////////////
#define MSG_PRIORITY	10

CMsgQueImp::CMsgQueImp(int size /* = 1024 */)
{
/************************************************************************
	消息队列的初始化工作：
	1、一次性把队列的所有消息结构体对象都创建起来;
	2、把这些对象都加到空闲队列m_FreeQueue;
	3、把消息队列可访问标志置为有效；
************************************************************************/
	m_nMaxMsg = size;
	m_nMsg = 0;
	m_bMsgFlg = true;
}

CMsgQueImp::~CMsgQueImp()
{

}

bool CMsgQueImp::SendMessage(uint msg, uint wpa /* = 0 */, uint lpa /* = 0 */, uint priority /* = 0 */)
{
	CMsgQue::MSG l_MSG;
	MSGQUEUE::iterator pi;

	m_Mutex.Enter();
	if(m_nMsg >= m_nMaxMsg)
	{
		m_Mutex.Leave();
		return false;
	}
	if(priority >= MSG_PRIORITY)
	{
		m_Mutex.Leave();
		return false;
	}
	if (!m_bMsgFlg) 
	{
		m_Mutex.Leave();
		return false;
	}

	l_MSG.msg = msg;
	l_MSG.wpa = wpa;
	l_MSG.lpa = lpa;
	l_MSG.time = static_cast<uint>(CTime::getCurrentMicroSecond());
	m_Queue.push_front(l_MSG);
	m_nMsg++;
	m_Mutex.Leave();

	m_Semaphore.Post();
	return true;
}

bool CMsgQueImp::RecvMessage(CMsgQue::MSG *pMsg, bool wait /* = true */)
{
/************************************************************************
	接收消息，如果等待则一直等到有消息时返回，否则直接返回。
	1、从消息忙队列m_Queue取元素，如果取成功，则直接返回；
	2、否则循环从消息忙m_Queue中取元素，直到取成功才退出循环；
************************************************************************/
	if(wait)
	{
		m_Semaphore.Pend();
	}

	CGuard guard(m_Mutex);
//	trace("RecvMessage %d\n", m_nMsg);

	if (m_Queue.empty()) 
	{
//		tracepoint();
		return false;
	}
	if(!wait)
	{
		m_Semaphore.Pend();
	}
	assert(m_nMsg);
	*pMsg = m_Queue.back();
	m_Queue.pop_back();
//	printf("pMsg==%x  l_MSG.msg==%x l_MSG.wpa==%x l_MSG.lpa==%x\n",	pMsg, l_MSG.msg, l_MSG.wpa, l_MSG.lpa);
	m_nMsg--;

	return true;
}

void CMsgQueImp::QuitMessage ()
{
	CGuard guard(m_Mutex);

	m_bMsgFlg = false;
}

void CMsgQueImp::ClearMessage()
{
	CGuard guard(m_Mutex);

	int n = m_nMsg;
	for(int i = 0; i < n; i++)
	{
		m_Semaphore.Pend();
		m_Queue.pop_back();
		m_nMsg--;
	}
}

int CMsgQueImp::GetMessageCount()
{
	CGuard guard(m_Mutex);

	return m_nMsg;
}

int CMsgQueImp::GetMessageSize()
{
	CGuard guard(m_Mutex);

	return m_nMaxMsg;
}

void CMsgQueImp::SetMessageSize(int size)
{
	CGuard guard(m_Mutex);

	m_nMaxMsg = size;
}
