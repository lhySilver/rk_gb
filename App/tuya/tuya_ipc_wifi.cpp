/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName: tuya_linux_wifi_demo.c
  *
  * 文件描述：
  * 1. WIFI操作API实现
  *
  * 开发者工作：
  * 1. 根据SSID和PASSWD连接WIFI。
  * 2. 抓取图片并且识别二维码。
  * 3. 特定WIFI芯片适配。
  *
**********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include "include/sdk_inc/tuya_iot_config.h"
#if defined(WIFI_GW) && (WIFI_GW==1)

#include "include/sdk_inc/cJSON.h"

#include "include/sdk_inc/tuya_hal_wifi.h"
//#include "include/sdk_inc/tuya_os_adapt.h"

#include "include/sdk_inc/tuya_ipc_api.h"

#include "include/tuya_ipc_system_control.h"

#include "Tuya.h"

#include "Ble.h"

#define WLAN_DEV    "wlan0"
#define MAX_AP_SEARCH (20)
int net_peiwang_status = 0;
int success_get_ip = 0;
static int s_all_wifi_num = 0;
static AP_IF_S s_all_wifi_list[MAX_AP_SEARCH];
extern int g_iBleInitFlag;

//降序排序
int descending_sort_wifi_list_by_quality(AP_IF_S *wifi_lsit, int num)
{
	AP_IF_S stWifi;
	
	if (!wifi_lsit || num  > MAX_AP_SEARCH || num < 2)
		return -1;

	for (int i = 0; i < num-1; i++)
	{
		for (int j = 0; j < num-i-1; j++)
		{
			if (wifi_lsit[j].rssi < wifi_lsit[j+1].rssi)
			{
				memcpy(&stWifi, &wifi_lsit[j], sizeof(AP_IF_S));
				memcpy(&wifi_lsit[j], &wifi_lsit[j+1], sizeof(AP_IF_S));
				memcpy(&wifi_lsit[j+1], &stWifi, sizeof(AP_IF_S));
			}
		}
	}
    return 0;
}

//
int filter_wifi(const char *essid, AP_IF_S *pstWifi)
{
	if (!essid || !pstWifi)
		return -1;

	//先查找搜索结果中是否存在该AP
	for (int i = 0; i < s_all_wifi_num; i++)
	{
		if (strcmp((char *)s_all_wifi_list[i].ssid, essid) == 0)
		{
			memcpy(pstWifi, &s_all_wifi_list[i], sizeof(AP_IF_S));
			return 0;
		}
	}

	char *p;
	p = strstr((char*)essid, "_5G");
	if (NULL == p)
		p = strstr((char*)essid, "-5G");
	if (NULL == p)
		return -1;

	char new_essid[65];
	memset(new_essid, '\0', sizeof(new_essid));
	strncpy(new_essid, essid, p-essid);
	strcpy(new_essid+(p-essid), p+3);
	printf("1 - new essid: %s\n", new_essid);
	
	for (int i = 0; i < s_all_wifi_num; i++)
	{
		printf("search ssid is %s    new_essid is %s\n",s_all_wifi_list[i].ssid,new_essid);
		if (strcmp((char *)s_all_wifi_list[i].ssid, new_essid) == 0)
		{
			memcpy(pstWifi, &s_all_wifi_list[i], sizeof(AP_IF_S));
			return 0;
		}
	}

	memset(new_essid, '\0', sizeof(new_essid));
	strncpy(new_essid, essid, p-essid+1);
	strncpy(new_essid+(p-essid+1), "2.4G", strlen("2.4G"));
	strcpy(new_essid+(p-essid+1)+strlen("2.4G"), p+3);
	printf("2 - new essid: %s\n", new_essid);

	for (int i = 0; i < s_all_wifi_num; i++)
	{
		if (strcmp((char *)s_all_wifi_list[i].ssid, new_essid) == 0)
		{
			memcpy(pstWifi, &s_all_wifi_list[i], sizeof(AP_IF_S));
			return 0;
		}
	}
	
	return -1;
}


/*
用户需要实现的接口：
hwl_wf_station_connect
__tuya_linux_get_snap_qrcode
其他接口理论上均可以直接使用，如果有用户有自己的实现可以替换掉
*/
static BOOL_T sniffer_set_done = FALSE;
#ifdef __cplusplus
extern "C" {
#endif
OPERATE_RET tuya_adapter_wifi_set_work_mode(IN CONST WF_WK_MD_E mode);
void Stop_net_peiwang(void);

OPERATE_RET tuya_adapter_wifi_station_connect(IN CONST CHAR_T *ssid,IN CONST CHAR_T *passwd)
{
    int ret = 0;
	printf("tuya_adapter_wifi_station_connect start\n");
    AppErr("ENUM_INDICATOR_LIGHT_FAST_FLICKER\n");

    g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_LINK_INDICATOR_LIGHT_ALWAYS_OFF);
    g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_POWER_INDICATOR_LIGHT_FAST_FLICKER);

#if 0
	static bool bNetWorkStarted = false;
