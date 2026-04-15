#include "NetWifi.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <Types/Defs.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <linux/ethtool.h>
#include <sys/ioctl.h>
#include <sys/types.h>  
#include <sys/wait.h>
#include <dirent.h>

#define NET_START_PROCESS( file, cmd, ... ) \
{\
	int status;\
	pid_t pid = vfork( );\
	if ( pid  == 0 )\
	{\
		execlp ( file, cmd, ##__VA_ARGS__ );\
		exit ( -1 );\
	}\
	waitpid ( pid, &status, 0 );\
}

#define NET_START_PROCESS_CLOSE_FD( file, cmd, ... ) \
{\
	int status;\
	pid_t pid = vfork( );\
	if ( pid  == 0 )\
	{\
		int pid;\
		char path[256];\
		DIR *dir = NULL;\
		struct dirent *entry = NULL;\
		pid = getpid();\
		snprintf(path, sizeof(path), "/proc/%d/fd", pid);\
		dir = opendir(path);\
		if(NULL == dir)\
		{\
			printf("open %s fail!\n", path);\
		}\
		else\
		{\
			while( ( entry = readdir(dir) ) != NULL )\
			{\
				char path2[256];\
				char realpath[256];\
				snprintf(path2, sizeof(path2), "%s/%s", path, entry->d_name);\
				memset(realpath, 0, sizeof(realpath));\
				readlink(path2, realpath, sizeof(realpath)-1);\
				if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && \
					strcmp(entry->d_name, "0") != 0 && strcmp(entry->d_name, "1") != 0 && strcmp(entry->d_name, "2") != 0 )\
				{\
					if( strcmp(path, realpath) != 0 )\
					{\
						int fd = atoi(entry->d_name);\
						close(fd);\
					}\
				}\
			}\
			closedir(dir);\
		}\
		\
		execlp ( file, cmd, ##__VA_ARGS__ );\
		exit ( -1 );\
	}\
	waitpid ( pid, &status, 0 );\
}

PATTERN_SINGLETON_IMPLEMENT(CNetWifi)

CNetWifi::CNetWifi():CThread("NetWifi", TP_DEFAULT)
{
	m_ssid = "";
	m_pwd  = "";
	m_ip   = "";
	m_gw   = "";
	m_mask = "";
	m_eth = 0;
	m_wifi_check_status = NET_CHECK_WAITE_NONE;
	m_wifi_connect_result = NET_WIFI_DISCONNECTED;
    IEventManager::instance()->attach(IEventManager::Proc(&CNetWifi::onAppEvent, this));
}

CNetWifi::~CNetWifi()
{
}

int CNetWifi::Init()
{
	printf("======================================CNetWifi::Init()====================1==================\n");
	if( false == IsThreadOver() ) 	//防止多次调用
	{
		return 0;
	}
	
	bool bRet = CreateThread();

	printf("======================================CNetWifi::Init()====================2==================\n");

	if (true != bRet)
	{
		printf("Create thread error!\n");
		return -1;
	}

	return 0;
}

int CNetWifi::UnInit()
{
	printf("======================================CNetWifi::UnInit()======================================\n");
	if( true == IsThreadOver() )
	{
		DestroyThread();
	}
	return 0;
}

int CNetWifi::CreateWpaConfig(int encrypt, const char *ssid, const char *passwd)
{
	FILE *fp = NULL;
	char buffer[128];
	int len, ret;
	const char *encryType[] = {"NONE", "WEP", "WPA", "WPA2"};

	if ((fp = fopen("/tmp/wpa_supplicant.conf", "wb")) == NULL)
	{
		printf("devmanage wifimanager fopen is NULL!");	
		return -1;
	}

	//ctrl_interface
	sprintf(buffer, "ctrl_interface=/var/run/wpa_supplicant\n\t\n");
	len = strlen(buffer);
	if((ret = fwrite(buffer, 1, len, fp)) != len)
	{
		printf("devmanage wifimanager fwrite is err, len:%d,ret:%d!", len, ret);
		fclose(fp);
		return -1;
	}

	//config
	sprintf(buffer, "network={\n");
	len = strlen(buffer);
	if((ret = fwrite(buffer, 1, len, fp)) != len)
	{
		printf("devmanage wifimanager fwrite is err, len:%d,ret:%d!", len, ret);
		fclose(fp);
		return -1;
	}

	sprintf(buffer, "\tssid=\"%s\"\n", ssid);
	len = strlen(buffer);
	if((ret = fwrite(buffer, 1, len, fp)) != len)
	{
		printf("devmanage wifimanager fwrite is err, len:%d,ret:%d!", len, ret);
		fclose(fp);
		return -1;
	}

	if(( encrypt == 0) || (0 == strlen(passwd)) )
	{
		sprintf(buffer, "\tkey_mgmt=%s\n}\n", encryType[0]);
		len = strlen(buffer);

		if((ret = fwrite(buffer, 1, len, fp)) != len)
		{
			printf("devmanage wifimanager fwrite is err, len:%d,ret:%d!", len, ret);
			fclose(fp);
			return -1;
		}	 
		fclose(fp);
		return 0;
	}

	sprintf(buffer, "\tproto=WPA RSN\n");      
	len = strlen(buffer);
	if((ret = fwrite(buffer, 1, len, fp)) != len)
	{
		printf("devmanage wifimanager fwrite is err, len:%d,ret:%d!", len, ret);
		fclose(fp);
		return -1;
	}	    

	sprintf(buffer, "\tkey_mgmt=WPA-PSK\n");
	len = strlen(buffer);
	if((ret = fwrite(buffer, 1, len, fp)) != len)
	{
		printf("devmanage wifimanager fwrite is err, len:%d,ret:%d!", len, ret);
		fclose(fp);
		return -1;
	}


	sprintf(buffer, "\tpairwise=TKIP CCMP\n");
	len = strlen(buffer);
	if((ret = fwrite(buffer, 1, len, fp)) != len)
	{
		printf("devmanage wifimanager fwrite is err, len:%d,ret:%d!", len, ret);
		fclose(fp);
		return -1;
	}

	sprintf(buffer, "\tgroup=TKIP CCMP\n");
	len = strlen(buffer);
	if((ret = fwrite(buffer, 1, len, fp)) != len)
	{
		printf("devmanage wifimanager fwrite is err, len:%d,ret:%d!", len, ret);
		fclose(fp);
		return -1;
	}

	sprintf(buffer, "\tpsk=\"%s\"\n", passwd);
	len = strlen(buffer);
	if((ret = fwrite(buffer, 1, len, fp)) != len)
	{
		printf("devmanage wifimanager fwrite is err, len:%d,ret:%d!", len, ret);
		fclose(fp);
		return -1;
	}

	sprintf(buffer, "\tpriority=1\n}\n");
	len = strlen(buffer);
	if((ret = fwrite(buffer, 1, len, fp)) != len)
	{
		printf("devmanage wifimanager fwrite is err, len:%d,ret:%d!", len, ret);
		fclose(fp);
		return -1;
	}

	fclose(fp);
	return 0;
}

int CNetWifi::StartWifiStationMode(void)
{
	NET_START_PROCESS("sh", "sh", "-c", "killall udhcpd", NULL);
	NET_START_PROCESS("sh", "sh", "-c", "killall hostapd", NULL);
	NET_START_PROCESS("sh", "sh", "-c", "killall wpa_supplicant", NULL);
	NET_START_PROCESS("sh", "sh", "-c", "killall udhcpc", NULL);

	usleep(1000*100);
    NET_START_PROCESS("sh", "sh", "-c", "ifconfig wlan0 up", NULL);
    usleep(1000*100);

	int encrypt = 3;
	const char *ssid = m_ssid.c_str();
	const char *passwd  = m_pwd.c_str();

	if( 0 != strlen(passwd) )
		CreateWpaConfig(encrypt, ssid, passwd);
	else
		CreateWpaConfig(0, ssid, passwd);

	return 0;
}

int CNetWifi::StopWifiStationMode(void)
{
	NET_START_PROCESS("sh", "sh", "-c", "killall wpa_supplicant", NULL);
	NET_START_PROCESS("sh", "sh", "-c", "killall udhcpc", NULL);
	return 0;
}

int CNetWifi::Check_netlink_status(const char * if_name) 
{
	int sockfd;
    struct ifreq ifr;
	int status = 0;
	
    // 打开网络接口设备
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Failed to open socket");
        return -1;
    }

    // 设置要查询的网络接口名称
    strncpy(ifr.ifr_name, if_name, IFNAMSIZ - 1);

    // 查询接口状态
    if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0) {
        perror("Failed to get interface flags");
        close(sockfd);
        return -1;
    }

    // 检查接口的插入状态
    if (ifr.ifr_flags & IFF_RUNNING) {
        // printf("%s interface link up\n",if_name);

		g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_EPI_INDICATOR_LIGHT_FAST_FLICKER);
		status = 1;
    } else {

        // printf("%s interface link down\n",if_name);
		g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_EPI_INDICATOR_LIGHT_ALWAYS_OFF);
		status = 0;
    }

    // 关闭网络接口设备
    close(sockfd);

    return status;
}

