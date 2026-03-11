#ifndef __DEBUG_DEF_H__
#define __DEBUG_DEF_H__
//#include "Log/IDebug.h"
#include "DebugInfo/debuginfo.h"

#define MODE_APP		"APP"  //应用程序
#define MODE_MANAGER 	"MANAGER"
#define MODE_NETSERVICE "NETSERVICE"
#define MODE_NETIP		"NETIP"
#define MODE_COMM		"COMM"
#define MODE_RTSP		"RTSP"
#define MODE_HTTP		"HTTPD"
#define MODE_STORAGE	"STORAGE"
#define MODE_GUI		"GUI"
#define MODE_GDI		"GDI"
#define MODE_ENC		"ENC"
#define MODE_PLAY		"PLAY"
#define MODE_SNAP		"SNAP"
#define MODE_TPBASE		"TPBASE"
#define MODE_NETPLATFORM	"PLATFORM"
#define MODE_EXCHANGE	"EXCHANGE"


#define DEBUG
#ifdef DEBUG
//APP
#define AppDebugReg()
#define AppDebug(fmt...)   DebugPrintf(LEVEL_DEBUG, __FILE__, __FUNCTION__, __LINE__, fmt)
#define AppErr(fmt...) 	 DebugPrintf(LEVEL_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt)
#define AppWarning(fmt...) DebugPrintf(LEVEL_WARNING, __FILE__, __FUNCTION__, __LINE__, fmt)
#define AppInfo(fmt...)    DebugPrintf(LEVEL_MESSAGE, __FILE__, __FUNCTION__, __LINE__, fmt)

//Manager
#define ManagerDebugReg()
#define ManagerDebug(fmt...)   DebugPrintf(LEVEL_DEBUG, __FILE__, __FUNCTION__, __LINE__, fmt)
#define ManagerErr(fmt...) 	 DebugPrintf(LEVEL_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt)
#define ManagerWraning(fmt...) DebugPrintf(LEVEL_WARNING, __FILE__, __FUNCTION__, __LINE__, fmt)
#define ManagerInfo(fmt...)	 DebugPrintf(LEVEL_MESSAGE, __FILE__, __FUNCTION__, __LINE__, fmt)

//Netservice
#define NsDebugReg()
#define NsDebug(fmt...) 		DebugPrintf(LEVEL_DEBUG, __FILE__, __FUNCTION__, __LINE__, fmt)
#define NsErr(fmt...) 		DebugPrintf(LEVEL_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt)
#define NsWraning(fmt...) 	DebugPrintf(LEVEL_WARNING, __FILE__, __FUNCTION__, __LINE__, fmt)
#define NsInfo(fmt...)		DebugPrintf(LEVEL_MESSAGE, __FILE__, __FUNCTION__, __LINE__, fmt)

//netip
#define NiDebugReg()
#define NiDebug(fmt...) 		DebugPrintf(LEVEL_DEBUG, __FILE__, __FUNCTION__, __LINE__, fmt)
#define NiErr(fmt...) 		DebugPrintf(LEVEL_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt)
#define NiWraning(fmt...) 	DebugPrintf(LEVEL_WARNING, __FILE__, __FUNCTION__, __LINE__, fmt)
#define NiInfo(fmt...)		DebugPrintf(LEVEL_MESSAGE, __FILE__, __FUNCTION__, __LINE__, fmt)

//Comm
#define CommDebugReg()
#define CommDebug(fmt...) 	DebugPrintf(LEVEL_DEBUG, __FILE__, __FUNCTION__, __LINE__, fmt)
#define CommErr(fmt...) 		DebugPrintf(LEVEL_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt)
#define CommWraning(fmt...) 	DebugPrintf(LEVEL_WARNING, __FILE__, __FUNCTION__, __LINE__, fmt)
#define CommInfo(fmt...)		DebugPrintf(LEVEL_MESSAGE, __FILE__, __FUNCTION__, __LINE__, fmt)

//Rtsp
#define RtspDebugReg()
#define RtspDebug(fmt...) 	DebugPrintf(LEVEL_DEBUG, __FILE__, __FUNCTION__, __LINE__, fmt)
#define RtspErr(fmt...) 		DebugPrintf(LEVEL_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt)
#define RtspWraning(fmt...) 	DebugPrintf(LEVEL_WARNING, __FILE__, __FUNCTION__, __LINE__, fmt)
#define RtspInfo(fmt...)	    DebugPrintf(LEVEL_MESSAGE, __FILE__, __FUNCTION__, __LINE__, fmt)

