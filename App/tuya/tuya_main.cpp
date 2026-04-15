#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/prctl.h>
#include <libgen.h>

#include <iostream>
#include <fstream>
#include <algorithm>

#include "Tuya.h"


#include "include/sdk_inc/tuya_hal_wifi.h"
#include "include/sdk_inc/tuya_ipc_skill.h"
#include "include/tuya_ipc_sdk_simple_start.h"
#include "include/tuya_ipc_upgrade_demo.h"
#include "include/tuya_ipc_sd_demo.h"
#include "include/tuya_ipc_p2p_demo.h"
#include "include/tuya_ipc_dp_utils.h"
#include "include/sdk_inc/tuya_ipc_doorbell.h"
#include "include/sdk_inc/tuya_hal_bt.h"
#include "include/sdk_inc/uni_log.h"

#include "Onvif/Onvif.h"
#include "Ble.h"

INT_T s_mqtt_status = 0;

INT_T g_time_zone = 0;
INT_T g_time_dst = 0;
int g_iBleInitFlag = 0;

#if defined BLE_AIC8800DL
bool lbh_ble_advertising = false;
bool lbh_ble_connected = false;

char g_lbh_ble_conidx = -1; //蓝牙连接的设备id
bool g_get_ble_notify_uuid = false; //是否获取到蓝牙notify服务的uuid
unsigned char g_lbh_ble_notify_uuid[16] = {0}; //蓝牙notify服务的uuid
#endif

IPC_MGR_INFO_S s_mgr_info = {0};

extern int iAllowMotorTrackTime;

extern bool bIsSyncUTCTime;

//tuya ring buffer初始化
extern int open_tuya_ring_buffer_handle();
//音视频初始化
extern int create_streamout_thread();
extern OPERATE_RET tuya_ipc_sdk_start(IN CONST TUYA_IPC_SDK_RUN_VAR_S * pRunInfo);

//------------------------------- 由于使用有线时，无法获取到有效的mqtt状态，故利用回调的方式来对有线、无线进行监控
static INT_T IPC_APP_Get_MqttStatus()
{
    return s_mqtt_status;
}
//---------------------------------


static void *sync_time_func(void *arg)
{
    while(1)
    {
        OPERATE_RET ret = OPRT_OK;

        /* 调用本API同步时间。如果返回OK，说明时间同步成功 */
        ret = IPC_APP_Sync_Utc_Time();
        if(ret == OPRT_OK)
        {
            printf("sync utc time\n");
			bIsSyncUTCTime = true;		//获取到时间，就启动录像
            sleep(60);
        }
        else
        {
            sleep(1);
        }
    }

    pthread_exit(0);
}
static void OnNetWorkReboot(Param wParam)
{
    printf("\033[1;36m  OnNetWorkReboot     \033[0m\n");
	printf("======================OnNetWorkReboot======================\n");

	if (0 != wParam)
	{
		int reboot_count = wParam;
		Json::Value table;
		table["count"] = reboot_count;
		AppErr("SET CFG_REBOOT count=[%d] \n", reboot_count);
		g_configManager.setConfig(getConfigName((CFG_REBOOT)), table, 0, IConfigManager::applyOK);
	}

	AbnormalRestart();
}
static bool s_bFirstBound = false;
static void *check_network_status_thread(void *arg)
{
	bool ret;
	int reboot_count = 0;
	int reboot_flag = 0;
	int offline_count = 0;
	int debug_count = 0;
    int mqtt_status = 0;
    CTimer timerNetWorkReboot;
	
	Json::Value table;
	g_configManager.getConfig(getConfigName(CFG_REBOOT), table);
	reboot_count = table["count"].asInt();
	AppErr("GET CFG_REBOOT count=[%d] \n", reboot_count);
	
	if (true == s_bFirstBound)
		sleep(2*60);
	
	while (1)
	{
        //插入网口连上服务器tuya_ipc_get_mqtt_status还是返回离线的，目前不知道什么原因
        //此出插入网口不检查网络状态
#if 0
        if(g_NetConfigHook.GetNetWorkLindMode() == NET_WORK_MODE_ETH0)
        {
            if (0 != reboot_count)
			{
				reboot_count = 0;
				
				table["count"] = reboot_count;
				AppErr("SET CFG_REBOOT count=[%d] \n", reboot_count);
				g_configManager.setConfig(getConfigName((CFG_REBOOT)), table, 0, IConfigManager::applyOK);				
			}

			if (1 == reboot_flag)
			{
				reboot_flag = 0;
				AppErr("stop m_timerNetWorkReboot\n");
				timerNetWorkReboot.Stop(false);
			}
            sleep(5);
            continue;
        }
		int ret = tuya_ipc_get_mqtt_status();
        printf("tuya_ipc_get_mqtt_status=%d\n",tuya_ipc_get_mqtt_status());
#else
        int ret = IPC_APP_Get_MqttStatus();
        printf("IPC_APP_Get_MqttStatus=%d\n",ret);
#endif
		if ( 1 == ret)
		{
			mqtt_status = 1;
			offline_count = 0;
		}
		else
		{
			mqtt_status = 0;
			offline_count++;
		}
		if (25 == offline_count) //2分钟后重连网络
		{
			AppErr("MqttCheck error cuont=%d\n", offline_count);
			g_NetConfigHook.ReConn();
		}
		else if (offline_count > 50)
			offline_count = 0;

		debug_count++;
		if (0 == (debug_count%12) && 0 == ret)
			AppErr("tuya mqtt offline.\n"); 
		
		if (0 == mqtt_status)
		{
			if (reboot_flag == 0)
			{
				reboot_flag = 1;
				
				const int MAX_TIME = 6*60*60; //最多等待6小时
				int reboot_delay = 30*60*(++reboot_count);
				
				if (reboot_delay > MAX_TIME)
					reboot_delay = MAX_TIME;
								
				AppErr("==========NetWorkReboot start timer delay time=%d============\n", reboot_delay);
				timerNetWorkReboot.Start(OnNetWorkReboot, reboot_delay*1000, 0, reboot_count);
			}
		}
		else
		{
			if (0 != reboot_count)
			{
				reboot_count = 0;
				
				table["count"] = reboot_count;
				AppErr("SET CFG_REBOOT count=[%d] \n", reboot_count);
				g_configManager.setConfig(getConfigName((CFG_REBOOT)), table, 0, IConfigManager::applyOK);				
			}

			if (1 == reboot_flag)
			{
				reboot_flag = 0;
				AppErr("stop m_timerNetWorkReboot\n");
				timerNetWorkReboot.Stop(false);
			}
		}
		sleep(5);
	}
	
    pthread_exit(0);
}