#endif

    if(sniffer_set_done)
    {
        sniffer_set_done = FALSE;
        IPC_APP_Notify_LED_Sound_Status_CB(IPC_REV_WIFI_CFG);
        usleep(1000*1000);
    }

    IPC_APP_Notify_LED_Sound_Status_CB(IPC_CONNECTING_WIFI);

    if(NULL == ssid)
    {
        //get bind info from ethernet network
        printf("get bind info ...\n");
        return OPRT_INVALID_PARM;
    }

	//get bind info from ap / wifi-smart / qrcode
    printf("get wifi info ...\n");
    // printf("STA Con AP ssid:%s passwd:%s\r\n", ssid, passwd);
	// Stop_net_peiwang();
	
	char final_ssid[65];
	snprintf(final_ssid, sizeof(final_ssid), ssid);

	//获取WiFi配置
	Json::Value WifiTable;
	NetWifiConfig config;
	g_configManager.getConfig(getConfigName(CFG_WIFI), WifiTable);
	TExchangeAL<NetWifiConfig>::getConfig(WifiTable, config);

	/*未绑定获取配置中的ssid为空时，过滤wifi ssid
    配置为空可能是因为配网时有线已经连接上，涂鸦SDK没有调用 hwl_wf_station_connect*/
	if (tuya_ipc_get_register_status() != E_IPC_ACTIVEATED || config.strSSID == "")
	{		
		descending_sort_wifi_list_by_quality(s_all_wifi_list, s_all_wifi_num);
		
		AP_IF_S stWifi;
		int ret = filter_wifi(ssid, &stWifi);
		if (0 == ret)
			snprintf(final_ssid, sizeof(final_ssid), (char *)stWifi.ssid);
		printf("final_ssid: %s\n", final_ssid);
		// if(g_NetConfigHook.GetCurlWifiFreq() == WIFI_SINGLE_FREQ)
		// 	g_NetConfigHook.SetWifi(final_ssid, passwd);
		// else
		// 	g_NetConfigHook.SetWifi(ssid, passwd);
        if ((1 == g_NetConfigHook.Check_netlink_status("eth0")))//有线
        {
            WIFI_ST_t par = {0};
            memset(&par, 0, sizeof(par));
            if(g_NetConfigHook.GetCurlWifiFreq() == WIFI_SINGLE_FREQ)//区分单频双频
                strcpy(par.ssid, final_ssid);
            else
                strcpy(par.ssid, ssid);

            strcpy(par.psk, passwd);
            if (1 == (NetGetWifiConnectStatus(&par,30)))//手动连接，验证WiFi账号密码是否正确
            {
                printf("\033[1;36m    NetGetWifiConnectStatus --------> correct     \033[0m\n");
                    if(g_NetConfigHook.GetCurlWifiFreq() == WIFI_SINGLE_FREQ)
                        g_NetConfigHook.SetWifi(final_ssid, passwd);
                    else
                        g_NetConfigHook.SetWifi(ssid, passwd);
            }
            else
            {
                printf("\033[1;36m    NetGetWifiConnectStatus --------> error     \033[0m\n");
                SystemReset();
                return OPRT_OK;
            }
        }
        else//wifi
        {
            if(g_NetConfigHook.GetCurlWifiFreq() == WIFI_SINGLE_FREQ)
                g_NetConfigHook.SetWifi(final_ssid, passwd);
            else
                g_NetConfigHook.SetWifi(ssid, passwd);
            
        }


	}
	g_NetConfigHook.SetWifiSwitch(true);

    //TODO
    //在这里添加wifi连接的阻塞操作

    while(NET_WORK_MODE_NONE == g_NetConfigHook.GetNetWorkLindMode())
	{
		sleep(1);
	}
	//需要注意网络不通，一直不返回，会否有问题

	/*连接成功后再保存*/
	if( (config.strSSID != final_ssid || config.strKeys != passwd) && (tuya_ipc_get_register_status() != E_IPC_ACTIVEATED))
	{
		if(g_NetConfigHook.GetCurlWifiFreq() == WIFI_SINGLE_FREQ)
			config.strSSID = (char*)final_ssid;
		else
			config.strSSID = (char*)ssid;
		config.strKeys = (char*)passwd;
		TExchangeAL<NetWifiConfig>::setConfig(config, WifiTable);
		printJsonValue(WifiTable);
		g_configManager.setConfig(getConfigName(CFG_WIFI), WifiTable, 0, IConfigManager::applyOK);
	}
	printf("tuya_adapter_wifi_station_connect end\n");
    return OPRT_OK;
}

static void exec_cmd(char *pCmd)
{
	printf("Exec Cmd:%s \r\n", pCmd);
//	  system(pCmd);
	int status;
	pid_t pid = vfork( );
//	printf("MY_START_PROCESS------------------->>>>>pid = %d, errno = %d\n", pid, errno);
	if ( pid  == 0 )
	{
		execlp ( "sh", "sh", "-c", pCmd, NULL );
		exit ( -1 );
	}
	waitpid ( pid, &status, 0 );
}

/*
wlp3s0    Scan completed :
        Cell 01 - Address: 94:77:2B:0A:96:10
                    Channel:1
                    Frequency:2.412 GHz (Channel 1)
。。。。。。。
。。。。。。。

*/
OPERATE_RET tuya_adapter_wifi_all_ap_scan(OUT AP_IF_S **ap_ary,OUT UINT_T *num)
{
    if(NULL == ap_ary || NULL == num)
    {
        return OPRT_INVALID_PARM;
    }

    static AP_IF_S s_aps[MAX_AP_SEARCH];

    memset(s_aps, 0, sizeof(s_aps));
    *ap_ary = s_aps;
    *num = 0;

    FILE *pp = popen("iwlist " WLAN_DEV " scan", "r");
    if(pp == NULL)
    {
        AppErr("popen %s fails, errno : %d\r\n", "iwlist " WLAN_DEV " scan", errno);
        return OPRT_COM_ERROR;
    }

    char tmp[256] = {0};
    memset(tmp, 0, sizeof(tmp));

    int recordIdx = -1;
    while (fgets(tmp, sizeof(tmp), pp) != NULL)
    {
        /* 首先找BSSID	  以此为基准 */
        char *pBSSIDStart = strstr(tmp, " - Address: ");
        if(pBSSIDStart != NULL)
        {
            recordIdx++;
            if(recordIdx >= MAX_AP_SEARCH)
            {
                printf(" Reach Max Record \r\n");
                recordIdx--;
                break;
            }

            BYTE_T *pTmp = s_aps[recordIdx].bssid;
            unsigned int x1,x2,x3,x4,x5,x6;

            sscanf(pBSSIDStart + strlen(" - Address: "), "%x:%x:%x:%x:%x:%x",&x1,&x2,&x3,&x4,&x5,&x6);
            pTmp[0] = x1 & 0xFF;
            pTmp[1] = x2 & 0xFF;
            pTmp[2] = x3 & 0xFF;
            pTmp[3] = x4 & 0xFF;
            pTmp[4] = x5 & 0xFF;
            pTmp[5] = x6 & 0xFF;

            goto ReadNext;
        }
        else
        {
            if(recordIdx < 0)
            {/* 只有找到第一个BSSID 才能继续读下去 */
                goto ReadNext;
            }
        }

        {
            /* 查找signal  */
            char *pSIGNALStart = strstr(tmp, "Quality=");
            if(pSIGNALStart != NULL)
            {
                int x = 0;
                int y = 0;
                sscanf(pSIGNALStart + strlen("Quality=") , "%d/%d ",&x,&y);
                s_aps[recordIdx].rssi = x * 100/ (y+1);
                goto ReadNext;
            }
        }

        {
            /* 查找channel	*/
            char *pCHANNELStart = strstr(tmp, "(Channel ");
            if(pCHANNELStart != NULL)
            {
                int x = 0;
                sscanf(pCHANNELStart + strlen("(Channel "), "%d)", &x);
                s_aps[recordIdx].channel = x;
                goto ReadNext;
            }
        }

        {
            /* 查找ssid  */
            char *pSSIDStart = strstr(tmp, "ESSID:\"");
            if(pSSIDStart != NULL)
            {
                sscanf(pSSIDStart + strlen("ESSID:\""), "%s", s_aps[recordIdx].ssid);
                s_aps[recordIdx].s_len = strlen((const char*)(s_aps[recordIdx].ssid));
                if(s_aps[recordIdx].s_len != 0)
                {
                    s_aps[recordIdx].ssid[s_aps[recordIdx].s_len - 1] = 0;
                    s_aps[recordIdx].s_len--;
                }
                goto ReadNext;
            }
        }

ReadNext:
        memset(tmp, 0, sizeof(tmp));
    }

    pclose(pp);
    *num = recordIdx + 1;


	memset(s_all_wifi_list, 0, sizeof(s_all_wifi_list));
	s_all_wifi_num = *num;
	memcpy(s_all_wifi_list, s_aps, s_all_wifi_num*sizeof(AP_IF_S));


    printf("WIFI Scan AP Begin\r\n");
    int index = 0;
    for(index = 0; index < *num; index++)
    {
        printf("index:%d bssid:%02X-%02X-%02X-%02X-%02X-%02X RSSI:%d SSID:%s\r\n",
                index, s_aps[index].bssid[0],  s_aps[index].bssid[1],  s_aps[index].bssid[2],  s_aps[index].bssid[3],
                s_aps[index].bssid[4],  s_aps[index].bssid[5], s_aps[index].rssi, s_aps[index].ssid);
    }

    printf("WIFI Scan AP End\r\n");

    return OPRT_OK;
}

