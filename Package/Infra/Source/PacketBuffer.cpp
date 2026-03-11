#include "Infra/PacketBuffer.h"
#include "PacketBuffer.h"
#include "Infra/Time.h"
#include "Infra/Guard.h"
#include "Infra/Packet.h"
#include "Infra/List.h"
#include <assert.h>

CPacketBuffer::CPacketBuffer(int kiloSize) : m_pBuffer(NULL)
{
	CGuard guard(m_bufferMutex);

	m_iSize = kiloSize;

	trace("CPacketBuffer::CPacketBuffer size = %d KB\n", m_iSize);
	//转换成字节
	m_iSize = m_iSize << 10;

	//计算个数
	int num = 0;
	//包大小最小为1K
	num = m_iSize >> 10;

	assert(num != 0);

	m_pBuffer = new Block[num];
	assert(m_pBuffer);

	m_iLength = 0;

	//初始化链表
	INIT_LIST_HEAD(&m_dataList);
	INIT_LIST_HEAD(&m_emptyList);

	//组成空链表
	for (int i = 0; i < num; i++)
	{
		list_add(&m_pBuffer[i].timeList, &m_emptyList);
	}
}

CPacketBuffer::~CPacketBuffer()
{
	// 清空各个通道的缓冲数据
	for (std::map<int, struct list_head>::iterator pi = m_channelList.begin(); pi != m_channelList.end(); pi++)
	{
		clear((*pi).first);
	}
	m_iLength = 0;
	if (m_pBuffer)
	{
		delete [] m_pBuffer;
		m_pBuffer = NULL;
	}
}

bool CPacketBuffer::pushBuffer(int iChannel, CPacket *pPkt, bool autoDel)
{
	CGuard guard(m_bufferMutex);
	struct 	list_head *pList = NULL;

	listInit(iChannel);

	// 长度超出缓冲区总大小，返回失败
	if (m_iLength + pPkt->GetSize() > m_iSize)
	{
		if (!autoDel)
		{
			return false;
		}
		//判断长度
		while (m_iLength + pPkt->GetSize() > m_iSize)
		{
			//释放该通道数据，主要是预录使用
			pList = m_dataList.next;
			if (pList == &m_dataList)
			{
				// 出错了!!!
				return false;
			}
			Block* pBuffer = list_entry(pList, struct bufferBlock, timeList);
			m_iLength -= pBuffer->pPacket->GetSize();
			pBuffer->pPacket->Release();
			//从通道队列和预队队列删除
			list_del(&pBuffer->channelList);
			list_del(&pBuffer->timeList);
			//添加到空队列
			list_add(&pBuffer->timeList, &m_emptyList);		
		}
	}
	
	//从空队列里取出来,如果出错说明队列太小了
	pList = m_emptyList.next;
	assert (pList != &m_emptyList);
	//释放空队列
	list_del(pList);
	
	//映射对应结构的指针
	Block* pBuffer = list_entry(pList, struct bufferBlock, timeList);
	
	//取到的数据进行赋值并存放存相应的对列
	pBuffer->iChannel = iChannel;
	pBuffer->pPacket = pPkt;
	m_iLength += pPkt->GetSize();

	// 加到数据队列中
	list_add_tail(&pBuffer->timeList, &m_dataList);
	// 依次加到各个通道队列
	list_add_tail(&pBuffer->channelList, &m_channelList[iChannel]);
    m_listCount[iChannel]++;
	return true;
}

bool CPacketBuffer::pushHeadBuffer(int iChannel, CPacket *pPkt)
{
	CGuard guard(m_bufferMutex);
	struct 	list_head *pList = NULL;

	listInit(iChannel);
	//从空队列里取出来,如果出错说明队列太小了
	pList = m_emptyList.next;
	assert (pList != &m_emptyList);
	//释放空队列
	list_del(pList);

	//映射对应结构的指针
	Block* pBuffer = list_entry(pList, struct bufferBlock, timeList);

	//取到的数据进行赋值并存放存相应的对列
	pBuffer->iChannel = iChannel;
	pBuffer->pPacket = pPkt;
	m_iLength += pPkt->GetSize();

	// 加到数据队列中
	list_add(&pBuffer->timeList, &m_dataList);
	// 依次加到各个通道队列
	list_add(&pBuffer->channelList, &m_channelList[iChannel]);
    m_listCount[iChannel]++;
	return true;
}

bool CPacketBuffer::popBuffer(int iChannel, CPacket **pPkt)
{
	CGuard guard(m_bufferMutex);
	
	listInit(iChannel);
	if (list_empty(&m_channelList[iChannel]))
	{
		// 当前通道没有数据
		return false;
	}

	struct list_head *pList = m_channelList[iChannel].next;
	Block* pBuffer = list_entry(pList, struct bufferBlock, channelList);
	
	*pPkt = pBuffer->pPacket;
	
	m_iLength -= (*pPkt)->GetSize();
	
	//释放队列
	list_del(&pBuffer->channelList);
	list_del(&pBuffer->timeList);
	//添加到空队列
	list_add(&pBuffer->timeList, &m_emptyList);	
    m_listCount[iChannel]--;
	return true;
}

///
///	\b Description		:	清除通道数据\n
/// \param iChannel		:	通道号
/// \param 
///
///	\b Revisions		:	
///
void CPacketBuffer::clear(int iChannel)
{
	CGuard guard(m_bufferMutex);

	listInit(iChannel);
	delList(&m_channelList[iChannel], &m_channelList[iChannel]);
    m_listCount[iChannel] = 0;
}

///
///	\b Description		:	删除pFrom到pTo之间的通道列表,注意pFrom和pTo不会被删除\n
/// \param pFrom		:	开始队列指针
/// \param pTo			:	结束队列指针
///	\return 
///
///	\b Revisions		:	
///
void CPacketBuffer::delList(struct list_head* pFrom, struct list_head* pTo)
{
	struct list_head *pList = NULL;
	Block* pBuffer = NULL;
	
	for (pList = pFrom->next; pList != pTo; )
	{
		//释放队列
		pBuffer = list_entry(pList, struct bufferBlock, channelList);

		m_iLength -= pBuffer->pPacket->GetSize();
		pBuffer->pPacket->Release();
		pList = pList->next;
		list_del(&pBuffer->channelList);
		list_del(&pBuffer->timeList);
		//添加到空队列
		list_add(&pBuffer->timeList, &m_emptyList);
	}
}

void CPacketBuffer::listInit(int iChannel)
{
	if (m_channelList.find(iChannel) == m_channelList.end())
	{
		INIT_LIST_HEAD(&m_channelList[iChannel]);
	}

    if(m_listCount.find(iChannel) == m_listCount.end())
    {
        m_listCount[iChannel] = 0;
    }
}

int CPacketBuffer::getBufferCount(int iChannel)
{
    CGuard guard(m_bufferMutex);
    listInit(iChannel);
    return m_listCount[iChannel];
}

/// 创建包缓冲管理对象
IPacketBuffer *IPacketBuffer::create(int kiloBytes)
{
	return new CPacketBuffer(kiloBytes);
}

/// 虚析构函数
IPacketBuffer::~IPacketBuffer()
{
}