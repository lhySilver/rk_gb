#include "TestMonitor.h"
#include "ITestModuleV2.h"//add on 2025.02.08
#include "ILog.h"
#include "Common.h"
#include "NetWifi.h"

#include "../../Include/PathDefinition.h"
#include "TestAskBase.h"

//add on 2025.02.10 同步产测时间
#include <sys/time.h>
#include <time.h>
//add on 2025.02.10 同步产测时间

extern char g_rtsp_url[100];
extern char g_rtsp_url_sub[100];

bool g_bDevInfoChanged = false;//add on 2025.01.02 设备信息同步

//add on 2024.12.28 音频测试
extern bool g_mic_is_open;
ProductAudioRecord_S s_productAudioRecord = {0};//存储录音
bool bIsAudioRecording = false; 	//是否正在录音
bool bStartAudioRec = false; 		//开始录音
//add on 2024.12.28 end


void GetDevInfo(DeviceInfo *info)
{
	unsigned char wifi_mac[6] = {0};
	char str_wifi_mac[24] = {0};

	NetGetMac_2("wlan0", wifi_mac);
	snprintf(str_wifi_mac, sizeof(str_wifi_mac), "%02X:%02X:%02X:%02X:%02X:%02X", wifi_mac[0], wifi_mac[1], wifi_mac[2], wifi_mac[3], wifi_mac[4], wifi_mac[5]);

    char str_devmode[10] = {0};
    sprintf(str_devmode, "%u", DevInfoFromEEPROM_g.DevMode);  // 转换为无符号整数的字符串

	info->mProductModel  = str_devmode;
	info->mVersion 	     = IPC_APP_VERSION;
	info->mCloudPlatform = "tuya";
    info->mMac 		    = str_wifi_mac;
	info->mPid 		    = (char *)(DevInfoFromEEPROM_g.TUYA_PID);
	info->mUid		    = (char *)(DevInfoFromEEPROM_g.TUYA_UID);
#if defined(RC0240) || defined(RC0240V20) || defined(RC0240V30) || defined(RC0240V40)
	if(g_NetWifi.GetEthStatus()){
		info->mEthernetStatus = EthernetStatus::INSERT_CABLE;
	}
	else if(0 == g_NetWifi.GetEthStatus())
	{
		info->mEthernetStatus = EthernetStatus::NOT_INSERT_CABLE;
	}
#else
	info->mEthernetStatus = EthernetStatus::NOT_SUPPORT;
#endif
}



std::shared_ptr<TestMonitor> &TestMonitor::GetInstance(std::shared_ptr<TestMonitor> *impl)
{
    static auto instance = std::make_shared<TestMonitor>();
    if (impl) {
        if (instance.use_count() == 1) {
            LogWarning("Instance changed succeed.\n");
            instance = *impl;
        }
        else {
            LogError("Can't changing the instance becase of using by some one.\n");
        }
    }
    return instance;
}

TestCode TestMonitor::PeerGetRtspUrl(std::string &url, int &videoWidth, int &videoHeight, std::string &errorMessage)
{
    LogWarning("Mock response peer get rtsp url.\n");
    const std::string EMPTY_ERROR_MESSAGE = "";
    errorMessage = EMPTY_ERROR_MESSAGE;
	//主码流
    // url = g_rtsp_url;//"rtsp://192.168.0.1:554/test";
    // videoWidth = 1920*2;
    // videoHeight = 1080;

	//主码流
    url = g_rtsp_url;//"rtsp://192.168.0.1:554/test";
    videoWidth = 1920*2;
    videoHeight = 1080;
    return TestCode::SUCCESS;
}

TestCode TestMonitor::PeerGetRtspUrlV2(std::string &url, int &videoWidth, int &videoHeight,
                                       std::string &urlSub, int &videoWidthSub, int &videoHeightSub,
                                       std::string &errorMessage)
{
	LogWarning("Mock response peer get rtsp url.\n");
    const std::string EMPTY_ERROR_MESSAGE = "";
    errorMessage = EMPTY_ERROR_MESSAGE;	

		//主码流
    url = g_rtsp_url;
    videoWidth = 1920 * 2;
    videoHeight = 1080;

	//子码流
	urlSub = g_rtsp_url_sub;
    videoWidthSub = 640 * 2;
    videoHeightSub = 360;
    return TestCode::SUCCESS;
}

static void *thread_reboot(void *arg)
{
	NormalRestart();
	return NULL;
}
TestCode TestMonitor::PeerRebootDevice(std::string &errorMessage)
{
    LogWarning("Device reboot.\n");
	CreateDetachedThread((char*)"thread_reboot",thread_reboot, (void *)NULL, true);
    const std::string EMPTY_ERROR_MESSAGE = "";
    errorMessage = EMPTY_ERROR_MESSAGE;
    return TestCode::SUCCESS;
}

static void *thread_reset(void *arg)
{
	SystemReset();
	return NULL;
}

TestCode TestMonitor::PeerResetDevice(std::string &errorMessage)//设备复位
{
	LogWarning("Device reset.\n");
	CreateDetachedThread((char*)"thread_reset",thread_reset, (void *)NULL, true);
    const std::string EMPTY_ERROR_MESSAGE = "";
    errorMessage = EMPTY_ERROR_MESSAGE;
    return TestCode::SUCCESS;
}

