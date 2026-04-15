#ifndef __STORAGE_MANAGER_H__
#define __STORAGE_MANAGER_H__


#include "mp4_muxing.h"
#include "Mp4_Demuxer.h"
#include "RingBuffer.h"


#include "StorageCommon.h"

#include "Infra/Thread.h"


#define RECORD_FILE_DURATION_SECONDS 	(180) 		//180 seconds
#define RECORD_RESERVE_MEMORY_SIZE_BYTES 	(104857600) 	//100MB

#define ALARM_PRERECORD_TIME_MS 		(3000) 	//报警预录时长, 单位毫秒
#define ALARM_RECORD_MAX_NUM_PER_DAY 	(1000) 	//一天的报警录像个数上限, 超过此值当天就不再进行报警录像

#define MAX_PLAYBACK_NUM 		(1) 		//最大回放用户数

#define STORAGE_MAX_FRAME_SIZE 			(512*1024) 		//单帧最大字节数 	262144 = 2 * 1024 *1024 / 8

#define STORAGE_MIN_PB_SPEED 			(0.25) 		//最小回放倍速
#define STORAGE_MAX_PB_SPEED 			(4) 		//最大回放倍速
#define STORAGE_DEF_PB_SPEED 			(1.0) 		//默认回放倍速

#define STORAGE_MAX_FRAME_TIME_DIFF 	(5000) 		//两帧间的最大时间间隔，单位毫秒

#define STORAGE_MAX_FILE_PER_DAY 		(1440*2) 	//一天最多的录像文件个数

typedef enum
{
	DISK_RECORD_MODE_CLOSE = 0,
	DISK_RECORD_MODE_ALARM,
	DISK_RECORD_MODE_FULLTIME,
} DISK_RECORD_MODE_E;


typedef struct{
	int iStartTime;
	int iEndTime;
} record_file_time_s;

typedef std::list<record_file_time_s> TRecordFileTimeList;
typedef std::map<int, TRecordFileTimeList> TRecordFileTimeMap;


typedef struct{
	int iStartTime;
	int iEndTime;
	int iRecType; 				///< 0-全天录像  1-报警录像
} record_file_info_s;


typedef struct{
	void *p;
	int index;
} playback_thread_param_s;


typedef struct{
	void *p;
	int iRecordMode;
	int date_index;
	unsigned long long ullStartTimestamp;
	char filePath[128];
	char fileName[128];
} RecoverThradParam_S;
typedef std::list<RecoverThradParam_S *> TRecoverThradParamList;



/*
 *@回放/下载结束的时候，会再调用一次回调函数, 传入参数 : data=NULL, size=0, pStMp4DemuxerFrameInfo=NULL
 */
typedef void (*StoragePlaybackDownloadProc)(UInt8 *data, Int32 size, Mp4DemuxerFrameInfo_s *pStMp4DemuxerFrameInfo, void *parm);//@parm 用户传入的自定义参数


class CStorageManager : public CThread
{
public:
	CStorageManager();
	~CStorageManager();
	
	static CStorageManager* defaultStorageManager();

	int Init();
	int UnInit();
	
	//启动录像
	int StartRecord(int iRecordMode, bool bCycleRecord);

	//停止录像
	int StopRecord();
	
	//设置录像模式
	void SetRecordMode(int mode);

	//触发报警录像标志
	void TriggerAlarmRecord();

	//清除报警录像标志
	void ClearAlarmRecord();

	/// 线程执行体
	void ThreadProc();

	/// 开启
	bool start();

	/// 停止
	bool stop();
	
	/// 线程是否正在运行
	bool isRunning();

	Int32 WriteFrameData(unsigned char *pData, UInt32 uiSize, void *param);
	Int32 ClearFrameBuffer();

private:
	int GetTodayRecNum();
	/*
	 * @mode 1-year, 2-month, 3-day
	 */
	int ConvertDateStrToInt(int mode, const char *strDate);
	
