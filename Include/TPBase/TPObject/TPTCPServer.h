#ifndef TPTCPSERVER
#define TPTCPSERVER

#include "ITPObject.h"

class TPTCPServer : public ITPObject
{
public:
    TPTCPServer(TPProc connProc = 0, TPProc sendProc = 0, TPProc recvProc = 0, TPProc closeProc = 0, int engineId = 0);
    virtual ~TPTCPServer();

public:
	virtual int Listen(char *pIp, int iPort);
	virtual int Accept();
	virtual int Send(const char *pBuf, int iBufLen, int iID = 0);
	virtual int sendDirect(const char *pBuf, int iBufLen, int iFlag = 0, int iID = 0);
	virtual int recvDirect(char *pBuf, int iBufLen, struct timeval *pTimeVal = NULL, int iID = 0);
	virtual int Heartbeat();
	virtual void Close(void);
	virtual int CloseClient(int nID);

	void checkAlive();
	virtual void kickOutInvalidClients();

};

#endif