OPERATE_RET my_adapter_wifi_all_ap_scan(void)
{
    static AP_IF_S s_aps[MAX_AP_SEARCH];
	int num = 0;
    memset(s_aps, 0, sizeof(s_aps));

    FILE *pp = popen("iwlist " WLAN_DEV " scan", "r");
    if(pp == NULL)
    {
        AppErr("popen %s fails, errno : %d\r\n", "iwlist " WLAN_DEV " scan", errno);
        return OPRT_COM_ERROR;
    }

    char tmp[256] = {0};
    memset(tmp, 0, sizeof(tmp));

    int recordIdx = -1;
    while (fgets(tmp, sizeof(tmp), pp) != NULL)
    {
        /* 首先找BSSID	  以此为基准 */
        char *pBSSIDStart = strstr(tmp, " - Address: ");
        if(pBSSIDStart != NULL)
        {
            recordIdx++;
            if(recordIdx >= MAX_AP_SEARCH)
            {
                printf(" Reach Max Record \r\n");
                recordIdx--;
                break;
            }

            BYTE_T *pTmp = s_aps[recordIdx].bssid;
            unsigned int x1,x2,x3,x4,x5,x6;

            sscanf(pBSSIDStart + strlen(" - Address: "), "%x:%x:%x:%x:%x:%x",&x1,&x2,&x3,&x4,&x5,&x6);
            pTmp[0] = x1 & 0xFF;
            pTmp[1] = x2 & 0xFF;
            pTmp[2] = x3 & 0xFF;
            pTmp[3] = x4 & 0xFF;
            pTmp[4] = x5 & 0xFF;
            pTmp[5] = x6 & 0xFF;

            goto ReadNext;
        }else
        {
            if(recordIdx < 0)
            {/* 只有找到第一个BSSID 才能继续读下去 */
                goto ReadNext;
            }
        }

        {
            /* 查找signal  */
            char *pSIGNALStart = strstr(tmp, "Quality=");
            if(pSIGNALStart != NULL)
            {
                int x = 0;
                int y = 0;
                sscanf(pSIGNALStart + strlen("Quality=") , "%d/%d ",&x,&y);
                s_aps[recordIdx].rssi = x * 100/ (y+1);
                goto ReadNext;
            }
        }

        {
            /* 查找channel	*/
            char *pCHANNELStart = strstr(tmp, "(Channel ");
            if(pCHANNELStart != NULL)
            {
                int x = 0;
                sscanf(pCHANNELStart + strlen("(Channel "), "%d)", &x);
                s_aps[recordIdx].channel = x;
                goto ReadNext;
            }
        }

        {
            /* 查找ssid  */
            char *pSSIDStart = strstr(tmp, "ESSID:\"");
            if(pSSIDStart != NULL)
            {
                sscanf(pSSIDStart + strlen("ESSID:\""), "%s", s_aps[recordIdx].ssid);
                s_aps[recordIdx].s_len = strlen((const char*)(s_aps[recordIdx].ssid));
                if(s_aps[recordIdx].s_len != 0)
                {
                    s_aps[recordIdx].ssid[s_aps[recordIdx].s_len - 1] = 0;
                    s_aps[recordIdx].s_len--;
                }
                goto ReadNext;
            }
        }

ReadNext:
        memset(tmp, 0, sizeof(tmp));
    }

    pclose(pp);
    num = recordIdx + 1;


	memset(s_all_wifi_list, 0, sizeof(s_all_wifi_list));
	s_all_wifi_num = num;
	memcpy(s_all_wifi_list, s_aps, s_all_wifi_num*sizeof(AP_IF_S));


    printf("WIFI Scan AP Begin\r\n");
    int index = 0;
    for(index = 0; index < num; index++)
    {
        printf("index:%d bssid:%02X-%02X-%02X-%02X-%02X-%02X RSSI:%d SSID:%s\r\n",
                index, s_aps[index].bssid[0],  s_aps[index].bssid[1],  s_aps[index].bssid[2],  s_aps[index].bssid[3],
                s_aps[index].bssid[4],  s_aps[index].bssid[5], s_aps[index].rssi, s_aps[index].ssid);
    }

    printf("WIFI Scan AP End\r\n");

    return OPRT_OK;
}

OPERATE_RET tuya_adapter_wifi_assign_ap_scan(IN CONST CHAR_T *ssid,OUT AP_IF_S **ap)
{
    if(NULL == ssid || NULL == ap)
    {
        return OPRT_INVALID_PARM;
    }

    /*
    这里简单处理，扫描所有ap后进行查找
    */
    AP_IF_S *pTotalAp = NULL;
    UINT_T tatalNum = 0;
    int index = 0;
    tuya_adapter_wifi_all_ap_scan(&pTotalAp, &tatalNum);

    *ap = NULL;

    for(index = 0; index < tatalNum; index++)
    {
        if(memcmp(pTotalAp[index].ssid, ssid, pTotalAp[index].s_len) == 0)
        {
            *ap = pTotalAp + index;
            break;
        }
    }

    return OPRT_OK;
}

OPERATE_RET tuya_adapter_wifi_release_ap(IN AP_IF_S *ap)
{//静态变量，无需释放
    return OPRT_OK;
}

static int s_curr_channel = 1;
OPERATE_RET tuya_adapter_wifi_set_cur_channel(IN CONST BYTE_T chan)
{
    char tmpCmd[100] = {0};
    snprintf(tmpCmd, 100, "iwconfig %s channel %d", WLAN_DEV, chan);
    exec_cmd(tmpCmd);
    s_curr_channel = chan;

    printf("WIFI Set Channel:%d \r\n", chan);

#ifdef WIFI_CHIP_7601
    tuya_linux_wf_wk_mode_set(WWM_SNIFFER);
#endif

    return OPRT_OK;
}

/*
wlp3s0    13 channels in total; available frequencies :
            Channel 01 : 2.412 GHz
。。。。。。
。。。。。。
            Channel 13 : 2.472 GHz
            Current Frequency:2.452 GHz (Channel 9)
*/
OPERATE_RET tuya_adapter_wifi_get_cur_channel(OUT BYTE_T *chan)
{
    if(NULL == chan) {
        return OPRT_INVALID_PARM;
    }

    FILE *pp = popen("iwlist " WLAN_DEV " channel", "r");

    if(pp == NULL)
    {
        return OPRT_COM_ERROR;
    }

    char tmp[128] = {0};
    memset(tmp, 0, sizeof(tmp));
    while (fgets(tmp, sizeof(tmp), pp) != NULL)
    {
        char *pIPStart = strstr(tmp, " (Channel ");
        if(pIPStart != NULL)
        {
            break;
        }
    }

    /* 查找channel	*/
    char *pCHANNELStart = strstr(tmp, " (Channel ");
    if(pCHANNELStart != NULL)
    {
        int x = 0;
        sscanf(pCHANNELStart + strlen(" (Channel "), "%d", &x);
        *chan = x;
    }else
    {
        *chan = s_curr_channel;
    }

    pclose(pp);
	
   // printf("WIFI Get Curr Channel:%d \r\n", *chan);

    return OPRT_OK;
}


