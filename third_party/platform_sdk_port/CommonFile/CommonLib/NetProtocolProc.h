#ifndef __NETPROTOCOLPROC_H__
#define __NETPROTOCOLPROC_H__

#include "base_type_define.h"
#include "NetCommSDK.h"
#include "Lock.h"
#include <list>
extern tuint32 g_BeatDataHaveSendLen; ////GetBeatData得到len的长度为g_BeatDataHaveSendLen，表示底层已发送过心跳
class CNetNode;
class CCmdProcObject;
//网络协议处理 
class _EXPORT_NETCOMM_CLASS CNetProtocolProc
{
public:
    virtual bool Initial(CNetNode *pNetNode,tint32   hNetCommunication,int recvBuffCount, int SendBuffCount)=0;
    virtual void Quit()=0;
    virtual int Start(char *pSysHead,int sysHeadLen)=0;
    virtual void Stop()=0;

    virtual bool ProcDevCmd()=0;//此命令只有设备需要处理//实际只有编码设备和解码设备，其它并没有按这个套路实现
	virtual bool ProcNvmsCmd()=0;//NVMS命令处理

	virtual CNetProtocolProc* CreateSelf()=0;

	virtual char* GetBeatData(size_t& len)=0;//得到能够直接发送的心跳数据包

    virtual bool  IsOnline()=0;//检查设备是否在线,只有第三方设备需要实现此函数

public:
	CNetProtocolProc(void);

	virtual ~CNetProtocolProc(void);

protected:
	//////////////////////////////////////////
	CNetNode *m_pCurNetNode;
	//命令队列
	struct CMD_PROC_INFO
	{
		CCmdProcObject *pCmdProcObject;//命令处理对象
		BYTE *pBuf;//命令数据
		tuint32 dwLen;//命令数据长度
	};
	std::list<CMD_PROC_INFO> *m_pCmdProcList;
	CLock					 m_CmdProcListLock;
};

//////////////////////////////////////////////////////////////////////////

#endif 
