
#ifndef __IMPLEMENT_MEDIA_RECORDSTROAGE_H__
#define __IMPLEMENT_MEDIA_RECORDSTROAGE_H__

//#include "Storage/FSFile.h"
#include "Infra/Packet.h"

/// 信息内容
typedef struct tagInfoGroup
{
	bool bUsed;							/*!< 是否使用 FALSE:未使用　TRUE:使用*/
	int iStartPos;						/*!< 开始位置(绝对值) */
	int iLength;						/*!< 长度 */
}MEDIAINFORGROUP;

/// 录像数据头信息，占用64字节
typedef struct tagMEDIAINFORGROUP 
{
	uchar			videoHead[4];			
	int				iTotalLen;			/*!< 头所占的长度 */
	MEDIAINFORGROUP	FrameIndex;			/*!< I帧定位信息 */
	MEDIAINFORGROUP	MotionIndex;		/*!< 动检信息 */
	MEDIAINFORGROUP	CardIndex;			/*!< 卡号信息 */
	uchar			Reserved[20];		/*!< 保留 */
}MEDIAFILEHEAD;

/// 录像文件类型
enum RecordFileTypes
{
	recFileAudio = 0x01,
	recFileVideo = 0x02,
	recFileAV = recFileAudio | recFileVideo
};

class IStorageBase;
class ICardItem;

/// 创建本地存储类
IStorageBase *createLocalStorage(void *attri);

/// 创建远程存储类
IStorageBase *createRemoteStorage(void *attri);

/// 创建可移动存储类
IStorageBase *createRemovableStorage(void *attri);

/// \brief 媒体存储接口
/// 
class IStorageBase
{
public:
	IStorageBase(void* p);
	virtual ~IStorageBase();
	virtual bool createStorage() = 0;
	virtual bool createStorage(int type) = 0;
	virtual bool closeStorage() = 0;
	virtual bool writeStorage(void *pBuffer, uint dwCount) = 0;
	virtual bool writeStorage(void *pBuffer, uint dwCount,int type) = 0;
	virtual std::string getFilePath() = 0;

	
	//触发报警录像
	virtual void DoRecord() = 0;

	//消除报警录像
	virtual void ClearRecord() = 0;
};

class CLocalStorage : public IStorageBase
{
public:
	CLocalStorage(void* pAttr, int iChannel);
	virtual ~CLocalStorage();

	bool createStorage();
	bool createStorage(int type);
	bool closeStorage();
	bool writeStorage(void* pBuffer, uint dwCount);
	bool writeStorage(void* pBuffer, uint dwCount, int type);
	std::string getFilePath(){ return m_mediaFile.getFilePath(); };
	
	//触发报警录像
	void DoRecord();

	//消除报警录像
	void ClearRecord();

private:
	int					m_iChannel;
	STM_ATT*			m_pAttr;
	CFSFile				m_mediaFile;
	CPacket*			m_pHeadPacket;
	CMutex				m_StorageMutex;
	bool 				m_status;
};

bool checkDisk(int hddtype = DRIVER_READ_WRITE);

#endif