TestCode TestMonitor::PeerSetConfigParam(void)
{
    LogWarning("set config param.\n");
    const std::string EMPTY_ERROR_MESSAGE = "unsupport";
//    errorMessage = EMPTY_ERROR_MESSAGE;
    return TestCode::UN_SUPPORT_FUNCTION;
}
TestCode TestMonitor::PeerGetConfigParam(void)
{
    LogWarning("get config param.\n");
    const std::string EMPTY_ERROR_MESSAGE = "unsupport";
//    errorMessage = EMPTY_ERROR_MESSAGE;
    return TestCode::UN_SUPPORT_FUNCTION;
}

TestCode TestMonitor::PeerSetSN(const std::string &sn, std::string &errorMessage)//废弃
{
    LogWarning("set sn.\n");
    const std::string EMPTY_ERROR_MESSAGE = "unsupport";
    errorMessage = EMPTY_ERROR_MESSAGE;
    return TestCode::UN_SUPPORT_FUNCTION;
}

TestCode TestMonitor::PeerGetSN(std::string &sn, std::string &errorMessage)//废弃
{
    LogWarning("get sn.\n");
    const std::string EMPTY_ERROR_MESSAGE = "unsupport";
    errorMessage = EMPTY_ERROR_MESSAGE;
    return TestCode::UN_SUPPORT_FUNCTION;
}

TestCode TestMonitor::PeerControlPTZ(const PTZCommand &command, const unsigned int &step, std::string &errorMessage)//不用,暂且保留
{
	LogWarning("ctrl ptz.\n");
    const std::string EMPTY_ERROR_MESSAGE = "unsupport";
    errorMessage = EMPTY_ERROR_MESSAGE;
    return TestCode::UN_SUPPORT_FUNCTION;
}

TestCode TestMonitor::PeerControlPTZ(const PTZCommand &command, const bool &keep, std::string &errorMessage)
{
    LogError("ctrl ptz.\n");
	printf("\033[1;32m ctrl ptz, cmd:%d keep:%d\033[0m\n",static_cast<int>(command), keep);

	PTZDirection_e dir;
	switch (command)
	{
		case PTZCommand::PTZ_UP:
			dir = PTZ_UP;
			break;
		case PTZCommand::PTZ_DOWN:
			dir = PTZ_DOWN;
			break;
		case PTZCommand::PTZ_LEFT:
			dir = PTZ_LEFT;
			break;
		case PTZCommand::PTZ_RIGHT:
			dir = PTZ_RIGHT;
			break;
		case PTZCommand::PTZ_SELF_LEVEL://水平
			break;
		case PTZCommand::PTZ_SELF_VERTICAL://垂直
			break;
		case PTZCommand::PTZ_SELF_TEST:
		{
			g_PtzHandle.DoAutoTest();	
			printf("\033[1;32m  PTZ_SELF_TEST \033[0m\n"); 
    		return TestCode::SUCCESS;
		}
		case PTZCommand::PTZ_RESET:
		{
			// g_PtzHandle.DoSelfCheck();	
			printf("\033[1;32m  PTZ_RESET \033[0m\n"); 
    		return TestCode::SUCCESS;
		}
		default :
		{
			const std::string EMPTY_ERROR_MESSAGE = "false ptz_command!";
    		errorMessage = EMPTY_ERROR_MESSAGE;
			return TestCode::TEST_FAILED;
		}
	}

	if(keep)
		g_PtzHandle.DoMotorMove(dir);
	else
	{
		g_PtzHandle.DoMotorStop();

		const std::string EMPTY_ERROR_MESSAGE = "";
		errorMessage = EMPTY_ERROR_MESSAGE;
		return TestCode::SUCCESS;
	}
	// 	g_PtzHandle.DoMotorStop();

    // const std::string EMPTY_ERROR_MESSAGE = "";
    // errorMessage = EMPTY_ERROR_MESSAGE;
    // return TestCode::SUCCESS;
}



#define MW_START_PROCESS( file, cmd, ... ) \
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