#pragma pack(1)
/*
http://www.radiotap.org/
*/
typedef struct {
    /**
     * @it_version: radiotap version, always 0
     */
    BYTE_T it_version;

    /**
     * @it_pad: padding (or alignment)
     */
    BYTE_T it_pad;

    /**
     * @it_len: overall radiotap header length
     */
    USHORT_T it_len;

    /**
     * @it_present: (first) present word
     */
    UINT_T it_present;
}ieee80211_radiotap_header;
#pragma pack()

static volatile SNIFFER_CALLBACK s_pSnifferCall = NULL;
static volatile BOOL_T s_enable_sniffer = FALSE;

/////////////////////////////add by shang. 2018.09.05////////////////////////////
BOOL_T tuya_get_sniffer_enable()
{
	return s_enable_sniffer;
}
/////////////////////////////add by shang. 2018.09.05////////////////////////////
#define TYPE_MASK  0x0C
#define TYPE_DATA  0x08


static void * func_Sniffer(void *pReserved)
{
    printf("Sniffer Thread Create\r\n");

    int sock = socket(PF_PACKET, SOCK_RAW, htons(0x03));//ETH_P_ALL
    if(sock < 0)
    {
        AppErr("Sniffer Socket Alloc Fails %d \r\n", sock);
        perror("Sniffer Socket Alloc");
        return (void *)0;
    }

    {/* 强制绑定到wlan0 上。后续可以考虑去掉 */
        struct ifreq ifr;
        memset(&ifr, 0x00, sizeof(ifr));
        //strncpy(ifr.ifr_name, WLAN_DEV , strlen(WLAN_DEV) + 1);
		strncpy(ifr.ifr_name, WLAN_DEV , sizeof(ifr.ifr_name)-1);
        setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifr, sizeof(ifr));
    }

{
	int flag = fcntl(sock, F_GETFL, 0);
	if (flag < 0)
	{
		perror("fcntl F_GETFL fail");
		close(sock);
        return (void *)0;
	}
	if (fcntl(sock, F_SETFL, flag | O_NONBLOCK) < 0)
	{
		perror("fcntl F_SETFL fail");
		close(sock);
        return (void *)0;
	}
}

	int ret;
	fd_set readfd;
	struct timeval timeout;

	FD_ZERO(&readfd);
	FD_SET(sock, &readfd);

	

    #define MAX_REV_BUFFER 512
    BYTE_T rev_buffer[MAX_REV_BUFFER];

    int skipLen = 26;/* radiotap 默认长度为26 */

    while((s_pSnifferCall != NULL) && (TRUE == s_enable_sniffer))
    {
		timeout.tv_sec=3;
		timeout.tv_usec=0;

		ret=select(sock+1,&readfd, NULL, NULL, &timeout);
		if( ret < 0 )
			break;
		else if( ret == 0 )
			continue;
		
		if( ! FD_ISSET(sock, &readfd) )
		{
			continue;
		}
		
//		printf("sniffer------------1\n");
        int rev_num = recvfrom(sock, rev_buffer, MAX_REV_BUFFER, 0, NULL, NULL);
		if( rev_num < sizeof(ieee80211_radiotap_header) )
		{
			continue;
		}
//		printf("sniffer------------2   rev_num : %d\n", rev_num);
        ieee80211_radiotap_header *pHeader = (ieee80211_radiotap_header *)rev_buffer;
        skipLen = pHeader->it_len;

#ifdef WIFI_CHIP_7601
        skipLen = 144;
#endif
        if(skipLen >= MAX_REV_BUFFER)
        {/* 有出现过header全ff的情况，这里直接丢掉这个包 */
            continue;
        }

        #if 0
        {
            printf("skipLen:%d ", skipLen);
            int index = 0;
            for(index = 0; index < 180; index++)
            {
                printf("%02X-", rev_buffer[index]);
            }
            printf("\r\n");
        }
		#endif
        if(rev_num > skipLen)
        {
            unsigned char type = rev_buffer[skipLen] & TYPE_MASK;
            if(type == TYPE_DATA)
            {
                s_pSnifferCall(rev_buffer + skipLen, rev_num - skipLen,80);
            }
        }
    }

    s_pSnifferCall = NULL;

    close(sock);

    printf("Sniffer Proc Finish\r\n");
    return (void *)0;
}

static pthread_t sniffer_thId; // 抓包线程ID

static int s_QrcodeExit = 0;
//提取到二维码信息时的回调
static bool QrCodeExtractedCB(const std::string &strResult)
{
	bool bRet;
	CAudioPrompt::AudioFileParm audioFile;
	OPERATE_RET ret = tuya_ipc_direct_connect(strResult.c_str(), TUYA_IPC_DIRECT_CONNECT_QRCODE);
	if(ret == OPRT_OK)
	{
		bRet = true;
		audioFile.strFileName = AUDIO_FILE_QRCODE_GET_COMPLETE;

		AppErr("\n\n=========ENUM_INDICATOR_LIGHT_FAST_FLICKER============\n\n");

		g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_LINK_INDICATOR_LIGHT_ALWAYS_OFF);
		g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_POWER_INDICATOR_LIGHT_FAST_FLICKER);
	}
	else
	{
		bRet = false;
		audioFile.strFileName = AUDIO_FILE_QRCODE_INVALID;
	}
    s_QrcodeExit = 1;		//停止播报
	//语音提示
    audioFile.type = 0;
	g_AudioPrompt.aoPlay(audioFile);
	return bRet;
}

static void* DoQrcode(void* args)
{
	printf("DoQrcode start\n");
    IPC_APP_Notify_LED_Sound_Status_CB(IPC_START_WIFI_CFG);
	int count = 15;
	while (!s_QrcodeExit) {

		//红色指示灯灭
		g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_POWER_INDICATOR_LIGHT_ALWAYS_OFF);
		//联网指示灯慢闪
		g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_LINK_INDICATOR_LIGHT_FAST_FLICKER);
		if (count -- < 0) {
			count = 15;
			
			IPC_APP_Notify_LED_Sound_Status_CB(IPC_START_WIFI_CFG);
		}
		sleep(1);
	}
	return NULL;
}

