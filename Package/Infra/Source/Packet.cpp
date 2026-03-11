#include <stdio.h>
#include <time.h>
#include <string.h>
#include "Infra/Packet.h"
#include "Infra/Time.h"
#include "Packet.h"

//////////////////////////////////////////////////////////////////////////
PATTERN_SINGLETON_IMPLEMENT(CPacketManager);

CPacketManager::CPacketManager()
{
	CPacketManagerImp::instance();
}

CPacketManager::~CPacketManager()
{
}

void CPacketManager::config(uchar *pbuf, int packetKiloBytes)
{
	CPacketManagerImp::config(pbuf, packetKiloBytes);
}

CPacket* CPacketManager::GetPacket(uint dwBytes)
{
	return CPacketManagerImp::instance()->GetPacket(dwBytes);
}

void CPacketManager::Dump()
{
	CPacketManagerImp::instance()->Dump();
}

void CPacketManager::DumpNodes()
{
	CPacketManagerImp::instance()->DumpNodes();
}

CPacket::CPacket()
{
}

CPacket::~CPacket()
{
}
//////////////////////////////////////////////////////////////////////////

#define PKT_KILOS_DEFAULT (4096)
#define PKT_PAGE_SIZE (1024)

static int log2i(uint x)
{
	int r = 31;

	if (!x)
		return 0;
	if (!(x & 0xffff0000u)) {
		x <<= 16;
		r -= 16;
	}
	if (!(x & 0xff000000u)) {
		x <<= 8;
		r -= 8;
	}
	if (!(x & 0xf0000000u)) {
		x <<= 4;
		r -= 4;
	}
	if (!(x & 0xc0000000u)) {
		x <<= 2;
		r -= 2;
	}
	if (!(x & 0x80000000u)) {
		x <<= 1;
		r -= 1;
	}
	return r;
}

CPacketImp::CPacketImp()
{
}

CPacketImp::~CPacketImp()
{
}

void CPacketImp::Init()
{
	m_RefCount = 1;
	m_Length = 0;
	m_Size = 0;
	m_pBuffer = NULL;
}

uint CPacketImp::PutBuffer(const void * pdat, uint dwLength)
{
	uint left;

	CGuard guard(m_Mutex);

	left = m_Size - m_Length;
	if (left < dwLength) 
    {       //包容量不足
		dwLength = left;
	}
	
	// 从后面增加，可以连续使用。
	memcpy(m_pBuffer + m_Length, pdat, dwLength);

	m_Length += dwLength;

	return dwLength;
}

uchar * CPacketImp::GetBuffer()
{
	return m_pBuffer;
}

uint CPacketImp::SetLength(uint dwLength)
{
	assert(dwLength <= m_Size);
	m_Length = dwLength;
	return m_Length;
}

uint CPacketImp::GetLength()
{
	return m_Length;
}

uint CPacketImp::GetSize()
{
	return m_Size;
}

uint CPacketImp::GetLeft()
{
	CGuard guard(m_Mutex);
	return (m_Size - m_Length);
}

uchar* CPacketImp::GetHeader()
{
	return m_Header;
}

void CPacketImp::ClearHeader()
{
	memset(m_Header, 0, PKT_HDR_SIZE);
}

uint CPacketImp::Release()
{
	m_Mutex.Enter();

	assert(m_RefCount > 0);

	if(m_RefCount == 0)
	{
		trace("CPacketImp::Release() reffrence count error\n");
		m_Mutex.Leave();
		return 0;
	}

	--m_RefCount;

	uint count = m_RefCount;
	m_Mutex.Leave();//必须在回收包之前解锁，否则引起严重错误

	if (count == 0) 
	{
		CPacketManagerImp::instance()->PutPacket(this);
	}
	return count;
}

uint CPacketImp::AddRef()
{
	CGuard guard(m_Mutex);

	m_RefCount++;
	return m_RefCount;
}

uint CPacketImp::GetRef()
{
	CGuard guard(m_Mutex);
	return m_RefCount;
}