int WifiList_3(router_list_2_s* list, int MAX_Len)
{
	int wifi_num = 0;
	int line_num = 0;
	char tmp[512] = {0};
	memset(tmp, 0, sizeof(tmp));
	
	MW_START_PROCESS("sh", "sh", "-c", "wpa_cli scan", NULL);
	sleep(4);

	FILE *pp = popen("wpa_cli scan_r", "r");
	if(pp == NULL)
	{
		printf("WifiList--->popen fails\r\n");
		goto exit;
	}

	/*
	Selected interface 'wlan0'
	bssid / frequency / signal level / flags / ssid
	ec:b9:70:f0:66:e4       2462    -39     [WPA-PSK-CCMP+TKIP][WPA2-PSK-CCMP+TKIP][ESS]    DGIOT
	*/
	while (fgets(tmp, sizeof(tmp), pp) != NULL)
	{
		//printf("[line %d]: %s", line_num, tmp);
		line_num++;
		if (line_num < 3)
			continue;

		unsigned char SPACE_CHAR = 0x09;
		int frequency = 0;
		int iSignalLevel = 0;
		char strSignalLevel[6];
		char strFrequency[6];

		int pos = 0;
		int i = 0;
		//bssid
		for (; SPACE_CHAR != tmp[pos] && '\0' != tmp[pos]; pos++);
		for (; SPACE_CHAR == tmp[pos]; pos++);
		if ('\0' == tmp[pos]) continue;
		//frequency
		// for (; SPACE_CHAR != tmp[pos] && '\0' != tmp[pos]; pos++);
		for (i = 0; SPACE_CHAR != tmp[pos] && '\0' != tmp[pos] && i < sizeof(strFrequency)-1; pos++, i++)
		{
			strFrequency[i] = tmp[pos];
		}
		strFrequency[i] = '\0';
		frequency = atoi(strFrequency);
		printf("frequency = %d\r\n", frequency);
		for (; SPACE_CHAR == tmp[pos]; pos++);
		if ('\0' == tmp[pos]) continue;

		//signal level
		for (i = 0; SPACE_CHAR != tmp[pos] && '\0' != tmp[pos] && i < sizeof(strSignalLevel)-1; pos++, i++)
			strSignalLevel[i] = tmp[pos];
		strSignalLevel[i] = '\0';
		iSignalLevel = atoi(strSignalLevel);
		//----------
		if(frequency > 2000 && frequency < 3000)
		{
			printf("----------------->2.4G\r\n");
			if (iSignalLevel > (-30))
				iSignalLevel = -30;

			if (iSignalLevel < (-100))
				iSignalLevel = -100;

			list[wifi_num].quality = (iSignalLevel - (-100)) * 100 / 70;

		}
		else if(frequency > 4000 && frequency < 6000)
		{
			printf("----------------->5G\r\n");
			if (iSignalLevel > (-40))
				iSignalLevel = -40;

			if (iSignalLevel < (-100))
				iSignalLevel = -100;

			list[wifi_num].quality = (iSignalLevel - (-100)) * 100 / 60;
		}
		//------------
		// if (iSignalLevel > (-40))
		// 	iSignalLevel = -40;
		// list[wifi_num].quality = (iSignalLevel - (-110)) * 100 / ((-40) - (-110));
		printf("quality = %d\r\n", list[wifi_num].quality);
		for (; SPACE_CHAR == tmp[pos]; pos++);
		if ('\0' == tmp[pos]) continue;

		//flags
		for (; SPACE_CHAR != tmp[pos] && '\0' != tmp[pos]; pos++);
		for (; SPACE_CHAR == tmp[pos]; pos++);
		if ('\0' == tmp[pos]) continue;
		//ssid
		for (i = 0; SPACE_CHAR != tmp[pos] && '\0' != tmp[pos] && '\r' != tmp[pos] && '\n' != tmp[pos] && i <= WIFI_ESSID_NAME_LEN; pos++, i++)
			list[wifi_num].essid[i] = tmp[pos];
		list[wifi_num].essid[i] = '\0';//这个地方会出现换行符，所以这里要减去1个字符的长度
		// list[wifi_num].essid[i-1] = '\0';
		printf("essid = %s\r\n", list[wifi_num].essid);

		wifi_num++;
		if (wifi_num >= MAX_Len)
			break;
		
		memset(tmp, 0, sizeof(tmp));
	}

	pclose(pp);

exit:

	return wifi_num;
}


#define MAX_SEARCH_WIFI (50)
TestCode TestMonitor::PeerGetWifiList(const std::string &recvSsid, std::vector<WifiInfo> &wifiList, std::string &errorMessage)
{
    LogWarning("get wifi list.\n");
	std::string ssid = recvSsid;
	LogWarning("ssid:%s\n", ssid.c_str());
#if 1
	int wifiNum = 0;
	router_list_2_s tmpWifiList[MAX_SEARCH_WIFI];
	router_list_2_s s_wifiResult = {0};

	memset(tmpWifiList, 0, sizeof(tmpWifiList));
	memset(&s_wifiResult, 0, sizeof(router_list_2_s));

	wifiNum = WifiList_3(tmpWifiList, MAX_SEARCH_WIFI);
	if(wifiNum > 0)
	{
		for(int i = 0; i < wifiNum; i++)//查找指定ssid的wifi信息
		{
			printf("ssid:%s, quality:%d\n", tmpWifiList[i].essid, tmpWifiList[i].quality);
			if(strcmp(tmpWifiList[i].essid, ssid.c_str()) == 0)
			{
				memcpy(&s_wifiResult, &tmpWifiList[i], sizeof(router_list_2_s));
				printf("ssid:%s, quality:%d\n", s_wifiResult.essid, s_wifiResult.quality);
				break;
			}
		}

		if(s_wifiResult.essid[0] == '\0')
		{
			errorMessage = "wifi ssid not found!";
			return TestCode::TEST_FAILED;
		}
		else
		{
			WifiInfo info0(s_wifiResult.essid, s_wifiResult.quality);
			wifiList.push_back(info0);
			errorMessage = "";
			return TestCode::SUCCESS;

		}
				
	}
	else
	{
		errorMessage = "get wifi list failed!";
		return TestCode::TEST_FAILED;	
	}

#else
	//获取WiFi列表 add on 2025.03.07<添加代码---start> 
	int wifiNum = 0;
	int testResult = 0;
	CNetWifi::WIFI_LIST stWifi = {0};
	CNetWifi::WIFI_LIST s_getWifiList[MAX_SEARCH_WIFI];
	// CNetWifi::WIFI_LIST s_tpWifiList = {0};
	
	memset(&stWifi, 0, sizeof(CNetWifi::WIFI_LIST));
	memset(s_getWifiList, 0, sizeof(s_getWifiList));
	// memset(s_tpWifiList, 0, sizeof(s_tpWifiList));

	wifiNum = g_NetWifi.GetWifiWifiList(s_getWifiList, MAX_SEARCH_WIFI);

	if(wifiNum > 0)
	{	
		for(int i = 0; i < wifiNum; i++)//查找指定ssid的wifi信息
		{
			if(strcmp(s_getWifiList[i].essid, recvSsid.c_str()) == 0)
			{
				memcpy(&stWifi, &s_getWifiList[i], sizeof(CNetWifi::WIFI_LIST));
				break;
			}
		}
		if(stWifi.essid[0] == '\0')
		{
			errorMessage = "wifi ssid not found!";
			return TestCode::TEST_FAILED;
		}
		else
		{
			WifiInfo info0(stWifi.essid, stWifi.quality);
			wifiList.push_back(info0);
			errorMessage = "";
			return TestCode::SUCCESS;
		}

	}
	else
	{
		errorMessage = "get wifi list failed!";
		return TestCode::TEST_FAILED;	
	}
#endif

}



