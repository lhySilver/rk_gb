#ifndef TPUDPSERVER
#define TPUDPSERVER

#include "ITPObject.h"

class TPUDPServer : public ITPObject
{
public:
    TPUDPServer(TPProc connProc = 0, TPProc sendProc = 0, TPProc recvProc = 0, TPProc closeProc = 0, int engineId = 0);
	virtual ~TPUDPServer();

    //<Methods>
public:        
	virtual int Listen(char *pIp, int iPort);
	virtual int Send(const char *pBuf, int iBufLen, int iID = 0);
	virtual int sendDirect(const char *pBuf, int iBufLen, int iFlag, int iID = 0);
	virtual int recvDirect(char *pBuf, int iBufLen, struct timeval *pTimeVal = NULL, int iID = 0);
	virtual int Heartbeat();
};

#endif

