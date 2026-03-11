#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Pcba.h"

static int gProcessRun = 1;

static void* do_audio_test(void* args)
{
	AppInfo("do_audio_test start\n");

	while (1)
	{
		CAudioPrompt::AudioFileParm audioFile;
		audioFile.strFileName = AUDIO_FILE_DINGDINGDING;
		audioFile.type = 0;
		g_AudioPrompt.aoPlay(audioFile);

		sleep(2);
	}
	return NULL;
}

static wifi_status_enum pcba_wifi_connect_result_g = WIFI_DISCONNECTED;
static void pcba_wifi_connect_callback(wifi_status_enum status)
{
	pcba_wifi_connect_result_g = status;
}


static bool QrCodeExtractedCB(const std::string &strResult)
{
	bool bRet=false;
	const char *token = "test";//二维码token
	int ret=-1;

	if(!strncmp(token, strResult.c_str(), strlen(token)))
	{
		static bool s_bThreadAudioPlayStart = false;
		if( false == s_bThreadAudioPlayStart )
		{
			s_bThreadAudioPlayStart = true;
			CreateDetachedThread((char*)"do_audio_test",do_audio_test, (void *)NULL, true);
		}
		return true;
	}

	return false;
}


PATTERN_SINGLETON_IMPLEMENT(CPcba);

CPcba::CPcba() : CThread("Pcba", 50)
{
}

CPcba::~CPcba()
{
}


void CPcba::ThreadProc()
{
	int ret , SID;
	
	SystemWifiPwrCtl(0);			//WiFi模块上电	

	g_PtzHandle.DoPtzTest(true);
	
    if(false==g_QrCodeConHandle.GetCreatedStatus())
	{
		g_QrCodeConHandle.Create(QrCodeExtractedCB);
		g_QrCodeConHandle.Start();
	}

	//开白光灯			
	g_Camera.SetWhiteLed(true);
	

	while(gProcessRun)
	{
        AppInfo("run pcba mode\n");
		
		WIFI_ST_t par = {0};
		memset(&par, 0, sizeof(par));
		strcpy(par.ssid, "pcba_test");
		strcpy(par.psk, "pcba_test");
		pcba_wifi_connect_result_g = WIFI_DISCONNECTED;
		WifiStationModeCreate(&par, pcba_wifi_connect_callback);
        while( 1 )
        {
			if( (WIFI_CONNECTED == pcba_wifi_connect_result_g) ||
				(WIFI_PSW_ERROR == pcba_wifi_connect_result_g) )
				break;
			sleep(1);
		}
		sleep(2);
	}

}

///< 0:succ 	-1:failed
int CPcba::init()
{
	return 0;
}

bool CPcba::start()
{
	return CreateThread();
}

bool CPcba::stop()
{
	return DestroyThread();
}

void CPcba::onAppEvent(std::string code, int index, appEventAction action, const EventHandler *param, Json::Value* data)
{

}