static void* Stop_net(void* args)
{
	printf("Stop set_up net\n");
	int count_top = 300;
	while (!s_QrcodeExit) {
		if (count_top-- == 0) {
			break;
		}
		sleep(1);
	}

    if (count_top <=0 )
    {
        printf("timeout Stop_net_peiwang \n");
        Stop_net_peiwang();
    }
    //红色指示灯灭

    g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_POWER_INDICATOR_LIGHT_ALWAYS_ON);
    //联网指示灯灭
    g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_LINK_INDICATOR_LIGHT_ALWAYS_OFF);
	return NULL;
}

void Stop_net_peiwang(void)
{
    if (net_peiwang_status)
    {
        net_peiwang_status = 0;
        if(s_pSnifferCall != NULL)
        {
            s_enable_sniffer = FALSE;
            pthread_join(sniffer_thId, NULL);
            s_pSnifferCall = NULL;
        }
        
        if( true == g_QrCodeConHandle.GetCreatedStatus() )
        {
            g_QrCodeConHandle.Stop();
            g_QrCodeConHandle.Destory();		//退出二维码配网模式
            // g_Camera.SetNightModeQrcode(0);
        }
        WifiApModeDestroy();

        if(WIFI_BLE_SUPPORT == g_NetConfigHook.GetCurlWifiBleSupport())//change on 2025.01.11
        {
            if(1 == g_iBleInitFlag)
            {
                printf("\033[1;32m ==========================lib_ble_main_exit============================ \033[0m\n");
                lib_ble_main_exit();
                g_iBleInitFlag = 0;
            }
        }

        // if ( (g_NetConfigHook.GetCurlWifiModel() == WIFI_ATBM6062) || (g_NetConfigHook.GetCurlWifiModel() == WIFI_ATBM6012B))
        // {
        //     printf("==========================lib_ble_main_exit============================\n");
        //     lib_ble_main_exit();
        // }

        s_QrcodeExit = 1;		//停止播报
    }
	return;
}

//---------------------add on 2025.05.21
//为了避免退出蓝牙的接口被调用，导致蓝牙配网还未结束的时候，调用send_ble产生段错误
void Stop_net_ap(void)
{
    if (net_peiwang_status)
    {
        net_peiwang_status = 0;
        if(s_pSnifferCall != NULL)
        {
            s_enable_sniffer = FALSE;
            pthread_join(sniffer_thId, NULL);
            s_pSnifferCall = NULL;
        }
        
        if( true == g_QrCodeConHandle.GetCreatedStatus() )
        {
            g_QrCodeConHandle.Stop();
            g_QrCodeConHandle.Destory();		//退出二维码配网模式
            // g_Camera.SetNightModeQrcode(0);
        }
        WifiApModeDestroy();

        s_QrcodeExit = 1;		//停止播报
    }
	return;
}




//---------------------add on 2025.05.21


//需要防止重复被调用
int tuya_adapter_wifi_sniffer_set(const bool en, const SNIFFER_CALLBACK cb)
{
    if (en == s_enable_sniffer){
        AppWarning("Already in status %d\r\n",en);
        return OPRT_OK;
    }
    s_enable_sniffer = en;
    if(en == TRUE)
    {
        IPC_APP_Notify_LED_Sound_Status_CB(IPC_START_WIFI_CFG);
        net_peiwang_status = 1;
        printf("Enable1 Sniffer\r\n");
        tuya_adapter_wifi_set_work_mode(WWM_SNIFFER);
        //tuya_hal_wifi_set_work_mode(WWM_SNIFFER);

        s_pSnifferCall = cb;
        pthread_create(&sniffer_thId, NULL, func_Sniffer, NULL);
        #if 1
		//启动二维码扫描
		if( false == g_QrCodeConHandle.GetCreatedStatus() )
		{
			g_Camera.setMode_ScanQrcode(1);
			g_QrCodeConHandle.Create(QrCodeExtractedCB);
			g_QrCodeConHandle.Start();
		}
        #endif

		//间隔30 s 循环播报请配置WiFi语音提示
		CreateDetachedThread((char*)"DoQrcode",DoQrcode, (void *)NULL, true);	
		//超时5分钟停止配网
		CreateDetachedThread((char*)"Stop_net",Stop_net, (void *)NULL, true);
		AppErr("\n\n=========ENUM_INDICATOR_LIGHT_FAST_FLICKER============\n\n");

		//红色指示灯灭
		g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_POWER_INDICATOR_LIGHT_ALWAYS_OFF);
		//联网指示灯慢闪
		g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_LINK_INDICATOR_LIGHT_FAST_FLICKER);
    }
	else
    {
        AppWarning("Disable Sniffer\r\n");
        pthread_join(sniffer_thId, NULL);
		s_pSnifferCall = NULL;
        tuya_adapter_wifi_set_work_mode(WWM_STATION);

        printf("Disable Qrcode\r\n");

		if( true == g_QrCodeConHandle.GetCreatedStatus() )
		{
            g_QrCodeConHandle.Stop();
			g_QrCodeConHandle.Destory();		//退出二维码配网模式
            // g_Camera.SetNightModeQrcode(0);			
		}

        sniffer_set_done = TRUE;
		s_QrcodeExit = 1;		//停止播报
    }

    return OPRT_OK;
}


/*
wlan0	  Link encap:Ethernet  HWaddr 08:57:00:88:5c:16
        inet addr:192.168.22.3  Bcast:192.168.23.255	Mask:255.255.252.0
。。。。。
。。。。
*/
static OPERATE_RET hwl_get_local_ip_info(char *interface,OUT NW_IP_S *ip)
{
    char tmp[256];

    memset(tmp, 0, sizeof(tmp));
    snprintf(tmp,sizeof(tmp),"ifconfig %s",interface);
    FILE *pp = popen(tmp, "r");
    if(pp == NULL)
    {
		AppErr("popen ifconfig %s failed. errno : %d\n", interface, errno);
        return OPRT_COM_ERROR;
    }

    memset(tmp, 0, sizeof(tmp));
    while (fgets(tmp, sizeof(tmp), pp) != NULL)
    {
        char *pIPStart = strstr(tmp, "inet addr:");
        if(pIPStart != NULL)
        {/* It's all a line containing IP GW mask that jumps out directly  */
            break;
        }
    }
    
    {
        /* finding ip  */
        char *pIPStart = strstr(tmp, "inet addr:");
        if(pIPStart != NULL)
        {
            sscanf(pIPStart + strlen("inet addr:"), "%15s", ip->ip);
        }
    }
    
    {
        /* finding gw  */
        char *pGWStart = strstr(tmp, "broadcast ");
        if(pGWStart != NULL)
        {
            sscanf(pGWStart + strlen("broadcast "), "%s", ip->gw);
        }
    }
    
    {
        /* finding mask */
        char *pMASKStart = strstr(tmp, "netmask ");
        if(pMASKStart != NULL)
        {
            sscanf(pMASKStart + strlen("netmask "), "%s", ip->mask);
        }
    }
    
    pclose(pp);

    return OPRT_OK;
}

// if ethernet is used , replace ethernet name
#define NET_DEV "eth0"

