#ifndef _ITPOBJECT_H_
#define _ITPOBJECT_H_

#include "TPTypedef.h"
#include "Infra/Signals.h"
#include "Types/Types.h"
#include "../TPSocket/TPSocket.h"
#include <stdio.h>
#include <string>

#define BUF_SIZE 64*1024

#ifdef WIN32		/* Windows specific #includes and #defines */
#define PF_INET         AF_INET
#define	ERRNO			WSAGetLastError()
#define	NO_SOCKLEN_T
#define MSG_NOSIGNAL  0
#define MSG_MORE      0

#define	O_NONBLOCK		0
#define	waitpid(a,b,c)		0
#define	EWOULDBLOCK		WSAEWOULDBLOCK
#define	_POSIX_

#elif defined(LINUX)		/* UNIX specific #includes and #defines */
#define	O_BINARY			0
#define	ERRNO				errno
#endif	/* _WIN32 */

typedef TSignal4<int, int,  char *, int>::SigProc TPProc;

class ITPObject
{
public:
	// sendProc和recvProc分别为发送和接受的回调函数
	ITPObject(TPProc connProc = 0, TPProc sendProc = 0, TPProc recvProc = 0, TPProc closeProc = 0, int engineId = 0);
    virtual ~ITPObject();

	virtual int Listen(char *pIp, int iPort);
	virtual int Accept();
	virtual int Connect(char*pIp, int iPort);
	virtual int Connect(char *pLocalIp, int iLocalPort, char *pRemoteIp, int iRemotePort);
	virtual void Close(void);

	/// 连接已经建立,直接设置sock
	virtual int setSock(int iSocket);

	// 以下为间接发送,发送的数据会被缓存,
	// 实际的发送和接受在Heartbeat中处理,
	// 然后通过sendProc,recvProc进行回调
	virtual int Send(const char *pBuf, int iBufLen, int iID = 0);
	virtual int Heartbeat();

	// 直接发送和接受
	virtual int sendDirect(const char *pBuf, int iBufLen, int iFlag = 0, int iID = 0);
	virtual int recvDirect(char *pBuf, int iBufLen, struct timeval *pTimeVal = NULL, int iID = 0);

	virtual int getSock(void){ return m_iSocket; };
	virtual int getSequence(void);

	virtual int CloseClient(int nID);
protected:
	// IP,PORT 都是网络字节序
	unsigned int m_iLocalIp;
	unsigned short m_iLocalPort;
	unsigned int m_iRemoteIp;
	unsigned short m_iRemotePort;
	char	*m_pBuf;
	int		m_iSocket;
	TPProc	m_sendCallBack;
	TPProc	m_recvCallBack;
	TPProc	m_connCallBack;
	TPProc  m_closeCallBack;
	CMutex  m_Mutex;
	int     m_nEngineId;

	//异步发送的数据队列
	std::vector<DataRow*> m_vectDataRow;
	//客户端连接
	std::vector<CLIENT_LIST*> m_vectClients;

	unsigned int m_uiSeq;
};
#endif