STATIC VOID __IPC_APP_Get_Net_Status_cb(IN CONST BYTE_T stat)
{
    AppErr("Net status change to:%d", stat);
    switch(stat)
    {
        case STAT_CLOUD_CONN:        //for wifi ipc //WiFi有线来回切换时，有概率会使Mqtt处于这个状态
        // case GB_STAT_CLOUD_CONN:     //for wired ipc
        case STAT_MQTT_ONLINE:
        {
            AppErr("mqtt is online\n");
            if (tuya_ipc_get_register_status() == E_IPC_ACTIVEATED)
            {
                AppErr("ENUM_LINK_INDICATOR_LIGHT_ALWAYS_ON\n");
                g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_POWER_INDICATOR_LIGHT_ALWAYS_OFF);
                g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_LINK_INDICATOR_LIGHT_ALWAYS_ON);
            }
            s_mqtt_status = 1;
            break;
        }
        case STAT_MQTT_OFFLINE:
        {
            AppErr("mqtt is offline\n");
            if (tuya_ipc_get_register_status() == E_IPC_ACTIVEATED)
            {
                AppErr("ENUM_INDICATOR_LIGHT_FAST_FLICKER\n"); 
			    g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_POWER_INDICATOR_LIGHT_FAST_FLICKER);
                g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_LINK_INDICATOR_LIGHT_ALWAYS_OFF);
            }
            s_mqtt_status = 0;
            break;
        }
        default:
        {
            break;
        }
    }
}
OPERATE_RET TUYA_IPC_SDK_START(WIFI_INIT_MODE_E connect_mode, CHAR_T *p_token)
{
	//获取sensor 确定主码流参数
	int main_videow = 3840;
	int main_videoh = 1080;
	int sub_videow = 1920;
	int sub_videoh = 544;
	int denominator = 15;
	int gop = denominator * 2;

    CaptureGetResolution(0,&main_videow,&main_videoh);
    CaptureGetResolution(1,&sub_videow,&sub_videoh);

    printf("main_video : %d,%d\n", main_videow,main_videoh);
    printf("sub_video  : %d,%d\n", sub_videow,sub_videoh);
	
    PR_DEBUG("SDK Version:%s\r\n", tuya_ipc_get_sdk_info());
    printf("SDK Version:%s\r\n", tuya_ipc_get_sdk_info());
	TUYA_IPC_SDK_RUN_VAR_S ipc_sdk_run_var ={0};
	memset(&ipc_sdk_run_var,0,sizeof(ipc_sdk_run_var));

	/*certification information(essential)*/

	strcpy(ipc_sdk_run_var.iot_info.product_key, (char *)DevInfoFromEEPROM_g.TUYA_PID);
	strcpy(ipc_sdk_run_var.iot_info.uuid,(char *)DevInfoFromEEPROM_g.TUYA_UID);
	strcpy(ipc_sdk_run_var.iot_info.auth_key,(char *)DevInfoFromEEPROM_g.TUYA_AUTHKEY);
	strcpy(ipc_sdk_run_var.iot_info.dev_sw_version,IPC_APP_VERSION);
	strcpy(ipc_sdk_run_var.iot_info.cfg_storage_path,IPC_APP_STORAGE_PATH);

	//normal power  device
	ipc_sdk_run_var.iot_info.dev_type= NORMAL_POWER_DEV;

/*
	printf("product_key : %s\n", ipc_sdk_run_var.iot_info.product_key);
	printf("uuid        : %s\n", ipc_sdk_run_var.iot_info.uuid);
	printf("auth_key    : %s\n", ipc_sdk_run_var.iot_info.auth_key);
	printf("connect_mode: %d\n", connect_mode);
*/

	/*connect mode (essential)*/
	ipc_sdk_run_var.net_info.connect_mode = connect_mode;
	ipc_sdk_run_var.net_info.net_status_change_cb = __IPC_APP_Get_Net_Status_cb;
	if(p_token)
	{
        strcpy(ipc_sdk_run_var.debug_info.qrcode_token,p_token);
	}

	ipc_sdk_run_var.debug_info.log_level = 0;

	/*media info (essential)*/
    /* main stream(HD), video configuration*/
    /* NOTE
    FIRST:If the main stream supports multiple video stream configurations, set each item to the upper limit of the allowed configuration.
    SECOND:E_IPC_STREAM_VIDEO_MAIN must exist.It is the data source of SDK.
    please close the E_IPC_STREAM_VIDEO_SUB for only one stream*/
    ipc_sdk_run_var.media_info.media_info.channel_enable[E_IPC_STREAM_VIDEO_MAIN] = TRUE;    /* Whether to enable local HD video streaming */
    ipc_sdk_run_var.media_info.media_info.video_fps[E_IPC_STREAM_VIDEO_MAIN] = denominator;  /* FPS */
    ipc_sdk_run_var.media_info.media_info.video_gop[E_IPC_STREAM_VIDEO_MAIN] = gop;  /* GOP */
    ipc_sdk_run_var.media_info.media_info.video_bitrate[E_IPC_STREAM_VIDEO_MAIN] = TUYA_VIDEO_BITRATE_2M; /* Rate limit */
    ipc_sdk_run_var.media_info.media_info.video_width[E_IPC_STREAM_VIDEO_MAIN] = main_videow; /* Single frame resolution of width*/
    ipc_sdk_run_var.media_info.media_info.video_height[E_IPC_STREAM_VIDEO_MAIN] = main_videoh;/* Single frame resolution of height */
    ipc_sdk_run_var.media_info.media_info.video_freq[E_IPC_STREAM_VIDEO_MAIN] = 90000; /* Clock frequency */
    ipc_sdk_run_var.media_info.media_info.video_codec[E_IPC_STREAM_VIDEO_MAIN] = TUYA_CODEC_VIDEO_H265; /* Encoding format */

    /* substream(HD), video configuration */
    /* Please note that if the substream supports multiple video stream configurations, please set each item to the upper limit of the allowed configuration. */
    ipc_sdk_run_var.media_info.media_info.channel_enable[E_IPC_STREAM_VIDEO_SUB] = TRUE;     /* Whether to enable local SD video stream */
    ipc_sdk_run_var.media_info.media_info.video_fps[E_IPC_STREAM_VIDEO_SUB] = denominator;  /* FPS */
    ipc_sdk_run_var.media_info.media_info.video_gop[E_IPC_STREAM_VIDEO_SUB] = gop;  /* GOP */
    ipc_sdk_run_var.media_info.media_info.video_bitrate[E_IPC_STREAM_VIDEO_SUB] = TUYA_VIDEO_BITRATE_1M; /* Rate limit */
    ipc_sdk_run_var.media_info.media_info.video_width[E_IPC_STREAM_VIDEO_SUB] = sub_videow; /* Single frame resolution of width */
    ipc_sdk_run_var.media_info.media_info.video_height[E_IPC_STREAM_VIDEO_SUB] = sub_videoh;/* Single frame resolution of height */
    ipc_sdk_run_var.media_info.media_info.video_freq[E_IPC_STREAM_VIDEO_SUB] = 90000; /* Clock frequency */
    ipc_sdk_run_var.media_info.media_info.video_codec[E_IPC_STREAM_VIDEO_SUB] = TUYA_CODEC_VIDEO_H264; /* Encoding format */

    /* Audio stream configuration.
    Note: The internal P2P preview, cloud storage, and local storage of the SDK are all use E_IPC_STREAM_AUDIO_MAIN data. */
    ipc_sdk_run_var.media_info.media_info.channel_enable[E_IPC_STREAM_AUDIO_MAIN] = TRUE;         /* Whether to enable local sound collection */
    ipc_sdk_run_var.media_info.media_info.audio_codec[E_IPC_STREAM_AUDIO_MAIN] = TUYA_CODEC_AUDIO_PCM;/* Encoding format */
    ipc_sdk_run_var.media_info.media_info.audio_sample [E_IPC_STREAM_AUDIO_MAIN]= TUYA_AUDIO_SAMPLE_8K;/* Sampling Rate */
    ipc_sdk_run_var.media_info.media_info.audio_databits [E_IPC_STREAM_AUDIO_MAIN]= TUYA_AUDIO_DATABITS_16;/* Bit width */
    ipc_sdk_run_var.media_info.media_info.audio_channel[E_IPC_STREAM_AUDIO_MAIN]= TUYA_AUDIO_CHANNEL_MONO;/* channel */
    ipc_sdk_run_var.media_info.media_info.audio_fps[E_IPC_STREAM_AUDIO_MAIN] = 25;/* Fragments per second */

    /*local storage (custome whether enable or not)*/
    ipc_sdk_run_var.local_storage_info.enable = 0;
    ipc_sdk_run_var.local_storage_info.max_event_num_per_day = 500;
    ipc_sdk_run_var.local_storage_info.skills = 0;//0 means all skills
    ipc_sdk_run_var.local_storage_info.sd_status_cb = tuya_ipc_sd_status_upload ;
	strcpy(ipc_sdk_run_var.local_storage_info.storage_path, IPC_APP_SD_BASE_PATH);

	/*cloud storage (custome whether enable or not)*/
    /*if no ase,it can equal NULL;*/
    extern OPERATE_RET AES_CBC_init(VOID);
    extern OPERATE_RET AES_CBC_encrypt(IN BYTE_T *pdata_in,  IN UINT_T data_len,
            INOUT BYTE_T *pdata_out,  OUT UINT_T *pdata_out_len,
            IN BYTE_T *pkey, IN BYTE_T *piv);
    extern OPERATE_RET AES_CBC_destory(VOID);
	ipc_sdk_run_var.cloud_storage_info.enable = 1;
	ipc_sdk_run_var.aes_hw_info.aes_fun.init = AES_CBC_init;
	ipc_sdk_run_var.aes_hw_info.aes_fun.encrypt =AES_CBC_encrypt;
	ipc_sdk_run_var.aes_hw_info.aes_fun.destory = AES_CBC_destory;


	/*p2p function (essential)*/
	ipc_sdk_run_var.p2p_info.max_p2p_client=5;
	ipc_sdk_run_var.p2p_info.live_mode = TRANS_DEFAULT_STANDARD;
	ipc_sdk_run_var.p2p_info.transfer_event_cb = __TUYA_APP_p2p_event_cb;
	ipc_sdk_run_var.p2p_info.rev_audio_cb = __TUYA_APP_rev_audio_cb;

	/*AI detect (custome whether enable or not)*/
	ipc_sdk_run_var.cloud_ai_detct_info.enable = 1;

	/*door bell (custome whether enable or not)*/
	ipc_sdk_run_var.video_msg_info.enable = 0;
	ipc_sdk_run_var.video_msg_info.type = MSG_BOTH;
	ipc_sdk_run_var.video_msg_info.msg_duration = 10;

	/*dp function(essential)*/
	ipc_sdk_run_var.dp_info.dp_query = IPC_APP_handle_dp_query_objs;
	ipc_sdk_run_var.dp_info.raw_dp_cmd_proc = IPC_APP_handle_raw_dp_cmd_objs;
	ipc_sdk_run_var.dp_info.common_dp_cmd_proc = IPC_APP_handle_dp_cmd_objs;

	/*upgrade function(essential)*/
	ipc_sdk_run_var.upgrade_info.enable = true;
	ipc_sdk_run_var.upgrade_info.upgrade_cb = IPC_APP_Upgrade_Inform_cb;

	ipc_sdk_run_var.iot_info.gw_reset_cb = IPC_APP_Reset_System_CB;
	ipc_sdk_run_var.iot_info.gw_restart_cb = IPC_APP_Restart_Process_CB;


	OPERATE_RET ret ;
    ret = tuya_ipc_sdk_start(&ipc_sdk_run_var);
    if(ret !=0 )
    {
        printf("ipc sdk v5 start fail,please check run parameter,ret=%d\n",ret);

    }
	return ret;
}