void CPacketImp::GetSysTime(SystemTime *time)
{
	CGuard guard(m_Mutex);
	if(NULL == time)
	{
		return;
	}
	memcpy(time,&m_SysTime,sizeof(SystemTime));
}

void CPacketImp::SetSysTime(SystemTime *time)
{
	CGuard guard(m_Mutex);
	if(NULL == time)
	{
		return;
	}
	memcpy(&m_SysTime,time,sizeof(SystemTime));
//	trace("%d-%d-%d %d:%d:%d\n",m_SysTime.year,m_SysTime.month,m_SysTime.day,m_SysTime.hour,m_SysTime.minute,m_SysTime.second);
}

////////////////////////////////////////////////////////////////////////////////////
// CPacketManagerImp
////////////////////////////////////////////////////////////////////////////////////

int CPacketManagerImp::sm_packetBufKilo = PKT_KILOS_DEFAULT;
uchar *CPacketManagerImp::sm_packetBuf = NULL;

PATTERN_SINGLETON_IMPLEMENT(CPacketManagerImp);

void CPacketManagerImp::config(uchar *pbuf, int packetBufKilo)
{
	sm_packetBufKilo = packetBufKilo;
	sm_packetBuf = pbuf;
}

CPacketManagerImp::CPacketManagerImp()
{
	trace("CPacketManagerImp::CPacketManagerImp()>>>>>>>>>\n");
	int i;
	int size;
	PBN *pNode;

	m_pFreeList = NULL;
	m_nTypes = 0;
	
	m_nPages = sm_packetBufKilo * 1024 / PKT_PAGE_SIZE;
	if (sm_packetBuf)
	{
		trace("sm_packetBuf:%p, use external memory!\n", sm_packetBuf);
		m_pOriginBuffer = sm_packetBuf;
	}
	else
	{
		trace("sm_packetBuf null use self heap memory!\n");
		try
		{
			m_pOriginBuffer = new uchar[m_nPages * PKT_PAGE_SIZE];
		}
		catch (std::bad_alloc e)
		{
			trace("CPacketManagerImp::CPacketManagerImp() new buffer[%u] failed!!!!\n", m_nPages);
			return;
		}
	}
	m_pBuffer = (uchar *)(((unsigned long)m_pOriginBuffer + PKT_PAGE_SIZE) & ~(PKT_PAGE_SIZE - 1));//按页面大小对齐
	if (m_pBuffer != m_pOriginBuffer)
	{
		trace("Adjust Page!\n");
		m_nPages--;
	}
	for(m_nTypes = 0, size = m_nPages; ; m_nTypes++)
	{
		if(m_nPages < (1 << m_nTypes))
		{
			break;
		}

		// 取偶数，为了节点合并方便
		size = ((size + 1 ) & 0xfffffffe);

		m_PBAs[m_nTypes].pArray = new PBN[size];
		if(!m_PBAs[m_nTypes].pArray)
		{
			trace("CPacketManagerImp::CPacketManagerImp() new nodes failed!!!!\n");
			return;
		}

		// 所有节点标志清空
		for(i = 0; i < size; i++)
		{
			m_PBAs[m_nTypes].pArray[i].pNext = NULL;
			m_PBAs[m_nTypes].pArray[i].nIndex = i;
		}

		// 出始化空闲节点链表
		if (m_nPages & BITMSK(m_nTypes))
		{
			pNode = &m_PBAs[m_nTypes].pArray[m_nPages / (1 << m_nTypes) - 1];
			m_PBAs[m_nTypes].pHeader = pNode;
			pNode->pNext = pNode;
			pNode->pPrev = pNode;
		}
		else
		{
			m_PBAs[m_nTypes].pHeader = NULL;
		}
		m_PBAs[m_nTypes].nCount = size;
		size /= 2;
	}

    CPacketImp *q = new CPacketImp[m_nPages/16];
    CPacketImp *p = m_pFreeList;
    if(q)
    {    
        /* 链接自由对象 */
        for(p = m_pFreeList = &q[m_nPages/16 -1]; q<p; p--) 
        {
            p->m_pNext = p - 1;
        }
        
        (p + 1)->m_pNext = NULL; // 最后一个自由对象的后续为空
    }
	Dump();
}

