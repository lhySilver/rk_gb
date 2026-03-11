#ifndef __DISK_MANAGER_H__
#define __DISK_MANAGER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#include "Infra/Thread.h"
#include "StorageCommon.h"



typedef enum{
	DISK_STATUS_UNKNOWN 		= 0,
	DISK_STATUS_NO_DISK,
	DISK_STATUS_NOT_MOUNT,
	DISK_STATUS_NORMAL,
	DISK_STATUS_FORMATING,
	DISK_STATUS_ERROR,
} DISK_STATUS_E;


class CDiskManager : public CThread
{
public:
	CDiskManager();
	~CDiskManager();
	
	static CDiskManager* defaultStorageManager();

	/// 线程执行体
	void ThreadProc();

	/// 开启
	bool start();

	/// 停止
	bool stop();


	int Init();
	int UnInit();

	//挂载SD卡
	Int32 fork_mount();

	Int32 fork_umount();

	Int32 fork_fdisk_delete_partition_table();

	Int32 fork_fdisk();

	Int32 fork_mkdosfs();

	//获取SD卡容量
	Int32 GetDiskcapacity(UInt64 *pu64Total, UInt64 *pu64Used, UInt64 *pu64Free);
	UInt64 GetDiskStatus();

	Int32 fmtDisk(Int32 &iFmtState);

	Int32 handle_time_checkdisk();

	Int32 DiskFormatCheck();

	Int32 FormatDisk(bool bSync = true);
	//获取格式化进度
	Int32 GetFormatProgress();
	
	DISK_STATUS_E CheckDiskStatus();

	//外部设置SD卡读写错误
	void SetReadWriteErrorFlag(bool bError);
	int SaveSystemTime(bool bForce = false);

private:
	Int32 _Format();
	int GetSystemTime();

private:
	int m_iDiskStatus;

	int m_iFmtProgress;
	bool m_bFmtRunFlag;
	bool m_bReadWriteErrorFlag;
	CMutex m_mutex_write_time;
	int m_iLastWriteTime;
};


#endif //__DISK_MANAGER_H__
