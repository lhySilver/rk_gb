#include <sys/vfs.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "DiskManager.h"
#include "StorageManager.h"


#include "Log/DebugDef.h"


extern int disk_read_file(int fd, char *buffer, int size);
extern int disk_write_file(int fd, char *buffer, int size);
#if 0
void test_lsof()
{
	char res_buffer[1024];
	FILE *result_fp = popen("lsof", "r");
	if( NULL == result_fp )
	{
		//出错
		printf("popen \"lsof\" error!!!...\n");
		return ;
	}
	fread(res_buffer, 1024, 1, result_fp);
	pclose(result_fp);
	printf("lsof : \n%s\n", res_buffer);
}
#endif

CDiskManager::CDiskManager() : CThread("CDiskManager", 50)
{
	m_iDiskStatus = DISK_STATUS_UNKNOWN;
	
	m_iFmtProgress = 0;
	m_bFmtRunFlag = false;
	m_bReadWriteErrorFlag = false;
	m_iLastWriteTime = 0;
}

CDiskManager::~CDiskManager()
{

}

CDiskManager* CDiskManager::defaultStorageManager()
{
	static CDiskManager* p = NULL;
	if( NULL == p )
	{
		p = new CDiskManager();
	}

	return p;
}

bool CDiskManager::start()
{
	return CreateThread();
}

bool CDiskManager::stop()
{
	return DestroyThread(true);
}

void CDiskManager::ThreadProc()
{
	prctl(PR_SET_NAME, "DiskManager_thrad");
	
	#if 0
	//自动格式化SD卡
	if (DISK_STATUS_ERROR == m_iDiskStatus)
		_Format();
	#endif

	if( DISK_STATUS_NORMAL == m_iDiskStatus )
	{
		UInt64 u64Total, u64Used, u64Free;
		GetDiskcapacity(&u64Total, &u64Used, &u64Free);
		AppErr("Disk ---> total : %llu, used : %llu, free : %llu\n", u64Total, u64Used, u64Free);
		
		CStorageManager::defaultStorageManager()->Init();		//初始化录像 回放
	}
	
	while( m_bLoop )
	{
		handle_time_checkdisk();

		//格式化
		DiskFormatCheck();
		
		//记录时间
		SaveSystemTime();
		sleep(2);
	}
}

int CDiskManager::Init()
{
	m_iDiskStatus = CheckDiskStatus();
	if( DISK_STATUS_NOT_MOUNT == m_iDiskStatus )
	{
		//挂载
		int ret = fork_mount();
		if( 0 != ret ) {
			AppErr("mount sd fail.\n");
		}
		
		m_iDiskStatus = CheckDiskStatus();
	}
	if (DISK_STATUS_NORMAL == m_iDiskStatus)
	{
		//初始化时间，获取 systemTime 和 录像修复文件最后一帧时间 ，哪个新用哪个
		int rec_time = GetSystemTime();
		int recover_timestamp = CStorageManager::defaultStorageManager()->GetTmpRecordFileLatestTime();
		AppErr("rec_time: %d, recover_timestamp: %d\n", rec_time, recover_timestamp);
		int max_timestamp = rec_time > recover_timestamp ? rec_time : recover_timestamp;
		if (max_timestamp > 0)
		{				
			struct	timeval tv;
			tv.tv_sec = max_timestamp + 10;
			tv.tv_usec = 0;
			settimeofday(&tv, NULL);
			AppErr("update system time.\n");
		}
	}

	start();
	return 0;
}

int CDiskManager::UnInit()
{
	//停止录像和回放
	CStorageManager::defaultStorageManager()->UnInit();
	CStorageManager::defaultStorageManager()->FreeRingBuffer();

	stop();

	fork_umount();

	return 0;
}