//涂鸦 ble 蓝牙接口
TY_BT_MSG_CB  bt_msg_cb = NULL;
tuya_ble_data_buf_t g_adv_data = {0};
tuya_ble_data_buf_t g_rsp_data = {0};
STATIC OPERATE_RET tuya_ext_bt_send(BYTE_T *data, UINT8_T len);
STATIC OPERATE_RET tuya_ext_bt_port_init(ty_bt_param_t *p);
STATIC OPERATE_RET tuya_ext_bt_port_deinit(void);
STATIC OPERATE_RET tuya_ext_bt_gap_disconnect(void);
STATIC OPERATE_RET tuya_ext_bt_reset_adv(tuya_ble_data_buf_t *adv, tuya_ble_data_buf_t *scan_resp);
STATIC OPERATE_RET tuya_ext_bt_start_adv(void);
STATIC OPERATE_RET tuya_ext_bt_stop_adv(void);
STATIC OPERATE_RET tuya_ext_bt_assign_scan(ty_bt_scan_info_t *info);
STATIC OPERATE_RET tuya_ext_bt_get_rssi(signed char *rssi);
STATIC OPERATE_RET tuya_ext_bt_start_scan();
STATIC OPERATE_RET tuya_ext_bt_stop_scan();
STATIC OPERATE_RET tuya_ext_bt_setmac(CONST NW_MAC_S *mac);
STATIC OPERATE_RET tuya_ext_bt_getmac(NW_MAC_S *mac);
STATIC TUYA_OS_BT_INTF m_tuya_os_bt_intfs;

