
///////////////////////////NetDevMDVRSDK.h

#ifndef _NET_DEVDECODER_SDK_H
#define _NET_DEVDECODER_SDK_H

#ifdef WIN32 

#ifdef __DECODERNETPROTOCOL_DLL__//动态库编译 
#define _EXPORT_NETDEV_DECODER_SDK  __declspec(dllexport)
#else 
#define _EXPORT_NETDEV_DECODER_SDK  __declspec(dllimport)
#endif
#else
#define _EXPORT_NETDEV_DECODER_SDK __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"


_EXPORT_NETDEV_DECODER_SDK bool NET_DEV_DECODERSDK_Initial();/////支持的最低版本号限制
_EXPORT_NETDEV_DECODER_SDK void NET_DEV_DECODERSDK_Quit();


#endif
