#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <getopt.h>
#include <errno.h>
#include <pthread.h> 

#include "PAL/QrCode.h"
#include "PAL/MW_Common.h"

#include "log.h"
#include "video.h"

static int QrCodeStarted = 0;
static QrCodeDecodeCallback QrCodeDecodeCb_g = NULL;

////////////////////////////////////////////////////////////////////
///¨°???¨º??¨®?¨²////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void QrCodeSetCallback(QrCodeDecodeCallback cb)
{
    MSG("QrCodeSetCallback\n");
    QrCodeDecodeCb_g = cb;
}

int QrCodeCreate(QrCodeDecodeCallback cb)
{
    MSG("QrCodeCreate start\n");
	QrCodeDecodeCb_g = cb;
	rk_video_qrcode_init(QrCodeDecodeCb_g);
	MSG("QrCodeCreate end\n");
	return 0;
}

int QrCodeDestory(void)
{
    MSG("QrCodeDestory start\n");
	rk_video_qrcode_deinit();
	MSG("QrCodeDestory end\n");
	return 0;
}

int QrCodeStart()
{
	QrCodeStarted = 1;
	return 0;
}

int QrCodeStop()
{
	QrCodeStarted = 0;
	return 0;	
}