CPacketManagerImp::~CPacketManagerImp()
{
	if(m_pOriginBuffer)
	{
		delete []m_pOriginBuffer;
	}

	for(; m_nTypes; m_nTypes--)
	{
		delete []m_PBAs[m_nTypes-1].pArray;
	}
}

CPacketImp* CPacketManagerImp::AllocPacket()    // 从空闲链表中取一个结点出来
{
	CPacketImp * p = m_pFreeList;
	
	if(p) 
	{
		m_pFreeList = p->m_pNext;
	}
	else 
	{
		CPacketImp *q = new CPacketImp[NALL];
		
		if (q == NULL)
		{
			trace("AllocPacket new fail\n");
			return NULL;
		}
		//fprintf(stderr, "==>CPacketManagerImp: allocate buffer:0x%08X size:%d\n", (uint)q, NALL * sizeof(CPacketImp));
        if(!q)
        {
            fprintf(stderr, "============error\n");
            return NULL;
        }
        /* 链接自由对象 */
		for(p = m_pFreeList = &q[NALL -1]; q<p; p--) 
		{
			p->m_pNext = p - 1;
		}
		
		(p + 1)->m_pNext = NULL; // 最后一个自由对象的后续为空
	}
	
	return p;
}

void CPacketManagerImp::FreePacket(CPacketImp* p)   // 回收p结点到空闲链表中并作为头结点
{
	assert(p);

	p->m_pNext = m_pFreeList;
	m_pFreeList = p;
}

// 从空闲链表中取一个空闲结点并对其作适当的初始化后返回
CPacketImp * CPacketManagerImp::GetPacket(uint dwBytes /* = 0 */)
{
	CPacketImp* pPacket;
	int index = 0;
	int type;
	int pages;              // 页面数
	int i;

	CGuard guard(m_Mutex);

	pPacket = AllocPacket();
	
	if(!pPacket)
	{
		trace("CPacketManagerImp::GetPacket packet error!!!!!!\n");
		return NULL;
	}

	pPacket->Init();

	// 没有缓冲，只有包头的包
	if(dwBytes == 0)
	{
		pPacket->m_Size = 0;
		pPacket->m_pBuffer = NULL;
		return pPacket;
	}

	// 根据缓冲大小计算出节点级别
	pages = (dwBytes + PKT_PAGE_SIZE - 1) / PKT_PAGE_SIZE;
	type = log2i(pages);
	if((1u << type) != (uint)pages)
	{
		type++;
	}
	pages = 1 << type;//强制页面数为2幂次，也就是屏蔽包申请时大包小包互补策略。 

	// 查找最小的空闲节点
	for(i = type; i < m_nTypes; i++)
	{
		if(m_PBAs[i].pHeader)   // 找到
		{
			break;
		}
	}
	if(i >= m_nTypes)           // 未找到最小的空闲结点
	{
	    static uint cnt = 0;
        if(cnt++ % 100 == 0) 
        {
            trace("CPacketManagerImp::GetPacket none free node!!!!!! %d\n", cnt);
        }
		
		FreePacket(pPacket);
		return NULL;
	}

	// 取出最小的空闲节点
	index = m_PBAs[i].pHeader->nIndex;
	RemoveFree(m_PBAs[i].pHeader, &m_PBAs[i].pArray[index]);

	// 填充包数据成员
	pPacket->m_Size = pages * PKT_PAGE_SIZE;
	pPacket->m_pBuffer = m_pBuffer + (index << i) * PKT_PAGE_SIZE;

	//切分大规格的节点
	for(i--; i >= type; i--)
	{
		index *= 2;
		InsertFree(m_PBAs[i].pHeader, &m_PBAs[i].pArray[index + 1]);
	}

	// 分析出多余的空间包含的更小规格空闲节点，比如申请11个页面时， 实际取到的是16个页面， 还多出5 = 4 + 1个页面
	if((uint)pages != (1u << type))
	{
		for(; i >= 0; i--)
		{
			index = index * 2;
			if(!(pages & BITMSK(i)))
			{
				InsertFree(m_PBAs[i].pHeader, &m_PBAs[i].pArray[index + 1]);
			}
			else
			{
				index += 1;
				pages -= (1u << i);
				if(!pages)
				{
					InsertFree(m_PBAs[i].pHeader, &m_PBAs[i].pArray[index]);
					break;
				}
			}
		}
	}

	return pPacket;
}

