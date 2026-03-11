#ifndef NET_PROTOCOL_PROC_DEVICE_HEAD
#define NET_PROTOCOL_PROC_DEVICE_HEAD

#include "Lock.h"
#include "NetProtocolProc.h"

class CNetProtocolProcDevice:public CNetProtocolProc
{
public:
	CNetProtocolProcDevice(void);
	virtual ~CNetProtocolProcDevice(void);

	virtual void ProcOneCommand(CCmdProcObject *pCmdProcObject,BYTE *pBuf,tuint32 dwLen)=0;
	virtual bool ProcDevCmd();
	virtual bool ProcNvmsCmd(){return false;};
	virtual void AddOneCmd(CCmdProcObject *pCmdProcObject,BYTE *pBuf,tuint32 dwLen);
protected:
	
private:

};

//////////////////////////////////////////////////////////////////////////


#endif 
