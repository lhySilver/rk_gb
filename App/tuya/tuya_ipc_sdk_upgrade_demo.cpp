/*
 * tuya_ipc_sdk_upgrade_demo.c
 *
 *  Created on: 2020年12月28日
 *      Author: kuiba
 */

#include <stdbool.h>
#include "include/sdk_inc/tuya_ipc_api.h"
#include "include/sdk_inc/tuya_ipc_ai_detect_storage.h"
#include "include/sdk_inc/tuya_ipc_cloud_storage.h"
#include "include/tuya_ipc_common.h"
#include "include/sdk_inc/tuya_ipc_upgrade.h"
#include "include/sdk_inc/uni_log.h"

#include <sys/sysinfo.h>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "Tuya_common.h"
#include "../Main.h"

static FW_UG_S FW_UG_g = {0};

/* OTA */
//Callback after downloading OTA files
VOID __IPC_APP_upgrade_notify_cb(IN CONST FW_UG_S *fw, IN CONST INT_T download_result, IN PVOID_T pri_data)
{
    FILE *p_upgrade_fd = (FILE *)pri_data;
    fclose(p_upgrade_fd);

    printf("Upgrade Finish");
    printf("download_result:%d fw_url:%s", download_result, fw->fw_url);

    if(download_result == 0)
    {
        /* The developer needs to implement the operation of OTA upgrade,
        when the OTA file has been downloaded successfully to the specified path. [ p_mgr_info->upgrade_file_path ]*/

		// 涂鸦 4.7.14 版本的SDK 开始SDK内部进行了md5校验, 不需要我们再去校验, 并且不提供md5值, 即FW_UG_g.fw_md5是为空的
		printf("stop net manager\n");
		g_NetConfigHook.UnInit();
		AppErr("IPC_APP_Upgrade_Inform_cb---------getpid() : %d\n", getpid());
		{
			CFeedDog::instance()->stop();
			CFeedDog::instance()->destory();
			START_PROCESS("sh", "sh", "-c", "touch /tmp/ota_upgrade_flag", NULL);
		}
    }
    else
    {
        printf("\033[1;36m    __IPC_APP_upgrade_notify_cb     \033[0m\n");
        PR_ERR("download error reboot\n");
		AbnormalRestart();
    }
}

//To collect OTA files in fragments and write them to local files
OPERATE_RET __IPC_APP_get_file_data_cb(IN CONST FW_UG_S *fw, IN CONST UINT_T total_len,IN CONST UINT_T offset,
                             IN CONST BYTE_T *data,IN CONST UINT_T len,OUT UINT_T *remain_len, IN PVOID_T pri_data)
{
    // printf("Rev File Data");
    // printf("total_len:%d  fw_url:%s", total_len, fw->fw_url);
    // printf("Offset:%d Len:%d", offset, len);

    //report UPGRADE process, NOT only download percent, consider flash-write time
    //APP will report overtime fail, if uprgade process is not updated within 60 seconds

    int download_percent = (offset * 100) / (total_len+1);
    int report_percent = download_percent/2; // as an example, download 100% = 50%  upgrade work finished
    tuya_ipc_upgrade_progress_report(report_percent);

    FILE *p_upgrade_fd = (FILE *)pri_data;
    int ret = fwrite(data, 1, len, p_upgrade_fd);
	if( ret < 0 )
	{
		printf("__IPC_APP_get_file_data_cb, fwrite failed. errno : %d\n", errno);
		ret = 0;
	}
    *remain_len = len - ret;

    if(offset == total_len) // finished downloading
    {
        //start write OTA file to flash by parts
        /* only for example: 
        // finish 1st part
        report_percent+=10;
        tuya_ipc_upgrade_progress_report(report_percent);
        // finish 2nd part
        sleep(5);
        report_percent+=10;
        tuya_ipc_upgrade_progress_report(report_percent);
        // finish all parts, set to 90% for example
        report_percent = 90;
        tuya_ipc_upgrade_progress_report(report_percent);
		*/
    }

    //APP will report "uprage success" after reboot and new FW version is reported inside SDK automaticlly

    return OPRT_OK;
}


INT_T IPC_APP_Upgrade_Inform_cb(IN CONST FW_UG_S *fw)
{
    printf("Rev Upgrade Info");
	printf("fw->fw_url:%s", fw->fw_url);
	printf("fw->fw_md5:%s", fw->fw_md5);
	printf("fw->sw_ver:%s", fw->sw_ver);
	printf("fw->file_size:%u", fw->file_size);

	/// 释放系统资源
	IEventManager::instance()->notify("UpgradeReleaseResource", 0, appEventPulse, NULL, NULL, NULL);

	AppErr("stop onvif\n");
	g_OnvifHandle.Stop();
	AppErr("stop record\n");
	g_RecordManager.Stop();
	AppErr("stop siren\n");
    g_Siren.Stop();
//    AppErr("stop anbolight\n");
//    g_AnBaoLightManager.Stop();
    AppErr("stop alarm\n");
	g_Alarm.Stop();
    AppErr("stop camera\n");
    g_Camera.stop();
    AppErr("stop RealTimeStreamStop\n");
    g_AVManager.RealTimeStreamStop(DMC_MEDIA_TYPE_H264 | DMC_MEDIA_TYPE_H265 | DMC_MEDIA_TYPE_AUDIO);
    AppErr("stop RecordStreamStop\n");
    g_AVManager.RecordStreamStop(DMC_MEDIA_TYPE_H264 | DMC_MEDIA_TYPE_H265 | DMC_MEDIA_TYPE_AUDIO);
    AppErr("AudioDeInit\n");
    g_AVManager.AudioDeInit();
    AppErr("VideoDeInit\n");
    // g_AVManager.VideoDeInit();

	START_PROCESS("sh", "sh", "-c", "echo 3 > /proc/sys/vm/drop_caches;", NULL);
	sync();
	
	//获取剩余内存大小
	struct sysinfo si;
	sysinfo(&si);
	printf("---Totalram:		%d\n", si.totalram);
	printf("---Available: 	    %d\n", si.freeram);


	FILE *p_upgrade_fd = fopen(IPC_APP_UPGRADE_FILE, "w+b");
	if( NULL == p_upgrade_fd )
	{
		AppErr("gw_ug_inform_cb------------->fopen error! errno = %d\n", errno);
		AbnormalRestart();
		return 0;
	}
	
	memcpy(&FW_UG_g, fw, sizeof(FW_UG_S));
    tuya_ipc_upgrade_sdk(fw, __IPC_APP_get_file_data_cb, __IPC_APP_upgrade_notify_cb, p_upgrade_fd);

	return 0;
}