void CPacketManagerImp::PutPacket(CPacketImp *pPacket)
{
	CGuard guard(m_Mutex);
	int index;
	int theother;
	int type;
	int pages;
	int i;
	int merged = 0; // 0-没有合并 1-正在合并 2-合并完成

	if(!pPacket)
	{
		return;
	}
	if(!pPacket->m_Size || !pPacket->m_pBuffer)
	{
		FreePacket(pPacket);
		return;
	}

	// 取出包数据成员
	pages = pPacket->m_Size / PKT_PAGE_SIZE;
	index = (pPacket->m_pBuffer - m_pBuffer) / PKT_PAGE_SIZE;

	type = log2i(pages);
	if((1u << type) != (uint)pages)
	{
		type++;
		index += pages; // index为要释放的包内存块后紧跟的0级节点序号
	}
	else
	{
		i = type;
		index /= pages; // index为当前节点的序号
		goto post_merge;
	}

	// 分析出包缓冲包含的所有2幂次节点，如 11 = 8 + 2 + 1，从小到大一一进行合并
	for(i = 0; i < type; i++)
	{
		if(index & 0x1)
		{
			if(merged == 0)
			{
				if(m_PBAs[i].pArray[index].pNext)
				{
					merged = 1;
					RemoveFree(m_PBAs[i].pHeader, &m_PBAs[i].pArray[index]);
				}
				else
				{
					merged = 2;
					InsertFree(m_PBAs[i].pHeader, &m_PBAs[i].pArray[index - 1]);
				}
			}
			else if(merged == 2)
			{
				InsertFree(m_PBAs[i].pHeader, &m_PBAs[i].pArray[index - 1]);
			}
		}
		else
		{
			if(merged == 1)
			{
				if(m_PBAs[i].pArray[index + 1].pNext)
				{
					RemoveFree(m_PBAs[i].pHeader, &m_PBAs[i].pArray[index + 1]);
				}
				else
				{
					merged = 2;
					InsertFree(m_PBAs[i].pHeader, &m_PBAs[i].pArray[index]);
				}
			}
		}
		index /= 2;
	}

	//将相邻小节点合并成大节点, 如1->2->4->8, 直到不能再合并为止
	if(merged == 1)
	{
post_merge:
		for(; i < m_nTypes - 1; i++)
		{
			theother = (index % 2) ? (index - 1) : (index + 1);
			if(m_PBAs[i].pArray[theother].pNext)
			{
				RemoveFree(m_PBAs[i].pHeader, &m_PBAs[i].pArray[theother]);
			}
			else
			{
				break;
			}
			index /= 2;
		}

		InsertFree(m_PBAs[i].pHeader, &m_PBAs[i].pArray[index]);
	}

	// 回收包壳对象
	FreePacket(pPacket);
}

// 从双向链表中删除指定的空闲节点pThis
inline void CPacketManagerImp::RemoveFree(PBN * & pHeader, PBN * pThis)
{
	if(pThis == pHeader)
	{
		pHeader = pThis->pNext;
		if(pThis == pHeader) // 只有一个节点
		{
			pThis->pNext = NULL;
			pHeader = NULL;
			return;
		}
	}

	pThis->pPrev->pNext = pThis->pNext;
	pThis->pNext->pPrev = pThis->pPrev;
	pThis->pNext = NULL;
}

