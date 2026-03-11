#ifndef __config_x_h__
#define __config_x_h__
#if 0

#endif

	#define PRTSC_DIR 			""
	#define CONFIG_DIR 			"/userdata/conf/Config" 

    #define DEV_INFO            "/userdata/deviceinfo"
	#define DEV_INFO_BK			"/userdata/deviceinfo-bk"
    #define TMP_DEV_INFO        "/userdata/deviceinfo-tmp"

	#define LOG_DIR 			"/userdata/conf/Log"	
	#define DEV_CONFIG_FILE		"/userdata/product.cof"
	#define DEV_CONFIG_FILE_BK 	"/userdata/product.cof-bk"
	#define DEV_SYS_TIME_REC	"/userdata/systemTime"

	#define CUSTOM_DIR			"/userdata/conf/custom"		
	#define REBOOT_FLAG 		"/userdata/reboot.log"
	#define VOICE_PROMPT_FLAG   "/userdata/voice_prompt_flag"
	#define NO_VOICE_PROMPT_FLAG "/tmp/no_voice_prompt_flag"
	#define WIFI_FLAG           "/userdata/wifi8188"

    #define MMCP1               "/dev/mmcblk1p1"
    #define MOUNT_DIR           "/mnt/sdcard"
    #define MMC_DEV_CONFIG_FILE	"/mnt/sdcard/product.cof"
	#define AUDIO_SD_DIR        "/mnt/sdcard/audio" 
	#define AUDIO_CONFIG_DIR    "/userdata/audio"
	#define AUDIO_CONFIG_PATH   "/userdata"
	#define TUYA_NEW_PID_FILE 	"/mnt/sdcard/new_pid"	//Tuya新pid的路径 add on 2024.12.02

	#define IPC_APP_STORAGE_PATH  "/userdata/conf/tuya/" //涂鸦SDK内部配置文件存放路径，要求可读写，掉电不丢失
	#define IPC_APP_UPGRADE_FILE  "/tmp/upgrade.bin" 	 //OTA升级时下载的固件存放位置
	#define IPC_APP_SD_BASE_PATH  "/mnt/sdcard/" 		 // SD卡挂载目录：本地存储根目录
	#define AWB_TEST               "/userdata/awb_test"
	#define CAL_TEST               "/userdata/cal_test"
	#define PRO_TEST               "/userdata/PRO_test"
#include <iostream>
using namespace std;

#define starProcess( file, cmd, ... ) \
{\
	int status;\
	pid_t pid = vfork( );\
	printf("starProcess------------------->>>>>pid = %d, errno = %d\n", pid, errno);\
	if ( pid  == 0 )\
	{\
		execlp ( file, cmd, ##__VA_ARGS__ );\
		exit ( -1 );\
	}\
	waitpid ( pid, &status, 0 );\
}


#define printJsonValue(x) 								                    \
		{												                    \
			std::string outs;							                    \
			CConfigWriter writer;                                           \
			trace("printJsonValue\n");                                      \
			outs = writer.write(x);                                         \
			std::cout << "\t---zzb: " << __FILE__ << "-Line" << __LINE__;   \
			std::cout << ": "<< "=[";	                            \
			std::cout << outs << "]--" << std::endl;	                    \
		}

#endif