//add on 2024.12.28 音频测试 ------------->start
void DoProductAudioRecord(const unsigned char *pData, unsigned int size)
{
	if (g_mic_is_open && s_productAudioRecord.size < PRODUCT_AUDIO_REC_BUFF_SIZE)
	{
		int len = PRODUCT_AUDIO_REC_BUFF_SIZE - s_productAudioRecord.size;
		len = len < size ? len : size;
		memcpy(s_productAudioRecord.audio_data+s_productAudioRecord.size, pData, len);
		s_productAudioRecord.size += len;
	}
}

static int SaveProductRecord(unsigned char *buffer,int size)
{
    FILE *file = fopen(AUDIO_FILE_PRODUCT_RECORD, "wb");
    if (file == NULL) 
	{
        perror("Error opening file");
        return 1;
    }

    size_t written = fwrite(buffer, 1, size, file);
    if (written != size) 
	{
        perror("Error writing to file");
        fclose(file);
        return 1;
    }

    fclose(file);

    printf("Data has been written to '%s' successfully, size:%lld\n",AUDIO_FILE_PRODUCT_RECORD, written);

    return 0;
}



static void* pthread_product_audio_record(void* arg)
{
	prctl(PR_SET_NAME, "product_record");
	
	int ret = 0;
	s_productAudioRecord.size = 0;

	while (bStartAudioRec && s_productAudioRecord.size < PRODUCT_AUDIO_REC_BUFF_SIZE)
	{
		usleep(20*1000);
	}
	LogError("s_productAudioRecord.size: %d\n", s_productAudioRecord.size);

	ret = SaveProductRecord(s_productAudioRecord.audio_data, s_productAudioRecord.size);
	LogError("SaveProductRecord. ret: %d\n", ret);

	
	CAudioPrompt::AudioFileParm audioFile;
	audioFile.strFileName = AUDIO_FILE_PRODUCT_RECORD;
	audioFile.type = 0;
	g_AudioPrompt.aoPlay(audioFile);

	g_mic_is_open   = false;//回调中停止获取音频数据
	s_productAudioRecord.size = 0;
	bIsAudioRecording = false; //录音结束
}


int StartProductAudioRec()
{
	if (bIsAudioRecording)
	{
		LogError("Audio already recording.\n");
		return -1;
	}

	bIsAudioRecording = true;
	bStartAudioRec = true;

	int ret;
	pthread_t pthread_pid;
	pthread_attr_t pthread_attr;
	pthread_attr_init(&pthread_attr);
    pthread_attr_setstacksize(&pthread_attr, 100 * 1024);
	pthread_attr_setdetachstate( &pthread_attr, PTHREAD_CREATE_DETACHED);
	ret = pthread_create(&pthread_pid,  &pthread_attr, pthread_product_audio_record, NULL);
	pthread_attr_destroy(&pthread_attr);
	if (ret)
	{
		LogError("create thread fail. ret: %d, errno: %d\n", ret, errno);
		bStartAudioRec = false;
		bIsAudioRecording = false;
		return -1;
	}
	return 0;
}

//add on 2024.12.28 音频测试 ------------->end

TestCode TestMonitor::PeerTestMicAndHorn(std::shared_ptr<VTestReply> reply, const bool &isOpen,
                                         std::string &errorMessage)
{
	bool isReplyAsynchronously = false;
    if (isReplyAsynchronously) {
		//录音以及播放录音的流程
		return TestCode::RESPONE_ASYNCHRONOUSLY;
	}
	else{
		LogWarning("test mic. isOpen: %d\n", isOpen);
		g_mic_is_open = isOpen;

		StartProductAudioRec();//add on 2024.12.28

		const std::string EMPTY_ERROR_MESSAGE = "";
		errorMessage = EMPTY_ERROR_MESSAGE;
		return TestCode::SUCCESS;

	}
    
}

TestCode TestMonitor::PeerTestInfraredLamp(const bool &isOpen, std::string &errorMessage)
{
	LogWarning("test infrared lamp. isOpen: %d\n", isOpen);
	g_Camera.doIrLedCtrl(isOpen);
	
    const std::string EMPTY_ERROR_MESSAGE = "";
    errorMessage = EMPTY_ERROR_MESSAGE;
    return TestCode::SUCCESS;
} 