// 将新的空闲节点加入链表
// pHeader指向双向链表(空闲结点缓冲)的头结点
inline void CPacketManagerImp::InsertFree(PBN * & pHeader, PBN * pThis)
{
	if(pHeader)                         // 双向链表头插法
	{
		pHeader->pPrev->pNext = pThis;
		pThis->pPrev = pHeader->pPrev;
		pHeader->pPrev = pThis;
		pThis->pNext = pHeader;
		pHeader = pThis;
	}
	else                                // 插入的结点作为链表中的第一个结点
	{
		pHeader = pThis;
		pThis->pPrev = pThis;
		pThis->pNext = pThis;
	}
}

//返回缓冲的大小,以KB为单位
uint CPacketManagerImp::GetBufferSize()
{
	CGuard guard(m_Mutex);

	return m_nPages * PKT_PAGE_SIZE / 1024;
}

void CPacketManagerImp::Dump()
{
	int type;
	CGuard guard(m_Mutex);
	PBN* current;
	int freepages = 0;
	
	//打印利用率
	for(type = 0; type < m_nTypes; type++)
	{
		if(m_PBAs[type].pHeader)
		{
			current = m_PBAs[type].pHeader;
			while (1)
			{
				freepages += 1<<type;
				if(current == m_PBAs[type].pHeader->pPrev)
				{
					break;
				}
				current = current->pNext;
			}
		}
	}
	trace("Packet usage : %dK / %dK, %d%%\n", m_nPages - freepages, m_nPages, 100-100*freepages/m_nPages);

	
	SystemTime stNow;
	CTime::getCurrentTime(stNow);
	trace("\n----------------------[%02d-%02d %02d:%02d:%02d]\n",
			stNow.month, stNow.day,
			stNow.hour, stNow.minute, stNow.second);
}
	
void CPacketManagerImp::DumpNodes()
{
	int type;
	CGuard guard(m_Mutex);
	PBN* current;
	//打印所有节点的状态
#if 0
	for(type = 0; type < m_nTypes; type++)
	{
		trace("%4d : ", (1u <<type));
		for(int i = 0; i < (1 << type) - 1; i++)
		{
			trace(" ");
		}
		for(i = 0; i < m_PBAs[type].nCount; i++)
		{
			trace("%x", m_PBAs[type].pArray[i].pNext ? 1 : 0);
			for(int j = 0; (j < (2 << type) - 1) && (i != m_PBAs[type].nCount - 1); j++)
			{
				trace(" ");
			}
		}
		trace("\n");
	}
#endif
	//打印空闲链表
	trace("______________________________\n");
	for(type = 0; type < m_nTypes; type++)
	{//顺序打印
		trace("%4d : ", (1u <<type));
		if(m_PBAs[type].pHeader)
		{
			current = m_PBAs[type].pHeader;
			while (1)
			{
				trace("%d ", current->nIndex);
				if(current == m_PBAs[type].pHeader->pPrev)
				{
					break;
				}
				current = current->pNext;
			}
		}
		trace("\n");
	}

	trace("\n");
}

void CPacketManagerImp::Test()
{
#ifdef _DEBUG_THIS
	int i;
	int count = 1024;
	unsigned long size = 0;
	unsigned long page = 0;
	unsigned long len;
	CPacketImp * pPacket[1024];
	SystemTime systime;

	CTime::getCurrentTime(systime);
	memset(pPacket, 0 , sizeof(CPacketImp *) * count);
	DumpNodes();
	srand(systime.second);
	for(i=0; i<count; i++)
	{
		len = ((rand() % 65536) + 1);
		//len = (1 << log2i(len));
		pPacket[i] = CPacketManagerImp::instance()->GetPacket(len);
		if(!pPacket[i]){
			break;
		}
		size += len;
		page += (len + PKT_PAGE_SIZE - 1) / PKT_PAGE_SIZE;
	}
	trace("CPacketManagerImp::Test() rand allocate usage: in byte %d%%, in page %d%%.\n", 100*size/PKT_PAGE_SIZE/m_nPages, 100*page/m_nPages);
	DumpNodes();
	for(i=0; i<count; i++)
	{
		if(pPacket[i]){
			pPacket[i]->Release();
		}
	}
	DumpNodes();
#endif
}