//Http
#define HttpDebugReg()
#define HttpDebug(fmt...) 	DebugPrintf(LEVEL_DEBUG, __FILE__, __FUNCTION__, __LINE__, fmt)
#define HttpErr(fmt...) 		DebugPrintf(LEVEL_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt)
#define HttpWraning(fmt...) 	DebugPrintf(LEVEL_WARNING, __FILE__, __FUNCTION__, __LINE__, fmt)
#define HttpInfo(fmt...)	    DebugPrintf(LEVEL_MESSAGE, __FILE__, __FUNCTION__, __LINE__, fmt)

//Storage
#define StorageDebugReg()
#define StorageDebug(fmt...) 		DebugPrintf(LEVEL_DEBUG, __FILE__, __FUNCTION__, __LINE__, fmt)
#define StorageErr(fmt...) 		DebugPrintf(LEVEL_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt)
#define StorageWraning(fmt...) 	DebugPrintf(LEVEL_WARNING, __FILE__, __FUNCTION__, __LINE__, fmt)
#define StorageInfo(fmt...)		DebugPrintf(LEVEL_MESSAGE, __FILE__, __FUNCTION__, __LINE__, fmt)

//Gui
#define GuiDebugReg()
#define GuiDebug(fmt...) 			DebugPrintf(LEVEL_DEBUG, __FILE__, __FUNCTION__, __LINE__, fmt)
#define GuiErr(fmt...) 			DebugPrintf(LEVEL_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt)
#define GuiWraning(fmt...) 		DebugPrintf(LEVEL_WARNING, __FILE__, __FUNCTION__, __LINE__, fmt)
#define GuiInfo(fmt...)			DebugPrintf(LEVEL_MESSAGE, __FILE__, __FUNCTION__, __LINE__, fmt)

//Gdi
#define GdiDebugReg()
#define GdiDebug(fmt...) 		DebugPrintf(LEVEL_DEBUG, __FILE__, __FUNCTION__, __LINE__, fmt)
#define GdiErr(fmt...) 		DebugPrintf(LEVEL_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt)
#define GdiWraning(fmt...) 	DebugPrintf(LEVEL_WARNING, __FILE__, __FUNCTION__, __LINE__, fmt)
#define GdiInfo(fmt...)		DebugPrintf(LEVEL_MESSAGE, __FILE__, __FUNCTION__, __LINE__, fmt)

//Enc
#define EncDebugReg()
#define EncDebug(fmt...) 		DebugPrintf(LEVEL_DEBUG, __FILE__, __FUNCTION__, __LINE__, fmt)
#define EncErr(fmt...) 		DebugPrintf(LEVEL_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt)
#define EncWraning(fmt...) 	DebugPrintf(LEVEL_WARNING, __FILE__, __FUNCTION__, __LINE__, fmt)
#define EncInfo(fmt...)		DebugPrintf(LEVEL_MESSAGE, __FILE__, __FUNCTION__, __LINE__, fmt)

//Play
#define PlayDebugReg()
#define PlayDebug(fmt...) 	DebugPrintf(LEVEL_DEBUG, __FILE__, __FUNCTION__, __LINE__, fmt)
#define PlayErr(fmt...) 		DebugPrintf(LEVEL_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt)
#define PlayWraning(fmt...) 	DebugPrintf(LEVEL_WARNING, __FILE__, __FUNCTION__, __LINE__, fmt)
#define PlayInfo(fmt...)		DebugPrintf(LEVEL_MESSAGE, __FILE__, __FUNCTION__, __LINE__, fmt)
//Snap
#define SnapDebugReg()
#define SnapDebug(fmt...) 	DebugPrintf(LEVEL_DEBUG, __FILE__, __FUNCTION__, __LINE__, fmt)
#define SnapErr(fmt...) 		DebugPrintf(LEVEL_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt)
#define SnapWraning(fmt, ...) 	DebugPrintf(LEVEL_WARNING, __FILE__, __FUNCTION__, __LINE__, fmt)
#define SnapInfo(fmt...)		DebugPrintf(LEVEL_MESSAGE, __FILE__, __FUNCTION__, __LINE__, fmt)

//Tpbase
#define TpbDebugReg()
#define TpbDebug(fmt...) 		DebugPrintf(LEVEL_DEBUG, __FILE__, __FUNCTION__, __LINE__, fmt)
#define TpbErr(fmt...) 		DebugPrintf(LEVEL_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt)
#define TpbWraning(fmt...) 	DebugPrintf(LEVEL_WARNING, __FILE__, __FUNCTION__, __LINE__, fmt)
#define TpbInfo(fmt...)		DebugPrintf(LEVEL_MESSAGE, __FILE__, __FUNCTION__, __LINE__, fmt)