//挂载SD卡
Int32 CDiskManager::fork_mount()
{	
	char tmpBufSys[128] = "";
    char mounts_info[1024] ={0};
	FILE *fp = NULL;
	int retry_count;

	retry_count = 3;
	while(retry_count-- > 0)
	{
		//snprintf(tmpBufSys, 128, "%s %s %s", "mount -t auto", "/dev/mmcblk1p1", __STORAGE_SD_MOUNT_PATH__);
		snprintf(tmpBufSys, 128, "%s %s %s", "mount -t auto -o errors=continue", "/dev/mmcblk1p1", __STORAGE_SD_MOUNT_PATH__);
		AppInfo("cmd: %s\n", tmpBufSys);
		__STORAGE_SYSTEM__("sh", "sh", "-c", tmpBufSys, NULL);
		
	    fp = fopen("/proc/mounts", "rb");
	    if(fp)
	    {
	        if(fread(mounts_info,1, 1024, fp) == 0) {
	            printf("read error\n");
	        }
	        fclose(fp);
	        if(NULL == strstr(mounts_info,"/dev/mmcblk")){//Device or resource busy
				printf("%s\n", mounts_info);
                printf("fork_mount failed, retry\n");
                sleep(2);
	        }
			else{
				return 0;
			}
	    }
	}

	printf("fork_mount failed,maybe device busy\n");
	return -1;
}

Int32 CDiskManager::fork_umount()
{
	char tmpBufSys[128] = "";
    char mounts_info[1024] ={0};
	FILE *fp = NULL;
	int retry_count;
	
	retry_count = 6;
	while(retry_count-- > 0)
	{
		//snprintf(tmpBufSys, 128, "%s %s", "umount -l", __STORAGE_SD_MOUNT_PATH__);
		//snprintf(tmpBufSys, 128, "%s %s", "umount", "/dev/mmcblk0p1");
		snprintf(tmpBufSys, 128, "%s %s", "umount -l", __STORAGE_SD_MOUNT_PATH__);
		AppInfo("cmd: %s\n", tmpBufSys);
		__STORAGE_SYSTEM__("sh", "sh", "-c", tmpBufSys, NULL);
		
	    fp = fopen("/proc/mounts", "rb");
	    if(fp)
	    {
	        if(fread(mounts_info,1, 1024, fp) == 0) {
	            printf("read error\n");
	        }
	        fclose(fp);
	        if(NULL != strstr(mounts_info,"/dev/mmcblk")){//Device or resource busy
				printf("%s\n", mounts_info);
                printf("fork_umount failed, retry\n");
                sleep(3);
				//test_lsof();
	        }
			else{
				return 0;
			}
	    }
	}
	
	printf("fork_umount failed,maybe device busy\n");
	return -1;
}

Int32 CDiskManager::fork_fdisk_delete_partition_table()
{
	//char* const path    = "/sbin/fdisk";
	//使用应用自己的fdisk工具,目录为/usr/netview/mkdos/fdisk,不再使用/sbin目录下的fdisk,
	char tmpBufSys[128] = "";
	
	// snprintf(tmpBufSys, 128, "echo \"\nd\n1\nd\n2\nd\n3\nd\nw\n\" | fdisk %s", "/dev/mmcblk0");
	snprintf(tmpBufSys, 128, "echo \"\nd\n1\nd\n2\nd\n3\nd\nw\n\" | fdisk %s", "/dev/mmcblk1");//当前RK使用的是mmcblk1
    
	AppInfo("cmd: %s\n", tmpBufSys);
//	__STORAGE_SYSTEM__(tmpBufSys);
	__STORAGE_SYSTEM__("sh", "sh", "-c", tmpBufSys, NULL);
	
	return 0;
}

Int32 CDiskManager::fork_fdisk()
{
	char tmpBufSys[128] = "";
    
	snprintf(tmpBufSys, 128, "echo \"\nn\np\n1\n\n\nt\nc\nw\n\" | fdisk %s", "/dev/mmcblk1");

	AppInfo("cmd: %s\n", tmpBufSys);
//	__STORAGE_SYSTEM__(tmpBufSys);
	__STORAGE_SYSTEM__("sh", "sh", "-c", tmpBufSys, NULL);

	return 0;
}