/*
wlan0	  Link encap:Ethernet  HWaddr 08:57:00:88:5c:16
        inet addr:192.168.22.3  Bcast:192.168.23.255	Mask:255.255.252.0
。。。。。
。。。。
*/
OPERATE_RET tuya_adapter_wifi_get_ip(IN CONST WF_IF_E wf,OUT NW_IP_S *ip)
{
    if(NULL == ip)
    {
        return OPRT_INVALID_PARM;
    }

    if(wf == WF_AP)
    {/* Simple Processing in AP Mode */
        memcpy(ip->ip, (char*)"192.168.175.1", strlen("192.168.175.1"));
        memcpy(ip->gw, (char*)"192.168.175.1", strlen("192.168.175.1"));
        memcpy(ip->mask, (char*)"255.255.255.0", strlen("255.255.255.0"));
    }

    if(wf == WF_STATION)
    {
        //get the ip of ethernet
        hwl_get_local_ip_info((char*)NET_DEV,ip);

        NW_IP_S tmp;
        memset(&tmp,0,sizeof(NW_IP_S));
        //get the ip of wifi
        if( true == g_TuyaHandle.GetRegisterStatus() )
        {
            hwl_get_local_ip_info((char*)WLAN_DEV,&tmp);
            if(strlen(tmp.ip)){
        	    //replace ip 
                memcpy(ip,&tmp,sizeof(NW_IP_S));
            }
        }
    }

    //printf("WIFI[%d] Get IP:%s\r\n", wf, ip->ip);
    return OPRT_OK;
}

/*
wlp3s0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.3.14  netmask 255.255.255.0  broadcast 192.168.3.255
        ether a4:02:b9:b1:99:50  txqueuelen 1000  (以太网)
。。。。。。
。。。。。。
*/
OPERATE_RET tuya_adapter_wifi_get_mac(IN CONST WF_IF_E wf,INOUT NW_MAC_S *mac)
{
    if(NULL == mac)
    {
        return OPRT_INVALID_PARM;
    }

#ifdef __HuaweiLite__
    //todo liteos系统的mac获取实现
#else
    FILE *pp = popen("ifconfig " WLAN_DEV, "r");
    if(pp == NULL)
    {
        AppErr("popen ifconfig %s failed. errno : %d\n", WLAN_DEV, errno);
        return OPRT_COM_ERROR;
    }

    char tmp[256];
    memset(tmp, 0, sizeof(tmp));
    while (fgets(tmp, sizeof(tmp), pp) != NULL)
    {
        char *pMACStart = strstr(tmp, "ether ");
        if(pMACStart != NULL)
        {
            int x1,x2,x3,x4,x5,x6;
            sscanf(pMACStart + strlen("ether "), "%x:%x:%x:%x:%x:%x",&x1,&x2,&x3,&x4,&x5,&x6);
            mac->mac[0] = x1 & 0xFF;
            mac->mac[1] = x2 & 0xFF;
            mac->mac[2] = x3 & 0xFF;
            mac->mac[3] = x4 & 0xFF;
            mac->mac[4] = x5 & 0xFF;
            mac->mac[5] = x6 & 0xFF;

            break;
        }
    }
    pclose(pp);
#endif

    printf("WIFI Get MAC %02X-%02X-%02X-%02X-%02X-%02X\r\n",
            mac->mac[0],mac->mac[1],mac->mac[2],mac->mac[3],mac->mac[4],mac->mac[5]);

    return OPRT_OK;
}

OPERATE_RET tuya_adapter_wifi_set_mac(IN CONST WF_IF_E wf,IN CONST NW_MAC_S *mac)
{
    if(NULL == mac)
    {
        return OPRT_INVALID_PARM;
    }
    printf("WIFI Set MAC\r\n");
    //reserved
    return OPRT_OK;
}

OPERATE_RET tuya_adapter_wifi_set_work_mode(IN CONST WF_WK_MD_E mode)
{
#ifdef __DEBUG_MODE
		return OPRT_OK;
#endif
    char tmpCmd[100] = {0};

    snprintf(tmpCmd, 100, "ifconfig %s up", WLAN_DEV);
    exec_cmd(tmpCmd);

    switch (mode)
    {
        case WWM_LOWPOWER:
        {
            //linux系统不关心低功耗
            break;
        }
        case WWM_SNIFFER:
        {
#ifndef WIFI_CHIP_7601
            snprintf(tmpCmd, 100, "ifconfig %s down", WLAN_DEV);
            exec_cmd(tmpCmd);
#endif
            snprintf(tmpCmd, 100, "iwconfig %s mode Monitor", WLAN_DEV);
            exec_cmd(tmpCmd);
#ifndef WIFI_CHIP_7601
            snprintf(tmpCmd, 100, "ifconfig %s up", WLAN_DEV);
            exec_cmd(tmpCmd);
#endif
            break;
        }
        case WWM_STATION:
        {
#ifndef WIFI_CHIP_7601
            snprintf(tmpCmd, 100, "ifconfig %s down", WLAN_DEV);
            exec_cmd(tmpCmd);
#endif
            snprintf(tmpCmd, 100, "iwconfig %s mode Managed", WLAN_DEV);
            exec_cmd(tmpCmd);
#ifndef WIFI_CHIP_7601
            snprintf(tmpCmd, 100, "ifconfig %s up", WLAN_DEV);
            exec_cmd(tmpCmd);
#endif
            break;
        }
        case WWM_SOFTAP:
        {
#ifndef WIFI_CHIP_7601
            snprintf(tmpCmd, 100, "ifconfig %s down", WLAN_DEV);
            exec_cmd(tmpCmd);
#endif
            snprintf(tmpCmd, 100, "iwconfig %s mode Master", WLAN_DEV);
            exec_cmd(tmpCmd);
#ifndef WIFI_CHIP_7601
            snprintf(tmpCmd, 100, "ifconfig %s up", WLAN_DEV);
            exec_cmd(tmpCmd);
#endif
            break;
        }
        case WWM_STATIONAP:
        {//reserved
            break;
        }
        default:
        {
            break;
        }
    }
    printf("WIFI Set Mode %d\r\n", mode);

    return OPRT_OK;
}


/*
wlp3s0    IEEE 802.11  ESSID:"H"
        Mode:Managed  Frequency:2.452 GHz  Access Point: 58:7F:66:04:73:A8
        Bit Rate=130 Mb/s   Tx-Power=22 dBm
。。。。。。
。。。。。。
*/
OPERATE_RET tuya_adapter_wifi_get_work_mode(OUT WF_WK_MD_E *mode)
{
    if(NULL == mode)
    {
        return OPRT_INVALID_PARM;
    }

#ifdef __HuaweiLite__
    //todo liteos系统的模式获取实现
    char scan_mode[10] = {0};
#else
    FILE *pp = popen("iwconfig " WLAN_DEV, "r");
    if(pp == NULL)
    {
        AppErr("WIFI Get Mode Fail. Force Set Station . errno : %d\r\n", errno);
        *mode = WWM_STATION;
        return OPRT_OK;
    }

    char scan_mode[10] = {0};
    char tmp[256] = {0};
    while (fgets(tmp, sizeof(tmp), pp) != NULL)
    {
        char *pModeStart = strstr(tmp, "Mode:");
        if(pModeStart != NULL)
        {
            int x1,x2,x3,x4,x5,x6;
            sscanf(pModeStart + strlen("Mode:"), "%s ", scan_mode);
            break;
        }
    }
    pclose(pp);
#endif

    *mode = WWM_STATION;
    if(strncasecmp(scan_mode, "Managed", strlen("Managed")) == 0)
    {
        *mode = WWM_STATION;
    }

    if(strncasecmp(scan_mode, "Master", strlen("Master")) == 0)
    {
        *mode = WWM_SOFTAP;
    }

    if(strncasecmp(scan_mode, "Monitor", strlen("Monitor")) == 0)
    {
        *mode = WWM_SNIFFER;
    }
    //printf("WIFI Get Mode [%s] %d\r\n", scan_mode, *mode);

    return OPRT_OK;
}


