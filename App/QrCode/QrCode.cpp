#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "Infra/Packet.h"
#include "Infra/Thread.h"
#include "Infra/Timer.h"
#include "Infra/Time.h"
#include "PAL/Net.h"
#include "Log/DebugDef.h"

#include <Types/Defs.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include <Json/reader.h>
#include <Json/writer.h>

#include "ExchangeAL/NetWorkExchange.h"
#include "ExchangeAL/Exchange.h"
#include "Manager/ConfigManager.h"
#include "ExchangeAL/ExchangeKind.h"
#include "PathDefinition.h"
#include "../Net/NetConfigHook.h"
#include "../Media/AudioPrompt.h"

#include "QrCode.h"
#include "PAL/QrCode.h"

#define USE_ZBAR

#ifdef USE_ZBAR
#include "rkbar_scan_api.h"
#define DEFAULT_SCAN_IMAGE_WIDTH 640
#define DEFAULT_SCAN_IMAGE_HEIGHT 360
static int scan_image_width_ = DEFAULT_SCAN_IMAGE_WIDTH;
static int scan_image_height_ = DEFAULT_SCAN_IMAGE_HEIGHT;
void *rkbar_hand = NULL;
#endif
///////////////////////////////////////

static string m_result = "";

PATTERN_SINGLETON_IMPLEMENT(CQrCode);

CQrCode::CQrCode() : CThread("QrCode", 50)
{
	m_token = "";
	m_bCerated = false;
	m_bGetCode = false;
	m_bExtracting = false;
	m_bIsExit = true;
}

CQrCode::~CQrCode()
{
}
static string expected;
static long elapsed;
static  struct timespec st, en;
#if 0
#else
int CQrCode::OnCapture(char *pYUVBuffer, int bufferLen,int w,int h)
{
	int ret = 0;
	image_t *img = NULL;
    img = (image_t *)malloc(sizeof(image_t));
    img->width = w;
    img->height = h;
    img->crop_x = 0;
    img->crop_y = 0;
    img->crop_w = w;
    img->crop_h = h;
    img->bin = (unsigned char *)malloc(img->width * img->height);
    img->tmp = NULL;
    img->data = (uint8_t *)pYUVBuffer;
    
	clock_gettime(CLOCK_MONOTONIC, &st);
	ret = rkbar_scan(rkbar_hand, img);
    if (ret > 0) 
	{
      	const char *test = rkbar_getresult(rkbar_hand);
		m_result = test;
		printf("rkbar the decoding result is \" %s \" \n", test);
	}
	clock_gettime(CLOCK_MONOTONIC, &en);
	elapsed = (en.tv_sec - st.tv_sec) * 1e3 + (en.tv_nsec - st.tv_nsec) / 1e6;
	// if (1)
	// 	printf("spent time %ld ms\n", elapsed);

	if (img->bin)
		free(img->bin);
    if (img)
		free(img);
	return 0;
}
#endif

void CQrCode::ThreadProc()
{
		int ret;
		uchar *pBuffer;
		uint length;
		uint failed_last_time = 0;
		CAudioPrompt::AudioFileParm audioFile;

		m_bIsExit = false;

		std::string strLastResult = "";

		while (m_bLoop) 
		{	
			if( true == m_bGetCode )
			{
				usleep(500000); 	//500ms
				printf("CQrCode::get wifi succ.....\n");
				continue;
			}
			
			if (m_result != strLastResult)
			{
				printf("m_result : %s\n", m_result.c_str());
				//回调
				if( true == m_cb(m_result) )
				{
					m_bGetCode = true;
					
				}
				strLastResult = m_result;
			}
			
			usleep(50*1000);
		}
		m_bIsExit = true;;

}


///创建
bool CQrCode::Create(QrCodeExtractedCallback cb)
{
	printf("CQrCode::Create.....m_bCerated = %d\n", m_bCerated);
	if( true == m_bCerated )
		return true;

	m_result = "";

	#ifdef USE_ZBAR
    int rc = rkbar_init(&rkbar_hand);
    if (rc == -1) 
	{
		printf("rkbar init is err");
		return false;
    }
	#endif
	
	bool ret = false;;
	
	if( 0 == QrCodeCreate(OnCapture) )
	{
		ret = CreateThread();
		if( true == ret )
		{
			m_cb = cb;
			m_bCerated = true;
		}
		else
			QrCodeDestory();
	}
	return ret;
}
///销毁
bool CQrCode::Destory()
{
	printf("CQrCode::Destory.....\n");
	if( false == m_bCerated )
		return true;

	m_bCerated = false;

	DestroyThread();

	//等待取线程先退出
	while( false == m_bIsExit )
		usleep(10000); 	//10ms
	
	m_bGetCode = false;
	m_cb = NULL;

	QrCodeDestory();

	#ifdef USE_ZBAR
	if (rkbar_hand)
	{
		rkbar_deinit(rkbar_hand);
	}
	#endif
	
	return 0;
}


bool CQrCode::Start()
{	
	printf("CQrCode::Start.....\n");
	m_bGetCode = false;
	m_bExtracting = true;
	
	QrCodeStart();
	return true;
}

bool CQrCode::Stop()
{
	printf("CQrCode::Stop.....\n");	
	m_bExtracting = false;
	
	QrCodeStop();
	return true;
}

bool CQrCode::GetCreatedStatus()
{
	return m_bCerated;
}

bool CQrCode::GetWorkStatus()
{
	return m_bExtracting;
}

void CQrCode::GetToken(std::string &token)
{
	token = m_token;
}