Int32 CDiskManager::fork_mkdosfs()
{
	char cmd[36] = "mkdosfs"; 	
	char tmpBufSys[128] = "";
	
	snprintf(tmpBufSys, 128, "%s %s", cmd, "/dev/mmcblk1p1");
	AppInfo("cmd: %s\n", tmpBufSys);
//	__STORAGE_SYSTEM__(tmpBufSys);
	__STORAGE_SYSTEM__("sh", "sh", "-c", tmpBufSys, NULL);

	return 0;
}


//获取SD卡容量
Int32 CDiskManager::GetDiskcapacity(UInt64 *pu64Total, UInt64 *pu64Used, UInt64 *pu64Free)
{
	if (DISK_STATUS_NORMAL != m_iDiskStatus)
	{
		if( pu64Total )
			*pu64Total = 0;
		if( pu64Used )
			*pu64Used = 0;
		if( pu64Free )
			*pu64Free = 0;
		return -1;
	}

	struct statfs sd_fs;
	if (statfs(__STORAGE_SD_MOUNT_PATH__, &sd_fs) != 0)
	{  
		AppErr("statfs failed!\n");
		return -1;
	}

	if( pu64Total )
		*pu64Total = (UInt64)sd_fs.f_blocks * (UInt64)sd_fs.f_bsize;
	if( pu64Used )
		*pu64Used = ((UInt64)sd_fs.f_blocks - (UInt64)sd_fs.f_bfree) * (UInt64)sd_fs.f_bsize;
	if( pu64Free )
		*pu64Free = (UInt64)sd_fs.f_bavail * (UInt64)sd_fs.f_bsize;
	
    return 0;
}

//获取SD卡状态
UInt64 CDiskManager::GetDiskStatus()
{
	return m_iDiskStatus;
}


///dev/mmcblk1p1 on /mnt/sd type vfat (ro,relatime,fmask=0022,dmask=0022,codepage=437,iocharset=iso8859-1,shortname=mixed,errors=remount-ro)
///dev/mmcblk1p1 /mnt/sd vfat ro,relatime,fmask=0022,dmask=0022,codepage=437,iocharset=iso8859-1,shortname=mixed,errors=remount-ro 0 0
DISK_STATUS_E CDiskManager::CheckDiskStatus()
{
    FILE *fp = NULL;
	char mounts_info[1024];
	DISK_STATUS_E status = DISK_STATUS_UNKNOWN;

    if(access("/dev/mmcblk1",F_OK))
    {
        return DISK_STATUS_NO_DISK;
    }

	if(access("/dev/mmcblk1p1",F_OK))
	{
		return DISK_STATUS_ERROR;
	}
	
    fp = fopen("/proc/mounts", "rb");
    if(fp)
    {
        memset(mounts_info, 0, sizeof(mounts_info));
        if(fread(mounts_info, 1, sizeof(mounts_info)-1, fp) <= 0) {
            printf("read error\n");
        }
        fclose(fp);

		char *mmcblk_name_start = strstr(mounts_info, "/dev/mmcblk1p1");
		if( !mmcblk_name_start )
			return DISK_STATUS_NOT_MOUNT;
		char *mount_point_start = strstr(mmcblk_name_start, __STORAGE_SD_MOUNT_PATH__);
		if( !mount_point_start )
			return DISK_STATUS_ERROR;
		
        if (NULL == strstr(mmcblk_name_start,"vfat") && NULL == strstr(mmcblk_name_start,"exfat"))
        {
            return DISK_STATUS_ERROR;
        }

        if(NULL != strstr(mmcblk_name_start," ro,")) //SD卡出现异常，被remount为只读
        {
            return DISK_STATUS_ERROR;
        }

		if (m_bReadWriteErrorFlag)
			status = DISK_STATUS_ERROR;
		else
			status = DISK_STATUS_NORMAL;
	}
	else
	{
		status = DISK_STATUS_UNKNOWN;
	}

	return status;
}

void CDiskManager::SetReadWriteErrorFlag(bool bError)
{
	AppErr("read write error!\n");
	m_bReadWriteErrorFlag = bError;
}