/***********************************************************
*  Function: wf_station_disconnect
*  Input: none
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET tuya_adapter_wifi_station_disconnect(VOID)
{
    printf("STA Disconn AP\r\n");
    //Reserved

    return OPRT_OK;
}



#if 0
int WifiSignalStrength(router_signal_s* stSignal)
{
	int ret = -1;

	if( ! stSignal )
	{
		return -1;
	}
	
	FILE *pp = popen("iwconfig wlan0", "r");
	if(pp == NULL)
	{
		printf("WifiSignal--->popen fails\r\n");
		return -1;
	}

	char tmp[512] = {0};
	memset(tmp, 0, sizeof(tmp));

	unsigned char bNeedFindHead = 1;
	int recordIdx = -1;
	while (fgets(tmp, sizeof(tmp), pp) != NULL)
	{
		printf("%s", tmp);
		
		{
			/* 查找ssid  */
			char *pSSIDStart = strstr(tmp, "ESSID:\"");
			if(pSSIDStart != NULL)
			{
			#if 01
				char *pSSIDEnd = strchr(pSSIDStart+strlen("ESSID:\""), '"');
				if( pSSIDEnd )
				{
					int s_len = pSSIDEnd - pSSIDStart - strlen("ESSID:\"");
					strncpy(stSignal->essid, pSSIDStart+strlen("ESSID:\""), s_len);
					stSignal->essid[s_len] = '\0';
					
					goto ReadNext;
				}
			#else
				sscanf(pSSIDStart + strlen("ESSID:") , "\"%s\" ", stSignal->essid);
				goto ReadNext;
			#endif
			}
		}

		{
			/* 查找signal  */
			char *pSIGNALStart = strstr(tmp, "Signal level=");
			if(pSIGNALStart != NULL)
			{
				//[-30, -100]
				int iDbmVal = strtol(pSIGNALStart+strlen("Signal level="), NULL, 10);
//				printf("wlan rssi %d dbm\n", iDbmVal);
				
				if( iDbmVal > -30 )
					iDbmVal = -30;
				
				if( iDbmVal < -100 )
					iDbmVal = -100;
	
				stSignal->uchSignal = ((iDbmVal - (-100)) * 100) / 70;
                if(stSignal->uchSignal < 1)
                    stSignal->uchSignal = 1;
                if(stSignal->uchSignal > 98)
                    stSignal->uchSignal = 98;


				ret = 0;				
				break;
			}
		}

ReadNext:
		memset(tmp, 0, sizeof(tmp));
	}

	pclose(pp);

exit:

	return ret;
}
#else
int WifiSignalStrength(router_signal_s* stSignal)
{
    float frequency = 0.0;

	int ret = -1;

	if( ! stSignal )
	{
		return -1;
	}
	
	FILE *pp = popen("iwconfig wlan0", "r");
	if(pp == NULL)
	{
		printf("WifiSignal--->popen fails\r\n");
		return -1;
	}

	char tmp[512] = {0};
	memset(tmp, 0, sizeof(tmp));

	unsigned char bNeedFindHead = 1;
	int recordIdx = -1;
	while (fgets(tmp, sizeof(tmp), pp) != NULL)
	{
		printf("%s", tmp);
		
		{
			/* 查找ssid  */
			char *pSSIDStart = strstr(tmp, "ESSID:\"");
			if(pSSIDStart != NULL)
			{
			#if 01
				char *pSSIDEnd = strchr(pSSIDStart+strlen("ESSID:\""), '"');
				if( pSSIDEnd )
				{
					int s_len = pSSIDEnd - pSSIDStart - strlen("ESSID:\"");
					strncpy(stSignal->essid, pSSIDStart+strlen("ESSID:\""), s_len);
					stSignal->essid[s_len] = '\0';
					
					goto ReadNext;
				}
			#else
				sscanf(pSSIDStart + strlen("ESSID:") , "\"%s\" ", stSignal->essid);
				goto ReadNext;
			#endif
			}
		}


        
        {
            /* 查找 Frequency  */
            char *pSSIDStart = strstr(tmp, "Frequency:");
            if(pSSIDStart != NULL)
            {
                char *endptr;
                frequency = strtod(pSSIDStart + strlen("Frequency:"), &endptr);
                // 检查转换是否成功
                if (endptr == pSSIDStart || *endptr != ' ') {
                    printf("Conversion failed or invalid format.\n");
                } else {
                    printf("Extracted frequency: %f\n", frequency);
                }
            }
        }
        //------------------------add on 2025.04.18
        //8188的格式不太一样 Signal level=69/100
        if(WIFI_RTL8188 == g_NetConfigHook.GetCurlWifiModel())
        {
            char *pSIGNALStart = strstr(tmp, "Signal level=");
            if(pSIGNALStart != NULL)
            {
                int val = 0;
                int y = 0;
                sscanf(pSIGNALStart + strlen("Signal level="), "%d/%d",&val,&y);
                
                //0~75
                if( val <= 1 )
                    val = 1;

                if( val >= 75)
                    val = 75;

                stSignal->uchSignal = val * 100 / 75;
                if(stSignal->uchSignal < 1)
                    stSignal->uchSignal = 1;
                else if(stSignal->uchSignal > 98)
                    stSignal->uchSignal = 98;

                ret = 0;				
                break;
            }
        }
        //------------------------add on 2025.04.18
        else
		{
			/* 查找signal  */
			char *pSIGNALStart = strstr(tmp, "Signal level=");
			if(pSIGNALStart != NULL)
			{
				//[-30, -100]
				int iDbmVal = strtol(pSIGNALStart+strlen("Signal level="), NULL, 10);
//				printf("wlan rssi %d dbm\n", iDbmVal);
				

                if( iDbmVal < -100 )
					iDbmVal = -100;

                if(frequency < 3.0 && frequency > 2.0)
                {
                    if( iDbmVal > -30 )
					    iDbmVal = -30;


                    stSignal->uchSignal = ((iDbmVal - (-100)) * 100) / 70;
                }
                else if(frequency > 4.0 && frequency < 6.0)
                {
                    if( iDbmVal > -40 )
					    iDbmVal = -40;

                    stSignal->uchSignal = ((iDbmVal - (-100)) * 100) / 60;
                }
				    
					
                if(stSignal->uchSignal < 1)
                    stSignal->uchSignal = 1;
                if(stSignal->uchSignal > 98)
                    stSignal->uchSignal = 98;


				ret = 0;				
				break;
			}
		}

ReadNext:
		memset(tmp, 0, sizeof(tmp));
	}

	pclose(pp);

exit:

	return ret;
}