	int ConvertYMDHMSToSecondInt(const char *pStrYMDHMS);
	
	int ParseRecordFile(const char *pStrFileName, int *piStatTime, int *piEndTime);
	
	int CheckIsTmpRecordFile(char *pStrFileName, unsigned long long *pullStartTimestamp, int *piRcdType);
	
	int RecoverRecordFile(unsigned long long *pullFileStartTime_ms, char *strOldPath, char *strNewPath, unsigned long long *pullFileEndTime_ms);

public:
	void DoRecoverRcdProc(RecoverThradParam_S *pstParam);
	
private:
	//创建录像文件索引
	int CreateRecordFileIndex();
	int RecoverTmpRecordFile();

	//取出录像修复文件中记录的最后一帧的时间戳
	int GetLastFrameTimestamp(const char *strPath);
public:
	int GetTmpRecordFileLatestTime();
	
private:
	//删除空的录像目录
	void CleanEmptyRecordDir();
	
	Int32 InsertlRecordFileIndex(const char *strPath, record_file_info_s *pstRecordFileInfo);
	
	//循环录像, SD卡满时, 删除最早的录像文件
	Int32 DeleteOrriginalRecordFile();
	
	//循环录像, SD卡满时, 删除最早的录像文件
	Int32 DeleteOrriginalRecordFile_2();

	Int32 InsertRecordTimeList(Int32 iStartTime, Int32 iEndTime);
	Int32 DeleteRecordTimeList(Int32 iStartTime, Int32 iEndTime);
	
	void GetRecordFilePath(unsigned long long time_ms, char *buffer, int bufferSize, int iRecordMode);
	int FilterFIle(const UInt64 *pu64FileStartTime_ms, const UInt64 *pu64LastFrameTimestamp_ms, const UInt64 *pu64NewFrameTimestamp_ms);
	int GetFilePathBaseOnTimestamp(UInt64 *pu64StartTimestamp_ms, UInt64 *pu64EndTimestamp_ms, char *buffer_file_path, int bufferSize, int iRecordMode);
	int GetDateIndex(int iUnixTimestamp);
	int GetDateIndexFilePath(int iUnixTimestamp, char *path);
	int do_save_sps_pps();
	int do_read_sps_pps();

	int InitMp4Muxer(const char *record_file_path);
	int UnInitMp4Muxer();
	/*
	 *@param u64FrameTimestamp_ms 与文件首帧时间戳的差值
	 */
	int WriteToMp4Muxer(UInt64 *pu64FileStartTime_ms, int iStreamType, int iFrameType, UInt64 *pu64FrameTimestamp_ms, UInt8 *pFrameData, int iFrameSize);

public:
	Int32 SearchRecord(record_file_time_s *pStRecordTime, Int32 buffer_size);
	Int32 SearchRecord(Int32 iStartTime, Int32 iEndTime, TRecordFileTimeList &listRecordTimeInfo);
	Int32 SearchRecord(Int32 year, Int32 month, Int32 day, TRecordFileTimeList &listRecordTimeInfo);
	Int32 CheckRecordOnMonth(Int32 iYear, Int32 iMonth);
	//开始回放
	void PlaybackProc(int index);
	Int32 StartPlayback(Int32 iStartTime, Int32 iEndTime, StoragePlaybackDownloadProc proc, void *param);
	Int32 StopPlayback(Int32 iPlayHandle);
	Int32 SeekTime(Int32 iPlayHandle, Int32 iStartTime);

	/*
	 *@param bPause true 暂停, false 取消暂停
	 */
	Int32 PausePlaybackOnFile(Int32 iPlayHandle, bool bPause);
	/*
	 * 设置回放倍速
	 *@param fSpeed 回放倍速, >=STORAGE_MIN_PB_SPEED && <=STORAGE_MAX_PB_SPEED
	 */
	Int32 SetPlaybackSpeed(Int32 iPlayHandle, float fSpeed);
	