int CNetWifi::GetEthStatus(void)
{
	return m_eth;
}

int CNetWifi::GetWifiConnetStatus(void)
{
	if (m_wifi_connect_result == CNetWifi::NET_WIFI_CONNECTED)
		return 1;
	else
		return 0;
}

void CNetWifi::ThreadProc(void)
{
	m_wifi_check_status = CNetWifi::NET_CHECK_WLAN0_VALID;

	Json::Value networkStatus;

	while (m_bLoop)
	{
		if( 1 == Check_netlink_status("eth0") ) 		//网线已插入
		{
			//printf("check eth0...\n");
			m_eth = 1;
		}
		else
		{
			m_eth = 0;
		}
		if( CNetWifi::NET_CHECK_WLAN0_VALID ==  m_wifi_check_status)
		{
			//通知其他模块，网络未连接
			networkStatus["param"] = 0;
			IEventManager::instance()->notify("NetWorkStatus", 0, appEventPulse, NULL, NULL, &networkStatus);

			StartWifiStationMode(); 		//开始连接WiFi
			m_wifi_check_status = CNetWifi::NET_CHECK_WLAN0_INLINE;
		}
		else if( CNetWifi::NET_CHECK_WLAN0_INLINE == m_wifi_check_status)
		{
			FILE *fp = NULL;
			int wrong_key_count = 0;
			int else_count = 0;
			
			char *p = NULL;
			char buffer[256];

			sprintf(buffer, "wpa_supplicant -Dnl80211 -i%s -c /tmp/wpa_supplicant.conf -f /tmp/wpa_output &","wlan0");
			printf("cmd : %s\n", buffer);

			NET_START_PROCESS_CLOSE_FD("sh", "sh", "-c", buffer, NULL);

			m_wifi_connect_result = CNetWifi::NET_WIFI_CONNECTING;
			
			while( CNetWifi::NET_WIFI_CONNECTING == m_wifi_connect_result )
			{
				sleep(1);

				printf("wifi connecting...\n");
				else_count++;
				if( else_count >= 60)
				{
					printf("wifi connect falied...\n");
					m_wifi_connect_result = CNetWifi::NET_WIFI_CONN_FAILED;
					break;
				}		

				fp = fopen("/tmp/wpa_output", "r");
				if( NULL == fp )
					continue;

				while (fgets(buffer, sizeof(buffer), fp) != NULL)
				{
					//printf("%s\n", buffer);
					if ((p = strstr(buffer, "CTRL-EVENT-CONNECTED")) != NULL)
					{
						printf("wifi connected...\n");
						m_wifi_connect_result = CNetWifi::NET_WIFI_CONNECTED;
						break;
					}
					
					if ((p = strstr(buffer, "reason=WRONG_KEY")) != NULL)
					{
						wrong_key_count++;
						if( wrong_key_count >= 2 )
						{
							printf("wifi passwd error...\n");
							m_wifi_connect_result = CNetWifi::NET_WIFI_PSW_ERROR;
							break;
						}
					}
				}
				
				if(fp)
					fclose(fp);
			}

			remove("/tmp/wpa_output");

			if( CNetWifi::NET_WIFI_CONNECTED == m_wifi_connect_result )
			{
			
				printf("udhcpc -b -i wlan0 -h dgiot -s /usr/share/udhcpc/default.script\n");
				NET_START_PROCESS("sh", "sh", "-c", "ifconfig eth0 down", NULL);

				char cmd[255];
				memset(cmd,0,sizeof(cmd));
				snprintf(cmd,sizeof(cmd),"ifconfig wlan0 %s netmask %s",m_ip.c_str(),m_mask.c_str());
				printf("cmd %s\n",cmd);
				NET_START_PROCESS("sh", "sh", "-c", cmd, NULL);

				memset(cmd,0,sizeof(cmd));
				snprintf(cmd,sizeof(cmd),"route add default gw %s",m_gw.c_str());
				printf("cmd %s\n",cmd);
				NET_START_PROCESS("sh", "sh", "-c", cmd, NULL);

				NET_START_PROCESS("sh", "sh", "-c", "ifconfig eth0 up", NULL);

				//add  on 2025.05.06 start
				memset(cmd,0,sizeof(cmd));
				snprintf(cmd,sizeof(cmd),"arping -U -I wlan0 %s -c 3",m_ip.c_str());//避免外界长时间无法ping通当前设备 ,如果部分设备没有arping这条命令，可以找替代这条命令的代码（C语言无偿arp）
				printf("cmd %s\n",cmd);
				NET_START_PROCESS("sh", "sh", "-c", cmd, NULL);
				//add  on 2025.05.06 end
			}

			m_wifi_check_status = CNetWifi::NET_CHECK_WAITE_FOR;
		}
		else if (CNetWifi::NET_CHECK_WAITE_FOR == m_wifi_check_status)
		{
			//WIFI已连接
			//printf("wifi is conected...\n");
			static unsigned char uchWifiUnexpectedCount = 0; 		//WiFi 异常计数
			char ip[20] = {0};
			int b = GetLocalIp("wlan0",ip);
			if ((b == 0) && (ip[0] != '\0')&&(GetNetIfInLine((char*)"wlan0") == 0)) 
			{
				uchWifiUnexpectedCount = 0;
				networkStatus["param"] = 3;
				IEventManager::instance()->notify("NetWorkStatus", 0, appEventPulse, NULL, NULL, &networkStatus);
			}
			else
			{
				printf("wifi get ip unsucc...\n");
				uchWifiUnexpectedCount++;
				if( uchWifiUnexpectedCount > 20 )
				{
					StopWifiStationMode();
					m_wifi_check_status = CNetWifi::NET_CHECK_WLAN0_VALID;
					m_wifi_connect_result = CNetWifi::NET_WIFI_DISCONNECTED;
					uchWifiUnexpectedCount = 0;
				}
			}
		}
		
		CTime::sleep(3 * 1000);
	}
}
int CNetWifi::GetLocalIp(const char *eth_inf, char *ip)
{  
	int sd;  
	//char ip[IP_SIZE];  
	#define IP_SIZE	   16 
	struct sockaddr_in sin;  
	struct ifreq ifr;  

	sd = socket(AF_INET, SOCK_DGRAM, 0);  
	if (-1 == sd)  
	{  
		printf("socket error: %s\n", strerror(errno));  
		return -1;        
	}  

	strncpy(ifr.ifr_name, eth_inf, IFNAMSIZ);  
	ifr.ifr_name[IFNAMSIZ - 1] = 0;  

	// if error: No such device  
	if (ioctl(sd, SIOCGIFADDR, &ifr) < 0)  
	{  
		printf("ioctl error: %s\n", strerror(errno));  
		close(sd);  
		return -1;  
	}  

	memcpy(&sin, &ifr.ifr_addr, sizeof(sin));  
	snprintf(ip, IP_SIZE, "%s", inet_ntoa(sin.sin_addr));

	close(sd);  
	return 0;  
}
int CNetWifi::GetNetIfInLine(char *name)
{
	FILE *fp = NULL;
	int ret;
    char buf[1024] = {0};
	char cmd[255];
	snprintf(cmd,sizeof(cmd),"ifconfig %s",name);
    fp = popen(cmd,"r");
	if( !fp )
	{
		return -1;
	}
	ret = fread(buf, 1, 1023, fp);
	if(ret >= 0)
		buf[ret] = '\0';
    //printf("cmd:%s\n",cmd);
	//printf("buf:%s\n",buf);
	pclose(fp);
	if (strstr(buf,"RUNNING") != NULL) {
		return 0;
	}
	return -1;
}
void CNetWifi::SetWifi(std::string ssid, std::string pwd,std::string ip, std::string gw, std::string mask)
{
	m_ssid = ssid;
	m_pwd = pwd;
	m_ip = ip;
	m_gw = gw;
	m_mask = mask;

	m_wifi_connect_result = CNetWifi::NET_WIFI_DISCONNECTED;
	m_wifi_check_status = CNetWifi::NET_CHECK_WLAN0_VALID;

	printf("wifi m_ssid : %s\n", m_ssid.c_str());
	printf("wifi m_pwd: %s\n", m_pwd.c_str());
	printf("wifi m_ip : %s\n", m_ip.c_str());
	printf("wifi m_gw : %s\n", m_gw.c_str());
	printf("wifi m_mask : %s\n", m_mask.c_str());
}