// ble 蓝牙模块
static void tuya_ble_rec_data(unsigned char *data,int len)
{
    printf("tuya_ble_rec_data\n");
    if (bt_msg_cb)
    {
        printf("\nTY_BT_EVENT_RX_DATA\n");
        tuya_ble_data_buf_t databuf;
        databuf.data = (unsigned char*)(data);
        databuf.len = len;
        bt_msg_cb(0, TY_BT_EVENT_RX_DATA, &databuf);
    }
}

static void tuya_ble_event(int action)
{
    if (1 == action)
    {
        printf("BLE Connected\r\n");
        printf("\n<----\n");
        if (bt_msg_cb)
        {
            printf("\nTY_BT_EVENT_CONNECTED\n");
            tuya_ble_data_buf_t databuf;
            databuf.data = NULL;
            databuf.len = 0;
            bt_msg_cb(0, TY_BT_EVENT_CONNECTED,&databuf);
        }
    }
    else if (0 == action)
    {
        printf("BLE disconnected\r\n");
        printf("\n<----\n");
        if (bt_msg_cb)
        {
            printf("\nTY_BT_EVENT_DISCONNECTED\n");
            tuya_ble_data_buf_t databuf;
            databuf.data = NULL;
            databuf.len = 0;
            bt_msg_cb(0, TY_BT_EVENT_DISCONNECTED,&databuf);
            bt_msg_cb(0, TY_BT_EVENT_ADV_READY,&databuf);
        }
    }
    else
    {
        printf("tuya_ble_event unknown error\r\n");
    }
}

STATIC OPERATE_RET wifi_model_check(void)
{
    for(unsigned char mod = 0; mod < WIFI_NONE; mod ++)
    {
        if (g_NetConfigHook.GetCurlWifiModel() == mod)
        {
            return OPRT_OS_ADAPTER_OK;
        }
    }
    return OPRT_OS_ADAPTER_NOT_SUPPORTED;
}

#if defined BLE_AIC8800DL
/********************************************************* Ble.h的函数实现（24/11/18） 开始 *********************************************************/
void *lbh_client_socket(void *arg)
{
	prctl(PR_SET_NAME, "lbh_client_socket");
	//创建套接字去绑定服务端的套接字并注册消息回调，该函数会阻塞
	lbh_client_socket_and_msg_cb_init();
	return NULL;
}
static void lbh_ble_server_msg_cb(unsigned char *data, unsigned short len)
{
    BLE_HST_MSG msg;
    if(len > 0)
	{
		//获取消息ID
        msg.msg_id = data[0];
		//获取协议头后面的数据
        msg.len = len - LBH_SERVER_HEADER;
        msg.buff = data + LBH_SERVER_HEADER;
        switch(msg.msg_id)
		{
			//以下是一些在配网过程中会出现，而且需要处理的消息
            case BLE_INIT_DONE: //蓝牙初始化完成
                {
					//设置安全模式 0:no bond 1:just work 2:passkey display
                    app_ble_set_sec_mode(1);
					//设置设备名称
					char ble_gap_name[] = "AIC8800DC_BLE";
                    app_ble_gap_name_set_msg_send((uint8_t *)ble_gap_name, strlen(ble_gap_name));
					//设置广播参数
                    app_ble_adv_param_set_msg_send(160, 160, 8);
					//设置广播数据
					if(g_adv_data.data != NULL && g_adv_data.len > 0)
					{
						app_ble_adv_data_set_msg_send(g_adv_data.data, g_adv_data.len);
						g_adv_data.len = 0;
						free(g_adv_data.data);
					}
					//设置扫描响应
					if(g_rsp_data.data != NULL && g_rsp_data.len > 0)
					{
                    	app_ble_scan_response_set_msg_send(g_rsp_data.data, g_rsp_data.len);
						g_rsp_data.len = 0;
						free(g_rsp_data.data);
					}
					//设置默认最大传输单元，必须在连接前设置
                    app_ble_set_defalut_mtu(512);
					//启动蓝牙广播
                    app_ble_adv_start_msg_send();
                }
                break;
            case BLE_SMARTCONFIG_DATA_RECV: //数据接收
                {
					/*此时服务端发来的消息格式为：协议头(1 byte)+长度(2 byte)+连接索引(1 byte)+服务UUID(16 byte)+数据域(n byte)。
					因为msg.buff跳过了协议头和长度，因此再往后偏移17 byte就可以指向需要发给tuya SDK的数据域*/
					tuya_ble_rec_data(msg.buff + 17, msg.len - 17);
                }
                break;
            case BLE_CONNECTION_IND: //蓝牙连接
                {
					g_lbh_ble_conidx = msg.buff[0]; //配网时只连接一个设备，因此连接索引一般不会变化
					lbh_ble_connected = true;
					//通知tuya SDK蓝牙已连接
					tuya_ble_event(1);
                }
                break;
            case BLE_DISCONNECTED: //蓝牙断开
                {
					g_lbh_ble_conidx = -1; //重置连接索引
					lbh_ble_connected = false;
					//断开蓝牙后重新启动广播
					app_ble_adv_start_msg_send();
                    //通知tuya SDK蓝牙已断开
					tuya_ble_event(0);
                }
                break;
			case BLE_PKT_SIZE_IND: //空中包大小更新
                {
					if(g_lbh_ble_conidx != -1)
					{
						struct ble_pkt_size_ind *ptr = (struct ble_pkt_size_ind *)msg.buff;
						app_ble_set_data_len(g_lbh_ble_conidx, ptr->max_tx_octets, ptr->max_tx_time);
					}
                }
                break;
			case BLE_SERVICE_CHANGED_IND: //服务改变
                {
					//在这里把notify的uuid保存下来，涂鸦sdk调发送接口的时候要用
					unsigned short *lbh_uuid_type = (unsigned short *)(msg.buff + 13);
					//检查是不是notify服务的uuid
                    if (*lbh_uuid_type == 0x2B10) 
					{
						memcpy(g_lbh_ble_notify_uuid, msg.buff + 1, sizeof(g_lbh_ble_notify_uuid));
						g_get_ble_notify_uuid = true;
                    }
				}
				break;
			case BLE_ADV_ENABLE: //蓝牙广播已启动
				{
					lbh_ble_advertising = true;
				}
				break;
            case BLE_ADV_DISABLE: //蓝牙广播已关闭
				{
					lbh_ble_advertising = false;
				}
				break;

			//以下是一些在配网过程中会出现，但是不需要处理的消息
			case BLE_MSG_DONE:                //客户端消息发送完成
			case BLE_CONNECTION_PARAM_UPDATE: //连接参数更新
			case BLE_NTF_SENT_DONE:           //NTF发送完成
			case BLE_CON_UPDATE_CMP_DONE:     //连接参数更新完成
                break;

			//正常配网过程中不会出现的消息
            default:
				AppWarning("Unprocessed lbh server msg: 0x%X\n", msg.msg_id);
                break;
        }
    }
}
void lib_ble_main_init(void)
{
	int time_cut = 0;
    pthread_t thread;
	//启动蓝牙lbh服务进程，并指定uuid配置文件路径
	// START_PROCESS("sh", "sh", "-c", "/oem/usr/bin/aic8800_ble/lbh_server -p /oem/usr/ko/aic8800dl/ble_userconfig.json -s ble usb &", NULL);
    START_PROCESS("sh", "sh", "-c", "/oem/usr/bin/lbh_server -p /oem/usr/ko/aic8800dl/ble_userconfig.json -s ble usb &", NULL);
	//服务起来后会创建一个socket文件
	while(access("/tmp/app_queue_tool_socket", F_OK) != 0)
	{
		usleep(100*1000);
		time_cut ++;
		if(time_cut > 50)//服务一般起来很快，因此只设置5s超时
		{
            AppErr("lbh_server start error\n");
			return;
		}
	}
    //指定数据接收回调函数
	app_queue_reg_callback(lbh_ble_server_msg_cb);
	// 创建线程
    if (pthread_create(&thread, NULL, lbh_client_socket, NULL))
	{
		AppErr("Error creating thread: lbh_client_socket.\n");
        return;
    }
	// 设置线程为分离状态
    pthread_detach(thread);
}
#define CMDLINE_PATH_SIZE 256
static int find_process_pid(void)
{
    DIR *dir;
    struct dirent *entry;
    char cmdline_path[CMDLINE_PATH_SIZE];
    FILE *file;
    char cmdline[CMDLINE_PATH_SIZE];
    
    dir = opendir("/proc");
    if (dir == NULL){
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && entry->d_name[0] != '.') {
            snprintf(cmdline_path, CMDLINE_PATH_SIZE, "/proc/%s/cmdline", entry->d_name);
            file = fopen(cmdline_path, "r");
            if (file == NULL) {
                continue;
            }
            if (fgets(cmdline, CMDLINE_PATH_SIZE, file) != NULL) {
                cmdline[strcspn(cmdline, "\n")] = 0;

                if (strstr(cmdline, "lbh_server") != NULL) {
                    fclose(file);
                    closedir(dir);
                    return atoi(entry->d_name);
                }
            }
            fclose(file);
        }
    }
    closedir(dir);
    return -1; // Not found
}
static int kill_lbh_server(void)
{
	char cmd_kill[32];
    int pid = find_process_pid();
    if (pid != -1)
    {
        printf(" ==========================>> lbh_server process id: %d\n", pid);
        snprintf(cmd_kill, 32, "kill -9 %d", pid);
        START_PROCESS_CLOSE_FD("sh", "sh", "-c", cmd_kill, NULL);
    }
    else
    {
        printf(" ==========================>> lbh_server process id not found.\n");
        START_PROCESS("sh", "sh", "-c", "killall lbh_server", NULL);
    }
	
    return 0;
}