#endif

router_signal_s stSignal;

/*
wlp3s0    IEEE 802.11  ESSID:"HUAWEI-1234"
。。。。。。
        Link Quality=70/70  Signal level=-6 dBm
。。。。。。
*/
OPERATE_RET tuya_adapter_wifi_station_get_conn_ap_rssi(OUT SCHAR_T *rssi)
{
    if(NULL == rssi)
    {
        return OPRT_INVALID_PARM;
    }
    *rssi = 0;

	if(g_NetConfigHook.GetNetWorkLindMode() == NET_WORK_MODE_ETH0)
	{
		*rssi = 100;
        return OPRT_OK;
	}
#ifdef __HuaweiLite__
    //todo liteos系统的rssi获取实现
#else
#if 0
    FILE *pp = popen("iwconfig " WLAN_DEV, "r");
    if(pp == NULL)
    {
        AppErr("WIFI iwconfig %s failed . errno : %d\r\n", WLAN_DEV, errno);
        return OPRT_COM_ERROR;
    }

    char tmp[128] = {0};
    while (fgets(tmp, sizeof(tmp), pp) != NULL)
    {
        /* 查找signal  */
        char *pSIGNALStart = strstr(tmp, "Quality=");
        if(pSIGNALStart != NULL)
        {
            int x = 0;
            int y = 0;
            sscanf(pSIGNALStart + strlen("Quality="), "%d/%d",&x,&y);
            *rssi = x * 100/ (y+1);
            break;
        }
    }
    pclose(pp);

#else

    WifiSignalStrength(&stSignal);
    *rssi = stSignal.uchSignal * 100 / 100;

    stSignal.uchSignal = 0;
#endif
#endif
    //printf("Get Conn AP RSSI:%d\r\n", *rssi);

    return OPRT_OK;
}

//note：High frequency interface
OPERATE_RET tuya_adapter_wifi_station_get_status(OUT WF_STATION_STAT_E *stat)
{
    if(NULL == stat)
    {
        AppErr("tuya_adapter_wifi_station_get_status\n");
        return OPRT_INVALID_PARM;
    }
	*stat = WSS_IDLE; //请务必按照实时状态返回
	WLAN_STATE eWifiStat = g_NetConfigHook.GETNETSTATUS();
	switch(eWifiStat)
		{
			case WLAN_IDLE:
				*stat = WSS_IDLE;
				break;
			case WLAN_CONNECTING:
				*stat = WSS_CONNECTING;
				break;
			case WLAN_CONN_FAIL:
				*stat = WSS_CONN_FAIL;
				break;
			case WLAN_CONNECTED:
				*stat = WSS_CONN_SUCCESS;
				break;
			case WLAN_GOT_IP:
				*stat = WSS_GOT_IP;
				break;
			default:
				*stat = WSS_IDLE;
				break;
		}

	//printf("**********tuya_adapter_wifi_station_get_status  %d--getip %d***************\n",*stat,success_get_ip);
    //Reserved
    return OPRT_OK;
}


OPERATE_RET tuya_adapter_wifi_ap_start(IN CONST WF_AP_CFG_IF_S *cfg)
{
    if(NULL == cfg)
    {
        return OPRT_INVALID_PARM;
    }

    net_peiwang_status = 1;
    my_adapter_wifi_all_ap_scan();

    printf("Start AP SSID:%s \r\n", cfg->ssid);
    //std::string strSSID = "IPCAM-";
    std::string strSSID = "SmartLife-";
    
    //取tuya UID后4位
    const char *pP2P_UID = (char *)(DevInfoFromEEPROM_g.TUYA_UID);
    if( strlen(pP2P_UID) >= 4 )
    {
        for(int i = strlen(pP2P_UID) - 4; i < strlen(pP2P_UID); i++)
        {
            strSSID += pP2P_UID[i];
        }
        WifiApModeCreate((const char *)strSSID.c_str(), (const char *)cfg->passwd);
    }
    else
    {
        WifiApModeCreate((const char *)cfg->ssid, (const char *)cfg->passwd);
    }
    
	//启动二维码扫描
	if( false == g_QrCodeConHandle.GetCreatedStatus() )
	{
		g_Camera.setMode_ScanQrcode(1);
		g_QrCodeConHandle.Create(QrCodeExtractedCB);
		g_QrCodeConHandle.Start();
	}

	//间隔30 s 循环播报请配置WiFi语音提示
	CreateDetachedThread((char*)"DoQrcode",DoQrcode, (void *)NULL, true);	

	//超时5分钟停止配网
	CreateDetachedThread((char*)"Stop_net",Stop_net, (void *)NULL, true);

	//红色指示灯灭

	g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_POWER_INDICATOR_LIGHT_ALWAYS_OFF);
	//联网指示灯慢闪
	g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_LINK_INDICATOR_LIGHT_FAST_FLICKER);

    return OPRT_OK;
}

OPERATE_RET tuya_adapter_wifi_ap_stop(VOID)
{
    printf("Stop AP \r\n");
    // Stop_net_peiwang();
    Stop_net_ap();
    return OPRT_OK;
}

int tuya_adapter_wifi_send_mgnt(const uint8_t *buf, const uint32_t len)
{
    // use to send pro pack
    return 0;
}

OPERATE_RET tuya_adapter_wifi_set_country_code(IN CONST CHAR_T *p_country_code)
{
    printf("Set Country Code:%s \r\n", p_country_code);

    return OPRT_OK;
}

OPERATE_RET tuya_adapter_wifi_register_recv_mgnt_callback(CONST BOOL_T enable, CONST WIFI_REV_MGNT_CB recv_cb)
{
	return 0;
}

OPERATE_RET tuya_adapter_wifi_set_lp_mode(CONST BOOL_T en, CONST UCHAR_T dtim)
{
	return 0;
}

OPERATE_RET tuya_adapter_wifi_fast_station_connect_v2(CONST FAST_WF_CONNECTED_AP_INFO_V2_S *fast_ap_info)
{
	return 0;
}

BOOL_T      tuya_adapter_wifi_rf_calibrated(VOID_T)
{
	return 1;
}
OPERATE_RET tuya_adapter_wifi_station_get_err_stat(WF_STATION_STAT_E *stat)
{
	return 0;
}
OPERATE_RET tuya_adapter_wifi_get_bssid(UCHAR_T *mac)
{
	return 0;
}

OPERATE_RET tuya_adapter_wifi_get_connected_ap_info_v2(FAST_WF_CONNECTED_AP_INFO_V2_S **fast_ap_info)
{
	return 0;
}
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

