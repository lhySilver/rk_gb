#include "Storage.h"

CStorage* CStorage::_instance = NULL;

//CStorage::CStorage() : CThread("Storage", TP_CAPTURE), m_sigBuffer(32*16)
CStorage::CStorage()
{
	
}

CStorage::~CStorage()
{

}

int CStorage::Init()
{
	return Storage_Module_Init();
}

int CStorage::UnInit()
{
	return Storage_Module_UnInit();
}

int CStorage::DiskFmt(bool bSync/* = true*/)
{
	return Storage_Module_Format(bSync);
}

int CStorage::GetFormatProgress()
{
	return Storage_Module_GetFormatProgress();
}

int CStorage::GetDiskState()
{
	return Storage_Module_GetDiskStatus();
}
#ifdef RC0240_LGV10
void CStorage::SetReadWriteErrorFlag(bool bError)
{
	Storage_Module_SetReadWriteErrorFlag(bError);
}
#endif
int CStorage::GetDiskcapacity(unsigned long long *pu64Total, unsigned long long *pu64Used, unsigned long long *pu64Free)
{
	return Storage_Module_GetDiskcapacity(pu64Total, pu64Used, pu64Free);
}
#ifdef RC0240_LGV10
int CStorage::SaveSystemTime(bool bForce/* = false*/)
{
	return Storage_Module_SaveSystemTime(bForce);
}
#endif
int CStorage::GetDiskInfo(/*T_DiskInfo &tDiskInfo*/)
{
//<shang>	return STORE_GetDiskInfo(STO_SDCARD, 1, tDiskInfo);
	return 0;
}

int CStorage::SearchFiles(unsigned int recType, unsigned int fileType, unsigned int sTime, unsigned int eTime, StoragePlayProc proc,void*parm)
{
	/*
	int iRealChannel = 1;
	int uiChannelType = 0;
	RECFILESLIST fileResult;
	m_pSearchProc = proc;
	m_parm = parm;
	if (m_pSearchProc == NULL)
		return -1;
	int ret = STORE_SearchFiles(iRealChannel, uiChannelType, recType, fileType, sTime, eTime, fileResult);
	AppInfo("STORE_SearchFiles[%d]\n",ret);
	m_pSearchProc(&fileResult,fileResult.size(),0,m_parm);
	*/
	return 0;
}

int CStorage::SearchFiles(unsigned int sTime, unsigned int eTime, StoragePlayProc proc,void*parm)
{
	/*
	int iRealChannel = 1;
	int uiChannelType = 0;

	
	int fileType =255;	//255=全部（缺省），0=定时录像，1=移动侦测， 2=报警触发， 	//REC_TYPE
	int fileProperty = 255; //255=全部，0 非加锁  1 加锁
	
	RECFILESLIST fileResult;
	m_pSearchProc = proc;
	m_parm = parm;
	if (m_pSearchProc == NULL)
		return -1;
	int ret = STORE_SearchFiles(iRealChannel, uiChannelType, fileType, fileProperty, sTime, eTime, fileResult);
	m_pSearchProc(&fileResult,fileResult.size(),0,m_parm);
	AppInfo("STORE_SearchFiles[%d]\n",ret);
	*/
	return 0;
}
#if 0
int CStorage::SearchFiles(unsigned int sTime, unsigned int eTime, RECFILESLIST &ListSearchResult)
{
	/*
	int iRealChannel = 1;
	int uiChannelType = 0;

	int fileType =255;	//255=全部（缺省），0=定时录像，1=移动侦测， 2=报警触发， 	//REC_TYPE
	int fileProperty = 255; //255=全部，0 非加锁  1 加锁
		
	int ret = STORE_SearchFiles(iRealChannel, uiChannelType, fileType, fileProperty, sTime, eTime, ListSearchResult);
	AppInfo("STORE_SearchFiles[%d]\n",ret);
	return ret;
	*/
}
#endif








int CStorage::StartRec(int type)
{
	if( REC_TIM == type )
		Storage_Module_StartRecord(DISK_RECORD_MODE_FULLTIME, true);
	else if( REC_ALM == type )
		Storage_Module_StartRecord(DISK_RECORD_MODE_ALARM, true);
	else
		Storage_Module_StartRecord(DISK_RECORD_MODE_CLOSE, true);

	return 0;
}

int CStorage::StopRec()
{
	Storage_Module_StopRecord();
	
	return 0;
}

int CStorage::SetRecMode(int mode)
{
	Storage_Module_SetRecordMode(mode);
	
	return 0;
}

int CStorage::WriteFrameData(unsigned char *pData, unsigned int uiSize, void *param)
{
	return Storage_Module_WriteFrame((unsigned char*)pData, uiSize, (void*)param);
}
#ifdef RC0240_LGV10
int CStorage::ClearFrameBuffer()
{
	return Storage_Module_ClearFrameBuffer();
}
#endif



