#ifndef TPUDPCLIENT
#define TPUDPCLIENT

#include "ITPObject.h"

class TPUDPClient : public ITPObject
{
public:
    TPUDPClient(TPProc connProc = 0, TPProc sendProc = 0, TPProc recvProc = 0, TPProc closeProc = 0, int engineId = 0);
    virtual ~TPUDPClient();

public:
	virtual int Connect(char *pIp, int iPort);
	virtual int Connect(char *pLocalIp, int iLocalPort, char *pRemoteIp, int iRemotePort);
	virtual int Send(const char *pBuf, int iBufLen, int iID = 0);
	virtual int sendDirect(const char *pBuf, int iBufLen, int iFlag = 0, int iID = 0);
	virtual int recvDirect(char *pBuf, int iBufLen, struct timeval *pTimeVal = NULL, int iID = 0);
	virtual int Heartbeat();
};
#endif

