#ifndef __PAL_MOTIONTRACK_H__
#define __PAL_MOTIONTRACK_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "MotionDetect.h"


typedef int(* motion_track_callback)(int dx,int dy);


int StartMotionTrack(motion_track_callback cb,motion_detect_callback cb_person,int mode);
int StopMotionTrack();

void EnableMotionTrackWork();

void DisableMotionTrackWork();

#ifdef __cplusplus
}
#endif

#endif 	//__PAL_MOTIONTRACK_H__