TestCode TestMonitor::PeerGetSDCartInfo(unsigned int &totalSize_MB, unsigned int &usedSize_MB,
                           std::string &errorMessage)
{
    LogWarning("get sd catd info.\n");
	int iDiskStatus = DISK_STATUS_UNKNOWN;
	unsigned int total = 0;
	unsigned int free = 0;
	iDiskStatus = g_StorageManager->GetDiskState();
	
	//未检测到SD 卡
	if (DISK_STATUS_NORMAL != iDiskStatus)
	{
		errorMessage = "sd card error.";
		return TestCode::TEST_FAILED;
	}
	//有卡
	else
	{
		unsigned long long ullDiskSize = 0;
		unsigned long long ullDiskUsedSize = 0;
		g_StorageManager->GetDiskcapacity(&ullDiskSize, &ullDiskUsedSize, NULL);
		totalSize_MB = ullDiskSize / 1024;
		usedSize_MB = ullDiskUsedSize / 1024;
		errorMessage = "";
		return TestCode::SUCCESS;
	}
}

TestCode TestMonitor::PeerTestPWMLamp(const unsigned short &brightness, std::string &errorMessage)
{
#if 1
	LogWarning("test pwm lamp. brightness: %d\n", brightness);
	int brightness_value = brightness;
	if(0 == brightness_value){
		g_Camera. doWhiteLedCtrlSwitch(false);
		
	}	
	else {
		if(brightness_value > 100){
			brightness_value = 100;
		}
		g_Camera. doWhiteLedCtrlSwitch(true);
		g_Camera. doWhiteLedCtrlBrightness(brightness_value);
	}
		
    const std::string EMPTY_ERROR_MESSAGE = "";
    errorMessage = EMPTY_ERROR_MESSAGE;
    return TestCode::SUCCESS;


#else
    LogWarning("test pwm lamp. brightness: %d\n", brightness);
	static bool s_isPWMLampOpen = false;
	s_isPWMLampOpen  = !s_isPWMLampOpen;
	// if()
	g_Camera. doWhiteLedCtrlSwitch(s_isPWMLampOpen);
	// doWhiteLedCtrlBrightness
	// g_Camera.SetWhiteLed(0 != brightness);
    const std::string EMPTY_ERROR_MESSAGE = "";
    errorMessage = EMPTY_ERROR_MESSAGE;
    return TestCode::SUCCESS;
#endif
}

TestCode TestMonitor::PeerSetDayNightMode(const DayNightMode &mode, std::string &errorMessage)
{
    LogWarning("set day night mode. mode: %d\n", mode);
	if (DayNightMode::DAY == mode)
		g_Camera.setMode(CAMERA_MODE_DAY);
	if (DayNightMode::NIGHT == mode)
		g_Camera.setMode(CAMERA_MODE_NIGHT);
    const std::string EMPTY_ERROR_MESSAGE = "";
    errorMessage = EMPTY_ERROR_MESSAGE;
    return TestCode::SUCCESS;
}

TestCode TestMonitor::PeerIrcutSwitch(const bool &isOpen, std::string &errorMessage)//切 ir-cut
{
	LogWarning("set IrcutSwitch. state: %s\n", isOpen?"true":"false");
#if 1
	g_Camera.doIrcutCtrl(isOpen);
	const std::string EMPTY_ERROR_MESSAGE = "";
    errorMessage = EMPTY_ERROR_MESSAGE;
    return TestCode::SUCCESS;

#else
	static bool s_isIrcutOpen = false;
	s_isIrcutOpen  = !s_isIrcutOpen;
	g_Camera.doIrcutCtrl(s_isIrcutOpen);
    const std::string EMPTY_ERROR_MESSAGE = "";
    errorMessage = EMPTY_ERROR_MESSAGE;
    return TestCode::SUCCESS;
#endif
}


#define LIGHT_NO_SAMETIME //九联反馈不要让灯同时亮

/**
 * @brief 根据LED测试类型设置指示灯状态
 *
 * 根据输入的LED测试类型，设置相应的指示灯状态，并返回操作结果。
 *
 * @param test LED测试类型
 * @param errorMessage 错误信息
 * @return 操作结果，成功则返回TestCode::SUCCESS
 */
TestCode TestMonitor::PeerTestIndicatorLed(const std::string &switchName, const LEDTest &test,
                                          std::string &errorMessage)
{
    LogWarning("test indicator led. switchName: %s ,test: %s\n", switchName.c_str(),ITestModuleV2::PrintfLEDTest(test));
	
	
	if(switchName == LED_RED){//指示灯红
		if(LEDTest::ON == test){
			g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_POWER_INDICATOR_LIGHT_ALWAYS_ON);
			#ifdef LIGHT_NO_SAMETIME
			g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_LINK_INDICATOR_LIGHT_ALWAYS_OFF);
			#endif
		}
		else if(LEDTest::OFF == test){
			g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_POWER_INDICATOR_LIGHT_ALWAYS_OFF);
		}
		errorMessage = "";
	}
	else if(switchName == LED_BLUE){//指示灯蓝
		if(LEDTest::ON == test){
			g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_LINK_INDICATOR_LIGHT_ALWAYS_ON);
			#ifdef LIGHT_NO_SAMETIME
			g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_POWER_INDICATOR_LIGHT_ALWAYS_OFF);
			#endif
		}
		else if(LEDTest::OFF == test){
			g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_LINK_INDICATOR_LIGHT_ALWAYS_OFF);
		}
		errorMessage = "";
		return TestCode::SUCCESS;
	}
	else{//指示灯---其他指示灯
		errorMessage = "Current LED is not Supported...";
		return TestCode::TEST_FAILED;
	}
	
