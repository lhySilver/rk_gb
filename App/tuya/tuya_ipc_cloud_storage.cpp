/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName:    tuya_ipc_cloud_storage_demo
**********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/statfs.h>
#include <sys/time.h>

#include "include/sdk_inc/tuya_ipc_cloud_storage.h"
#include "include/tuya_ipc_media_handler.h"

#include "openssl/opensslconf.h"
#include "openssl/evp.h"
#include "openssl/engine.h"
#include "openssl/err.h"
#include "openssl/conf.h"

#include "Log/DebugDef.h"



/*
---------------------------------------------------------------------------------
云存储由内部管理线程获取存储订单的状态、数据的上传等。无需开发者实现逻辑。
无论终端用户购买的时连续存储或者事件存储模式，都需要开发者实现以下两个接口的调用.

tuya_ipc_cloud_storage_event_start: 在触发了事件，比如检测到有移动时调用；
tuya_ipc_cloud_storage_event_stop：在事件结束，比如见到到画面超过4秒钟无变化时调用；
---------------------------------------------------------------------------------
*/
#ifdef __cplusplus
	extern "C" {
#endif

extern OPERATE_RET OpensslAES_CBC128_encrypt(IN CONST BYTE_T *pdata_in,  IN CONST UINT_T data_len,
                                OUT BYTE_T *pdata_out,  OUT UINT_T *pdata_out_len,
                                IN CONST BYTE_T *pkey, IN BYTE_T *piv);
#ifdef __cplusplus
}
#endif

/* 云存储必须对数据进行加密以满足安全要求。
目前涂鸦采用AES CBC模式方式。上述接口时用软件实现的加密处理，在部分设备上无法满足实时性要求，
建议在这里替换成硬件加密的实现。
*/

OPERATE_RET AES_CBC_init(VOID)
{
    //Initialization operations required for encryption
    //If not, return directly
    return OPRT_OK;
}

/* 该临时接口使用CPU进行加密，在IPC SOC上运行延时不能满足要求 */
/*
extern OPERATE_RET OpensslAES_CBC128_encrypt(IN CONST BYTE_T *pdata_in,  IN CONST UINT_T data_len,
                                OUT BYTE_T *pdata_out,  OUT UINT_T *pdata_out_len,
                                IN CONST BYTE_T *pkey, IN BYTE_T *piv);
*/

/* 
pdata_in： 加密源数据，不能更改，否则会影响其他功能
data_len： 加密源数据大小
pdata_out：加密后数据存放地址，已经申请好空间，无需free
pdata_out_len： 加密后数据大小
pkey： 加密密钥key，16字节， 在SDK内部管理，与账号/设备有绑定关系
piv： 加密向量iv，16字节，在SDK内部管理
*/
OPERATE_RET AES_CBC_encrypt(IN BYTE_T *pdata_in,  IN UINT_T data_len,
                                 INOUT BYTE_T *pdata_out,  OUT UINT_T *pdata_out_len,
                                 IN BYTE_T *pkey, IN BYTE_T *piv)
{
    //Note that you cannot change the raw data in pdata_in
    //If the encrypted interface changes the original data, please copy pdata_in to other buffers first.
    return OpensslAES_CBC128_encrypt(pdata_in,data_len,pdata_out,pdata_out_len,pkey,piv);
}

OPERATE_RET AES_CBC_destory(VOID)
{
    //Need to add the anti-initialization operation required at the end of the program.
    //If not, return directly
    return OPRT_OK;
}

extern IPC_MEDIA_INFO_S s_media_info;
OPERATE_RET TUYA_APP_Enable_CloudStorage(VOID)
{
    AES_HW_CBC_FUNC aes_func;
    aes_func.init = AES_CBC_init;
    aes_func.encrypt = AES_CBC_encrypt;
    aes_func.destory = AES_CBC_destory;
    tuya_ipc_cloud_storage_init(&s_media_info, &aes_func);
    return OPRT_OK;
}


