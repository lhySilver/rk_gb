#ifndef __TUYA_IPC_COMMON_H_
#define __TUYA_IPC_COMMON_H_
#include <stdio.h>
#include "sdk_inc/tuya_cloud_types.h"
#include "sdk_inc/tuya_ipc_api.h"

#ifdef RC0240
#define IPC_APP_VERSION         "202.302.14" 			//显示在涂鸦APP上的固件版本信息
#elif defined  RC0240_LGV10
#define IPC_APP_VERSION         "202.305.01" 			//显示在涂鸦APP上的固件版本信息
#elif defined  RC0330_V20
#define IPC_APP_VERSION         "202.102.1" 			//显示在涂鸦APP上的固件版本信息
#endif

typedef struct
{
    CHAR_T storage_path[IPC_STORAGE_PATH_LEN + 1];/**< 涂鸦SDK内部配置文件存放路径，要求可读写，掉电不丢失 */
    CHAR_T upgrade_file_path[IPC_STORAGE_PATH_LEN + 1];/**< OTA文件保存位置 */
    CHAR_T sd_base_path[IPC_STORAGE_PATH_LEN + 1];/**< SD卡目录位置 */
    CHAR_T product_key[IPC_PRODUCT_KEY_LEN + 1]; /**< product key */
    CHAR_T uuid[IPC_UUID_LEN + 1]; /**< 指定涂鸦SDK使用的UUID,如果为空，那么使用配置文件中预先写入的值 */
    CHAR_T auth_key[IPC_AUTH_KEY_LEN + 1]; /**< 指定涂鸦SDK使用的auth_key,如果为空，那么使用配置文件中预先写入的值 */
    CHAR_T p2p_id[IPC_P2P_ID_LEN + 1]; /**< 指定涂鸦SDK使用的p2p id, 当前版本不需要指定，自动从云端获取 */
    CHAR_T dev_sw_version[IPC_SW_VER_LEN + 1]; /**< 设备固件版本号 */
    UINT_T max_p2p_user;/**< p2p最大数目 */
}IPC_MGR_INFO_S;

#endif 	//__TUYA_IPC_COMMON_H_
