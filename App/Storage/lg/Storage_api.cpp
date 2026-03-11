#include "Storage_api.h"



int Storage_Module_Init()
{
	CDiskManager::defaultStorageManager()->Init();
	
	return 0;
}

int Storage_Module_UnInit()
{
	CDiskManager::defaultStorageManager()->UnInit();
	
	return 0;
}

int Storage_Module_StartRecord(int iRecordMode, bool bCycleRecord)
{
	return CStorageManager::defaultStorageManager()->StartRecord(iRecordMode, bCycleRecord);
}

int Storage_Module_StopRecord()
{
	return CStorageManager::defaultStorageManager()->StopRecord();
}

//设置录像模式
void Storage_Module_SetRecordMode(int mode)
{
	CStorageManager::defaultStorageManager()->SetRecordMode(mode);
}

//触发报警录像标志
void Storage_Module_TriggerAlarmRecord()
{
	CStorageManager::defaultStorageManager()->TriggerAlarmRecord();
}

//清除报警录像标志
void Storage_Module_ClearAlarmRecord()
{
	CStorageManager::defaultStorageManager()->ClearAlarmRecord();
}


int Storage_Module_Format(bool bSync)
{
	return CDiskManager::defaultStorageManager()->FormatDisk(bSync);
}

int Storage_Module_GetFormatProgress()
{
	return CDiskManager::defaultStorageManager()->GetFormatProgress();
}

int Storage_Module_GetDiskStatus()
{
	return CDiskManager::defaultStorageManager()->GetDiskStatus();
}

void Storage_Module_SetReadWriteErrorFlag(bool bError)
{
	CDiskManager::defaultStorageManager()->SetReadWriteErrorFlag(bError);
}

Int32 Storage_Module_GetDiskcapacity(UInt64 *pu64Total, UInt64 *pu64Used, UInt64 *pu64Free)
{
	return CDiskManager::defaultStorageManager()->GetDiskcapacity(pu64Total, pu64Used, pu64Free);
}

int Storage_Module_SaveSystemTime(bool bForce/* = false*/)
{
	return CDiskManager::defaultStorageManager()->SaveSystemTime(bForce);
}

int Storage_Module_WriteFrame(unsigned char *pData, UInt32 uiSize, void *param)
{
	return CStorageManager::defaultStorageManager()->WriteFrameData(pData, uiSize, param);
}

int Storage_Module_ClearFrameBuffer()
{
	return CStorageManager::defaultStorageManager()->ClearFrameBuffer();
}


int Storage_Module_SearchRecord(record_file_time_s *pStRecordTime, Int32 buffer_size)
{
	return CStorageManager::defaultStorageManager()->SearchRecord(pStRecordTime, buffer_size);
}

int Storage_Module_SearchRecordOnTime(Int32 iStartTime, Int32 iEndTime, TRecordFileTimeList &listRecordTimeInfo)
{
	return CStorageManager::defaultStorageManager()->SearchRecord(iStartTime, iEndTime, listRecordTimeInfo);
}

int Storage_Module_SearchRecordByDay(Int32 year, Int32 month, Int32 day, TRecordFileTimeList &listRecordTimeInfo, TRecordFileTimeList &listAlarmRecordTimeInfo)
{
	return CStorageManager::defaultStorageManager()->SearchRecord(year, month, day, listRecordTimeInfo, listAlarmRecordTimeInfo);
}

int Storage_Module_CheckRecordOnMonth(Int32 iYear, Int32 iMonth)
{
	return CStorageManager::defaultStorageManager()->CheckRecordOnMonth(iYear, iMonth);
}

int Storage_Module_StartPlayback(Int32 iStartTime, Int32 iEndTime, StoragePlaybackDownloadProc proc, void *param)
{
	return CStorageManager::defaultStorageManager()->StartPlayback(iStartTime, iEndTime, proc, param);
}

int Storage_Module_StopPlayback(Int32 iPlayHandle)
{
	return CStorageManager::defaultStorageManager()->StopPlayback(iPlayHandle);
}

int Storage_Module_SeekTime(Int32 iPlayHandle, Int32 iStartTime)
{
	return CStorageManager::defaultStorageManager()->SeekTime(iPlayHandle, iStartTime);
}

/*
 *@param bPause true 暂停, false 取消暂停
 */
int Storage_Module_PausePlaybackOnFile(Int32 iPlayHandle, bool bPause)
{
	return CStorageManager::defaultStorageManager()->PausePlaybackOnFile(iPlayHandle, bPause);
}

/*
 * 设置回放倍速
 *@param fSpeed 回放倍速, >=STORAGE_MIN_PB_SPEED && <=STORAGE_MAX_PB_SPEED
 */
int Storage_Module_SetPlaybackSpeed(Int32 iPlayHandle, float fSpeed)
{
	return CStorageManager::defaultStorageManager()->SetPlaybackSpeed(iPlayHandle, fSpeed);
}


/*
 *按天删除录像
 */
int Storage_Module_DeleteRecordByDay(Int32 year, Int32 month, Int32 day)
{
	return CStorageManager::defaultStorageManager()->DeleteRecordByDay(year, month, day);
}

//开始下载
int Storage_Module_StartDownload(Int32 iStartTime, Int32 iEndTime, StoragePlaybackDownloadProc proc, void *param)
{
	return CStorageManager::defaultStorageManager()->StartDownload(iStartTime, iEndTime, proc, param);
}
//停止下载
int Storage_Module_StopDownload(Int32 iDownloadHandle)
{
	return CStorageManager::defaultStorageManager()->StopDownload(iDownloadHandle);
}
/*
 *暂停
 *@param bPause true 暂停, false 取消暂停
 */
int Storage_Module_PauseDownload(Int32 iDownloadHandle, bool bPause)
{
	return CStorageManager::defaultStorageManager()->PauseDownload(iDownloadHandle, bPause);
}


//log
int Storage_Module_WriteLog(const char *pBuf, Int32 iLen, T_LogRecInfo* pRecInfo, UInt32 timestamp)
{
	return 0;//CLogManager::defaultLogManager().WriteLogFileData(pBuf, iLen, pRecInfo, timestamp);
}

int Storage_Module_StartSearchLog(UInt32 uiStartTime, UInt32 uiEndTime, UInt32 uiType)
{
	return 0;//CLogManager::defaultLogManager().StartSearchLog(uiStartTime, uiEndTime, uiType);
}

int Storage_Module_ReadLog(Int32 iNum, T_Log* pLog)
{
	return 0;//CLogManager::defaultLogManager().LoadPuLog(iNum, pLog);
}

int Storage_Module_StopSearchLog()
{
	return 0;//CLogManager::defaultLogManager().StopSearchPuLog();
}