void lib_ble_main_exit(void)
{
	int time_cut = 0;
    if (lbh_ble_advertising)
	{
        app_ble_adv_stop_msg_send();
    }
    if (lbh_ble_connected && g_lbh_ble_conidx > -1)
	{
        app_ble_disconnect_msg_send(g_lbh_ble_conidx);//断开与server的连接
		lbh_ble_connected = false; //断开之后收不到服务器的消息，所以直接置为false
    }
    while(lbh_ble_advertising || (lbh_ble_connected && g_lbh_ble_conidx > -1))
	{
		printf("adv: %d, conn: %d\n", lbh_ble_advertising, lbh_ble_connected);
        usleep(100*1000);
		time_cut ++;
		if(time_cut >= 10)
		{
			break;
		}
    }
	//关闭8800的蓝牙服务进程
    kill_lbh_server();
	return;
}

int ble_smt_advertise_adv_data(unsigned char* data, int len)
{
	//储存蓝牙广播数据
	g_adv_data.data = (unsigned char *)malloc(len);
	if(g_adv_data.data != NULL)
	{
		g_adv_data.len = len;
		memcpy(g_adv_data.data, data, len);
	}
	return 0;
}

int  ble_smt_advertise_adv_resp_data(unsigned char* data, int len)
{
	//储存蓝牙扫描响应数据
	g_rsp_data.data = (unsigned char *)malloc(len);
	if(g_rsp_data.data != NULL)
	{
		g_rsp_data.len = len;
		memcpy(g_rsp_data.data, data, len);
	}
	return 0;
}

int ble_smt_send_ble(const char *data, int len)
{
#define BUFFER_SIZE 128
	//如果未获取到notify服务的uuid，或要发送的数据超长，返回发送失败
	if((g_get_ble_notify_uuid == false) || (len > (BUFFER_SIZE - sizeof(g_lbh_ble_notify_uuid))))
	{
		AppErr("ble_smt_send_ble error\n");
		return -1;
	}
	//把uuid和data组合起来
	unsigned char buff[BUFFER_SIZE] = {0};
	memcpy(buff, g_lbh_ble_notify_uuid, sizeof(g_lbh_ble_notify_uuid));
	memcpy(buff + sizeof(g_lbh_ble_notify_uuid), data, len);
	//发送数据
	return app_ble_smartconfig_send_notification(buff, len + sizeof(g_lbh_ble_notify_uuid));
}
/********************************************************* Ble.h的函数实现（24/11/18） 结束 *********************************************************/

/**
 * @brief 用于重置蓝牙广播内容
 *
 * @param[in]       adv
  * @param[in]      scan_resp
 * @return int 0=成功，非0=失败
 */
STATIC OPERATE_RET tuya_ext_bt_reset_adv(tuya_ble_data_buf_t *adv, tuya_ble_data_buf_t *scan_resp)
{
	if (WIFI_BLE_NOT_SUPPORT == g_NetConfigHook.GetCurlWifiBleSupport())
    {
        return OPRT_OS_ADAPTER_OK;
    }
    if( true == g_TuyaHandle.GetRegisterStatus() )
    {
        return OPRT_OS_ADAPTER_OK;
    }
    // if (NET_WORK_MODE_NONE != g_NetConfigHook.GetNetWorkLindMode())
    // {
    //     return OPRT_OS_ADAPTER_OK;
    // }
	//转存蓝牙广播数据，待蓝牙服务起来之后再发给蓝牙服务端
	ble_smt_advertise_adv_data(adv->data, adv->len);
    ble_smt_advertise_adv_resp_data(scan_resp->data, scan_resp->len);
	//启动蓝牙服务
    printf("\033[1;32m ==========================lib_ble_main_init============================ \033[0m\n");
    if(0 == g_iBleInitFlag)
    {
        lib_ble_main_init();
        g_iBleInitFlag = 1;
    }
    return 0;
}