#if 0
	
	switch (test)
	{
		case LEDTest::RED:
			printf("\033[1;35m  ---------------->RED Led  is on  \033[0m\n");
			g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_LINK_INDICATOR_LIGHT_ALWAYS_OFF);
			g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_POWER_INDICATOR_LIGHT_ALWAYS_ON);
			
			errorMessage = "";
					
				
        break;

        case LEDTest::GREEN:
			errorMessage = "Green LED is not supported...";
			
        break;

		case LEDTest::BLUE:
			printf("\033[1;36m  ---------------->RED Led  is on   \033[0m\n");	
			g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_POWER_INDICATOR_LIGHT_ALWAYS_OFF);
			g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_LINK_INDICATOR_LIGHT_ALWAYS_ON);
			
			errorMessage = "";
				
        break;

		case LEDTest::OFF:
			g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_POWER_INDICATOR_LIGHT_ALWAYS_OFF);
			g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_LINK_INDICATOR_LIGHT_ALWAYS_OFF);
			errorMessage = "";

		break;


		default:
			errorMessage = "Current LED is not Supported...";
		break;
	}
    // const std::string EMPTY_ERROR_MESSAGE = "";
    // errorMessage = EMPTY_ERROR_MESSAGE;
    return TestCode::SUCCESS;
#endif
}

TestCode TestMonitor::PeerGetTemperatureHumidity(float &temperature, float &humidity, std::string &errorMessage)
{
    LogWarning("get temp hum.\n");
    const std::string EMPTY_ERROR_MESSAGE = "unsupport";
    errorMessage = EMPTY_ERROR_MESSAGE;
    return TestCode::UN_SUPPORT_FUNCTION;
}

TestCode TestMonitor::PeerPirAlarm(const unsigned short &pir, std::string &errorMessage)//废弃
{
    LogWarning("pir alarm.\n");
    const std::string EMPTY_ERROR_MESSAGE = "unsupport";
    errorMessage = EMPTY_ERROR_MESSAGE;
    return TestCode::UN_SUPPORT_FUNCTION;
}

TestCode TestMonitor::PeerHornTest(const bool &open, std::string &errorMessage)//喇叭测试
{
    LogWarning("horn test.\n");
	CAudioPrompt::AudioFileParm audioFile;
	audioFile.strFileName = AUDIO_FILE_DINGDINGDING;
	audioFile.type = 0;
	g_AudioPrompt.aoPlay(audioFile);
    const std::string EMPTY_ERROR_MESSAGE = "";
    errorMessage = EMPTY_ERROR_MESSAGE;
    return TestCode::SUCCESS;
}


TestCode TestMonitor::PeerSendLicense(const char *license, const unsigned int &length, const std::string &pid, std::string &errorMessage)
{
    LogError("license test.\n");

    if (license == NULL || length == 0 || pid.size() != 16) // 检查 license 是否为空 / pid长度是否合法
	{
		const std::string EMPTY_ERROR_MESSAGE = "Invalid license data or Invalid tuya_pid!";
		errorMessage = EMPTY_ERROR_MESSAGE;
		return TestCode::TEST_FAILED;
    }


	const char* str_pid = pid.c_str();  // 获取 C 风格的字符串

	int ret = g_EncryptionHandle.BurnLicense_NewProductProcess(license, length, str_pid);//烧录
	

	if(0 == ret)
	{
		g_bDevInfoChanged = true;////add on 2025.01.02 设备信息同步
		const std::string EMPTY_ERROR_MESSAGE = "Burn license success!";
		errorMessage = EMPTY_ERROR_MESSAGE;
		return TestCode::SUCCESS;
	}
	else//烧录失败信息上报
	{
		LogError( "Burn license fail, ret:%d\n", ret);		
		if(-1 == ret)
		{
			const std::string EMPTY_ERROR_MESSAGE = "Already burned...!";
			errorMessage = EMPTY_ERROR_MESSAGE;
		}
		else if(-2 == ret)
		{
			const std::string EMPTY_ERROR_MESSAGE = "License_pid and compare_pid is not same...!";
			errorMessage = EMPTY_ERROR_MESSAGE;
		}
		else if(-3 == ret)
		{
			const std::string EMPTY_ERROR_MESSAGE = "License is not math...!";
			errorMessage = EMPTY_ERROR_MESSAGE;
		}	
		else if(-4 == ret)
		{
			const std::string EMPTY_ERROR_MESSAGE = "CheckDevLicense error...!";
			errorMessage = EMPTY_ERROR_MESSAGE;
		} 			
		return TestCode::TEST_FAILED;	
	}
}
/************************************烧号************************************/