	/*
	 *按天删除录像
	 */
	Int32 DeleteRecordByDay(Int32 year, Int32 month, Int32 day);

	////////////////////////////////////////////////////////录像下载//////////////////////////////////////////////////////////////////
	void DownloadProc(int index);
	//启动下载
	Int32 StartDownload(Int32 iStartTime, Int32 iEndTime, StoragePlaybackDownloadProc proc, void *param);
	Int32 StopDownload(Int32 iDownloadHandle);
	/*
	 *@param bPause true 暂停, false 取消暂停
	 */
	Int32 PauseDownload(Int32 iDownloadHandle, bool bPause);
	
private:
	bool m_bInit; 		//是否已初始化
	
	UInt64 m_u64DiskTotalSize; 				//SD卡容量
	int m_iDiskStatus; 						//SD卡状态
	bool m_bDiskFull;						//SD卡录满标识
	
	bool m_bCycleRecordFlag; 		//循环录像标识
	int m_iRecordMode; 				//录像模式
	bool m_bStartRecord; 			//启动录像标识
	bool m_bRecordWorking; 			//正在录像标识
	
	bool m_bStartAlarmRecord; 		//报警录像标识

	typedef std::map<int, int> TAlarmRecordCountMap;
	TAlarmRecordCountMap m_mapAlarmRecordCount;

	
//	Int32 m_I32TodayDateIndex; 			//今天的日期
//	Int32 m_I32TodayTotalAlarmRecord; 	//今天的报警录像总个数

	char m_arrSpsPps[256];
	int m_iSpsPpsLen;

	typedef std::list<record_file_info_s> TRecordFileInfoList;
	typedef std::map<int, TRecordFileInfoList *> TRecordFileMap;

	CMutex			m_mutex;			// 数据锁
	TRecordFileMap m_mapRecordFile;

	
	
//	TRecordFileTimeList m_listRecordTime; 	//当前SD卡有录像的时间段
	CMutex			m_mutex_RcTime;			// 数据锁
	TRecordFileTimeMap m_mapRecordTime; 	//当前SD卡有录像的时间段
	

	
	
	CMp4Muxer m_Mp4Muxer;
	
//	FILE *m_fpRecover;
	int m_fdRecover;
	char m_strRecoverFile[128];
	
	CRingBuffer m_avStreamRingBuffer;

	int m_iWidth;
	int m_iHeight;
	int m_iBitRate;
	int m_iFrameRate;
	int m_iGop;
	STORAGE_VIDEO_ENC_TYPE_E m_eVideoEncType;

private:
	
	typedef struct{
		bool bThreadRunningFlag;
		bool bIdel;
		bool bEnablePlay;
		bool bSeekFlag;
		bool bPause;
		Int64 i64SeekTime_ms; 		//文件偏移时间
		char strFileName[40];
		Int32 iSeekTime; 			//seek时间点
		Int32 iStartTime;
		Int32 iEndTime;
		float fSpeed; 				//回放倍速
		StoragePlaybackDownloadProc PlaybackProc;
		void *pParam;
		pthread_t threadPlayback_tid;
	} playback_manager_s;
	
	playback_manager_s m_arrStPlaybackManager[MAX_PLAYBACK_NUM];
	pthread_mutex_t m_mutexPlaybackManager;

	typedef std::map<int, TRecordFileInfoList::iterator> TSearchRecordResultMap;
	
	TSearchRecordResultMap m_mapSearchRecordResult; 	//搜索录像的结果

	typedef struct{
		bool bThreadRunningFlag;
		bool bPause;
		Int32 iStartTime;
		Int32 iEndTime;
		StoragePlaybackDownloadProc DownloadProc;
		void *pParam;
		pthread_t threadDownload_tid;
	} download_manager_s;

	download_manager_s m_arStDownloadManager[MAX_PLAYBACK_NUM];
	pthread_mutex_t m_mutexDownloadManager;

};

#endif //__STORAGE_MANAGER_H__