//NetPlatform
#define NpDebugReg()
#define NpDebug(fmt...) 		DebugPrintf(LEVEL_DEBUG, __FILE__, __FUNCTION__, __LINE__, fmt)
#define NpErr(fmt...) 		DebugPrintf(LEVEL_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt)
#define NpWraning(fmt, ...) 	DebugPrintf(LEVEL_WARNING, __FILE__, __FUNCTION__, __LINE__, fmt)
#define NpInfo(fmt...)		DebugPrintf(LEVEL_MESSAGE, __FILE__, __FUNCTION__, __LINE__, fmt)

//Exchange
#define ExChgDebugReg()
#define ExChgDebug(fmt...) 	DebugPrintf(LEVEL_DEBUG, __FILE__, __FUNCTION__, __LINE__, fmt)
#define ExChgErr(fmt...) 		DebugPrintf(LEVEL_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt)
#define ExChgWraning(fmt...) 	DebugPrintf(LEVEL_WARNING, __FILE__, __FUNCTION__, __LINE__, fmt)
#define ExChgInfo(fmt...)		DebugPrintf(LEVEL_MESSAGE, __FILE__, __FUNCTION__, __LINE__, fmt)

//Middleware
#define MWDebugReg()
#define MWDebug(fmt...) 		DebugPrintf(LEVEL_DEBUG, __FILE__, __FUNCTION__, __LINE__, fmt)
#define MWErr(fmt...) 		DebugPrintf(LEVEL_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt)
#define MWWraning(fmt...) 	DebugPrintf(LEVEL_WARNING, __FILE__, __FUNCTION__, __LINE__, fmt)
#define MWInfo(fmt...)		DebugPrintf(LEVEL_MESSAGE, __FILE__, __FUNCTION__, __LINE__, fmt)

#else

//APP
#define AppDebugReg()
#define AppDebug(fmt...)
#define AppErr(fmt...)
#define AppWarning(fmt...)
#define AppInfo(fmt...)

//Manager
#define ManagerDebugReg()
#define ManagerDebug(fmt...)
#define ManagerErr(fmt...)
#define ManagerWraning(fmt...)
#define ManagerInfo(fmt...)
//Netservice
#define NsDebugReg()
#define NsDebug(fmt...)
#define NsErr(fmt...)
#define NsWraning(fmt...)
#define NsInfo(fmt...)

//netip
#define NiDebugReg()
#define NiDebug(fmt...)
#define NiErr(fmt...)
#define NiWraning(fmt...)
#define NiInfo(fmt...)
//Comm
#define CommDebugReg()
#define CommDebug(fmt...)
#define CommErr(fmt...)
#define CommWraning(fmt...)
#define CommInfo(fmt...)
//Rtsp
#define RtspDebugReg()
#define RtspDebug(fmt...)
#define RtspErr(fmt...)
#define RtspWraning(fmt...)
#define RtspInfo(fmt...)

//Http
#define HttpDebugReg()
#define HttpDebug(fmt...)
#define HttpErr(fmt...)
#define HttpWraning(fmt...)
#define HttpInfo(fmt...)

//Storage
#define StorageDebugReg()
#define StorageDebug(fmt...)
#define StorageErr(fmt...)
#define StorageWraning(fmt...)
#define StorageInfo(fmt...)

//Gui
#define GuiDebugReg()
#define GuiDebug(fmt...)
#define GuiErr(fmt...)
#define GuiWraning(fmt...)
#define GuiInfo(fmt...)

//Gdi
#define GdiDebugReg()
#define GdiDebug(fmt...)
#define GdiErr(fmt...)
#define GdiWraning(fmt...)
#define GdiInfo(fmt...)

//Enc
#define EncDebugReg()
#define EncDebug(fmt...)
#define EncErr(fmt...)
#define EncWraning(fmt...)
#define EncInfo(fmt...)

//Play
#define PlayDebugReg()
#define PlayDebug(fmt...)
#define PlayErr(fmt...)
#define PlayWraning(fmt...)
#define PlayInfo(fmt...)
//Snap
#define SnapDebugReg()
#define SnapDebug(fmt...)
#define SnapErr(fmt...)
#define SnapWraning(fmt, ...)
#define SnapInfo(fmt...)

//Tpbase
#define TpbDebugReg()
#define TpbDebug(fmt...)
#define TpbErr(fmt...)
#define TpbWraning(fmt...)
#define TpbInfo(fmt...)

//NetPlatform
#define NpDebugReg()
#define NpDebug(fmt...)
#define NpErr(fmt...)
#define NpWraning(fmt, ...)
#define NpInfo(fmt...)

//Exchange
#define ExChgDebugReg()
#define ExChgDebug(fmt...)
#define ExChgErr(fmt...)
#define ExChgWraning(fmt...)
#define ExChgInfo(fmt...)

//Middleware
#define MWDebugReg()
#define MWDebug(fmt...)
#define MWErr(fmt...)
#define MWWraning(fmt...)
#define MWInfo(fmt...)

#endif

#endif