TestCode TestMonitor::PeerGetDeviceInfo(DeviceInfo &info, std::string &errorMessage)//工具主动向设备获取设备信息
{

	GetDevInfo(&info);
#if 1
	AskCheckTimeFromPeer();//主动询问设备时间
#endif

	const std::string EMPTY_ERROR_MESSAGE = "";
	errorMessage = EMPTY_ERROR_MESSAGE;
	return TestCode::SUCCESS;
}


void TestMonitor::SendPirReportEvent(const std::string &pirValue)//触发pir上报
{
    class TestAskForTest : public TestAsk<TestCode>, public TestAskBase
    {
    public:
        TestAskForTest() : TestAskBase(TestAskBlock::NOT_BLOCK, TestAskReply::NEED_NOT_REPLY)
        {

        }
        virtual ~TestAskForTest() = default;
    };
    std::shared_ptr<VTestAsk> ask = std::make_shared<TestAskForTest>();
    const std::string EMPTY_ERROR_MESSAGE = "";
    ITestModuleV2::GetInstance()->SetPirEventToPeer(ask, pirValue, EMPTY_ERROR_MESSAGE);
}


void TestMonitor::SendKeyReportEvent(const std::string &keyName, const KeyTest &event)
{
	class TestAskForTest : public TestAsk<TestCode>, public TestAskBase
    {
    public:
        TestAskForTest() : TestAskBase(TestAskBlock::NOT_BLOCK, TestAskReply::NEED_NOT_REPLY)
        {
        }
        virtual ~TestAskForTest() = default;
    };
    std::shared_ptr<VTestAsk> ask = std::make_shared<TestAskForTest>();
    const std::string EMPTY_ERROR_MESSAGE = "";
    ITestModuleV2::GetInstance()->SetKeyEventToPeer(ask, keyName, event, EMPTY_ERROR_MESSAGE);

}


void TestMonitor::SendDeviceInfoToPeer(const DeviceInfo &info)//设备信息上报
{
	class TestAskForTest : public TestAsk<TestCode>, public TestAskBase
    {
    public:
        TestAskForTest() : TestAskBase(TestAskBlock::NOT_BLOCK, TestAskReply::NEED_NOT_REPLY)
        {
        }
        virtual ~TestAskForTest() = default;
    };
    std::shared_ptr<VTestAsk> ask = std::make_shared<TestAskForTest>();
    const std::string EMPTY_ERROR_MESSAGE = "";
    ITestModuleV2::GetInstance()->SendDeviceInfoToPeer(ask, info, EMPTY_ERROR_MESSAGE);

}


TestCode TestMonitor::PeerEraseLicense(std::string &errorMessage)//清号
{
    LogWarning("clear license test.\n");
	

	//清号
	bool ret = false;
	ret = g_EncryptionHandle.ClearLicense();

	if (false == ret)//返回 false,说明设备中 还有 号的信息,没有清除成功
	{
		LogError( "clear license fail!\n" );
		const std::string EMPTY_ERROR_MESSAGE = "clear license fail!";
		errorMessage = EMPTY_ERROR_MESSAGE;
		return TestCode::TEST_FAILED;
	}
	else								//返回 true,说明设备中 没有 号的信息,清号成功
	{
		g_bDevInfoChanged = true;//add on 2025.01.02 设备信息同步
		LogError( "clear license success!\n" );
		const std::string EMPTY_ERROR_MESSAGE = "clear license success!";
		errorMessage = EMPTY_ERROR_MESSAGE;
		return TestCode::SUCCESS;
	}


}
TestCode TestMonitor::PeerChangePid(const std::string &pid, std::string &errorMessage)//更换涂鸦PID
{
    LogWarning("change tuya_pid test.\n");
	std::cout << "tuya_pid, pid length is:"<< pid.size() <<",  Provided pid: " << pid << std::endl;

	if( pid.size() != 16 )//校验数据长度是否正确
	{
		LogError( "Invalid tuya_pid length\n" );
        errorMessage = "Invalid tuya_pid length";
        return TestCode::TEST_FAILED;
	}

	for(int i = 0; i < pid.size(); ++i)//检测数据的有效性，必须是16位小写字母或者数字
	{
		
		if((pid[i] >= 97 && pid[i] <= 122)|| (pid[i] >= 48 && pid[i] <= 57))
		{
			continue;//数据有效，继续检测下一个字符
		}
		else
		{
			LogError( "Invalid tuya_pid format\n" );
        	errorMessage = "Invalid tuya_pid format";
        	return TestCode::TEST_FAILED;
		}
	}

	if( 0 == g_EncryptionHandle.GetLicenseSavaType() )//获取烧号的状态，没烧号上报错误
	{
		LogError( "have no license!\n" );
        errorMessage = "have no license!";
        return TestCode::TEST_FAILED;
	}

	if( false == g_EncryptionHandle.ChangeTuyapidByProduce((const unsigned char *)(pid.c_str()),(pid.size())))//更换pid
	{
		LogError( "after changepid, checkDevLicense error!\n" );
        errorMessage = "after changepid, checkDevLicense error!";
        return TestCode::TEST_FAILED;

	}
	else
	{
		g_bDevInfoChanged = true;//add on 2025.01.02 设备信息同步
		errorMessage = "";
		return TestCode::SUCCESS;
	}
	


}