Int32 CDiskManager::fmtDisk(Int32 &iFmtState)
{
	Int32 ret=0;
	UInt32 i;
	
	AppInfo("fmtDisk: begin to format disk! please wait!\n");
	
	AppInfo("cmd: sync\n");
	__STORAGE_SYSTEM__("sh", "sh", "-c", "sync", NULL);
	sleep(1);
	iFmtState = 10;

	AppInfo("cmd: echo 3 > /proc/sys/vm/drop_caches;\n");
	__STORAGE_SYSTEM__("sh", "sh", "-c", "echo 3 > /proc/sys/vm/drop_caches;", NULL);
	sleep(1);
	iFmtState = 20;

	ret = fork_umount();
	if(ret<0)
	{
		return -1;
	}
	iFmtState = 30;
	
	#if 01
	//可能分区异常, 尝试重新分区
    if( access("/dev/mmcblk1",F_OK) == 0 && access("/dev/mmcblk1p1",F_OK) != 0 )
    {		
		AppInfo("fmtDisk: umount disk end! formating continue, please wait!\n");

		//删除分区表
		ret = fork_fdisk_delete_partition_table();
		if(ret<0)
		{
			AppErr("fmtDisk process fork_fdisk_delete_partition_table failed !\n");
			return -1;
		}
		iFmtState = 35;

		ret = fork_fdisk();//只有一个分区
		if(ret<0)
		{
			AppErr("fmtDisk process fork_fdisk failed !\n");
			return -1;
		}
		iFmtState = 40;

		__STORAGE_SYSTEM__("sh", "sh", "-c", "mdev -s", NULL);
		iFmtState = 45;
		AppInfo("fmtDisk: fdisk end! formating continue, please wait!\n");
		sleep(1);
	}
	#endif
		
	/* 所有类型盘的整盘格式化 */
	ret = fork_mkdosfs();
	if(ret<0)
	{
		AppErr("fmtDisk process fork_mkdosfs failed !\n");
		return -1;
	}
	iFmtState = 70;
	AppInfo("fmtDisk: mk_FAT32 disk end! formating continue, please wait!\n");
	sleep(1);

	ret = fork_mount();
	if(ret<0)
	{
		AppErr("fmtDisk process mk_mount failed !\n");
		return -1;
	}
	AppInfo("fmtDisk: mount disk end! formating continue, please wait!\n");

	__STORAGE_SYSTEM__("sh", "sh", "-c", "mkdir " __STORAGE_SD_MOUNT_PATH__ "/DCIM/", NULL);
	
	//__STORAGE_SYSTEM__("sh", "sh", "-c", "echo \""DG_STORAGE_MOUDLE_VER"\" > "__STORAGE_SD_MOUNT_PATH__ "/DCIM/Version", NULL);
	__STORAGE_SYSTEM__("sh", "sh", "-c", "echo " DG_STORAGE_MOUDLE_VER " > " __STORAGE_SD_MOUNT_PATH__ "/DCIM/Version", NULL);

	//记录时间
	SaveSystemTime();
	sleep(1);
	
	AppInfo("cmd: sync\n");
	__STORAGE_SYSTEM__("sh", "sh", "-c", "sync", NULL);
	sleep(1);
	
	//fork_umount();
	
	iFmtState = 90;
	AppInfo("format disk finished!\n");

	return 0;	
}


Int32 CDiskManager::handle_time_checkdisk()
{
	static int s_umount_ret = -1;
	DISK_STATUS_E eDiskStatus = DISK_STATUS_UNKNOWN;

	eDiskStatus = CheckDiskStatus();
	if( eDiskStatus == m_iDiskStatus )
		return 0;
	
	AppErr("disk state from %d to %d\n", m_iDiskStatus, eDiskStatus);
	switch(eDiskStatus)
	{
		case DISK_STATUS_NORMAL:
		{
			//卡恢复正常
			CStorageManager::defaultStorageManager()->Init();
			s_umount_ret = -1;
		}
		break;
		case DISK_STATUS_FORMATING:
		{
			s_umount_ret = -1;
		}
		break;			
		case DISK_STATUS_NOT_MOUNT:
		{
			m_bReadWriteErrorFlag = false;
			fork_mount();
			s_umount_ret = -1;
		}
		break;			
//		case DISK_STATUS_UNKNOWN:
		case DISK_STATUS_NO_DISK:
		case DISK_STATUS_ERROR:
		{
			CStorageManager::defaultStorageManager()->UnInit();
			if (0 != s_umount_ret)
				s_umount_ret = fork_umount();
		}
		break;
		default:
		{
			s_umount_ret = -1;
			break;
		}
	}

	m_iDiskStatus = eDiskStatus;
	
 	return 0;
}

