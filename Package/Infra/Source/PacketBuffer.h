#ifndef __INTER_INFRA_PACKETBUFFER_H__
#define __INTER_INFRA_PACKETBUFFER_H__

#include "Infra/List.h"
#include "Infra/Mutex.h"
#include "Infra/Types.h"
#include <map>


/// Packet数据缓冲管理，通道数不限制，线程安全
/// 
class CPacketBuffer : public IPacketBuffer
{
public:
	typedef struct bufferBlock
	{
		CPacket* 			pPacket;		///< the addr of packet
		int 				iChannel;		///< 通道号	
		struct 	list_head 	timeList;		///< 按时间存放的队列顺序
		struct	list_head 	channelList;	///< 按通道存放的队列顺序
	} Block;

	CPacketBuffer(int kiloSize);

	~CPacketBuffer();
	
	/// 存放数据
	/// \param [in] pPkt 要存入的缓冲数据
	/// \param [in] iChannel 数据所在通道号
	/// \param [in] autoDel 缓冲满时是否自动覆盖
	/// \return 存入数据是否成功
	///	\note 缓冲区满返回失败, 当压入成功时该接口会自动增加引用计数
	bool pushBuffer(int iChannel, CPacket *pPkt, bool autoDel = false);
	
	/// 存放数据，和popBuffer正相反
	/// \param [in] pPkt 要存入的缓冲数据
	/// \param [in] iChannel 数据所在通道号
	/// \param [in] autoDel 缓冲满时是否自动覆盖
	/// \return 存入数据是否成功
	///	\note 缓冲区满返回失败, 当压入成功时该接口会自动增加引用计数
	bool pushHeadBuffer(int iChannel, CPacket *pPkt);

	/// 获得数据
	/// \param [out] pPkt 存放获取的缓冲地址
	/// \param [in] iChannel 要取数据的通道
	/// \return 获取数据是否成功
	bool popBuffer(int iChannel, CPacket **pPkt);	

	///得到队列中pkt的个数
	int getBufferCount(int iChannel);
	
	/// 清除数据
	void clear(int iChannel);
	
private:
	void delList(struct list_head *pFrom, struct list_head *pTo);
	void listInit(int iChannel);

private:
	Block*		m_pBuffer;     					///< 整个缓冲区
	uint		m_iSize;						///< 缓冲区的长度
	uint		m_iLength;						///< 使用的空间大小，以字节为单位
	struct	list_head	m_dataList;				///< 录像队列
	struct	list_head	m_emptyList;			///< 空队列
	std::map<int, struct list_head> m_channelList;
	CMutex				m_bufferMutex;
	std::map<int, int> m_listCount;
};

#endif