int CNetWifi::GetWifiWifiList(WIFI_LIST* list, int MAX_Len)
{
	int realApNum = -1;
	char tmp[512] = {0};
	memset(tmp, 0, sizeof(tmp));
	unsigned char bNeedFindHead = 1;
	int recordIdx = -1;
	
	FILE *pp = popen("iwlist wlan0 scan", "r");
	if(pp == NULL)
	{
		printf("WifiList--->popen fails\r\n");
		return 0;
	}

#if 0 	/// debug
	while (fgets(tmp, sizeof(tmp), pp) != NULL)
	{
		printf("%s", tmp);
	}
	exit:
		return 0;
#else

	while (fgets(tmp, sizeof(tmp), pp) != NULL)
	{
//		printf("%s", tmp);
		/* 首先找BSSID	  以此为基准 */
		char *pBSSIDStart = strstr(tmp, " - Address: ");
		if(pBSSIDStart != NULL)
		{
			recordIdx++;
			if(recordIdx >= MAX_Len)
			{
				printf(" Reach Max Record \r\n");
				recordIdx--;
				break;
			}
			
			bNeedFindHead = 0;
			goto ReadNext;
		}
		else
		{
			if(recordIdx < 0)
			{/* 只有找到第一个BSSID 才能继续读下去 */
				goto ReadNext;
			}
		}

		if( 1 == bNeedFindHead )
		{
			goto ReadNext;
		}
		
		{
			/* 查找ssid  */
			char *pSSIDStart = strstr(tmp, "ESSID:\"");
			if(pSSIDStart != NULL)
			{
#if 1
                sscanf(pSSIDStart + strlen("ESSID:\""), "%s", list[recordIdx].essid);
                int s_len = strlen((char *)(list[recordIdx].essid));
                if(s_len != 0)
                {
                    list[recordIdx].essid[s_len - 1] = 0;
                    s_len--;
                }
#else
				snprintf(list[recordIdx].essid, sizeof(list[recordIdx].essid), "%s", pSSIDStart + strlen("ESSID:\""));
#endif
				if( 0 == strlen(list[recordIdx].essid) )
				{
					bNeedFindHead = 1;
					recordIdx--;
				}
				goto ReadNext;
			}
		}
#if 0
		{
			/* 查找channel	*/
			char *pCHANNELStart = strstr(tmp, "(Channel ");
			if(pCHANNELStart != NULL)
			{
				int x = 0;
				sscanf(pCHANNELStart + strlen("(Channel "), "%d)", &x);
				list[recordIdx].channel = x;
				goto ReadNext;
			}
		}
#endif
#if 0
		{
			/* 查找signal  */
			char *pSIGNALStart = strstr(tmp, "Quality=");
			if(pSIGNALStart != NULL)
			{
				int x = 0;
				int y = 0;
				sscanf(pSIGNALStart + strlen("Quality=") , "%d/%d ",&x,&y);
				list[recordIdx].quality = x*100/y;
				goto ReadNext;
			}
		}
#else
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
	
				list[recordIdx].quality = ((iDbmVal - (-100)) * 100) / 70;
				goto ReadNext;
			}
		}


#endif

ReadNext:
		memset(tmp, 0, sizeof(tmp));
	}

	pclose(pp);

	realApNum = recordIdx + 1;
	return realApNum;
#endif
}

void CNetWifi::onAppEvent(std::string code, int index, appEventAction action, const EventHandler *param, Json::Value* data)
{
    if ("MqttError" == code)
    {
        printf("MqttErrors\n");

        m_wifi_check_status = CNetWifi::NET_CHECK_WLAN0_VALID;
		m_wifi_connect_result = CNetWifi::NET_WIFI_DISCONNECTED;
    }
}
