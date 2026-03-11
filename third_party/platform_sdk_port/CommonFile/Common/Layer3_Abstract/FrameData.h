
#ifndef _FRAME_DATA_H_
#define _FRAME_DATA_H_

#include "Lock.h"
#include "NVMS_Define.h"
#include "NetProtocolDefine_ForNVMS.h"
#include "MemPoolSDK.h"
#include "Interlocked.h"

/************************************************************************
hasPackCMDHeader为TRUE时帧数据的结构
ECMS_PACKCMD
如果ECMS_PACKCMD里的byExtendInfo为非0值则包含ECMS_PACKCMD_EXTEND结构体，否则不包含此结构体
STREAM_HEADER_INFO
FRAME_HEADER_INFO
FRAME_EXTENSION_INFO
数据信息的具体内容

hasPackCMDHeader为FALSE时帧数据的结构
STREAM_HEADER_INFO
FRAME_HEADER_INFO
FRAME_EXTENSION_INFO
数据信息的具体内容
************************************************************************/
// class CFrameData
// {
// public:
// 	CFrameData(const void *pFrameData, int iBufLen, bool bIsAutoDestroy = false);
// 	virtual ~CFrameData(void);
// 
// 	///////////////计数加1
// 	int Add();
// 	///////////////计数减1，如果是自动删除的模式，为0的时候会销毁对象，非自动模式无效
// 	int Dec();	
// 
// 	///////////////获取缓冲区的长度
// 	int GetFrameDataLen() const;
// 	///////////////获取缓冲区
// 	const BYTE * GetFrameData() const;
// 
// 	///获取缓冲区的去掉头之后的帧长度
// 	int GetDataLen() const;
// 	///获取缓冲区的去掉头之后的帧数据
// 	const BYTE * GetData() const;
// 
// 	///获取缓冲区的扩展数据长度
// 	int GetExDataLen() const;
// 	///获取缓冲区的扩展数据
// 	const BYTE * GetExData() const;
// 
// 	///////////////判断此帧是否为关键帧，默认处理音频帧的方式为每一帧都是关键帧
// 	bool IsKeyframe();
// 
// 	///////////////获取帧的流类型
// 	int GetStreamType() const ;
// 
// 	void SetStreamType(BYTE streamType);
// 	///////////////获取帧的通道ID
// 	GUID GetChannelID();
// 
// 	/////////////////////////获取帧的流信息
// 	//BOOL GetStreamInfoOfFrame(STREAM_HEADER_INFO &FrameStreamInfo);
// 	NVMS_DEFINE::STREAM_HEADER_INFO & GetStreamInfoOfFrame() const;
// 	bool GetStreamInfoOfFrame(NVMS_DEFINE::STREAM_HEADER_INFO & StreamHeaderInfo) const;
// 
// 	///////////////获取帧信息
// 	NVMS_DEFINE::FRAME_HEADER_INFO & GetHeaderInfoOfFrame() const;
// 	bool GetHeaderInfoOfFrame(NVMS_DEFINE::FRAME_HEADER_INFO &FrameHeaderInfo) const;
// 
// 	//////////////设置发送索引的值    
// 	bool SetSendIndex(tuint32 dwSendIndex);
// 
// protected:		
// 	////////////////获取帧的扩充的信息
// /*	BOOL GetExInfoOfFrame(FRAME_EXTENSION_INFO &FrameExInfo);*/
// private:
// 
// 	//////////数据缓冲区
// 	BYTE *m_pFrameData;	
// 	
// 	//////////缓冲区的长度
// 	int   m_iLen;
// 
// 	////////数量计算
// 	int   m_Count;
// 
// 	///////////锁
// 	mutable CLock     m_Lock;
// 
// 	////////是否自动销毁传进来的内存及对象自身
// 	bool                 m_bAutoDestroy;
// };


/************************************************************************/
/*                                                                      */
/************************************************************************/
class CFrameDataEx
{
public:
	CFrameDataEx(const void *pFrameData, tuint32 iBufLen, bool bIsAutoDestroy = false);
	virtual ~CFrameDataEx(void);

	///////////////计数加1
	int Add();
	///////////////计数减1，如果是自动删除的模式，为0的时候会销毁对象，非自动模式无效
	int Dec();	

	///////////////获取缓冲区的长度
    tuint32 GetFrameDataLen();

	tuint32 GetRealFrameDataLen();	//获取缓冲区真实的长度,无论是内存池缓冲区还是内存都返回真实的数据长度

	///////////////获取缓冲区
	const BYTE * GetFrameData();

	///获取缓冲区去掉头之后的帧长度
	int GetDataLen();

	///获取缓冲区去掉头之后的帧数据
	const BYTE * GetData();

	///获取缓冲区扩展数据长度
	int GetExDataLen();

	///获取缓冲区扩展数据
	const BYTE * GetExData();

	///////////////判断此帧是否为关键帧，默认处理音频帧的方式为每一帧都是关键帧
	bool IsKeyframe();

	///////////////获取帧的流类型
	int GetStreamType();

	void SetStreamType(BYTE streamType);
	///////////////获取帧的通道ID
    const GUID &GetChannelID();

	/////////////////////////获取帧的流信息
	//BOOL GetStreamInfoOfFrame(STREAM_HEADER_INFO &FrameStreamInfo);
	NVMS_DEFINE::STREAM_HEADER_INFO & GetStreamInfoOfFrame();

	///////////////获取帧信息
	NVMS_DEFINE::FRAME_HEADER_INFO & GetHeaderInfoOfFrame();

    //////////////设置发送索引的值
	bool SetSendIndex(tuint32 dwSendIndex);
    tuint32 GetSendIndex();
protected:		
	////////////////获取帧的扩充的信息
	/*	BOOL GetExInfoOfFrame(FRAME_EXTENSION_INFO &FrameExInfo);*/
private:
	//////////数据缓冲区
	BYTE *m_pFrameData;	
    tuint_ptr   m_hMem;

	//////////缓冲区长度
	tuint32   m_iLen; 

    mutable  Interlocked m_InterLock;

	////////是否自动销毁传进来的内存及对象自身
	bool              m_bAutoDestroy;

	CChildPairContainer<BYTE *, tint32>  m_FrameContent; 	

    NVMS_DEFINE::STREAM_HEADER_INFO *m_pStreamHead;
    NVMS_DEFINE::FRAME_HEADER_INFO  *m_pFrameInfo;
    NVMS_DEFINE::FRAME_EXTENSION_INFO  *m_pExInfo;
};

#endif
