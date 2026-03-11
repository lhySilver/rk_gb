#ifndef __IMPLEMENT_PACKET_H__
#define __IMPLEMENT_PACKET_H__

#include "Infra/Mutex.h"
#include "Infra/Guard.h"
#include "Infra/Singleton.h"
#include "Infra/Types.h"
#include "Infra/Time.h"
#include <deque>
#include <assert.h>

class CPacketManagerImp;
////////////////////////////////////////////////////////////////////////////////////
// CPacketImp
////////////////////////////////////////////////////////////////////////////////////
class CPacketImp : public CPacket
{
public:
	uint	PutBuffer(const void * pdat, uint dwLength);
	uchar *	GetBuffer();
	uint   SetLength(uint dwLength);		//设置包长度	
	uint	GetLength();					//取数据长度
	uint	GetSize();						//获取容量
	uint	GetLeft();						//取剩余容量
	uchar *	GetHeader();					//清空包头数据
	void	ClearHeader();					//清空包头数据
	uint	Release();
	uint	AddRef();
	uint	GetRef();
	void	GetSysTime(SystemTime *time);
	void	SetSysTime(SystemTime *time);

private:
	uchar *	m_pBuffer;		//缓冲指针
	uint	m_Size;			//大小
	uint	m_Length;		//数据长度
	uint	m_RefCount;		//引用计数
	CMutex	m_Mutex;		//互斥量
	SystemTime  m_SysTime;  //保存包的当前时间
	uchar	m_Header[PKT_HDR_SIZE];     // 104 = 12(帧定位信息) * 8(个) + 8(1+7)
	CPacketImp *m_pNext;

private:
	CPacketImp();
	~CPacketImp();
	void Init();

	friend class CPacketManagerImp;
};

class CPacketManagerImp {
	
	typedef struct _PBN{//packet buffer node
		_PBN *	pPrev;
		_PBN *	pNext;
		int		nIndex;
	}PBN;

	typedef struct _PBA{//packet buffer array
		PBN *	pArray;
		PBN *	pHeader;
		uint	nCount;
	}PBA;

	PATTERN_SINGLETON_DECLARE(CPacketManagerImp);

public:
	static void config(uchar *pbuf, int packetBufKilo);

	CPacketImp*	GetPacket(uint dwBytes = 0);
	void		PutPacket(CPacketImp *pPacket);
	uint		GetBufferSize();
	void		Dump();
	void		DumpNodes();
	void		Test();

private:
	CPacketImp*	AllocPacket();
	void		FreePacket(CPacketImp *p);
	void		InsertFree(PBN * & pHeader, PBN * pThis);
	void		RemoveFree(PBN * & pHeader, PBN * pThis);
	
private:
	uchar *		m_pOriginBuffer;
	uchar *		m_pBuffer;
	CMutex		m_Mutex;
	PBA 		m_PBAs[32];         //管理每种缓冲的节点数组
	int			m_nTypes;
	int			m_nPages;
	enum		{ NALL = 1024 };
	CPacketImp*	m_pFreeList;        // 指向空闲链表的头结点
	static int  sm_packetBufKilo;
	static uchar *sm_packetBuf;
};

#endif
