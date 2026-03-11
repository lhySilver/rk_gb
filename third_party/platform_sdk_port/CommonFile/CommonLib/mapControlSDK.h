#ifndef _MAP_CONTROL_SDK_H
#define _MAP_CONTROL_SDK_H

#ifdef MAP_CONTROL_SDK_STATIC//静态库编译和使用
#define _EXPORT_MAPCONTROL_CLASS 
#define _EXPORT_MAPCONTORL_SDK 
#else
#ifdef MAP_CONTROL_SDK_EXPORT_API//动态库编译
#define _EXPORT_MAPCONTORL_CLASS __declspec(dllexport)
#define _EXPORT_MAPCONTORL_SDK  __declspec(dllexport)
#else//使用者编译
#define _EXPORT_MAPCONTORL_CLASS __declspec(dllimport)
#define _EXPORT_MAPCONTORL_SDK  __declspec(dllimport)
#endif
#endif

#include <string>
#include "base_type_define.h"

class CMapOperatorObserver
{
public:
	CMapOperatorObserver(void){};
public:
	virtual ~CMapOperatorObserver(void){};

	//创建地图是否成功，
	virtual void OnCreateMap(tuint32 mapID, bool isOK, LPVOID lParam)=0;
	
	////////
	
};


_EXPORT_MAPCONTORL_SDK bool MAP_CONTROL_Initial(std::string mapFile);
_EXPORT_MAPCONTORL_SDK void MAP_CONTROL_Quit();

_EXPORT_MAPCONTORL_SDK void MAP_CONTROL_AddObserver(CMapOperatorObserver *pObserver);//
_EXPORT_MAPCONTORL_SDK void MAP_CONTROL_DelObserver(CMapOperatorObserver *pObserver);//

_EXPORT_MAPCONTORL_SDK tuint32 MAP_CONTROL_CreateMap(void *mapView, LPVOID lParam = nullptr);
_EXPORT_MAPCONTORL_SDK void MAP_CONTROL_DestroyMap(tuint32 hMap);
_EXPORT_MAPCONTORL_SDK void MAP_CONTROL_MoveTo(tuint32 hMap, tuint32 carNodeID, double latitude, double longitude ,int direction );
_EXPORT_MAPCONTORL_SDK void MAP_CONTROL_Resize(tuint32 hMap);
_EXPORT_MAPCONTORL_SDK void MAP_CONTROL_RemoveCar(tuint32 hMap, tuint32 carNodeID);
_EXPORT_MAPCONTORL_SDK void MAP_CONTROL_RemoveAllCar(tuint32 hMap);
_EXPORT_MAPCONTORL_SDK std::string MAP_CONTROL_GetCarPos(tuint32 hMap, tuint32 carNodeID);//////获取汽车的实际地理位置字符串
	

#endif    ///////////////////////#ifndef _MAP_CONTROL_SDK_H




