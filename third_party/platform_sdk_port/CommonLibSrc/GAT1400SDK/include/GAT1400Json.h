#ifndef __GAT_1400_JSON_H_
#define __GAT_1400_JSON_H_
#include <string>
#include "CMS1400Struct.h"
#include <list>
class GAT1400Json
{
public:
	static std::string PackRegisterJson(const char* localID);   //打包注册消息
	static std::string PackUnRegisterJson(const char* localID); //打包注销消息
	static std::string PackKeepAliveJson(const char* localID);  //打包心跳消息

	static std::string PackFaceJson(const GAT_1400_Face& Face); //打包人脸消息
	static std::string PackFaceListJson(const std::list<GAT_1400_Face>& FaceList); //打包批量人脸消息

	static std::string PackPersonJson(const GAT_1400_Person& Person);//打包人员消息
	static std::string PackPersonListJson(const std::list<GAT_1400_Person>& PersonList); //打包批量人员消息

	static std::string PackMotorVehicleJson(const GAT_1400_Motor& MotorVehicle);//打包机动车对象消息
	static std::string PackMotorVehicleListJson(const std::list<GAT_1400_Motor>& MotorVehicleList); //打包批量机动车对象消息

	static std::string PackNonMotorVehicleJson(const GAT_1400_NonMotor& NonMotorVehicle);//打包非机动车对象消息
	static std::string PackNonmotorVehicleListJson(const std::list<GAT_1400_NonMotor>& NonmotorVehicleList);//打包批量非机动车对象消息

	static std::string PackThingJson(const GAT_1400_Thing& Thing);//打包物品对象消息
	static std::string PackThingListJson(const std::list<GAT_1400_Thing>& ThingList);//打包批量物品对象消息

	static std::string PackSceneJson(const GAT_1400_Scene& Scene);//打包场景对象消息
	static std::string PackSceneListJson(const std::list<GAT_1400_Scene>& SceneList);//打包批量场景对象消息

	static std::string PackDispositionJson(const GAT_1400_Disposition& Disposition);//打包布控对象消息
	static std::string PackDispositionListJson(const std::list<GAT_1400_Disposition>& DispositionList);//打包批量布控对象消息
	static int UnPackDispositionsList(const std::string& strJson, std::list<GAT_1400_Disposition>& dispositionList);//解析布控消息

	static std::string PackDispositionNotificationListJson(const std::list<GAT_1400_Disposition_Notification>& DispositionNotificationList);//打包批量告警消息
	static std::string PackSubscribeListJson(const std::list<GAT_1400_Subscribe>& SubscribeList);//打包批量订阅消息
	static std::string PackSubscribeNotificationListJson(const std::list<GAT_1400_Subscribe_Notification>& SubscribeNotificationList);//打包通知记录消息
	static int UnPackSubscribeList(const std::string& strJson, std::list<GAT_1400_Subscribe>& subscribeList);//解析订阅消息
	static int UnPackSubscribe(const std::string& strJson, GAT_1400_Subscribe& subscribe);//解析单个订阅消息

	static std::string PackApeJson(const GAT_1400_Ape& ape);//打包采集设备消息
	static std::string PackApeListJson(const std::list<GAT_1400_Ape>& apeList);//打包批量采集设备消息

	static std::string PackTollgateJson(const GAT_1400_Tollgate& tollgate);//打包视频卡口消息
	static std::string PackTollgateListJson(const std::list<GAT_1400_Tollgate>& tollgateList);//打包批量视频卡口消息

	static std::string PackLaneJson(const GAT_1400_Tollgate_Lane& lane);//打包车道消息
	static std::string PackLaneListJson(const std::list<GAT_1400_Tollgate_Lane>& laneList);//打包批量车道消息

	static std::string PackVideoSliceJson(const GAT_1400_VideoSliceSet& VideoSlice);//打包单个视频片段
	static std::string PackVideoSliceListJson(const std::list<GAT_1400_VideoSliceSet>& videoSliceList);//打包视频片段集合
	static std::string PackVideoSliceInfoJson(const GAT_1400_VideoSliceInfo& VideoSliceInfo);//打包视频片段对象

	static std::string PackImageListJson(const std::list<GAT_1400_ImageSet>& ImageList);//打包图像集合
	static std::string PackImageJson(const GAT_1400_ImageSet& Image);//打包单个图像
	static std::string PackImageInfoJson(const GAT_1400_Image& ImageInfo);//打包单个图像对象信息

	static std::string PackFileListJson(const std::list<GAT_1400_FileSet>& FileList);//打包文件集合
	static std::string PackFileJson(const GAT_1400_FileSet& File);//打包单个文件
	static std::string PackFileInfoJson(const GAT_1400_File& FileInfo);//打包单个文件对象信息

	static std::string PackAnalysisRuleJson(const GAT_1400_AnalysisRule& rule);//打包视频图像分析规则
	static std::string PackAnalysisRuleListJson(const std::list<GAT_1400_AnalysisRule>& ruleList);//打包批量视频图像分析规则

	static std::string PackVideoLabelJson(const GAT_1400_VideoLabel& label);//打包视频图像标签
	static std::string PackVideoLabelListJson(const std::list<GAT_1400_VideoLabel>& labelList);//打包批量视频图像标签

	static std::string PackCaseListJson(const std::list<GAT_1400_CaseSet>& CaseList);//打包案件集合
	static std::string PackCaseJson(const GAT_1400_CaseSet& Case);//打包单个案件
	static std::string PackCaseInfoJson(const GAT_1400_Case& CaseInfo);//打包单个视频案事件对象RUD信息


	static std::string PackResponseStatusList(const std::list<GAT1400_RESPONSESTATUS_ST>& resultList);//打包应答状态列表
	static int UnPackResponseStatus(const std::string& strJson, GAT1400_RESPONSESTATUS_ST& result);//解析应答状态
	static int UnPackResponseStatusList(const std::string& strJson, std::list<GAT1400_RESPONSESTATUS_ST>& resultList);//解析应答状态列表
	static int UnPackSystemTime(const std::string& strJson, GAT1400_SYSTEMTIME_ST& result);

    ///////////////////////////////////////////1400IN////////////////////////////////////////////////////////////////////////////
    static int UnPackRegisterJson(const std::string& strJson, std::string& ID);//解析注册消息
    static int UnPackKeepLiveJson(const std::string& strJson, std::string& ID);//解析保活消息
    static int UnPackUnRegisterJson(const std::string& strJson, std::string& ID);//解析注册消息
    static int UnPackSubImageListJson(const std::string& strJson, std::list<GAT_1400_SubImage>& subImageList); //解析附属图片信息
    static int UnPackFaceListJson(const std::string& strJson, std::list<GAT_1400_Face>& FaceList); //解析批量人脸消息
    static int UnPackMotorListJson(const std::string& strJson, std::list<GAT_1400_Motor>& MotorList); //解析批量机动车消息
	static int UnPackPersonListJson(const std::string& strJson, std::list<GAT_1400_Person>& PersonList); //解析批量人员消息
	static int UnPackNonMotorListJson(const std::string& strJson, std::list<GAT_1400_NonMotor>& NonMotorList); //解析批量非机动车消息

    static std::string PackResponseStatus(const GAT1400_RESPONSESTATUS_ST& result);//打包应答状态
    static std::string PackSystemTime(const GAT1400_SYSTEMTIME_ST& result);//打包校时状态
private:
	GAT1400Json();
	GAT1400Json(const GAT1400Json&);
	GAT1400Json& operator=(const GAT1400Json&);
};



#endif