//////////////////////////ble///////////////////////////
#elif (defined BLE_ATBM6062 || defined BLE_ATBM6012B || defined BLE_ATBM6132)
// ble 蓝牙模块

// static int ble_smt_start_flag = 0;
// static void *ble_smt_loop(void *arg)
// {
//     if (ble_smt_start_flag == 0)
//     {
//         printf("==========================ble_smt_loop_init loop============================\n");
//         ble_smt_start_flag = 1;

//         // 该接口阻塞内部循环 
//         // 调用ble_smt_exit 退出
//         // 退出前如果ble_smt_start需要ble_smt_stop
//         ble_smt_loop_init();
//         printf("==========================ble_smt_loop_init exit============================\n");
//         ble_smt_start_flag = 0;
//     }

//     pthread_exit(0);
// }

/**
 * @brief 用于重置蓝牙广播内容
 *
 * @param[in]       adv
  * @param[in]      scan_resp
 * @return int 0=成功，非0=失败
 */
STATIC OPERATE_RET tuya_ext_bt_reset_adv(tuya_ble_data_buf_t *adv, tuya_ble_data_buf_t *scan_resp)
{
    if (WIFI_BLE_NOT_SUPPORT == g_NetConfigHook.GetCurlWifiBleSupport())
    {
        return OPRT_OS_ADAPTER_OK;
    }
    if( true == g_TuyaHandle.GetRegisterStatus() )
    {
        return OPRT_OS_ADAPTER_OK;
    }
    // if (NET_WORK_MODE_NONE != g_NetConfigHook.GetNetWorkLindMode())
    // {
    //     return OPRT_OS_ADAPTER_OK;
    // }
    AppErr("tuya_ext_bt_reset_adv\n");
    int ret = 0;
    #if 0
    //u8 scan_resp[] = {0x04, 0x09, 0x53, 0x53, 0x53,0x19, 0xFF, 0xD0, 0x07, 0x01, 0x03, 0x00, 0x00, 0x0C, 0x00, 0x88, 0xD1, 0xC4, 0x89, 0x2B, 0x56, 0x7D, 0xE5, 0x65, 0xAC, 0xA1, 0x3F, 0x09, 0x1C, 0x43, 0x92};
    u8 scan_resp[] = {  0x03, 0x09, 0x54, 0x59, 
                        0x19, 0xff, 0xd0, 0x07, 0x09, 0x03, 0x03, 0x00, 0x0c, 0x00, 0x1c, 0x20, 0x90, 0x55, 0x9f, 0x8a, 0xc6, 0x13, 0x81, 0x1e, 0x0c, 0xa4, 0xf4, 0x58, 0x12, 0x4a};

    //u8 adv_data[] = {0x02, 0x01, 0x06, 0x03, 0x02, 0x01, 0xA2, 0x14, 0x16, 0x01, 0xA2, 0x01, 0x6B, 0x65, 0x79, 0x79, 0x66, 0x67, 0x35, 0x79, 0x33, 0x34, 0x79, 0x71, 0x78, 0x71, 0x67, 0x64};
    u8 adv_data[] = {   0x02, 0x01, 0x06,       /*** 0x01 - Flags. */
                        0x03, 0x02, 0x01, 0xa2, /*** 0x02,0x03 - 16-bit service class UUIDs. */
                        0x14, 0x16, 0x01, 0xa2, 0x00, 0x79, 0x79, 0x69, 0x72, 0x36, 0x79, 0x7a, 0x6a, 0x75, 0x79, 0x36, 0x33, 0x78, 0x65, 0x6f, 0x61}; /*** 0x16 - Service data - 16-bit UUID. */
    #else
    ble_smt_set_ble_cb(tuya_ble_event,tuya_ble_rec_data);
    int i = 0;
    printf("\n========0adv data=========\n");
    for(i=0;i < adv->len;i++){
        printf("0x%02x, ", adv->data[i]);

    }
    printf("\n========0resp data========\n");

    for(i=0;i < scan_resp->len;i++){
        printf("0x%02x, ", scan_resp->data[i]);
    }
    printf("\n");

    AppErr("ble_smt_advertise_adv_data\n");
    ble_smt_advertise_adv_data(adv->data,adv->len);
    AppErr("ble_smt_advertise_adv_resp_data\n");
    ble_smt_advertise_adv_resp_data(scan_resp->data, scan_resp->len);

    AppErr("lib_ble_main_init\n");
    printf("\033[1;32m ==========================lib_ble_main_init============================ \033[0m\n");
    
    if(0 == g_iBleInitFlag)
    {
        lib_ble_main_init();
        g_iBleInitFlag = 1;
    }
    #endif
    return 0;
}

//////////////////////////ble///////////////////////////
#endif
STATIC OPERATE_RET tuya_ext_bt_send(BYTE_T *data, UINT8_T len)
{
    if (WIFI_BLE_NOT_SUPPORT == g_NetConfigHook.GetCurlWifiBleSupport())
    {
        return OPRT_OS_ADAPTER_OK;
    }
    if( true == g_TuyaHandle.GetRegisterStatus() )
    {
        return OPRT_OS_ADAPTER_OK;
    }
    // if (NET_WORK_MODE_NONE != g_NetConfigHook.GetNetWorkLindMode())
    // {
    //     return OPRT_OS_ADAPTER_OK;
    // }
    AppErr("tuya_ext_bt_send\n");
    OPERATE_RET op_ret = OPRT_OK;
    if (data == NULL || len == 0) {
        return OPRT_INVALID_PARM;
    }
    op_ret = ble_smt_send_ble((char *)data, len);
    AppErr("ble_smt_send_ble=%d\n",op_ret);

    return op_ret;
}