Int32 CDiskManager::_Format()
{
	int ret = -1;

	if( (DISK_STATUS_UNKNOWN != m_iDiskStatus) && (DISK_STATUS_NO_DISK != m_iDiskStatus) )
	{
		AppInfo("+++++++++++++begin to format disk\n");
	
		m_iDiskStatus = DISK_STATUS_FORMATING;
	
		//停止录像和回放
		CStorageManager::defaultStorageManager()->UnInit();
	
		m_iFmtProgress = 0;
		if( -1 == fmtDisk(m_iFmtProgress) )
		{
			m_iDiskStatus = DISK_STATUS_UNKNOWN;
			AppErr("disk format fail!\n");
		}
		else
		{
			m_iDiskStatus = DISK_STATUS_NORMAL;
			m_iFmtProgress = 100;
			CStorageManager::defaultStorageManager()->Init();
			ret = 0;
		}
	}
	return ret;
}

Int32 CDiskManager::DiskFormatCheck()
{
	if(m_bFmtRunFlag) //if -> start to fmt
	{
		_Format();
		m_bFmtRunFlag = false;
	}	
	return 0;
}

Int32 CDiskManager::FormatDisk(bool bSync/* = true*/)
{
	if( bSync )
		return _Format();
	else
		m_bFmtRunFlag = true;

	return 0;
}

//获取格式化进度
Int32 CDiskManager::GetFormatProgress()
{
	return m_iFmtProgress;
}


int CDiskManager::SaveSystemTime(bool bForce/*=false*/)
{
	if (DISK_STATUS_NORMAL != m_iDiskStatus)
		return -1;
	if (false == bForce && 
		time(0) < (m_iLastWriteTime+(30*60)))
		return -1;
	if (false == m_mutex_write_time.TryEnter())
		return -1;
	
	int ret;
	int fd;
	char path[128];
	char strTime[12];
	snprintf(path, sizeof(path), __STORAGE_SD_MOUNT_PATH__"/DCIM/systemTime");
	fd = open(path, O_WRONLY | O_CREAT);
	if (-1 == fd)
	{
		printf("Disk -> open fail. errno: %d\n", errno);
		if (ENOENT == errno || //#define ENOENT 2 /* No such file or directory */
			EIO == errno) //#define EIO 5 /* I/O error */
			m_bReadWriteErrorFlag = true;
		
		m_mutex_write_time.Leave();
		return -1;
	}
	snprintf(strTime, sizeof(strTime), "%08x", time(0));
	ret = disk_write_file(fd, strTime, strlen(strTime));
	if (ret != strlen(strTime))
	{
		printf("Disk -> write time error!. ret: %d\n", ret);
	}
	else
	{
		m_iLastWriteTime = time(0);
	}
	//fdatasync(fd);
	close(fd);

	m_mutex_write_time.Leave();
	return 0;
}

int CDiskManager::GetSystemTime()
{
	if (DISK_STATUS_NORMAL != m_iDiskStatus)
		return 0;
	
	int ret;
	int fd;
	char path[128];
	char strTime[12];
	int timestamp = 0;
	
	snprintf(path, sizeof(path), __STORAGE_SD_MOUNT_PATH__"/DCIM/systemTime");
	fd = open(path, O_RDONLY);
	if (-1 == fd)
	{
		printf("Disk -> open fail. errno: %d\n", errno);
		return 0;
	}
	ret = disk_read_file(fd, strTime, sizeof(strTime)-1);
	if (ret <= 0)
	{
		printf("Disk -> write time error!. ret: %d\n", ret);
		close(fd);
		return 0;
	}
	strTime[ret] = '\0';

	timestamp = (int)strtol(strTime, NULL, 16);

	close(fd);

	return timestamp;
}