//add on 2025.02.10 用于同步时间
void TestMonitor::AskCheckTimeFromPeer(void)
{
    class TestAskForTest : public TestAsk<CheckTime>, public TestAskBase
    {
    public:
        // TestAskForTest() : TestAskBase(TestAskBlock::BLOCK, TestAskReply::NEED_REPLY)
		TestAskForTest() : TestAskBase(TestAskBlock::NOT_BLOCK, TestAskReply::NEED_NOT_REPLY)
        {
        }
        virtual ~TestAskForTest() = default;
#if 0	//当前版本暂时不支持当前接口一问一答，需要用别的接口接收数据，后续版本会进行优化 //add on 2025.02.10 用于同步时间
        void ReplyFinished(const TestCode &code) override
        {
            TestAskBase::ReplyFinished(code);
            LogTrace("Test ask finished, TestCode = %s.\n", ITestModuleV2::PrintfTestCode(code));
            LogInfo("time zone = %s.\n", mDataReply.mTimeZone.c_str());
            LogInfo("year = %d.\n", mDataReply.mYear);
            LogInfo("month = %d.\n", mDataReply.mMonth);
            LogInfo("day = %d.\n", mDataReply.mDay);
            LogInfo("hour = %d.\n", mDataReply.mHour);
            LogInfo("minute = %d.\n", mDataReply.mMinute);
            LogInfo("second = %d.\n", mDataReply.mSecond);

			//设置时间
			if(TestCode::SUCCESS == code)
			{
				// 将年月日转换为自1970年1月1日以来的秒数
				struct tm timeinfo = {0};
				timeinfo.tm_year = 	mDataReply.mYear - 1900; // tm_year从1900年开始计数
				timeinfo.tm_mon = 	mDataReply.mMonth - 1;    // tm_mon从0开始计数
				timeinfo.tm_mday = 	mDataReply.mDay;
				timeinfo.tm_hour = 	mDataReply.mHour;
				timeinfo.tm_min = 	mDataReply.mMinute;
				timeinfo.tm_sec = 	mDataReply.mSecond;

				time_t seconds_since_epoch = mktime(&timeinfo);

				// 将秒数转换为struct timeval
				struct timeval tv;
				tv.tv_sec = seconds_since_epoch;
				tv.tv_usec = 0; // 微秒部分设置为0

				// 设置系统时间
				if (settimeofday(&tv, NULL) == -1) {
					perror("settimeofday");
					printf("\033[1;36m settimeofday fail... \033[0m\n");
				}else{
					printf("\033[1;36m settimeofday success... \033[0m\n");
				}
			}

			
        }
#endif
    };

    std::shared_ptr<VTestAsk> ask = std::make_shared<TestAskForTest>();
    const std::string NO_ERROR_MESSAGE = "";
    ITestModuleV2::GetInstance()->AskCheckTimeFromPeer(ask, NO_ERROR_MESSAGE);
    std::shared_ptr<TestAskForTest> askImpl = std::dynamic_pointer_cast<TestAskForTest>(ask);
    LogInfo("time zone = %s.\n", askImpl->mDataReply.mTimeZone.c_str());
    LogInfo("year = %d.\n", askImpl->mDataReply.mYear);
    LogInfo("month = %d.\n", askImpl->mDataReply.mMonth);
    LogInfo("day = %d.\n", askImpl->mDataReply.mDay);
    LogInfo("hour = %d.\n", askImpl->mDataReply.mHour);
    LogInfo("minute = %d.\n", askImpl->mDataReply.mMinute);
    LogInfo("second = %d.\n", askImpl->mDataReply.mSecond);
}

//add on 2025.02.10 用于同步时间
TestCode TestMonitor::PeerSendTimeForCheck(const CheckTime &time, std::string &errorMessage)
{
	//设置时间
	LogInfo("time zone = %s.\n", time.mTimeZone.c_str());
	LogInfo("year = %d.\n", time.mYear);
	LogInfo("month = %d.\n", time.mMonth);
	LogInfo("day = %d.\n", time.mDay);
	LogInfo("hour = %d.\n", time.mHour);
	LogInfo("minute = %d.\n", time.mMinute);
	LogInfo("second = %d.\n", time.mSecond);
	// 将年月日转换为自1970年1月1日以来的秒数
	struct tm timeinfo = {0};
	timeinfo.tm_year = 	time.mYear - 1900; // tm_year从1900年开始计数
	timeinfo.tm_mon = 	time.mMonth - 1;    // tm_mon从0开始计数
	timeinfo.tm_mday = 	time.mDay;
	timeinfo.tm_hour = 	time.mHour;
	timeinfo.tm_min = 	time.mMinute;
	timeinfo.tm_sec = 	time.mSecond;

	time_t seconds_since_epoch = mktime(&timeinfo);

	// 将秒数转换为struct timeval
	struct timeval tv;
	tv.tv_sec = seconds_since_epoch;
	tv.tv_usec = 0; // 微秒部分设置为0

	// 设置系统时间
	if (settimeofday(&tv, NULL) == -1) {
		perror("settimeofday");
		printf("\033[1;36m settimeofday fail... \033[0m\n");
		errorMessage = "settimeofday fail...";
		return TestCode::TEST_FAILED;	
	}else{
		printf("\033[1;36m settimeofday success... \033[0m\n");
		errorMessage = "settimeofday success...";
		return TestCode::SUCCESS;	
	}
	
}