int CStorage::Pause()
{
	/*
	int iRet = STORE_ControlFileBack(m_uiHandle, "PAUSE", 0);
	AppInfo("STORE_ControlFileBack PAUSE iRet=%d,uiHandle=%u\n",iRet,m_uiHandle);
	return iRet;
	*/
	return 0;
}

int CStorage::PausePlay(int iPlayHandle, bool bPause)
{
	return Storage_Module_PausePlaybackOnFile(iPlayHandle, bPause);
}

int CStorage::SetPlaybackSpeed(int iPlayHandle, float fSpeed)
{
	return Storage_Module_SetPlaybackSpeed(iPlayHandle, fSpeed);
}

int CStorage::DeleteRecordByDay(Int32 year, Int32 month, Int32 day)
{
	return Storage_Module_DeleteRecordByDay(year, month, day);
}

int CStorage::ReStart()
{
	/*
	int iRet = STORE_ControlFileBack(m_uiHandle, "RESTART", 0);
//<shang>	m_fineI = true; 	//不需要，否则会造成卡顿几秒
	AppInfo("STORE_ControlFileBack RESTART iRet=%d,uiHandle=%u STORE_ControlFileBack\n",iRet,m_uiHandle);
	return iRet;
	*/
	return 0;
}


//触发报警录像
void CStorage::DoAlarmRecord()
{
	Storage_Module_TriggerAlarmRecord();
}

//消除报警录像
void CStorage::ClearAlarmRecord()
{
	Storage_Module_ClearAlarmRecord();
}

//log
int CStorage::WriteLog(T_LogRecInfo* pRecInfo,const char*msg,int len,unsigned int timestamp)
{
	return Storage_Module_WriteLog(msg, len, pRecInfo, timestamp);
}

int CStorage::StartSearchLog(unsigned int uiStartTime, unsigned int uiEndTime, unsigned int uiType)
{
	return Storage_Module_StartSearchLog(uiStartTime, uiEndTime, uiType);
}

int CStorage::ReadLog(unsigned int iNum, T_Log* pLog)
{
	return Storage_Module_ReadLog(iNum, pLog);
}

int CStorage::StopLogSearch()
{
	return Storage_Module_StopSearchLog();
}

/*void CStorage::ThreadProc()
{
	while (m_bLoop)
	{
	
	}
}*/


int CStorage::SearchRecordOnTime(int iStartTime, int iEndTime, TRecordFileTimeList &listRecordTimeInfo)
{
	return Storage_Module_SearchRecordOnTime(iStartTime, iEndTime, listRecordTimeInfo);
}
#ifdef RC0240_LGV10
int CStorage::SearchRecordByDay(int year, int month, int day, TRecordFileTimeList &listRecordTimeInfo, TRecordFileTimeList &listAlarmRecordTimeInfo)
{
	return Storage_Module_SearchRecordByDay(year, month, day, listRecordTimeInfo, listAlarmRecordTimeInfo);
}
#else
int CStorage::SearchRecordByDay(int year, int month, int day, TRecordFileTimeList &listRecordTimeInfo)
{
	return Storage_Module_SearchRecordByDay(year, month, day, listRecordTimeInfo);
}
#endif
int CStorage::CheckRecordOnMonth(Int32 iYear, Int32 iMonth)
{
	return Storage_Module_CheckRecordOnMonth(iYear, iMonth);
}

int CStorage::StartPlaybackOnTime(Int32 iStartTime, Int32 iEndTime, StoragePlaybackDownloadProc proc, void *param)
{
	return Storage_Module_StartPlayback(iStartTime, iEndTime, proc, param);
}

int CStorage::StopPlaybackOnTime(Int32 iPlayHandle)
{
	return Storage_Module_StopPlayback(iPlayHandle);
}

int CStorage::SeekTimeOnTime(Int32 iPlayHandle, Int32 iStartTime)
{
	return Storage_Module_SeekTime(iPlayHandle, iStartTime);
}


int CStorage::StartDownload(Int32 iStartTime, Int32 iEndTime, StoragePlaybackDownloadProc proc, void *param)
{
	return Storage_Module_StartDownload(iStartTime, iEndTime, proc, param);
}

int CStorage::StopDownload(Int32 iDownloadHandle)
{
	return Storage_Module_StopDownload(iDownloadHandle);
}

int CStorage::PauseDownload(int iDownloadHandle, bool bPause)
{
	return Storage_Module_PauseDownload(iDownloadHandle, bPause);
}


CStorage* CStorage::instance()
{
	if(NULL == CStorage::_instance)
	{
		CStorage::_instance = new CStorage();
	}
	return CStorage::_instance;
}

