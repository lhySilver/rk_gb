#include "PAL/MotionDetect.h"
//#include "devdetect.h"
//#include "Log/DebugDef.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <time.h>
#include <getopt.h>

#include "PAL/MW_Common.h"
#include "PAL/MotionTrack.h"
#include "PAL/Capture.h"

int StartMotionTrack(motion_track_callback cb, motion_detect_callback cb_person,int mode)
{
	MSG("StartMotionTrack\n");
	return 0;
}

int StopMotionTrack()
{
	MSG("StopMotionTrack\n");
	return 0;
}

void EnableMotionTrackWork()
{
}

void DisableMotionTrackWork()
{
}

