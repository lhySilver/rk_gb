#ifndef _STORAGE_H_
#define _STORAGE_H_

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <list>

#include "Common.h"
#include "StorageManager.h"
#include "Storage_api.h"

#define ZTE_FILE_HEAD			0			// 文件头
#define ZTE_VIDEO_I_FRAME		1			// 视频I帧
#define ZTE_VIDEO_B_FRAME		2			// 视频B帧
#define ZTE_VIDEO_P_FRAME		3			// 视频P帧
#define ZTE_VIDEO_BP_FRAME		4			// 视频BP帧
#define ZTE_VIDEO_BBP_FRAME	5			// 视频B帧B帧P帧
#define ZTE_AUDIO_PACKET		10			// 音频包

#define STREAM_ERROR	(-1)
#define STREAM_OK		(0)

//#if defined(_MSC_VER)
//	typedef signed __int64         	Int64;
//	typedef unsigned __int64       UInt64;
	
//#elif defined(__GNUC__)

	typedef signed long long   Int64;
	typedef unsigned long long UInt64;
//#endif


typedef struct ZTE_PACKET_INFO
{ 
	int		nPacketType;			// 包类型
							// 0-文件头
							// 1-I帧
							// 2-B帧
							// 3-P帧
							// 4-BP帧(2帧)
							// 5-BBP帧(3帧)
							// 10-音频包
	char*	pPacketBuffer;				// 缓存区地址
	int	dwPacketSize;				// 包的大小
	int dwTotalPos;					//总偏移位置
	
	// 绝对时标
	int		nYear;						// 时标:年		
	int		nMonth;						// 时标:月
	int		nDay;						// 时标:日
	int		nHour;						// 时标:时
	int		nMinute;					// 时标:分
	int		nSecond;					// 时标:秒
	// 相对时标
	long long int 		dwTimeStamp;					// 相对时标
	//视频相关内容
	int model;		//制式 ：0-PAL    1-NTSC
	int iResolution;//	分辨率：1：DCIF （）   2：CIF	（）	3.QCIF（）	4.2CIF（）	5:4CIF（）
	//音频相关内容
	int samplerate;//0表示16k 1表示8k 
	int samplebits;//0表示16位 1表示8位
	int channels;//0-单通道 1-双通道
	int audioType;//0-aac 1-g.729 2-g.726 3-speex 4-G711A 5-G711U	
}ZTE_PACKET_INFO;




//#define MAX_BUFLEN (1024*1024) //added by tsx 为解决1080p的帧数据过大问题 将大小改成1M
#define MAX_BUFLEN (512*1024) //added by ale

typedef struct _MixVideoDataHeader
{
	unsigned int header1;
	unsigned int header2;
	unsigned long long  timestamp;
	unsigned int len;
	unsigned int type;
	
}MixVideoDataHeader;


typedef void (*StoragePlayProc)(void *data, int size, int type, void*parm);//回调数据，大小，用户传入的自定义参数
typedef void (*StorageSearchProc)(void *data, int size, int type, void*parm);//回调数据，大小，用户传入的自定义参数

//class CStorage :public CThread
class CStorage
{
public:
    enum LOG_MODULE
    {
        SDCARD_MODULE  = 0 ,
        UPDATE_MODULE,
        VIDEO_MODULE,
        IO_MODULE,
        NET_MODULE,
        AI_MODULE,
        SYS_MODULE,
    };

    enum SYS_EVENT
    {
        SYS_RESET_CONFIG = 0,
        SYS_POWER_UP,
        SYS_REBOOT,
        SYS_NTP,
        SYS_USER_LOGIN,
    };

    enum AI_EVENT
    {
        AI_MD = 0,
        AI_MT,
    };
    
public:
	CStorage();
	~CStorage();
	static CStorage* instance();

	int Init();
	int UnInit();
	int DiskFmt(bool bSync = true);
	int GetFormatProgress();
	int GetDiskState();
#ifdef RC0240_LGV10
	void SetReadWriteErrorFlag(bool bError);
#endif
	int GetDiskcapacity(unsigned long long *pu64Total, unsigned long long *pu64Used, unsigned long long *pu64Free);
#ifdef RC0240_LGV10
	int SaveSystemTime(bool bForce = false);
#endif	
	int GetDiskInfo(/*T_DiskInfo &tDiskInfo*/);

	int SearchFiles(unsigned int recType, unsigned int fileType, unsigned int sTime, unsigned int eTime, StoragePlayProc proc,void*parm);
	int SearchFiles(unsigned int sTime, unsigned int eTime, StoragePlayProc proc,void*parm);
//	int SearchFiles(unsigned int sTime, unsigned int eTime, RECFILESLIST &ListSearchResult);
	/*
	 *搜索时间段内的录像文件
	 *@return 满足条件的录像文件个数
	 */
	int SearchFiles(int iStartTime_s, int iEndTime_s);
	
	
	int StartRec(int type);
	int StopRec();
	int SetRecMode(int mode);
	int WriteFrameData(unsigned char *pData, UInt32 uiSize, void *param=0);
#ifdef RC0240_LGV10
	int ClearFrameBuffer();
#endif
	//触发报警录像
	void DoAlarmRecord();
	//消除报警录像
	void ClearAlarmRecord();
	
	int Pause();
	int PausePlay(int iPlayHandle, bool bPause);
	int SetPlaybackSpeed(int iPlayHandle, float fSpeed);
	int DeleteRecordByDay(Int32 year, Int32 month, Int32 day);
	int ReStart();

    //log
    int WriteLog(T_LogRecInfo* pRecInfo,const char*msg,int len,unsigned int timestamp);
    int StartSearchLog(unsigned int uiStartTime, unsigned int uiEndTime, unsigned int uiType);
    int ReadLog(unsigned int iNum, T_Log* pLog);
    int StopLogSearch();


	
	int SearchRecordOnTime(int iStartTime, int iEndTime, TRecordFileTimeList &listRecordTimeInfo);
#ifdef RC0240_LGV10
	int SearchRecordByDay(int year, int month, int day, TRecordFileTimeList &listRecordTimeInfo, TRecordFileTimeList &listAlarmRecordTimeInfo);
#else
	int SearchRecordByDay(int year, int month, int day, TRecordFileTimeList &listRecordTimeInfo);
#endif
	int CheckRecordOnMonth(Int32 iYear, Int32 iMonth);
	int StartPlaybackOnTime(Int32 iStartTime, Int32 iEndTime, StoragePlaybackDownloadProc proc, void *param);
	int StopPlaybackOnTime(Int32 iPlayHandle);
	int SeekTimeOnTime(Int32 iPlayHandle, Int32 iStartTime);

	int StartDownload(Int32 iStartTime, Int32 iEndTime, StoragePlaybackDownloadProc proc, void *param);
	int StopDownload(Int32 iDownloadHandle);
	int PauseDownload(int iDownloadHandle, bool bPause);
	
	//void ThreadProc();
	
	bool m_fineI;
	void* m_parm;
	StoragePlayProc m_pPlayProc;
	StorageSearchProc m_pSearchProc;
private:
	//TSignal4<void *data, int size, int type, void*parm> m_sigBuffer;

	CMutex m_Mutex;
	unsigned int m_uiHandle;
	static CStorage* _instance;
};

#define g_StorageManager (CStorage::instance())

#endif