STATIC OPERATE_RET tuya_ext_bt_port_init(ty_bt_param_t *p)
{
    OPERATE_RET op_ret = OPRT_OK;
    if (WIFI_BLE_NOT_SUPPORT == g_NetConfigHook.GetCurlWifiBleSupport() )
    {
        return OPRT_OS_ADAPTER_OK;
    }
    if( true == g_TuyaHandle.GetRegisterStatus() )
    {
        return OPRT_OS_ADAPTER_OK;
    }
    AppErr("tuya_ext_bt_port_init\n");
    // // 获取配置信息
    // NetWifiConfig m_ConfigWifi;
	// Json::Value WifiTable;
	// g_configManager.getConfig(getConfigName(CFG_WIFI), WifiTable);
	// TExchangeAL<NetWifiConfig>::getConfig(WifiTable, m_ConfigWifi);

    // printf("m_ConfigWifi.strSSID=%s\n",m_ConfigWifi.strSSID.c_str());
    // printf("m_ConfigWifi.strSSID.length=%d\n",m_ConfigWifi.strSSID.length());
    // printf("m_ConfigWifi.strKeys=%s\n",m_ConfigWifi.strKeys.c_str());
    // printf("m_ConfigWifi.strKeys.length=%d\n",m_ConfigWifi.strKeys.length());

    // if ( !(m_ConfigWifi.strSSID == ""))
    // {
    //     return OPRT_OS_ADAPTER_OK;
    // }

    // if (NET_WORK_MODE_NONE != g_NetConfigHook.GetNetWorkLindMode())
    // {
    //     return OPRT_OS_ADAPTER_OK;
    // }

    g_adv_data.data = NULL;
	g_adv_data.len = 0;
	g_rsp_data.data = NULL;
	g_rsp_data.len = 0;

    if (p) {
        bt_msg_cb  = p->cb;
    }

    return op_ret;
}
STATIC OPERATE_RET tuya_ext_bt_port_deinit(void)
{
    if (WIFI_BLE_NOT_SUPPORT == g_NetConfigHook.GetCurlWifiBleSupport())
    {
        return OPRT_OS_ADAPTER_OK;
    }
    if( true == g_TuyaHandle.GetRegisterStatus() )
    {
        return OPRT_OS_ADAPTER_OK;
    }
    // if (NET_WORK_MODE_NONE != g_NetConfigHook.GetNetWorkLindMode())
    // {
    //     return OPRT_OS_ADAPTER_OK;
    // }
    AppErr("tuya_ext_bt_port_deinit\n");
    return OPRT_OS_ADAPTER_OK;
}

/**
 * @brief 用于断开蓝牙连接
 *
 * @return int 0=成功，非0=失败
 */
STATIC OPERATE_RET tuya_ext_bt_gap_disconnect(void)
{
    if (WIFI_BLE_NOT_SUPPORT == g_NetConfigHook.GetCurlWifiBleSupport())
    {
        return OPRT_OS_ADAPTER_OK;
    }
    if( true == g_TuyaHandle.GetRegisterStatus() )
    {
        return OPRT_OS_ADAPTER_OK;
    }
    // if (NET_WORK_MODE_NONE != g_NetConfigHook.GetNetWorkLindMode())
    // {
    //     return OPRT_OS_ADAPTER_OK;
    // }
    AppErr("tuya_ext_bt_gap_disconnect\n");
    OPERATE_RET ret = OPRT_OK;
    return ret;
}

/**
 * @brief 用于启动蓝牙广播
 *
 * @return int 0=成功，非0=失败
 */
STATIC OPERATE_RET tuya_ext_bt_start_adv(void)
{
    if (WIFI_BLE_NOT_SUPPORT == g_NetConfigHook.GetCurlWifiBleSupport())
    {
        return OPRT_OS_ADAPTER_OK;
    }
    if( true == g_TuyaHandle.GetRegisterStatus() )
    {
        return OPRT_OS_ADAPTER_OK;
    }
    // if (NET_WORK_MODE_NONE != g_NetConfigHook.GetNetWorkLindMode())
    // {
    //     return OPRT_OS_ADAPTER_OK;
    // }
    AppErr("tuya_ext_bt_start_adv\n");
    return OPRT_OK;
}

/**
 * @brief 用于停止蓝牙广播
 *
 * @return int 0=成功，非0=失败
 */
STATIC OPERATE_RET tuya_ext_bt_stop_adv(void)
{
    if (WIFI_BLE_NOT_SUPPORT == g_NetConfigHook.GetCurlWifiBleSupport())
    {
        return OPRT_OS_ADAPTER_OK;
    }
    if( true == g_TuyaHandle.GetRegisterStatus() )
    {
        return OPRT_OS_ADAPTER_OK;
    }
    // if (NET_WORK_MODE_NONE != g_NetConfigHook.GetNetWorkLindMode())
    // {
    //     return OPRT_OS_ADAPTER_OK;
    // }
    AppErr("tuya_ext_bt_stop_adv\n");
    return OPRT_OK;
}

/**
 * @brief 用于扫描蓝牙信标(厂测使用)
 *
 * @param[out]       rssi
 * @return int 0=成功，非0=失败
 */
STATIC OPERATE_RET tuya_ext_bt_assign_scan(ty_bt_scan_info_t *info)
{
    if (WIFI_BLE_NOT_SUPPORT == g_NetConfigHook.GetCurlWifiBleSupport())
    {
        return OPRT_OS_ADAPTER_OK;
    }
    if( true == g_TuyaHandle.GetRegisterStatus() )
    {
        return OPRT_OS_ADAPTER_OK;
    }
    // if (NET_WORK_MODE_NONE != g_NetConfigHook.GetNetWorkLindMode())
    // {
    //     return OPRT_OS_ADAPTER_OK;
    // }
    AppErr("tuya_ext_bt_assign_scan\n");
    return OPRT_OK;
}

/**
 * @brief 用于获取蓝牙信号强度
 *
 * @param[out]       rssi
 * @return int 0=成功，非0=失败
 */
STATIC OPERATE_RET tuya_ext_bt_get_rssi(signed char *rssi)
{
    if (WIFI_BLE_NOT_SUPPORT == g_NetConfigHook.GetCurlWifiBleSupport())
    {
        return OPRT_OS_ADAPTER_OK;
    }
    if( true == g_TuyaHandle.GetRegisterStatus() )
    {
        return OPRT_OS_ADAPTER_OK;
    }
    // if (NET_WORK_MODE_NONE != g_NetConfigHook.GetNetWorkLindMode())
    // {
    //     return OPRT_OS_ADAPTER_OK;
    // }
    AppErr("tuya_ext_bt_get_rssi\n");
    return OPRT_OK;
}

