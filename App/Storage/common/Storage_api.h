#ifndef __STORAGE_API_H__
#define __STORAGE_API_H__

#include "StorageCommon.h"
#include "DiskManager.h"
#include "StorageManager.h"


int Storage_Module_Init();
int Storage_Module_UnInit();

int Storage_Module_StartRecord(int iRecordMode, bool bCycleRecord);
int Storage_Module_StopRecord();
//设置录像模式
void Storage_Module_SetRecordMode(int mode);

//触发报警录像标志
void Storage_Module_TriggerAlarmRecord();

//清除报警录像标志
void Storage_Module_ClearAlarmRecord();

int Storage_Module_Format(bool bSync);
int Storage_Module_GetFormatProgress();
int Storage_Module_GetDiskStatus();
void Storage_Module_SetReadWriteErrorFlag(bool bError);

Int32 Storage_Module_GetDiskcapacity(UInt64 *pu64Total, UInt64 *pu64Used, UInt64 *pu64Free);
int Storage_Module_SaveSystemTime(bool bForce = false);

int Storage_Module_WriteFrame(unsigned char *pData, UInt32 uiSize, void *param);
int Storage_Module_ClearFrameBuffer();

int Storage_Module_SearchRecord(record_file_time_s *pStRecordTime, Int32 buffer_size);
int Storage_Module_SearchRecordOnTime(Int32 iStartTime, Int32 iEndTime, TRecordFileTimeList &listRecordTimeInfo);
int Storage_Module_SearchRecordByDay(Int32 year, Int32 month, Int32 day, TRecordFileTimeList &listRecordTimeInfo);
int Storage_Module_CheckRecordOnMonth(Int32 iYear, Int32 iMonth);
int Storage_Module_StartPlayback(Int32 iStartTime, Int32 iEndTime, StoragePlaybackDownloadProc proc, void *param);
int Storage_Module_StopPlayback(Int32 iPlayHandle);
int Storage_Module_SeekTime(Int32 iPlayHandle, Int32 iStartTime);

/*
 *@param bPause true 暂停, false 取消暂停
 */
int Storage_Module_PausePlaybackOnFile(Int32 iPlayHandle, bool bPause);
/*
 * 设置回放倍速
 *@param fSpeed 回放倍速, >=STORAGE_MIN_PB_SPEED && <=STORAGE_MAX_PB_SPEED
 */
int Storage_Module_SetPlaybackSpeed(Int32 iPlayHandle, float fSpeed);

/*
 *按天删除录像
 */
int Storage_Module_DeleteRecordByDay(Int32 year, Int32 month, Int32 day);
//开始下载
int Storage_Module_StartDownload(Int32 iStartTime, Int32 iEndTime, StoragePlaybackDownloadProc proc, void *param);
//停止下载
int Storage_Module_StopDownload(Int32 iDownloadHandle);
/*
 *暂停
 *@param bPause true 暂停, false 取消暂停
 */
int Storage_Module_PauseDownload(Int32 iDownloadHandle, bool bPause);


//log
int Storage_Module_WriteLog(const char *pBuf, Int32 iLen, T_LogRecInfo* pRecInfo, UInt32 timestamp);
int Storage_Module_StartSearchLog(UInt32 uiStartTime, UInt32 uiEndTime, UInt32 uiType);
int Storage_Module_ReadLog(Int32 iNum, T_Log* pLog);
int Storage_Module_StopSearchLog();


#endif //__STORAGE_API_H__