STATIC OPERATE_RET tuya_ext_bt_start_scan()
{
    if (WIFI_BLE_NOT_SUPPORT == g_NetConfigHook.GetCurlWifiBleSupport())
    {
        return OPRT_OS_ADAPTER_OK;
    }
    if( true == g_TuyaHandle.GetRegisterStatus() )
    {
        return OPRT_OS_ADAPTER_OK;
    }
    // if (NET_WORK_MODE_NONE != g_NetConfigHook.GetNetWorkLindMode())
    // {
    //     return OPRT_OS_ADAPTER_OK;
    // }
    AppErr("tuya_ext_bt_start_scan\n");
    return OPRT_OS_ADAPTER_BT_SCAN_FAILED;
}

STATIC OPERATE_RET tuya_ext_bt_stop_scan()
{
    if (WIFI_BLE_NOT_SUPPORT == g_NetConfigHook.GetCurlWifiBleSupport())
    {
        return OPRT_OS_ADAPTER_OK;
    }
    AppErr("tuya_ext_bt_stop_scan\n");
    return OPRT_OS_ADAPTER_BT_SCAN_FAILED;
}

STATIC OPERATE_RET tuya_ext_bt_setmac(CONST NW_MAC_S *mac)
{
    if (WIFI_BLE_NOT_SUPPORT == g_NetConfigHook.GetCurlWifiBleSupport())
    {
        return OPRT_OS_ADAPTER_OK;
    }
    if( true == g_TuyaHandle.GetRegisterStatus() )
    {
        return OPRT_OS_ADAPTER_OK;
    }
    // if (NET_WORK_MODE_NONE != g_NetConfigHook.GetNetWorkLindMode())
    // {
    //     return OPRT_OS_ADAPTER_OK;
    // }
    AppErr("tuya_ext_bt_setmac\n");
    OPERATE_RET ret = OPRT_OK;

    if (mac == NULL) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    // ret = wd_common_write(BT_MAC_ADDR_PARAM, mac->mac, TY_MAC_ADDR_LEN);
    // if (ret < 0) {
    //     return OPRT_OS_ADAPTER_BT_INIT_FAILED;
    // }

    // TUYA_BTUART_INFO("tuya_ext_bt_setmac %x:%x:%x:%x:%x:%x",
    //          mac->mac[0], mac->mac[1], mac->mac[2], mac->mac[3],
    //          mac->mac[4], mac->mac[5]);

    return OPRT_OS_ADAPTER_OK;
}

STATIC OPERATE_RET tuya_ext_bt_getmac(NW_MAC_S *mac)
{
    if (WIFI_BLE_NOT_SUPPORT == g_NetConfigHook.GetCurlWifiBleSupport())
    {
        return OPRT_OS_ADAPTER_OK;
    }
    if( true == g_TuyaHandle.GetRegisterStatus() )
    {
        return OPRT_OS_ADAPTER_OK;
    }
    // if (NET_WORK_MODE_NONE != g_NetConfigHook.GetNetWorkLindMode())
    // {
    //     return OPRT_OS_ADAPTER_OK;
    // }
    AppErr("tuya_ext_bt_getmac\n");
    OPERATE_RET ret = OPRT_OK;
    BYTE_T *p_mac = NULL;
    UINT_T len = 0;

    if (mac == NULL) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    // ret = wd_common_read(BT_MAC_ADDR_PARAM, &p_mac, &len);
    // if (ret < 0) {
    //     return OPRT_OS_ADAPTER_BT_INIT_FAILED;
    // }

    // memcpy(mac->mac, p_mac, TY_MAC_ADDR_LEN);
    // TUYA_BTUART_INFO("tuya_ext_bt_getmac %x:%x:%x:%x:%x:%x",
    //          mac->mac[0], mac->mac[1], mac->mac[2], mac->mac[3],
    //          mac->mac[4], mac->mac[5]);

    return OPRT_OS_ADAPTER_OK;
}

bool g_bTuyaApMode = false;
INT_T tuay_main()
{
    INT_T res = -1;
    CHAR_T token[30] = {0};
    WIFI_INIT_MODE_E mode = WIFI_INIT_AP;

	//<shang>判断配置文件存储目录是否存在，不存在则创建
	if(access(IPC_APP_STORAGE_PATH, F_OK) == -1)
	{
		AppErr("%s is not exsit, create now\n",IPC_APP_STORAGE_PATH);
		mkdir(IPC_APP_STORAGE_PATH, 0777);
	
	}

    m_tuya_os_bt_intfs.port_init      = tuya_ext_bt_port_init;
    m_tuya_os_bt_intfs.port_deinit    = tuya_ext_bt_port_deinit;
    m_tuya_os_bt_intfs.gap_disconnect = tuya_ext_bt_gap_disconnect;
    m_tuya_os_bt_intfs.send           = tuya_ext_bt_send;
    m_tuya_os_bt_intfs.reset_adv      = tuya_ext_bt_reset_adv;
    m_tuya_os_bt_intfs.get_rssi       = tuya_ext_bt_get_rssi;
    m_tuya_os_bt_intfs.start_adv      = tuya_ext_bt_start_adv;
    m_tuya_os_bt_intfs.stop_adv       = tuya_ext_bt_stop_adv;
    m_tuya_os_bt_intfs.assign_scan    = tuya_ext_bt_assign_scan;
    m_tuya_os_bt_intfs.scan_init      = NULL;
    m_tuya_os_bt_intfs.start_scan     = tuya_ext_bt_start_scan;
    m_tuya_os_bt_intfs.stop_scan      = tuya_ext_bt_stop_scan;
    m_tuya_os_bt_intfs.set_mac        = tuya_ext_bt_setmac;
    m_tuya_os_bt_intfs.get_mac        = tuya_ext_bt_getmac;
    tuya_os_adapt_reg_intf(INTF_BT, &m_tuya_os_bt_intfs);
    //tuya_os_bt_intf_init();//ble
    
    int ret = -1;
    ret = TUYA_IPC_SDK_START(mode, token);
    if(ret != 0)
    {
        return 0;
    }
    AppErr("TUYA_IPC_SDK_START ok\n");
    while(tuya_ipc_get_register_status() != E_IPC_ACTIVEATED)
    {
        s_bFirstBound = true;
        usleep(100000);
    }


    //蓝牙初始化之后，才能够退出
    if( (1 == g_iBleInitFlag) && (WIFI_BLE_SUPPORT == g_NetConfigHook.GetCurlWifiBleSupport()) )
    {
        printf("\033[1;32m ==========================lib_ble_main_exit============================ \033[0m\n");
        lib_ble_main_exit();
        g_iBleInitFlag = 0;
    }
	g_NetConfigHook.SetWifiSwitch(true);
    
    //检测网络线程
    CreateDetachedThread((char*)"check_network_status_thread",check_network_status_thread, (void *)NULL, true);
	//校时线程
	CreateDetachedThread((char*)"sync_time_func",sync_time_func, (void *)NULL, true);
    
    return 0;
}

