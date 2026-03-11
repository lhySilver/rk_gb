#ifndef _CMS_1400_STRUCT_H_
#define _CMS_1400_STRUCT_H_
#include <string>
#include <string.h>
#include <list>

//宏定义，是否是空格
#define ISspace(x) isspace((int)(x))

#define TCP_RECV_TIMEOUT      10

#define GB_ID_LEN                (20+1)
#define STR_LEN                   128
#define URL_LEN                   (256+1)

//+1是为了最后一个字节填充结束符
#define GAT1400_CASE_OBJECT_ID_TYPE             (30+1)    //案件关联对象ID
#define GAT1400_BUSINESS_OBJECT_ID_TYPE         (33+1)    //布控、订阅及相应通知管理对象ID
#define GAT1400_BASIC_OBJECT_ID_TYPE            (41+1)    //图像信息基本要素ID，视频、图像、文件
#define GAT1400_IMAGE_CNT_OBJECT_ID_TYPE        (48+1)    //图像信息内容要素ID，人员、人脸、机动车、非机动车、物品、场景等
#define GAT1400_PLACE_FULL_ADDRESS_TYPE         (100+1)   //具体到摄像机位置或街道门牌号，由DE00072(乡镇街道)+DE00074(街路巷)+DE00080(门楼牌号)+DE00081(门楼详细地址)构成
#define GAT1400_DEVICE_ID_TYPE                  (20+1)    //采集设备、卡口点位、采集系统、分析系统、视图库、应用平台等设备编码规则
#define GAT1400_ESCAPED_CRIMINAL_NUMBER_TYPE    (23+1)    //在逃人员信息编号规则
#define GAT1400_DETENTION_HOUSE_CODE_TYPE       (9+1)     //看守所编码
#define GAT1400_SUSPECTED_TERRORIST_NUMBER_TYPE (19+1)    //涉恐人员编号
#define GAT1400_ID_TYPE                         (3+1)     //常用证件代码
#define GAT1400_ETHIC_CODE_TYPE                 (2+1)     //中国各名族的罗马字母拼写法和代码
#define GAT1400_NATIONALITY_CODE_TYPE           (3+1)     //国籍代码、世界各国和地区名称代码
#define GAT1400_CHINESE_ACCENT_CODE             (6+1)     //汉语口音编码规则
#define GAT1400_JOB_CATEGORY_TYPE               (3+1)     //职业分类与代码
#define GAT1400_FACE_STYLE_TYPE                 (4+1)     //脸型
#define GAT1400_FACIAL_FEATURE_TYPE             (40+1)    //脸部特征
#define GAT1400_PHYSICAL_FEATURE_TYPE           (200+1)   //体貌特征
#define GAT1400_BODY_FEATURE_TYPE               (70+1)    //体表特征，有多个时用英文半角逗号分隔
#define GAT1400_IMMIGRANT_TYPE_CODE_TYPE        (2+1)     //出境人员分类代码
#define GAT1400_CRIMINAL_INVOLVED_SPECILISATION_CODE_TYPE (2+1) //涉恐人员专长代码
#define GAT1400_BODY_SPECIALL_MARK_TYPE         (7+1)     //体表特殊标记
#define GAT1400_CRIME_METHOD_TYPE               (4+1)     //作案手段
#define GAT1400_CRIME_CHARACTER_CODE_TYPE       (3+1)     //作案特点分类和代码
#define GAT1400_DETAINEES_SPECIAL_IDENTITY_TYPE (2+1)     //在押人员特殊身份代码
#define GAT1400_MEMBER_TYPE_CODE_TYPE           (2+1)     //成员类型代码
#define GAT1400_VICTIM_TYPE                     (3+1)     //被害人类型
#define GAT1400_CORPSE_CONDITION_CODE_TYPE      (2+1)     //尸体状况分类与代码
#define GAT1400_DRIVING_STATUS_CODE_TYPE        (4+1)     //机动车行驶状态代码
#define GAT1400_WHEEL_PRINTED_PATTERN_TYPE      (2+1)     //车轮印花纹
#define GAT1400_PLACE_TYPE                      (4+1)     //选择处所代码
#define GAT1400_WEATHER_TYPE                    (2+1)     //天气代码
#define GAT1400_SCENE_TYPE                      (2+1)     //道路类型代码
#define GAT1400_ROAD_ALIGNMENT_TYPE             (2+1)     //道路线形代码
#define GAT1400_ROAD_SURFACE_TYPE               (1+1)     //道路路面类型代码
#define GAT1400_ROAD_CONDITION_TYPE             (1+1)     //道路路面状况代码
#define GAT1400_ROAD_JUNCTION_SECTION_TYPE      (2+1)     //道路路口路段类型代码
#define GAT1400_ROAD_LIGHTING_TYPE              (1+1)     //道路照明条件代码
#define GAT1400_WIND_DIRECTION_TYPE             (2+1)     //现场风向代码
#define GAT1400_INVOLVED_OBJ_TYPE               (5+1)     //涉案物品分类和代码
#define GAT1400_FIREARMS_AMMUNITION_TYPE        (2+1)     //枪支弹药类型
#define GAT1400_TOOL_TRACE_TYPE                 (2+1)     //工具痕迹分类和代码
#define GAT1400_EVIDENCE_TYPE                   (5+1)     //物证类型
#define GAT1400_CASE_EVIDENCE_TYPE              (2+1)     //案(事)件物证形态代码
#define GAT1400_PLACE_CODE_TYPE                 (6+1)     //行政区划、籍贯省市县代码
#define GAT1400_IP_ADDR_TYPE                    (30+1)    //IP地址
#define GAT1400_NAME_TYPE                       (50+1)    //姓名
#define GAT1400_USED_NAME_TYPE                  (50+1)    //曾用名
#define GAT1400_ALIAS_TYPE                      (50+1)    //别名、绰号
#define GAT1400_ORG_TYPE                        (100+1)   //单位名称
#define GAT1400_FILE_NAME_TYPE                  (256+1)   //电子文件名
#define GAT1400_ID_NUMBER_TYPE                  (30+1)    //证件号码
//#define GAT1400_STATUS_TYPE                     (1+1)     //视频设备工作状态:1在线;2离线;9其他
#define GAT1400_PLATE_NO_TYPE                   (15+1)    //机动车号牌号码
#define GAT1400_CASE_NAME_TYPE                  (120+1)   //案件名称
#define GAT1400_CASE_ABSTRACT_TYPE              (4000+1)  //简要案件
#define GAT1400_DEVICE_NAME_TYPE                (100+1)   //设备名称
#define GAT1400_CASE_LINK_MARK_TYPE             (23+1)    //案件编号
#define GAT1400_PASSWORD_TYPE                   (32+1)    //口令
#define GAT1400_KEYWORD_TYPE                    (200+1)   //关键词
#define GAT1400_MODEL_TYPE                      (100+1)   //设备型号
#define GAT1400_VEHICLE_MODEL_TYPE              (32+1)    //车辆型号
#define GAT1400_ORG_CODE_TYPE                   (12+1)    //机构代码
#define GAT1400_IPV6_ADDR_TYPE                  (64+1)    //IPv6地址
#define GAT1400_TOLLGATE_TYPE                   (2+1)     //卡口类型:10国际;20省际;30市际;31市区;40县际;99其他
#define GAT1400_VEHICLE_CLASS_TYPE              (3+1)     //机动车车辆类型代码
#define GAT1400_DATA_SOURCE_TYPE                (2+1)     //视频图像数据来源
#define GAT1400_AUDIO_CODE_FORMAT_TYPE          (2+1)     //音频编码格式
#define GAT1400_VIDEO_CODE_FORMAT_TYPE          (2+1)     //视频编码格式
#define GAT1400_COLOR_TYPE                      (2+1)     //颜色
#define GAT1400_HAIR_STYLE_TYPE                 (2+1)     //发型
#define GAT1400_POSTURE_TYPE                    (2+1)     //姿态
#define GAT1400_PERSON_STATUS_TYPE              (2+1)     //人的状态
#define GAT1400_HABITUAL_ACTION_TYPE            (2+1)     //习惯动作
#define GAT1400_BEHAVIOR_TYPE                   (2+1)     //行为
#define GAT1400_APPENDAGE_TYPE                  (2+1)     //附属物
#define GAT1400_HAT_STYLE_TYPE                  (2+1)     //帽子款式
#define GAT1400_GLASSES_STYLE_TYPE              (2+1)     //眼睛款式
#define GAT1400_BAG_STYLE_TYPE                  (2+1)     //包款式
#define GAT1400_COAT_STYLE_TYPE                 (2+1)     //上衣款式
#define GAT1400_PANTS_STYLE_TYPE                (2+1)     //裤子款式
#define GAT1400_SHOES_STYLE_TYPE                (2+1)     //鞋子款式
#define GAT1400_FRONT_THING_TYPE                (2+1)     //车前部物品类型
#define GAT1400_REAR_THING_TYPE                 (2+1)     //车后部物品类型
#define GAT1400_THING_PROPERTY_TYPE             (2+1)     //物品性质
#define GAT1400_ILLUSTRATION_TYPE               (2+1)     //现场图示
#define GAT1400_DETAINEES_IDENTITY_TYPE         (2+1)     //在押人员身份
#define GAT1400_ID_TYPE                         (3+1)     //证件类型
#define GAT1400_PLATE_CLASS_TYPE                (2+1)     //号牌种类
#define GAT1400_ENPASSPORT_TYPE                 (2+1)     //护照证件类型
#define GAT1400_VIDEO_FORMAT_TYPE               (6+1)     //视频格式
#define GAT1400_IMAGE_FORMAT_TYPE               (6+1)     //图片格式
#define GAT1400_VEHICLE_BRAND_TYPE              (3+1)     //车辆品牌代码
#define GAT1400_SUBSCRIBE_DETAIL_TYPE           (2+1)     //订阅类型
#define GAT1400_DATETIME_TYPE                   (17+1)    //日期时间长度 |格式YYYYMMDDhhmmssMMM  
#define GAT1400_PATH_TYPE                       (256+1)   //路径
#define GAT1400_FILE_HASH_TYPE                  (128+1)   //哈希值

typedef enum
{
    OK = 0,                                   //正常
    OTHERERROR,                               //其他未知错误
    DEVICE_BUSY,                              //设备忙
    DEVICE_ERROR,                             //设备错
    INVALID_OPERATION,                        //无效操作
    INVALID_XML_FORMAT,                       //XML格式无效
    INVALID_XML_CONTENT,                      //XML内容无效
    INVALID_JSON_FORMAT,                      //JSON格式无效
    INVALID_JSON_CONTENT,                     //JSON内容无效
    REBOOT,                                   //系统重启中
}STATUS_CODE_H;

/* HTTP消息方法类型       */    
typedef enum 
{
    HTTP_REQUEST_POST = 1,                     //请求类型--创建
    HTTP_REQUEST_PUT,                          //请求类型--更新
    HTTP_REQUEST_GET,                          //请求类型--获取
    HTTP_REQUEST_DELETE,                       //请求类型--删除
    HTTP_RESPONSE_1_0 = 15,                    //响应类型--HTTP/1.0协议
    HTTP_RESPONSE_1_1,                         //响应类型--HTTP/1.1协议
    HTTP_OTHER = 20,                           
}HTTP_MSG_TYPE_H;

/* HTTP URI类型 */
typedef enum
{
	POST_REGISTER = 1,                         //注册
	POST_UNREGISTER,                           //注销
	POST_KEEPALIVE,                            //保活
	POST_DISPOSITION_LIST,                     //批量布控
	POST_VIDEOSLICES,                          //视频片段
	POST_VIDEOSLICESDATA,                      //视频片段数据
	POST_IMAGES,                               //批量图像
	POST_IMAGESDATA,                           //单个图像数据
	POST_FILES,                                //批量文件
	POST_FILESDATA,                            //单个文件
	POST_PERSONS,                              //批量人员
	POST_FACES,                                //批量人脸
	POST_MOTORVEHICLE,                         //批量机动车
	POST_NUNMOTORVEHICLE,                      //批量非机动车
	POST_VIDEOLABELS,                          //批量视频图像标签
	POST_SUBSCRIBE_NOTIFICATION_LIST,          //增加通知对象
	POST_SUBSCRIBE_LIST,                       //批量订阅

    GET_SYNCTIME,                              //校时
	GET_APES,                                  //采集设备查询
	GET_TOLLGATES,                             //查询视频卡口
	GET_LANES,                                 //查询车道
	GET_MOTORVEHICLE_LIST,                     //批量查询机动车
	GET_MOTORVEHICLE,                          //查询单个机动车
	GET_NONMOTORVEHICLE_LIST,                  //批量查询非机动车
	GET_NONMOTORVEHICLE,                       //查询单个非机动车
	GET_THING_LIST,                            //批量查询物品
	GET_THING,                                 //查询单个物品
	GET_SCENE_LIST,                            //批量查询场景
	GET_SCENE,                                 //查询单个场景
	GET_VIDEO_SLICE_LIST,                      //批量查询视频片段
	GET_VIDEO_SLICE,                           //查询单个视频片段RUD消息
	GET_VIDEO_SLICE_INFO,                      //查询单个视频片段对象信息RUD消息
	GET_VIDEO_SLICE_DATA,                      //查询单个视频片段数据CRD消息
	GET_IMAGE_LIST,                            //批量查询图像
	GET_IMAGE,                                 //查询单个图像
	GET_IMAGE_INFO,                            //查询单个图像对象信息
	GET_IMAGE_DATA,                            //查询单个图像数据
	GET_FILE_LIST,                             //批量查询文件
	GET_FILE,                                  //查询单个文件
	GET_FILE_INFO,                             //查询单个文件对象信息
	GET_FILE_DATA,                             //查询文件数据
	GET_PERSON_LIST,                           //批量查询人员
	GET_PERSON,                                //查询单个人员
	GET_FACE_LIST,                             //批量查询人脸
	GET_FACE,                                  //查询单个人脸    
	GET_CASE_LIST,                             //批量查询视频案事件
	GET_CASE,                                  //查询单个视频案事件
	GET_CASE_INFO,                             //查询单个视频案事件对象
	GET_DISPOSITION_LIST,                      //批量查询布控任务
	GET_DISPOSITION_NOTIFICATION_LIST,         //批量查询告警记录
	GET_SUBSCRIBE_LIST,                        //批量查询订阅任务
	GET_SUBSCRIBE_NOTIFICATION_LIST,           //批量查询通知记录
	GET_ANALYSIS_RULE_LIST,                    //批量查询视频图像分析规则
	GET_VIDEO_LABEL_LIST,                      //批量查询视频图像标签

	PUT_APES,                                  //更新设备口令
	PUT_DISPOSITION_LIST,                      //批量更新布控任务
	PUT_DISPOSITION,                           //撤控消息
	PUT_SUBSCRIBE_LIST,                        //批量更新订阅任务
	PUT_SUBSCRIBE,                             //取消订阅




	DELETE_DISPOSITION_LIST,                   //批量删除布控任务
	DELETE_DISPOSITION_NOTIFICATION_LIST,      //删除告警记录
	DELETE_SUBSCRIBE_LIST,                     //批量删除订阅任务
	DELETE_SUBSCRIBE_NOTIFICATION_LIST,        //删除通知记录
	
	OTHER = 0xFFFFFFFF
}HTTP_URI_TYPE_H;

typedef struct _HTTP_HEAD_MSG_H_
{
    char method[24];
    char uri[256];
    int  contentLength;
    bool authorizationFlag;
}HTTP_HEAD_MSG_H;

//////////////////////////////////////////////////////////////

//连接参数
typedef struct ConnectParam
{
	char   GBCode[GB_ID_LEN];
	char   ip[GAT1400_IP_ADDR_TYPE];
	unsigned short     port;
}CONNECT_PARAM_S;

//注册信息
typedef struct RegistParam
{
	unsigned int     expires;
	char    username[STR_LEN];
	char    password[STR_LEN];
	char    auth[STR_LEN];
}REGIST_PARAM_S;

//////////////////////////////////////////////////////////////////////////////////////////////

//定义GAT1400 XML节点枚举类型转换表
typedef struct 
{
    const char *path;                                  //该item的在xml中的路径，点分制
    const char *comment;                               //xml中注释
    int         iVal;                                  //枚举值

}GAT1400_XML_TYPES_S;


//子类型编码
enum SUB_TYPE_CODING
{
	PERSON_CODING=1, //人员
	MOTOR_VEHICLES_CODING,//机动车
	NON_MOTOR_VEHICLES_CODING, //非机动车
	THING_CODING,//物品
	SCENE_CODING,//场景
	FACE_CODING,//人脸
	VIDEO_SLICE_CODING,//视频图像标签
	OTHER_CODING=99 //其他
};


//值类型 string
enum DataSourceType
{
    GOVERNMENT_AGENCY_MONITORING = 1,       //政府机关监控
    SOCIAL_SECURITY_MONITORING,             //社会面治安监控 ，
    TRAFFIC_MONITORING,                     //交通监控（含轨道交通监控），
    IMMIGRATION_CONTROL_MONITORING,         //出入境监控，
    PART_MONITORING,                        //港口监控，
    FINANCIAL_SYSTEM_MONITORING,            //金融系统监控，
    HOTEL_MONITORING,                       //旅馆监控，
    INTERNET_BUSNESS_MONITORING,            //互联网营业场所监控，
    ENTERTAINMENT_SERVICE_PLACE_MONITORING, //娱乐服务场所监控，
    ENTERPRISES_AND_INSTITUTIONS_MONITORING,//其他企业、事业单位监控，
    RESIDENT_SETUP_OWN_MONITORING,          //居民自建监控，
    INTERNAL_POLICE_MONITORING,             //公安内部，
    PRISON_MONITORING,                      //监所，
    INQUIRY_ROOM_MONITORING,                //询问室，
    ON_BOARD_TERMINAL_MONITORING,           //车（船，直升机等）载终端拍摄，
    MOBLIE_ENFORCEMENT,                     //移动执法，
    PHONE_TABLET,                           //手机，平板电脑拍摄，
    DV_VIDEO_CAPTURE,                       //DV拍摄，
    CAMERA_CAPTURE,                         //相机拍摄，
    NETWORK_ACCESS,                         //网络获取，
    SOUND_VIDEO_EXPANSION_PACK,             //声像资料片，
    OTHER_MONITORING = 99                   //其他
};	
//值类型 string
enum AudioCodeFormaType
{
    AUDIO_G771a = 1,		//G.771a = 1,
    AUDIO_G771u,			//G.711u,
    AUDIO_G723,				//G.723,
    AUDIO_G729,				//G.729,
    AUDIO_SVAC,				//SVAC,
    AUDIO_OTHER				//其他
};
//值类型 string
enum VideoCOdeFormatType
{
    VIDEO_SVAC = 1,			//SVAC,
    VIDEO_H264,			//H.264,
    VIDEO_MPEG4,		//MPEG-4,
    VIDEO_MPEG2,		//MPEG-2,
    VIDEO_MJPEG,		//MJPEG,
    VIDEO_H263,			//H.263,
    VIDEO_H265,			//H.265,
    VIDEO_OTHER			//其他
};
//值类型 string
enum ColorType
{
    COLOR_BLACK = 1,//黑 = 1，
    COLOR_WHITE,//白，
    COLOR_GREY,//灰，
    COLOR_RED,//红，
    COLOR_BLUE,//蓝，
    COLOR_YELLOW,//黄，
    COLOR_ORANGE,//橙，
    COLOR_BROWN,//棕，
    COLOR_GREEN,//绿，
    COLOR_PURPLE,//紫，
    COLOR_CYAN,//青，
    COLOR_PINK,//粉，
    COLOR_TRANSPARENT,//透明，
    COLOR_OTHER//其他
};
//值类型 string
enum SecurityLevelType
{
    SECURITY_SUPERSECRET = 1,//绝密 = 1，
    SECURITY_CONFIDENTIAL,//机密，
    SECURITY_SECRET,//秘密，
    SECURITY_INTRINSIC,//内部，
    SECURITY_PUBLIC,//公开，
    SECURITY_OTHER//其他 = 9
};
//值类型 string
enum QualityGradeType
{
    QUALITY_POOR = 1,//极差 = 1，
    QUALITY_WORSE,//较差，
    QUALITY_COMMEN,//一般，
    QUALITY_BETTER,//较好，
    QUALITY_COMMENDABLE,//很好
};
//值类型 string
enum VerticalShotType
{
    VERTICAL_UP = 1,//上 = 1，
    VERTICAL_LEVEL,//水平，
    VERTICAL_DOWN,//下
};
//值类型 string
enum HairStyleType
{
    HAIR_STYLE_BRUSH = 1,//平头 = 1，
    HAIR_STYLE_CENTER_PARTED,//中分，
    HAIR_STYLE_SIDE_PART,//偏分，
    HAIR_STYLE_FOREHEAD_BALD,//额秃，
    HAIR_STYLE_NECK_BALD,//项秃，
    HAIR_STYLE_COMPLETELY_BALD,//全秃，
    HAIR_STYLE_CURLY,//卷发，
    HAIR_STYLE_WAVY,//波浪发，
    HAIR_STYLE_BRAID,//麻花辫，
    HAIR_STYLE_UPDO,//盘发，
    HAIR_STYLE_CAPPA,//披肩，
    HAIR_STYLE_OTHER = 99//其他=99
};
//值类型 string
enum PostureType
{
    POSTUER_STAND, //站，
    POSTUER_SQUAT,//蹲，
    POSTUER_PRONE_POSITION,//卧，
    POSTUER_LIE,//躺，
    POSTUER_SIT,//坐，
    POSTUER_WALK,//行走，
    POSTUER_RUN,//奔跑，
    POSTUER_JUMP,//跳跃，
    POSTUER_CLIMB,//攀登，
    POSTUER_CREEP,//匍匐，
    POSTUER_PTHER = 99//其他 = 99
};
//值类型 string
enum PersonStatusType
{
    PERSON_STATUS_EBRITY = 1,//醉酒 = 1，
    PERSON_STATUS_EXCITE,//亢奋，
    PERSON_STATUS_NORMAL,//正常，
    PERSON_STATUS_OTHER = 99//其他 = 99
};
//值类型 string
enum HabitualActionType			
{
    HABITUAL_ACTION_UPTURN_EYE = 1,//翻眼 = 1，
    HABITUAL_ACTION_BLINK,//眨眼，
    HABITUAL_ACTION_FROWM,//皱眉，
    HABITUAL_ACTION_SQUEEZE_EYE,//挑眉，
    HABITUAL_ACTION_PUSH_GLASSES,//推镜，
    HABITUAL_ACTION_GRIPPER,//抓头，
    HABITUAL_ACTION_PICK_NOSE,//挖鼻，
    HABITUAL_ACTION_TOUCH_CHIN,//摸下巴，
    HABITUAL_ACTION_GESTICULATE,//打手势，
    HABITUAL_ACTION_LEFT_HANDER,//左撇子，
    HABITUAL_ACTION_SHRINK_NEXK,//缩颈，
    HABITUAL_ACTION_WALK_SHAKING,//走路摇晃，
    HABITUAL_ACTION_TOE_OUT,//外八字，
    HABITUAL_ACTION_TOE_IN,//内八字，
    HABITUAL_ACTION_FACIAL_TIC,//面肌抽搐，
    HABITUAL_ACTION_HER_MOUTH,//说话歪嘴，
    HABITUAL_ACTION_SHAKE_HEAD,//摆头，
    HABITUAL_ACTION_TREMOE_HEAD,//手抖，
    HABITUAL_ACTION_OTHER = 99//其他 = 99
};
//值类型 string
enum BehaviorType
{
    BEHAVIOR_TAIL_AFTER = 1,//尾随 = 1，
    BEHAVIOR_WANDER,//徘徊，
    BEHAVIOR_WITHDRAWAL,//取款，
    BEHAVIOR_FIGHT,//打架，
    BEHAVIOR_DRIVE,//开车，
    BEHAVIOR_PHONE,//打手机，
    BEHAVIOR_OTHER = 99//其他= 99
};
//值类型 string
enum AppendageType
{
    APPENDAGE_CELLPHONE = 1,//手机 = 1，
    APPENDAGE_UMBRELLA,//伞，
    APPENDAGE_MASKS,//口罩，
    APPENDAGE_WATCH,//手表，
    APPENDAGE_HELMET,//头盔，
    APPENDAGE_GLASSES,//眼镜，
    APPENDAGE_HAT,//帽子，
    APPENDAGE_PACKAGE,//包，
    APPENDAGE_SCARF,//围巾，
    APPENDAGE_OTHER = 99//其他= 99
};

//值类型 string
enum HatStyleType
{
    HATSTYLE_WOOL_CAP = 1,//毛线帽 = 1，
    HATSTYLE_BERETS,//贝雷帽，
    HATSTYLE_BASEBALL_CAP,//棒球帽，
    HATSTYLE_FLAT_HAT,//平顶帽，
    HATSTYLE_FISHERMAN_CAP,//渔夫帽，
    HATSTYLE_HEAD_CAP,//套头帽，
    HATSTYLE_CAP,//鸭舌帽，
    HATSTYLE_COSRUME,//大檐帽，
    HATSTYLE_OTHER = 99//其他= 99.
};
//值类型 string
enum GlassesStyleType
{
    GLASSES_FULL_BOX = 1,//全框 = 1，
    GLASSES_HALF_BOX,//半框，
    GLASSES_WU_KUANG,//无框，
    GLASSES_EYEBROW_FRAME,//眉线框，
    GLASSES_MULTI_FUNCTION_BOX,//多功能框，
    GLASSES_BIANSEJING,//变色镜，
    GLASSES_SUNGLASSES,//太阳镜，
    GLASSES_NO_LEN,//无镜片，
    GLASSES_TRANSPARENT_COLOR,//透明色，
    GLASSES_OTHER = 99//其他 = 99
};
//值类型 string
enum BagStyleType
{
    BAGSTYLE_SHOULDER_BAG = 1,//单肩包 = 1，
    BAGSTYLE_HANDBAG,//手提包，
    BAGSTYLE_BACKPACK,//双肩包，
    BAGSTYLE_PURSE,//钱包，
    BAGSTYLE_HAND_BAG,//手拿包，
    BAGSTYLE_POCKETS,//腰包，
    BAGSTYLE_KEY_PACKAGE,//钥匙包，
    BAGSTYLE_CARD_BAG,//卡包，
    BAGSTYLE_HAND_BOX,//手拉箱，
    BAGSTYLE_TRAVEL_BAG,//旅行包，
    BAGSTYLE_COWBOY_PACKAGE,//牛仔包，
    BAGSTYLE_INCLINED_SHOULDER_BAG,//斜挎包，
    BAGSTYLE_OTHER = 99//其他 = 99
};
//值类型 string
enum CoatStyleType
{
    COATSTYLE_SUIT = 1,//西装 = 1，
    COATSTYLE_NATIONAL_CLOTHING,//民族服，
    COATSTYLE_TSHIRT,//T恤，
    COATSTYLE_SHIRT,//衬衫，
    COATSTYLE_FLEECE,//卫衣，
    COATSTYLE_JACKET,//夹克，
    COATSTYLE_LEATHER_JACKET,//皮夹克，
    COATSTYLE_COTE,//大衣，
    COATSTYLE_DUST_COTE,//风衣，
    COATSTYLE_SWEATHER,//毛衣，
    COATSTYLE_COTTON_PADDEN_JACKET,//棉衣，
    COATSTYLE_DOWN_JACKET,//羽绒服，
    COATSTYLE_SPORTS_CLOTHES,//运动服，
    COATSTYLE_WORK_CLOTHES,//工作服，
    COATSTYLE_JEANS,//牛仔服，
    COATSTYLE_PAJAMAS,//睡衣，
    COATSTYLE_DRESS,//连衣裙，
    COATSTYLE_NO_COAT,//无上衣，
    COATSTYLE_OTHER = 99//其他= 99
};
//值类型 string
enum CoatLengthType
{
    COATLENGTH_LONG_SLEEVES = 1,//长袖 = 1，
    COATLENGTH_SHORT_SLEEVES,//短袖，
    COATLENGTH_SLEEVELESS//无袖
};
//值类型 string
enum PantsStyleType
{
    PANTSSTYLE_JEANS = 1,//牛仔裤 = 1，
    PANTSSTYLE_TROUSER,//西裤，
    PANTSSTYLE_OVERALL,//工装裤，
    PANTSSTYLE_LEATHER_PANTS,//皮裤，
    PANTSSTYLE_BEATH_PANTS,//沙滩裤，
    PANTSSTYLE_SWEATPANTS,//运动裤，
    PANTSSTYLE_PYJAMAS,//睡裤，
    PANTSSTYLE_NO_PANTS,//无裤子，
    PANTSSTYLE_OTHER = 99//其他= 99
};
//值类型 string
enum PantsLengthType
{
    PANTSLENGTH_LONG = 1,//长裤=1，
    PANTSLENGTH_SHORT//短裤
};
//值类型 string
enum ShoesStyleType
{
    SHOSESTYLE_BOARDSHOE = 1,//板鞋= 1，
    SHOSESTYLE_LEATHER_SHOES,//皮鞋，
    SHOSESTYLE_SPORTS_SHOES,//运动鞋，
    SHOSESTYLE_SLIPPERS,//拖鞋，
    SHOSESTYLE_SANDALS,//凉鞋，
    SHOSESTYLE_CASUAL_SHOES,//休闲鞋，
    SHOSESTYLE_HIGH_BOOTS,//高筒靴，
    SHOSESTYLE_BOOTS,//中筒靴，
    SHOSESTYLE_LOW_BOOTS,//低筒靴，
    SHOSESTYLE_HIKING_BOOTS,//登山靴，
    SHOSESTYLE_MILITARY_BOOTS,//军装靴，
    SHOSESTYLE_NO_BOOTS,//无靴子，
    SHOSESTYLE_OTHER = 99//其他=99
};
//值类型 string
enum Boolean
{
    BOOLEAN_FALSE = 0,  //false
    BOOLEAN_TRUE        //true
};

//值类型 string
enum AtuoFoilColorType
{
    ATUOFOILCOLOR_DARK_COLOR = 1,//深色= 1，
    ATUOFOILCOLOR_LIGHT_COLOR,//浅色，
    ATUOFOILCOLOR_NO_COLOR//无
};

//值类型 string
enum DentInfoType
{
    DENTINFO_NO_BUMPS = 0,//没有撞痕 = 0，
    DENTINFO_FEW_BUMPS,//少量撞痕，
    DENTINFO_LARGELY_BUMPS//大量撞痕
};

//值类型 string
enum FrontThingType
{	
    FRONTTHING_DETECTION_MARK = 1,//检测标志 = 1，
    FRONTTHING_PERFUME,//香水，
    FRONTTHING_HANG_ADORN,//挂饰，
    FRONTTHING_VISOR,//遮阳板，
    FRONTTHING_NIGHT_VISOR,//夜间遮阳板，
    FRONTTHING_OTHER = 99//其他=99
};

//值类型 string
enum RearThingType
{
    REARTHING_PILLOW = 1,//枕头=1，
    REARTHING_UMBERLLA,//雨伞，
    REARTHING_PAPER_TOWEL,//纸巾，
    REARTHING_CAR_STICLERS,//车贴，
    REARTHING_LOGO,//标识，
    REARTHING_GRAFFITI,//涂鸦，
    REARTHING_OTHER = 99//其他=99
};

//值类型 string
enum ThingPropertyType
{
    PROPERTY_CRIME_TOOLS = 1,//作案工具=1，
    PROPERTY_PILFER_GRABBED,//被盗抢，
    PROPERTY_LOSSES,//损失，
    PROPERTY_CAPTURE,//缴获，
    PROPERTY_OTHER = 99//其他=99
};

//值类型 string
enum IllustrationType
{
    ILLUSTRATION_FLOOR_PLAN,//平面图，
    ILLUSTRATION_STEREOGRAM,//立体图，
    ILLUSTRATION_PROFILE,//侧面图，
    ILLUSTRATION_AERIAL_VIEW,//鸟瞰图，
    ILLUSTRATION_OTHER = 99//其他=99
};

//值类型 string
enum IlluminationType
{
    ILLUMINATION_NATURAL_LIGHT = 1,//自然光=1，
    ILLUMINATION_LIGHT,//灯光，
    ILLUMINATION_FIRE//火光
};


//值类型 string
enum FieldConditionType
{
    FIELDCONDITION_ORIGINAL_SITE = 1,//原始现场 = 1，
    FIELDCONDITION_CHANGE_SITE,//变动现场，
    FIELDCONDITION_FORGERY_SECNE,//伪造现场，
    FIELDCONDITION_UNKNOWN_SCENE,//未知现场
};

//值类型 string
enum HumidityType
{
    Humidity = 1,//潮湿= 1，
    DRY,//干燥，
    SUITABLE//适宜
};

//值类型 string
enum DenseDegreeType
{
    DENSE_DEGREE_VERY_SPARSE = 1,//很稀疏= 1，
    DENSE_DEGREE_SPARSE,//稀疏，
    DENSE_DEGREE_DENSE,//密集，
    DENSE_DEGREE_VERY_DENSE//很密集
};

//值类型 string
enum InjuredDegreeType
{
    INJURED_DEGREE_DEATH = 1,//死亡 = 1，
    INJURED_DEGREE_SERIOUSLY,//重伤，
    INJURED_DEGREE_MINOR_INJURIES,//轻伤，
    INJURED_DEGREE_MINOR_INJURY,//轻微伤，
    INJURED_DEGREE_OTHER = 9//其他=9
};

//值类型 string
enum VehicleColorDepthType
{
    VEHICLE_COLOR_DEEP = 0,//深 = 0，
    VEHICLE_COLOR_SHALLOW//浅
};

//值类型 string
enum RoadTerraintype
{
    ROAD_TERRAINT_PLAIN = 1,//平原=1，
    ROAD_TERRAINT_HILLS,//丘陵，
    ROAD_TERRAINT_MOUNTAIN,//山地，
    ROAD_TERRAINT_BASIN,//盆地，
    ROAD_TERRAINT_PLATEAU//高原
};

//值类型 string
enum DetainessIdentityType
{
    DETAINESS_NATIONAL_CIVIL_SERVANT = 1,//国家公务员=1，
    DETAINESS_BUSINESS_MANAGEMENT,//企事业管理人员，
    DETAINESS_WORKERS,//工人，
    DETAINESS_FARMERS,//农民，
    DETAINESS_INDIVIFUAL_BUSINESSMEN,//个体工商业者，
    DETAINESS_STUDENT,//在校学生，
    DETAINESS_RETIREES,//离退休人员，
    DETAINESS_UNEMPLOYED_PEOPLE,//无业人员，
    DETAINESS_SOLDIER,//军人，
    DETAINESS_OTHER = 99//其他=99
};

//值类型 string[3]
enum IDType
{
    ID_RESIDENT_IDENTITY_CARD = 111,//居民身份证=111，
    ID_MILITARY_ID = 114,//军官证=114，
    ID_POLICE_ID = 123,//警官证=123，
    ID_ORDINARY_PASSPORT = 414//普通护照= 414
};

//值类型 string
enum GenderType
{
    //Unknown sex = 0,
    //Male,
    //Women,
    //Unstated sex =9
    GENDERTYPE_UNKNOWN_SEX = 0,//未知的性别= 0，
    GENDERTYPE_MALE,//男性，
    GENDERTYPE_WOMEN,//女性，
    GENDERTYPE_UNSTATED_SEX = 9//未说明的性别=9
};

//值类型 string
enum PlateClassType
{
    PLATE_LARGE_CAR = 1,//大型汽车号牌 = 01，
    PLATE_SMALL_CAR,//小型汽车号牌
    PLATE_EMBASSY_BUS,//使馆汽车号牌
    PLATE_CONSULATE_CAR,//领馆汽车号牌
    PLATE_OVERSEAS_CAR_LICENSE,//境外汽车号牌
    PLATE_FOREIGN_CAR_LICENSE,//外籍汽车号牌
    PLATE_OEDINARY_MOTORCYCLE_LICENSE,//普通摩托车号牌，
    PLATE_MOPEDS,//轻便摩托车号牌，
    PLATE_EMBASSY_MOTROCYCLE,//使馆摩托车号牌，
    PLATE_CONSULATE_MOTORCYCLE,//领馆摩托车号牌，
    PLATE_OVERSEAS_MOTORCYCLE,//境外摩托车号牌，
    PLATE_FOREIGN_MOTORCYCLE,//外籍摩托车号牌，
    PLATE_LOW_SPEED_LICENSE,//低速车号牌，
    PLATE_TRACTOR,//拖拉机号牌，
    PLATE_COACH,//教练汽车号牌，
    PLATE_COACH_MOTORCYCLE,//教练摩托车号牌，
    PLATE_TEMPORARY_ENTRY_VEHICLE = 20,//临时入境汽车号牌 = 20，
    PLATE_TEMPORARY_ENTRY_MOTORCYCLE,//临时入境摩托车号牌，
    PLATE_TEMPOARAY_DRVING_LICENSE,//临时行驶车号牌，
    PLATE_POLICE_CAR,//警用汽车号牌，
    PLATE_POLICE_MOTORCYCLE,//警用摩托车号牌，
    PLATE_ORIGINAL_LICENSE_AGRICULRUTAL_MACHINERY,//原农机号牌，
    PLATE_HONGKONG_EXIT,//香港出入境号牌，
    PLATE_MACAO_EXIT,//澳门出入境号牌，
    PLATE_ARMED_POLICE = 31,//武警号牌 = 31，
    PLATE_MILITARY,//军队号牌，
    PLATE_OTHER = 99,//其他号牌=99
};

//值类型 string
enum enPassportType
{
    PASSPORT_DIPLOMATIC = 11,//外交护照 = 11，
    PASSPORT_OFFICIAL,//公务护照，
    PASSPORT_ORDINARY_BUSINESS,//因公普通护照，
    PASSPORT_ORDINARY,//普通护照，
    PASSPORT_CHINA_PEOPLE_TRAVEL,//中华人民共和国旅行证，
    PASSPORT_TAIWAN_PERMIT_TRAVEL,//台湾居民来往大陆通行证，
    PASSPORT_SEAMAN,//海员证，
    PASSPORT_CREW_CERTIFICATE,//机组人员证，
    PASSPORT_RAILWAY_EMPLOYEE,//铁路员工证，
    PASSPORT_ENTRY_EXIT_PERMIT_CHINA_PEOPLE,//中华人民共和国出入境通行证，
    PASSPORT_TRAVEL_PERMIT_HONGKONG_MACAO,//往来港澳通行证，
    PASSPORT_PASS_HONGKONG_MACAO = 23,//前往港澳通行证 = 23，
    PASSPORT_COMPATRIOTS_HONGKONG_MACAO,//港澳同胞回乡证，
    PASSPORT_MAINLAND_TAIWAN_RESIDENT_TRAVEL,//大陆居民往来台湾通行证，
    PASSPORT_MAINLAND_HONGKONG_MACAO_TRAVEL,//往来香港澳门特别行政区通行证，
    PASSPORT_OVERSESAS_CHINESE_DINGJUZHENG,//华侨回国定居证，
    PASSPORT_TAIWAN_COMPATRIOTS_DINGJUZHENG,//台湾同胞定居证，
    PASSPORT_ALIEN_ENTRY_AND_EXIT_PERMIN,//外国人出入境证，
    PASSPORT_FOREIGNER_TRAVEL_PERMIT,//外国人旅行证，
    PASSPORT_RESIDENCE_PERMIT,//外国人居留证，
    PASSPORT_TEMPORARY_RESIDENCE_PERMIT_FOREIGNCE,//外国人临时居留证，
    PASSPORT_NATURALIZATION_CERTIFICATE = 35,//入籍证书= 35，
    PASSPORT_CERTIFICATE_ORIGIN,//出籍证书，
    PASSPORT_RENEWAL_CERTIFIVATE,//复籍证书，
    PASSPORT_TEMPORARY_RESIDENCE_PERMIT,//暂住证，
    PASSPORT_FISHING_PERMIT = 40,//出海渔船民证 = 40，
    PASSPORT_TEMPORARY_FISHING_PERMIT,//临时出海渔船民证，
    PASSPORT_HOUSEHOLD_SHIP_SEA,//出海船舶户口簿，
    PASSPORT_CERTIFICATE_ACCOUNT_VESSEL,//出海船舶户口证，
    PASSPORT_FLOAT_FISHERMEN_CERTIFICATE_GUANGDONG_HONGKONG_MACAO,//粤港澳流动渔民证，
    PASSPORT_TEMPORARY_HOUSEHOLD_REGISTER_FISHING_GUANGDONG_HONGKONG_MACAO,//粤港澳临时渔船户口簿，
    PASSPORT_PERMIT_RIDE_PLATFROM_WHEEL,//搭靠台轮许可证，
    PASSPORT_LABOR_SERVICE_PERSONMON_BORDING_WORK,//劳务人员登轮作业证，
    PASSPORT_RESIDENT_LOGIN_TAIWAN,//台湾居民登陆证，
    PASSPORT_TRADE_CARD,//贸易证，
    PASSPORT_BORDER_PASS = 60,//边境通行证 = 60，
    PASSPORT_TRANSIT_CULTIVATION_CERTIFICATE_SHENZHEN,//深圳市过境耕作证，
    PASSPORT_HKSAR_PASSPORT = 70,//香港特别行政区护照 = 70，
    PASSPORT_MACAO_PASSPORT = 71,//澳门特别行政区护照 = 71，
    PASSPORT_BURMESE_PASSPORT = 81,//缅甸中方（缅方）通行证 = 81，
    PASSPORT_ENTRY_EXIT_YUNNAN_BOARD_AREAS,//云南边境地区境外边民出入境证 ，
    PASSPORT_ENTRY_EXIT_CHINA_DPRK = 90,//中朝边境地区出入境通行证 = 90，
    PASSPORT_ENTRY_EXIT_DPRK_CHINA,//朝中边境地区居民过境通行证，
    PASSPORT_HYDROLOGY_OPERATION_YALU_TUMEN_RIVER,//鸭绿江、图们江水文作业证，
    PASSPORT_FIXED_REPRESENTATUVE_CHINESE_LOREAN,//中朝（朝中）流筏固定代表证，
    PASSPORT_CREW_CERTIFICATE_CARD,//中朝（朝中）鸭绿江、图们江航行船舶船员证，
    PASSPORT_GENERAL_PUBLIC_CARD,//中朝（朝中）边境地区公安总代表证，
    PASSPORT_DEPUTY_REPRESENTATIVE,//中朝（朝中）边境地区公安副总代表证，
    PASSPORT_CERTIFICATE_PUBLIC_SECURITY,//中朝（朝中）边境地区公安代表证，
    PASSPORT_OTHER,//其他证件 = 99
};

//值类型 string
enum VideoFormatType
{

    VIDEO_FORMAT_MPG,//Mpg = MPG
    VIDEO_FORMAT_MOV,//Mov = MOV
    VIDEO_FORMAT_AVI,//Avi = AVI
    VIDEO_FORMAT_RM,//Rm = RM
    VIDEO_FORMAT_RMVB,//Rmvb = RMVB
    VIDEO_FORMAT_FLV,//Flv = FLV
    VIDEO_FORMAT_VOB,//Vob = VOB
    VIDEO_FORMAT_M2TS,//M2ts = M2TS
    VIDEO_FORMAT_MP4,//Mp4 = MP4
    VIDEO_FORMAT_ES,//Es = ES
    VIDEO_FORMAT_PS,//Ps = PS
    VIDEO_FORMAT_TS,//Ts = TS文件
    VIDEO_FORMAT_OTHER,//Other = 其他
};

//值类型 int
enum InfoType
{
    INFO_TYPE_OTHER,//其他
    INFO_TYPE_AUTOMATIC,//自动采集
    INFO_TYPE_MANUAL,//手动采集
};

//值类型 string
enum ImageType
{
    IMAGE_CAR_ORIGINAL = 1, //车辆大图
    IMAGE_PLATE_COLOR, //车辆彩色小图
    IMAGE_PLATE_BINARIZATION,//车辆二值化图
    IMAGE_DRIVER_FACIAL,//驾驶员面部特征图
    IMAGE_CO_PILOT_FACIAL,//副驾驶面部特征图
    IMAGE_CAR_LOGO,//车标
    IMAGE_ILLEGAL_COMPOSITE_DRAWING,//违章合成图
    IMAGE_CAR_CROSS,//过车合成图
    IMAGE_VEHICLE_VIEW,//车辆特写图 ---- 车我们默认写的这个，待确认 TODO
    IMAGE_PERSON,//人员图
    IMAGE_FACE,//人脸图
    IMAGE_NON_MOTOR,//非机动车图
    IMAGE_ITEMS_FIGURE,//物品图
    IMAGE_SENCE,//场景图
    IMAGE_VEHICLE_FEATURE,//车辆特征图
    IMAGE_PERSONNEL_FEATURE,//人员特征图
    IMAGE_FACE_FEATURE,//人脸特征图
    IMAGE_NON_MOTOR_FEEEATURE,//非机动车特征图
    IMAGE_GENERAL_FEATURE,//一般特征图
    IMAGE_GENERAL = 100//一般图片
};

enum DispositionCategoryType
{
	DISPOSITION_PERSON=1, //人员
	DISPOSITION_FACE,   //人脸
	DISPOSITION_MOTOR,  //机动车
	DISPOSITION_NONMOTOR, //非机动车
	DISPOSITION_KEYWORD //关键字
};

enum SubscribeDetailType
{
	SUB_CASE_LIST=1, //案件目录
	SUB_CASE_CONTENT, //案件内容
	SUB_APES_LIST, //采集设备目录
	SUB_APES_STATUS, //采集设备状态
	SUB_APSS_LIST, //采集系统目录
	SUB_APSS_STATUS,//采集系统状态
	SUB_TOLLGATES_LIST, //视频卡口目录
	SUB_TOLLGATE_RECORD, //单个卡口记录
	SUB_LANES_LIST, //车道目录
	SUB_LANE_RECORD, //单个车道记录
	SUB_PERSON, //自动采集的人员信息
	SUB_FACE,   //自动采集的人脸信息
	SUB_MOTOR, //自动采集的车辆信息
	SUB_NONMOTOR, //自动采集的非机动车辆信息
	SUB_THING, //自动采集的物品信息
	SUB_FILE, //自动采集的文件信息
};

enum StatusType
{
	STATUS_ON_LINE  = 1, //在线
	STATUS_OFF_LINE = 2, //离线
	STATUS_OTHER = 9,   //其他
};

enum SubscribeStatus
{
	SUBSCRIBE_ING = 0, //订阅中
	SUBSCRIBE_CANCEL,  //取消订阅
	SUBSCRIBE_EXPIRATION,//订阅到期
	SUBSCRIBE_NONE = 9 //未订阅
};

enum Gat1400EventType
{
	OTHER_EVENT = 0,  //其他
	PASS_VEHICLE_EVENT, //过车
	PASS_PERSON_EVENT,//过人
	FIGHT_EVENT,      //打架
	RUN_EVENT,        //快速奔跑
	MOVING_OBJ_DETECT_EVENT, //运动目标检测
	TARGET_CLASSIFICATION_EVENT, //目标分类
	TARGET_COLOR_DETECT_EVENT, //目标颜色检测
	PERSON_DETECT_EVENT, //行人检测
	PERSON_ATTRIBUTE_ANALYSIS_EVENT, //人员属性分析
	FACE_DETECT_EVENT, //人脸检测
	FACE_MATCH_EVENT, //人脸比对
	VEHICLE_DETECT_EVENT, //车辆检测
	VEHICLE_MATCH_EVENT, //车辆比对
	TRAFFIC_STATISTICS_EVENT, //流量统计
	DENSITY_DETECT_EVENT, //密度检测
	PLATE_NO_RECOGNITION_EVENT, //车牌识别
	VEHICLE_BASIC_FEATURE_RECOGNITION_EVENT, //车辆基本特征识别
	VEHICLE_INDIVIDUAL_FEATURE_RECOGNITION_EVENT, //车辆个体特征识别
	TRIP_WIRE_DETECT_EVENT, //绊线检测
	INTRUSION_DETECT_EVENT, //入侵检测
	RETROGRADE_DETECT_EVENT, //逆行检测
	LOITERING_DETECT_EVENT, //徘徊检测
	ABANDONED_DETECT_EVENT, //遗留物检测
	TARGET_REMOVE_DETECT_EVENT, //目标移除检测
	VIDEO_SUMMARY_EVENT, //视频摘要
	DEFOGGING_EVENT, //去雾
	DEBLURRING_EVENT, //去模糊
	CONTRAST_ENHANCEMENT_EVENT, //对比度增强
	LOW_ILLUMINATION_VIDEO_IMAGE_ENHANCEMENT_EVENT, //低照度视频图像增强
	COLOR_CORRECTION_EVENT, //偏色校正
	WIDE_DYNAMIC_ENHANCEMENT_EVENT, //宽动态增强
	SUPER_RESOLUTION_RECONSTRUCTION_EVENT, //超分辨率重建
	GEOMETRIC_DISTORTION_CORRECTION_EVENT, //几何畸变校正
	ODD_EVEN_FIELD_CORRECTION_EVENT, //奇偶场校正
	COLOR_SPACE_COMPONENT_SEPARATION_EVENT, //颜色空间分量分离
	NOISE_ELIMINATION_EVENT, //去噪声
};

//设备功能类型
enum ApeFunctionType
{
	APE_VEHICLE = 1,  //车辆卡口
	APE_PERSON,       //人员卡口
	APE_MINI,         //微卡口
	APE_FEATURE,      //特征摄像机
	APE_COMMON,       //普通监控
	APE_HIGHALTITUDE, //高空瞭望摄像机
	APE_OTHER=99      //其他
};

/* SystemTime系统时间对象 属性结构体 */
typedef struct _GAT1400_SYSTEMTIME_ST_
{
    char VIIDServerID[GAT1400_DEVICE_ID_TYPE];     //R |DeviceIDType--string(20) |服务器标识符
    char TimeMode[4];                              //R |枚举类型 TimeCorrectModeType--string |校时模式 1网络; 2手动
    char LocalTime[GAT1400_DATETIME_TYPE];         //R |dateTime--格式YYYYMMDDhhmmssMMM   |日期时间
    char TimeZone[32];                             //R |string |时区

	_GAT1400_SYSTEMTIME_ST_()
	{
		memset(VIIDServerID, 0, sizeof(VIIDServerID));
		memset(TimeMode, 0, sizeof(TimeMode));
		memset(LocalTime, 0, sizeof(LocalTime));
		memset(TimeZone, 0, sizeof(TimeZone));
	}
}GAT1400_SYSTEMTIME_ST;

/* ResponseStatus应答状态对象      属性结构体 */
typedef struct _GAT1400_RESPONSESTATUS_ST_
{
    char ReqeustURL[256+1];                //R |string--256 |对应操作的URL
    int StatusCode;                        //R |枚举类型 ConfirmStatusType--string |确认状态码 |
    //0-OK,正常;
    //1-OtherError,其他未知错误;
    //2-Device Busy,设备忙;
    //3-Device Error,设备错;
    //4-Invalid Operation,无效操作;
    //5-Invalid XML Format,XML格式无效;
    //6-Invalid XML Content,XML内容无效;
    //7-Invalid JSON Format,JSON格式无效;
    //8-Invalid JSON Content,JSON内容无效;
    //9-Reboot,系统重启中,以附录B中类型定义为准
    char StatusString[256+1];               //R |string--256 |状态描述
    char ID[64+1];                          //R/O |string--64 |资源ID |POST方法创建资源时会返回ID,创建成功必须返回新的ID,创建失败则无此ID
    char LocalTime[GAT1400_DATETIME_TYPE];  //O |dateTime--格式YYYYMMDDhhmmssMMM   |日期时间 |当前时间,用于需要校时的场合

	_GAT1400_RESPONSESTATUS_ST_()
	{
		memset(ReqeustURL, 0, sizeof(ReqeustURL));
		StatusCode = 0;
		memset(StatusString, 0, sizeof(StatusString));
		memset(ID, 0, sizeof(ID));
		memset(LocalTime, 0, sizeof(LocalTime));
	}
}GAT1400_RESPONSESTATUS_ST;

enum HDireetionType{
    SOUTHTONORTH = 1,           //西向东
    NORTHTOSOUTH,               //东向西
    WESTTOEAST,                 //北向南
    EASTTOWEST,                 //南向北
    SOUTHWESTTONORTHEAST,       //西南到东北
    NORTHEASTTOSOUTHWEST,       //东北到西南
    NORTHWESTTOSOUTHEAST,       //西北到东南
    SOUTHEASTTONORTHWEST,       //东南到西北
    HDIREETIONOTHER,            //其他
};

typedef struct tagGatImage
{
    char ImageID[GAT1400_BASIC_OBJECT_ID_TYPE];      //R |BasicObiectIDType--string(41) 固定长度41|图像标识
    int InfoKind;                                    //R |枚举类型 InfoType--int |信息分类 |自动采集/人工采集 视频图像信息类型
    int ImageSource;                                 //R |枚举类型 DataSourceType--int |图像来源，（需要从string转换为int）
    char SourceVideoID[GAT1400_BASIC_OBJECT_ID_TYPE];//R |BasicObiectIDType--string(41) 固定长度41|来源视频标识|如果此图像是视频截图，此字段为来源视频的视频ID
    char OriginImageID[GAT1400_BASIC_OBJECT_ID_TYPE];//R/O |BasicObiectIDType--string(41) |原始图像标识 |增强处理后图像必选
    int  EventSort;                                  //R/O |EventType--int |事件分类|自动分析事件类型，设备采集必选
    char DeviceID[GAT1400_DEVICE_ID_TYPE];           //O |DeviceIDType--string(20) |设备编码 |采集设备ID
    char StoragePath[GAT1400_PATH_TYPE];             //O |存储路径 |图像文件的存储路径，采用URI命名规则
    char FileHash[GAT1400_FILE_HASH_TYPE];           //O |图像文件哈希值 |采用MD5算法
    char FileFormat[GAT1400_IMAGE_FORMAT_TYPE];      //R |ImageFormatType--string(6) |图像文件格式
    char ShotTime[GAT1400_DATETIME_TYPE];            //R |dateTime--格式YYYYMMDDhhmmssMMM   |拍摄时间
    char Title[128+1];                               //R |题名 |图像资料名称的汉语描述
    char TitleNote[128+1];                           //O |题名补充 |题名的补充和备注信息
    char SpecialName[128+1];                         //O |专项名 |图像资料所属的专项名称
    char Keyword[GAT1400_KEYWORD_TYPE];              //O |KeywordType--string(0..200) |关键字 |能够正确表述图像资料主要内容、具有检索意义的词
    char ContentDescription[256+1];                  //R |内容描述 |对图像内容的简要描述
    char SubjectCharacter[256+1];                    //O |主题人物 |图像资料中出现的主要人物的中文姓名全称，有多个用英文半角符号;分隔
    char ShotPlaceCode[GAT1400_PLACE_CODE_TYPE];       //O |PlaceCodeType--string(6) |拍摄地点行政区划代码 |人工采集图像需要
    char ShotPlaceFullAdress[GAT1400_PLACE_FULL_ADDRESS_TYPE];//R |PlaceFullAddressType--string(100) |拍摄地点区划内详细地址 |具体到街道门牌号
    double ShotPlaceLongitude;                      //O |LongitudeType--double |拍摄地点经度
    double ShotPlaceLatitude;                       //O |LatitudeType--double |拍摄地点纬度
    std::string HorizontalShotDirection;            //O |HorizontalShotType --string |水平拍摄方向
    std::string VerticalShotDirection;              //O |VerticalShotType --string |垂直拍摄方向
    int SecurityLevel;                              //R |枚举类型 SecurityLevleType |密级代码 |自动采集为5
    int Width;                                      //R |int |宽度 |水平像素值
    int Height;                                     //R |int |高度 |垂直像素值
    char CameraManufacturer[GAT1400_ORG_TYPE];      //O |OrgType--string(0..100) |照相机制造商 |若图片来自于照相机，则填写照相机的制造商
    char CameraVersion[GAT1400_MODEL_TYPE];         //O |ModelType--string(0..100) |照相机型号 |
    int ApertureValue;                              //O |int |光圈值 |光圈F值
    int ISOSensitivity;                             //O |int |ISO感光值 |照片的ISO感光值
    int FocalLength;                                //O |int |焦距 |拍摄照片使用的焦距
    int QualityGrade;                               //O |枚举类型 QualityGradeType--int |质量等级
    char CollectorName[GAT1400_NAME_TYPE];          //R/O |NameType--string(50) |采集人 |采集人名称或者采集系统名称，人工采集必选
    char CollectorOrg[GAT1400_ORG_TYPE];            //R/O |OrgType--string(100) |采集单位名称 |人工采集必选
    int CollectorIDType;                            //O |枚举类型 IDType--int |采集人证件类型 |
    char CollectorID[GAT1400_ID_NUMBER_TYPE];       //O |IDNumberType--string(30) |采集人证件号码 |
    char EntryClerk[GAT1400_NAME_TYPE];             //R/O |NameType--string(50) |入库人 |入库人名称或者入库系统名称，人工采集必选
    char EntryClerkOrg[GAT1400_ORG_TYPE];           //R/O |OrgType--string(100) |入库单位名称 |人工采集必选
    int EntryClerkIDType;                           //O |枚举类型 IDType--int |入库人证件类型 |
    char EntryClerkID[GAT1400_ID_NUMBER_TYPE];      //O |IDNumberType--string(30) |入库人证件号码 |
    char EntryTime[GAT1400_DATETIME_TYPE];          //R/O |dataTime| 保温不需要此字段，视图库自动生成
    int ImageProcFlag;                              //O |int |图像处理标志 | 0：未处理， 1：图像经过处理
    int FileSize;                                   //O |int |图像文件大小 | 单位byte
    //std::string Data;                               //O |string|图像数据  //base64Binary  

	tagGatImage()
	{
		memset(ImageID, 0, sizeof(ImageID));
		InfoKind = 0;
		ImageSource = 0;
		memset(SourceVideoID, 0, sizeof(SourceVideoID));
		memset(OriginImageID, 0, sizeof(OriginImageID));
		EventSort = 0;
		memset(DeviceID, 0, sizeof(DeviceID));
		memset(StoragePath, 0, sizeof(StoragePath));
		memset(FileHash, 0, sizeof(FileHash));
		memset(FileFormat, 0, sizeof(FileFormat));
		memset(ShotTime, 0, sizeof(ShotTime));
		memset(Title, 0, sizeof(Title));
		memset(TitleNote, 0, sizeof(TitleNote));
		memset(SpecialName, 0, sizeof(SpecialName));
		memset(Keyword, 0, sizeof(Keyword));
		memset(ContentDescription, 0, sizeof(ContentDescription));
		memset(SubjectCharacter, 0, sizeof(SubjectCharacter));
		memset(ShotPlaceCode, 0, sizeof(ShotPlaceCode));
		memset(ShotPlaceFullAdress, 0, sizeof(ShotPlaceFullAdress));
		ShotPlaceLongitude = 0.0f;
		ShotPlaceLatitude = 0.0f;
		SecurityLevel = 0;
		Width = 0;
		Height = 0;
		memset(CameraManufacturer, 0, sizeof(CameraManufacturer));
		memset(CameraVersion, 0, sizeof(CameraVersion));
		ApertureValue = 0;
		ISOSensitivity = 0;
		FocalLength = 0;
		QualityGrade = 0;
		memset(CollectorName, 0, sizeof(CollectorName));
		memset(CollectorOrg, 0, sizeof(CollectorOrg));
		CollectorIDType = 0;
		memset(CollectorID, 0, sizeof(CollectorID));
		memset(EntryClerk, 0, sizeof(EntryClerk));
		memset(EntryClerkOrg, 0, sizeof(EntryClerkOrg));
		EntryClerkIDType = 0;
		memset(EntryClerkID, 0, sizeof(EntryClerkID));
		memset(EntryTime, 0, sizeof(EntryTime));
		ImageProcFlag = 0;
		FileSize = 0;
	}
}GAT_1400_Image;


typedef struct tagGatSubImage
{
    char ImageID[GAT1400_BASIC_OBJECT_ID_TYPE];     //R |BasicObiectIDType--string(41) 固定长度41|图像标识
    int  EventSort;                                 //R/O |EventType--int |事件分类|自动分析事件类型，设备采集必选
    char DeviceID[GAT1400_DEVICE_ID_TYPE];          //O |DeviceIDType--string(20) |设备编码 |采集设备ID
    char StoragePath[GAT1400_PATH_TYPE];            //O |存储路径 |图像文件的存储路径，采用URI命名规则
    ImageType Type;								    //R |图像类型
    char FileFormat[GAT1400_IMAGE_FORMAT_TYPE];     //R |ImageFormatType--string(6) |图像文件格式
    char ShotTime[GAT1400_DATETIME_TYPE];           //R |dateTime--格式YYYYMMDDhhmmssMMM   |拍摄时间
    int Width;                                      //R |int |宽度 |水平像素值
    int Height;                                     //R |int |高度 |垂直像素值
    std::string Data;                               //O |string|图像数据  //base64Binary  

	tagGatSubImage()
	{
		memset(ImageID, 0, sizeof(ImageID));
		EventSort = 0;
		memset(DeviceID, 0, sizeof(DeviceID));
		memset(StoragePath, 0, sizeof(StoragePath));
		Type = IMAGE_CAR_ORIGINAL;
		memset(FileFormat, 0, sizeof(FileFormat));
		memset(ShotTime, 0, sizeof(ShotTime));
		Width = 0;
		Height = 0;
	}
}GAT_1400_SubImage;

//人脸对象
typedef struct tagGat1400Face
{
    char FaceID[GAT1400_IMAGE_CNT_OBJECT_ID_TYPE];  //R |ImageCntObjectIDtype--string(48) 固定长度48|人脸标识
    int InfoKind;                                   //R |枚举类型 InfoType--int |信息分类 |自动采集/人工采集 视频图像信息类型
    char SourceID[GAT1400_BASIC_OBJECT_ID_TYPE];    //R |BasicObjectIDType--string(41) |来源标识 |来源图像信息标识 
    char DeviceID[GAT1400_DEVICE_ID_TYPE];          //R/O |DeviceIDType--string(20) |设备编码 |采集设备ID 自动采集必选
    int LeftTopX;                                   //R/O |int |左上角X坐标 |人脸区域 自动采集必选
    int LeftTopY;                                   //R/O |int |左上角Y坐标 |人脸区域 自动采集必选
    int RightBtmX;                                  //R/O |int |右下角X坐标 |人脸区域 自动采集必选
    int RightBtmY;                                  //R/O |int |右下角Y坐标 |人脸区域 自动采集必选  
    char LocationMarkTime[GAT1400_DATETIME_TYPE];   //O |dateTime--格式YYYYMMDDhhmmssMMM   |日期时间 |位置标记时间 人工采集有效
    char FaceAppearTime[GAT1400_DATETIME_TYPE];     //O |dateTime--格式YYYYMMDDhhmmssMMM   |日期时间 |人脸出现时间 人工采集有效
    char FaceDisAppearTime[GAT1400_DATETIME_TYPE];  //O |dateTime--格式YYYYMMDDhhmmssMMM   |日期时间 |人脸消失时间 人工采集有效
    int  IDType;                                    //O |枚举类型 IDType--int |证件类型 |
    char IDNumber[GAT1400_ID_NUMBER_TYPE];          //O |IDNumberType--string(30) |有效证件号码 |
    char Name[GAT1400_NAME_TYPE];                   //O |NameType--string(50) |姓名 |
    char UsedName[GAT1400_USED_NAME_TYPE];          //O |UsedNameType--string(50) |曾用名 |
    char Alias[GAT1400_ALIAS_TYPE];                 //O |AliasType--string(50) |绰号 |
    int GenderCode;                                 //O |枚举类型 GenderType--int |性别
    int AgeUpLimit;                                 //O |int |最大可能年龄
    int AgeLowerLimit;                              //O |int |最小可能年龄
    char EthicCode[GAT1400_ETHIC_CODE_TYPE];        //O |EthicCodeType--string(2) |中国各民族的罗马字母拼写法和代码 |
    char NationalityCode[GAT1400_NATIONALITY_CODE_TYPE]; //O |NationalityCodeType--string(3) |国籍代码、世界各国和地区名称代码 |
    char NativeCityCode[GAT1400_PLACE_CODE_TYPE];        //O |PlaceCodeType--string(6) |籍贯省市县代码 |
    char ResidenceAdminDivision[GAT1400_PLACE_CODE_TYPE];//O |PlaceCodeType--string(6) |居住地行政区划 |       
    char ChineseAccentCode[GAT1400_CHINESE_ACCENT_CODE]; //O |ChineseAccentCode--string(6) |汉语口音 |       
    char JobCategory[GAT1400_PLACE_CODE_TYPE];           //O |PlaceCodeType--string(3) |职业 |       
    int AccompanyNumber;                            //O |int |同行人脸数
    std::string SkinColor;                          //O |string |肤色
    int HairStyle;                                  //O |枚举类型 HairStyleType--int |发型 |
    int HairColor;                                  //O |枚举类型 ColorType--int |发色 |
    char FaceStyle[GAT1400_FACE_STYLE_TYPE];        //O |FaceStyleType--string(4) |脸型 |       
    char FacialFeature[GAT1400_FACIAL_FEATURE_TYPE];    //O |FaccialFeatureType--string(40) |脸部特征 |       
    char PhysicalFeature[GAT1400_PHYSICAL_FEATURE_TYPE];//O |PhysicalFeatureType--string(200) |体貌特征 |       
    int RespiratorColor;                            //O |枚举类型 ColorType--int |口罩颜色 |
    int CapStyle;                                   //O |枚举类型 HatStyleType--int |帽子款式 |
    int CapColor;                                   //O |枚举类型 ColorType--int |帽子颜色 |
    int GlassStyle;                                 //O |枚举类型 GlassesStyleType--int |眼睛款式 |
    int GlassColor;                                 //O |枚举类型 ColorType--int |眼镜颜色 |
    int IsDriver;                                   //R/O |int |是否驾驶员 |0否;1是;2不确定
    int IsForeigner;                                //R/O |int |是否涉外人员 |0否;1是;2不确定
    int PassportType;                               //O |枚举类型 enPassportType--int |护照证件类型 |
    char ImmigrantTypeCode[GAT1400_IMMIGRANT_TYPE_CODE_TYPE];//O |ImmigrantTypeCodeType--string(2) |出入境人员类型代码 |       
    int IsSuspectedTerrorist;                       //R |int |是否涉恐人员 |0否;1是;2不确定
    char SuspectedTreeoristNumber[GAT1400_SUSPECTED_TERRORIST_NUMBER_TYPE]; //O |SuspectedTerroristNumberType--string(19) |涉恐人员编号 |       
    int IsCriminalInvolved;                         //R |int |是否涉案人员 |0否;1是;2不确定
    char CriminalInvolvedSpecilisationCode[GAT1400_CRIMINAL_INVOLVED_SPECILISATION_CODE_TYPE]; //O |CriminalInvolvedSpecilisationCodeType--string(19) |涉案人员专长代码 |       
    char BodySpeciallMark[GAT1400_BODY_SPECIALL_MARK_TYPE]; //O |BodySpeciallMarkType--string(7) |体表特殊标记 |       
    char CrimeMethod[GAT1400_CRIME_METHOD_TYPE];           //O |CrimeMethodType--string(4) |作案手段 |       
    char CrimeCharacterCode[GAT1400_CRIME_CHARACTER_CODE_TYPE]; //O |CrimeCharacterCodeType--string(19) |作案特点代码 |       
    char EscapedCriminalNumber[GAT1400_ESCAPED_CRIMINAL_NUMBER_TYPE]; //O |EscapedCriminalNumberType--string(23) |在逃人员信息编号规则 |       
    int IsDetainees;                                //R |int |是否在押人员 |0否;1是;2不确定
    char DetentionHouseCode[GAT1400_DETENTION_HOUSE_CODE_TYPE];//O |DetentionHouseCodeType--string(9) |看守所编码 |       
    int DetainessIdentity;                          //O |枚举类型 DetainessIdentityType--int |在押人员身份 |
    char DetaineesSpecialIdentity[GAT1400_DETAINEES_SPECIAL_IDENTITY_TYPE]; //O |DetaineesSpecialIdentityType--string(2) |在押人员特殊身份编码 |       
    char MemberTypeCode[GAT1400_MEMBER_TYPE_CODE_TYPE];  //O |MemberTypeCodeType--string(2) |在押人员类型 |      
    int IsVictim;                                   //R |int |是否被害人员 |0否;1是;2不确定
    char VictimType[GAT1400_VICTIM_TYPE];           //O |VictimType--string(3) |被害人类型 |      
    std::string InjuredDegree;                      //O |枚举类型 InjureDegreeType--string |受伤害程度 |
    char CorpseConditionCode[GAT1400_CORPSE_CONDITION_CODE_TYPE];  //O |CorpseConditionCodeType--string(2) |尸体状态 |      
    int IsSuspiciousPerson;                         //R |int |是否可疑人员 |0否;1是;2不确定  
    int Attitude;                                   //O |int |姿态分布 |1、平视；2、微仰；3、微俯；4、左微侧脸；5、左斜侧脸；6、左全侧脸；7、右微侧脸；8、右斜侧脸；9、右全侧脸
    double Similaritydegree;                        //O |double |人脸相似度 |【0,1】
    std::string EyebrowStyle;                       //O |string |眉型
    std::string NoseStyle;                          //O |string |鼻型
    std::string MustacheStyle;                      //O |string |胡型
    std::string LipStyle;                           //O |string |嘴唇
    std::string WriklePouch;                        //O |string |皱纹眼袋
    std::string AcneStain;                          //O |string |痤疮色斑
    std::string FreckleBirthmark;                   //O |string |黑痣胎记
    std::string ScarDimple;                         //O |string |疤痕酒窝
    std::string OtherFeature;                       //O |string |其他特征
    std::list<GAT_1400_SubImage>  SubImageList;   

	tagGat1400Face()
	{
		memset(FaceID, 0, sizeof(FaceID));
		InfoKind = 0;
		memset(SourceID, 0, sizeof(SourceID));
		memset(DeviceID, 0, sizeof(DeviceID));
		LeftTopX = 0;
		LeftTopY = 0;
		RightBtmX = 0;
		RightBtmY = 0;
		memset(LocationMarkTime, 0, sizeof(LocationMarkTime));
		memset(FaceAppearTime, 0, sizeof(FaceAppearTime));
		memset(FaceDisAppearTime, 0, sizeof(FaceDisAppearTime));
		IDType = ID_RESIDENT_IDENTITY_CARD;
		memset(IDNumber, 0, sizeof(IDNumber));
		memset(Name, 0, sizeof(Name));
		memset(UsedName, 0, sizeof(UsedName));
		memset(Alias, 0, sizeof(Alias));
		GenderCode = 0;
		AgeUpLimit = 0;
		AgeLowerLimit = 0;
		memset(EthicCode, 0, sizeof(EthicCode));
		memset(NationalityCode, 0, sizeof(NationalityCode));
		memset(NativeCityCode, 0, sizeof(NativeCityCode));
		memset(ResidenceAdminDivision, 0, sizeof(ResidenceAdminDivision));
		memset(ChineseAccentCode, 0, sizeof(ChineseAccentCode));
		memset(JobCategory, 0, sizeof(JobCategory));
		AccompanyNumber = 0;
		HairStyle = 0;
		HairColor = 0;
		memset(FaceStyle, 0, sizeof(FaceStyle));
		memset(FacialFeature, 0, sizeof(FacialFeature));
		memset(PhysicalFeature, 0, sizeof(PhysicalFeature));
		RespiratorColor = 0;
		CapStyle = 0;
		CapColor = 0;
		GlassStyle = 0;
		GlassColor = 0;
		IsDriver = 0;
		IsForeigner = 0;
		PassportType = 0;
		memset(ImmigrantTypeCode, 0, sizeof(ImmigrantTypeCode));
		IsSuspectedTerrorist = 0;
		memset(SuspectedTreeoristNumber, 0, sizeof(SuspectedTreeoristNumber));
		IsCriminalInvolved = 0;
		memset(CriminalInvolvedSpecilisationCode, 0, sizeof(CriminalInvolvedSpecilisationCode));
		memset(BodySpeciallMark, 0, sizeof(BodySpeciallMark));
		memset(CrimeMethod, 0, sizeof(CrimeMethod));
		memset(CrimeCharacterCode, 0, sizeof(CrimeCharacterCode));
		memset(EscapedCriminalNumber, 0, sizeof(EscapedCriminalNumber));
		IsDetainees = 0;
		memset(DetentionHouseCode, 0, sizeof(DetentionHouseCode));
		DetainessIdentity = 0;
		memset(DetaineesSpecialIdentity, 0, sizeof(DetaineesSpecialIdentity));
		memset(MemberTypeCode, 0, sizeof(MemberTypeCode));
		IsVictim = 0;
		memset(VictimType, 0, sizeof(VictimType));
		memset(CorpseConditionCode, 0, sizeof(CorpseConditionCode));
		IsSuspiciousPerson = 0;
		Attitude = 0;
		Similaritydegree = 0.0f;
	}
}GAT_1400_Face;

//人员对象
typedef struct tagGat1400Person
{
    char PersonID[GAT1400_IMAGE_CNT_OBJECT_ID_TYPE]; //R |ImageCntObjectIDtype--string(48) 固定长度48|人员标识
    int InfoKind;                                    //R |枚举类型 InfoType--int |信息分类 |自动采集/人工采集
    char SourceID[GAT1400_BASIC_OBJECT_ID_TYPE];     //R |BasicObjectIDType--string(41) |来源标识 |来源图像信息标识 
    char DeviceID[GAT1400_DEVICE_ID_TYPE];           //R/O |DeviceIDType--string(20) |设备编码 |采集设备ID 自动采集必选
    int LeftTopX;                                    //R/O |int |左上角X坐标 |人脸区域 自动采集必选
    int LeftTopY;                                    //R/O |int |左上角Y坐标 |人脸区域 自动采集必选
    int RightBtmX;                                   //R/O |int |右下角X坐标 |人脸区域 自动采集必选
    int RightBtmY;                                   //R/O |int |右下角Y坐标 |人脸区域 自动采集必选  
    char LocationMarkTime[GAT1400_DATETIME_TYPE];    //O |dateTime--格式YYYYMMDDhhmmssMMM   |日期时间 |位置标记时间 人工采集有效
    char PersonAppearTime[GAT1400_DATETIME_TYPE];    //O |dateTime--格式YYYYMMDDhhmmssMMM   |日期时间 |人脸出现时间 人工采集有效
    char PersonDisAppearTime[GAT1400_DATETIME_TYPE]; //O |dateTime--格式YYYYMMDDhhmmssMMM   |日期时间 |人脸消失时间 人工采集有效
    int IDType;                                      //O |枚举类型 IDType--int |证件类型 |
    char IDNumber[GAT1400_ID_NUMBER_TYPE];           //O |IDNumberType--string(30) |有效证件号码 |
    char Name[GAT1400_NAME_TYPE];                    //O |NameType--string(50) |姓名 |
    char UsedName[GAT1400_NAME_TYPE];                //O |UsedNameType--string(50) |曾用名 |
    char Alias[GAT1400_NAME_TYPE];                   //O |AliasType--string(50) |绰号 |
    int GenderCode;                                  //O |枚举类型 GenderType--int |性别
    int AgeUpLimit;                                  //O |int |最大可能年龄
    int AgeLowerLimit;                               //O |int |最小可能年龄
    char EthicCode[GAT1400_ETHIC_CODE_TYPE];         //O |EthicCodeType--string(2) |中国各民族的罗马字母拼写法和代码 |
    char NationalityCode[GAT1400_NATIONALITY_CODE_TYPE];  //O |NationalityCodeType--string(3) |国籍代码、世界各国和地区名称代码 |
    char NativeCityCode[GAT1400_PLACE_CODE_TYPE];         //O |PlaceCodeType--string(6) |籍贯省市县代码 |
    char ResidenceAdminDivision[GAT1400_PLACE_CODE_TYPE]; //O |PlaceCodeType--string(6) |居住地行政区划 |       
    char ChineseAccentCode[GAT1400_CHINESE_ACCENT_CODE];  //O |ChineseAccentCode--string(6) |汉语口音 |       
    char PersonOrg[GAT1400_ORG_TYPE];                 //O |OrgType--string(100) |单位名称 |    
    char JobCategory[GAT1400_JOB_CATEGORY_TYPE];      //O |JobCategoryType--string(3) |职业 |       
    int AccompanyNumber;                              //O |int |同行人脸数
    int HeightUpLimit;                                //O |int |身高上限 |单位（cm）
    int HeightLowerLimit;                             //O |int |身高下限 |单位（cm）
    std::string BodyType;                             //O |string |体型 |       
    std::string SkinColor;                            //O |string |肤色
    int HairStyle;                                    //O |枚举类型 HairStyleType--int |发型 |
    int HairColor;                                    //O |枚举类型 ColorType--int |发色 |
    int Gesture;                                      //O |枚举类型 PostureType--int |姿态
    int Status;                                       //O |枚举类型 PersonStatusType--int |状态
    char FaceStyle[GAT1400_FACE_STYLE_TYPE];          //O |FaceStyleType--string(4) |脸型 |       
    char FacialFeature[GAT1400_FACIAL_FEATURE_TYPE];  //O |FaccialFeatureType--string(40) |脸部特征 |       
    char PhysicalFeature[GAT1400_PHYSICAL_FEATURE_TYPE];  //O |PhysicalFeatureType--string(200) |体貌特征 |       
    char BodyFeature[GAT1400_BODY_FEATURE_TYPE];      //O |BodyFeatureType--string(70) |体表特征
    int HabitualMovement;                             //O |枚举类型 HabitualActionType--int | 习惯动作
    int Behavior;                                     //O |枚举类型 BehaviorType--int |行为
    std::string BehaviorDescription;                  //O |string |行为描述，对行为备注中没有的行为进行描述
    std::list<int> Appendant;                         //O |枚举类型 AppendageType--int | 附属物 |有多个时报文使用英文半角分号分隔
    std::string AppendantDescription;                 //O |string |附属物描述， 对代码表中没有的附属物进行描述
    int UmbrellaColor;                                //O |枚举类型 ColorType--int |伞颜色 |
    int RespiratorColor;                              //O |枚举类型 ColorType--int |口罩颜色 |
    int CapStyle;                                     //O |枚举类型 HatStyleType--int |帽子款式 |
    int CapColor;                                     //O |枚举类型 ColorType--int |帽子颜色 |
    int GlassStyle;                                   //O |枚举类型 GlassesStyleType--int |眼睛款式 |
    int GlassColor;                                   //O |枚举类型 ColorType--int |眼镜颜色 |
    int ScarfColor;                                   //O |枚举类型 ColorType--int |围巾颜色 |
    int BagStyle;                                     //O |枚举类型 BagStyleType--int |包款式 |
    int BagColor;                                     //O |枚举类型 ColorType--int |包颜色 |
    int CoatStyle;                                    //O |枚举类型 CoatStyleType--int |上衣款式 |
    std::string CoatLength;                           //O |枚举类型 CoatLengthType--string |上衣长度 |
    int CoatColor;                                    //O |枚举类型 ColorType--int |上衣颜色 |
    int TrousersStyle;                                //O |枚举类型 TrousersStyleType--int |裤子款式 |
    int TrousersColor;                                //O |枚举类型 ColorType--int |裤子颜色 |
    std::string TrousersLen;                          //O |枚举类型 TrousersLengthType--string |裤子长度 |
    int ShoesStyle;                                   //O |枚举类型 ShoesStyleType--int |鞋子款式 |
    int ShoesColor;                                   //O |枚举类型 ColorType--int |鞋子颜色 |
    int IsDriver;                                     //R/O |int |是否驾驶员 |0否;1是;2不确定
    int IsForeigner;                                  //R/O |int |是否涉外人员 |0否;1是;2不确定
    int PassportType;                                 //O |枚举类型 enPassportType--int |护照证件类型 |
    char ImmigrantTypeCode[GAT1400_IMMIGRANT_TYPE_CODE_TYPE];          //O |ImmigrantTypeCodeType--string(2) |出入境人员类型代码 |       
    int IsSuspectedTerrorist;                                          //R |int |是否涉恐人员 |0否;1是;2不确定
    char SuspectedTreeoristNumber[GAT1400_SUSPECTED_TERRORIST_NUMBER_TYPE];//O |SuspectedTerroristNumberType--string(19) |涉恐人员编号 |       
    int IsCriminalInvolved;                                            //R |int |是否涉案人员 |0否;1是;2不确定
    char CriminalInvolvedSpecilisationCode[GAT1400_CRIMINAL_INVOLVED_SPECILISATION_CODE_TYPE]; //O |CriminalInvolvedSpecilisationCodeType--string(19) |涉案人员专长代码 |       
    char BodySpeciallMark[GAT1400_BODY_SPECIALL_MARK_TYPE];            //O |BodySpeciallMarkType--string(7) |体表特殊标记 |       
    char CrimeMethod[GAT1400_CRIME_METHOD_TYPE];                       //O |CrimeMethodType--string(4) |作案手段 |       
    char CrimeCharacterCode[GAT1400_CRIME_CHARACTER_CODE_TYPE];        //O |CrimeCharacterCodeType--string(19) |作案特点代码 |       
    char EscapedCriminalNumber[GAT1400_ESCAPED_CRIMINAL_NUMBER_TYPE];  //O |EscapedCriminalNumberType--string(23) |在逃人员信息编号规则 |       
    int IsDetainees;                                                   //R |int |是否在押人员 |0否;1是;2不确定
    char DetentionHouseCode[GAT1400_DETENTION_HOUSE_CODE_TYPE];        //O |DetentionHouseCodeType--string(9) |看守所编码 |       
    int DetainessIdentity;                                             //O |枚举类型 DetainessIdentityType--int |在押人员身份 |
    char DetaineesSpecialIdentity[GAT1400_DETAINEES_SPECIAL_IDENTITY_TYPE]; //O |DetaineesSpecialIdentityType--string(2) |在押人员特殊身份编码 |       
    char MemberTypeCode[GAT1400_MEMBER_TYPE_CODE_TYPE];                //O |MemberTypeCodeType--string(2) |在押人员类型 |      
    int IsVictim;                                                      //R |int |是否被害人员 |0否;1是;2不确定
    char VictimType[GAT1400_VICTIM_TYPE];                              //O |VictimType--string(2) |被害人类型 |      
    std::string InjuredDegree;                                         //O |枚举类型 InjureDegreeType--string |受伤害程度 |
    char CorpseConditionCode[GAT1400_CORPSE_CONDITION_CODE_TYPE];      //O |CorpseConditionCodeType--string(2) |尸体状态 |      
    int IsSuspiciousPerson;                                            //R |int |是否可疑人员 |0否;1是;2不确定  
    std::list<GAT_1400_SubImage>  SubImageList;

	tagGat1400Person()
	{
		memset(PersonID, 0, sizeof(PersonID));
		InfoKind = 0;
		memset(SourceID, 0, sizeof(SourceID));
		memset(DeviceID, 0, sizeof(DeviceID));
		LeftTopX = 0;
		LeftTopY = 0;
		RightBtmX = 0;
		RightBtmY = 0;
		memset(LocationMarkTime, 0, sizeof(LocationMarkTime));
		memset(PersonAppearTime, 0, sizeof(PersonAppearTime));
		memset(PersonDisAppearTime, 0, sizeof(PersonDisAppearTime));
		IDType = 0;
		memset(IDNumber, 0, sizeof(IDNumber));
		memset(Name, 0, sizeof(Name));
		memset(UsedName, 0, sizeof(UsedName));
		memset(Alias, 0, sizeof(Alias));
		GenderCode = 0;
		AgeUpLimit = 0;
		AgeLowerLimit = 0;
		memset(EthicCode, 0, sizeof(EthicCode));
		memset(NationalityCode, 0, sizeof(NationalityCode));
		memset(NativeCityCode, 0, sizeof(NativeCityCode));
		memset(ResidenceAdminDivision, 0, sizeof(ResidenceAdminDivision));
		memset(ChineseAccentCode, 0, sizeof(ChineseAccentCode));
        memset(PersonOrg, 0, sizeof(PersonOrg));
		memset(JobCategory, 0, sizeof(JobCategory));
		AccompanyNumber = 0;
		HeightUpLimit = 0;
		HeightLowerLimit = 0;
		HairStyle = 0;
		HairColor = 0;
		Gesture = 0;
		Status = 0;
		memset(FaceStyle, 0, sizeof(FaceStyle));
		memset(FacialFeature, 0, sizeof(FacialFeature));
		memset(PhysicalFeature, 0, sizeof(PhysicalFeature));
		memset(BodyFeature, 0, sizeof(BodyFeature));
		HabitualMovement = 0;
		Behavior = 0;
		UmbrellaColor = 0;
		RespiratorColor = 0;
		CapStyle = 0;
		CapColor = 0;
		GlassStyle = 0;
		GlassColor = 0;
		ScarfColor = 0;
		BagStyle = 0;
		BagColor = 0;
		CoatStyle = 0;
		CoatColor = 0;
		TrousersStyle = 0;
		TrousersColor = 0;
		ShoesStyle = 0;
		ShoesColor = 0;
		IsDriver = 0;
		IsForeigner = 0;
		PassportType = 0;
		memset(ImmigrantTypeCode, 0, sizeof(ImmigrantTypeCode));
		IsSuspectedTerrorist = 0;
		memset(SuspectedTreeoristNumber, 0, sizeof(SuspectedTreeoristNumber));
		IsCriminalInvolved = 0;
		memset(CriminalInvolvedSpecilisationCode, 0, sizeof(CriminalInvolvedSpecilisationCode));
		memset(BodySpeciallMark, 0, sizeof(BodySpeciallMark));
		memset(CrimeMethod, 0, sizeof(CrimeMethod));
		memset(CrimeCharacterCode, 0, sizeof(CrimeCharacterCode));
		memset(EscapedCriminalNumber, 0, sizeof(EscapedCriminalNumber));
		IsDetainees = 0;
		memset(DetentionHouseCode, 0, sizeof(DetentionHouseCode));
		DetainessIdentity = 0;
		memset(DetaineesSpecialIdentity, 0, sizeof(DetaineesSpecialIdentity));
		memset(MemberTypeCode, 0, sizeof(MemberTypeCode));
		IsVictim = 0;
		memset(VictimType, 0, sizeof(VictimType));
		memset(CorpseConditionCode, 0, sizeof(CorpseConditionCode));
		IsSuspiciousPerson = 0;
	}
}GAT_1400_Person;

//机动车对象
typedef struct tagGat1400Motor
{
    char MotorVehicleID[GAT1400_IMAGE_CNT_OBJECT_ID_TYPE];  //R |ImageCntObjectIDtype--string(48) 固定长度48|车辆标识 
    int InfoKind;                                   //R |枚举类型 InfoType--int |信息分类 |自动采集/人工采集
    char SourceID[GAT1400_BASIC_OBJECT_ID_TYPE];    //R |BasicObjectIDType--string(41) |来源标识 |来源图像信息标识 
    char TollgateID[GAT1400_DEVICE_ID_TYPE];        //O |DeviceIDType--string(20) |关联卡口编码 |卡口编码
    char DeviceID[GAT1400_DEVICE_ID_TYPE];          //R/O |DeviceIDType--string(20) |设备编码 |采集设备ID 自动采集必选
    char StorageUrl1[URL_LEN];                      //R |string(256) |近景照片
    char StorageUrl2[URL_LEN];                      //R |string(256) |车牌照片
    char StorageUrl3[URL_LEN];                      //R |string(256) |远景照片
    char StorageUrl4[URL_LEN];                      //R |string(256) |合成图
    char StorageUrl5[URL_LEN];                      //R |string(256) |缩略图
    int LeftTopX;                                   //R/O |int |左上角X坐标 | 自动采集必选
    int LeftTopY;                                   //R/O |int |左上角Y坐标 | 自动采集必选
    int RightBtmX;                                  //R/O |int |右下角X坐标 | 自动采集必选
    int RightBtmY;                                  //R/O |int |右下角Y坐标 | 自动采集必选  
    char MarkTime[GAT1400_DATETIME_TYPE];           //O |dateTime--格式YYYYMMDDhhmmssMMM   |日期时间 |位置标记时间 人工采集有效
    char AppearTime[GAT1400_DATETIME_TYPE];         //O |dateTime--格式YYYYMMDDhhmmssMMM   |日期时间 |车辆出现时间 人工采集有效
    char DisappearTime[GAT1400_DATETIME_TYPE];      //O |dateTime--格式YYYYMMDDhhmmssMMM   |日期时间 |车辆消失时间 人工采集有效
    int LaneNo;                                     //O |int |车道号 |车辆行驶方向最左车道为1，由左向右顺序编号
    bool HasPlate;                                  //R/O |Boolean |有无车牌
    PlateClassType PlateClass;                      //R/O |枚举类型 PlateClassType--int |车牌种类                                
    ColorType PlateColor;                           //R/O |枚举类型 ColorType--int |车牌颜色 |指号牌底色
    char PlateNo[GAT1400_PLATE_NO_TYPE];            //R/O |PlateNoType--string(15) |车牌号
    char PlateNoAttach[GAT1400_PLATE_NO_TYPE];      //O |PlateNoType--string(15) |挂车牌号 |各类机动车挂车号牌编号
    char PlateDescribe[64+1];                       //O |string(64) |车牌描述
    bool IsDecked;                                  //O |boolean |是否套牌
    bool IsAltered;                                 //O |boolean |是否涂改
    bool IsCovered;                                 //O |boolean |是否遮挡
    double Speed;                                   //O |SpeedType--double |行驶速度 |单位千米/每小时
    int Direction;                                  //O |枚举 HDirectionType --int |行驶方向
    int DrivingStatusCode;                          //O |int |行驶状态代码
    int UsingPropertiesCode;                        //O ||车辆使用性质代码
    int VehicleClass;                               //O | 车辆类型
    int VehicleBrand;                               //O |车辆品牌
    std::string VehicleModel;                       //O |车辆型号
    char VehicleStyles[16+1];                       //O string(16) |车辆年款
    int VehicleLength;                              //O int |车辆长度
    int VehicleWidth;                               //O int |车辆宽度
    int VehicleHeight;                              //O int |车辆高度
    ColorType VehicleColor;                         //R 枚举类型 ColorType--int |车辆颜色 |必需字段    
    VehicleColorDepthType VehicleColorDepth;        //O 枚举类型 VehicleColorDepthType--int |颜色深浅
    char VehicleHood[64+1];                         //O string(64) |车前盖
    char VehicleTrunk[64+1];                        //O string(64) |车后盖
    char VehicleWheel[64+1];                        //O string(64) |车轮
    char WheelPrintedPattern[GAT1400_WHEEL_PRINTED_PATTERN_TYPE];//O string(2) |车轮印花纹
    char VehicleWindow[64+1];                       //O string(64) |车窗
    char VehicleRoof[64+1];                         //O string(64) |车顶
    char VehicleDoor[64+1];                         //O string(64) |车门
    char SideOfVehicle[64+1];                       //O stirng(64) |车侧
    char CarOfVehicle[64+1];                        //O string(64) |车厢
    char RearviewMirror[64+1];                      //O string(64) |后视镜
    char VehicleChassis[64+1];                      //O string(64) |地盘
    char VehicleShielding[64+1];                    //O string(64) |遮挡
    std::string FilmColor;                          //O AutoFoilColorType string |车身贴膜颜色
    bool IsModified;                                //O boolean |改装标志
    std::string HitMarkInfo;                        //O DentInfoType string |撞痕信息
    char VehicleBodyDesc[128+1];                    //O string(128) |车身描述
    std::list<int> VehicleFrontItem;                //O 枚举类型 --FrontThingType |车前部物品
    char DescOfFrontItem[256+1];                    //O string(256) |车前部描述
    std::list<int> VehicleRearItem;                 //O 枚举类型--RearThingType |车后部物品
    char DescOfRearItem[256+1];                     //O string(256) |车后部描述
    int NumOfPassenger;                             //O int |车内人数
    char PassTime[GAT1400_DATETIME_TYPE];           //O dataTime |经过时刻 |卡口事件有效
    char NameOfPassedRoad[64+1];                    //O stirng(64) |经过道路名称
    bool IsSuspicious;                              //O boolean |是否可疑车
    int Sunvisor;                                   //O int |遮阳板状态 |(0收起， 1放下)
    int SafetyBelt;                                 //O int |安全带状态 |(0未系， 1有系)
    int Calling;                                    //O int |打电话状态 |(0未打电话， 1打电话中)
    char PlatgeReliablity[3+1];                     //O string(3) |号码识别可信度 |(0~100 可信度，数值越大可信度越高)    
    char PlateCharReliability[64+1];                //O string(64) |每位车牌号码可信度 |(苏 B12345取值为苏-80，B-90,1-90,2-88,3-90,4-65,5-87)
    char BrandReliability[3+1];                     //O string(3) |品牌标识可信度 |(0~100 可信度，数值越大可信度越高)
    std::list<GAT_1400_SubImage> SubImageList;                  

	tagGat1400Motor()
	{
		memset(MotorVehicleID, 0, sizeof(MotorVehicleID));
		InfoKind = 0;
		memset(SourceID, 0, sizeof(SourceID));
		memset(TollgateID, 0, sizeof(TollgateID));
		memset(DeviceID, 0, sizeof(DeviceID));
		memset(StorageUrl1, 0, sizeof(StorageUrl1));
		memset(StorageUrl2, 0, sizeof(StorageUrl2));
		memset(StorageUrl3, 0, sizeof(StorageUrl3));
		memset(StorageUrl4, 0, sizeof(StorageUrl4));
		memset(StorageUrl5, 0, sizeof(StorageUrl5));
		LeftTopX = 0;
		LeftTopY = 0;
		RightBtmX = 0;
		RightBtmY = 0;
		memset(MarkTime, 0, sizeof(MarkTime));
		memset(AppearTime, 0, sizeof(AppearTime));
		memset(DisappearTime, 0, sizeof(DisappearTime));
		LaneNo = 0;
		HasPlate = false;
		PlateClass = PLATE_LARGE_CAR;
		PlateColor = COLOR_BLACK;
		memset(PlateNo, 0, sizeof(PlateNo));
		memset(PlateNoAttach, 0, sizeof(PlateNoAttach));
		memset(PlateDescribe, 0, sizeof(PlateDescribe));
		IsDecked = false;
		IsAltered = false;
		IsCovered = false;
		Speed = 0.0f;
		Direction = 0;
		DrivingStatusCode = 0;
		UsingPropertiesCode = 0;
		VehicleClass = 0;
		VehicleBrand = 0;
		memset(VehicleStyles, 0, sizeof(VehicleStyles));
		VehicleLength = 0;
		VehicleWidth = 0;
		VehicleHeight = 0;
		VehicleColor = COLOR_BLACK;
		VehicleColorDepth = VEHICLE_COLOR_DEEP;
		memset(VehicleHood, 0, sizeof(VehicleHood));
		memset(VehicleTrunk, 0, sizeof(VehicleTrunk));
		memset(VehicleWheel, 0, sizeof(VehicleWheel));
		memset(WheelPrintedPattern, 0, sizeof(WheelPrintedPattern));
		memset(VehicleWindow, 0, sizeof(VehicleWindow));
		memset(VehicleRoof, 0, sizeof(VehicleRoof));
		memset(VehicleDoor, 0, sizeof(VehicleDoor));
		memset(SideOfVehicle, 0, sizeof(SideOfVehicle));
		memset(CarOfVehicle, 0, sizeof(CarOfVehicle));
		memset(RearviewMirror, 0, sizeof(RearviewMirror));
		memset(VehicleChassis, 0, sizeof(VehicleChassis));
		memset(VehicleShielding, 0, sizeof(VehicleShielding));
		IsModified = false;
		memset(VehicleBodyDesc, 0, sizeof(VehicleBodyDesc));
		memset(DescOfFrontItem, 0, sizeof(DescOfFrontItem));
		memset(DescOfRearItem, 0, sizeof(DescOfRearItem));
		NumOfPassenger = 0;
		memset(PassTime, 0, sizeof(PassTime));
		memset(NameOfPassedRoad, 0, sizeof(NameOfPassedRoad));
		IsSuspicious = false;
		Sunvisor = 0;
		SafetyBelt = 0;
		Calling = 0;
		memset(PlatgeReliablity, 0, sizeof(PlatgeReliablity));
		memset(PlateCharReliability, 0, sizeof(PlateCharReliability));
		memset(BrandReliability, 0, sizeof(BrandReliability));
	}
}GAT_1400_Motor;

//非机动车对象
typedef struct tagGat1400NonMotor
{
    char NonMotorVehicleID[GAT1400_IMAGE_CNT_OBJECT_ID_TYPE]; //R |ImageCntObjectIDtype--string(48) 固定长度48|车辆标识 
    int InfoKind;                                   //R |枚举类型 InfoType--int |信息分类 |自动采集/人工采集
    char SourceID[GAT1400_BASIC_OBJECT_ID_TYPE];    //R |BasicObjectIDType--string(41) |来源标识 |来源图像信息标识 
    char DeviceID[GAT1400_DEVICE_ID_TYPE];          //R/O |DeviceIDType--string(20) |设备编码 |采集设备ID 自动采集必选
    int LeftTopX;                                   //R/O |int |左上角X坐标 | 自动采集必选
    int LeftTopY;                                   //R/O |int |左上角Y坐标 | 自动采集必选
    int RightBtmX;                                  //R/O |int |右下角X坐标 | 自动采集必选
    int RightBtmY;                                  //R/O |int |右下角Y坐标 | 自动采集必选  
    char MarkTime[GAT1400_DATETIME_TYPE];           //O |dateTime--格式YYYYMMDDhhmmssMMM   |日期时间 |位置标记时间 人工采集有效
    char AppearTime[GAT1400_DATETIME_TYPE];         //O |dateTime--格式YYYYMMDDhhmmssMMM   |日期时间 |车辆出现时间 人工采集有效
    char DisappearTime[GAT1400_DATETIME_TYPE];      //O |dateTime--格式YYYYMMDDhhmmssMMM   |日期时间 |车辆消失时间 人工采集有效
    bool HasPlate;                                  //R |Boolean |有无车牌
    int PlateClass;                                 //R |枚举类型 PlateClassType--int |车牌种类                                
    int PlateColor;                                 //R |枚举类型 ColorType--int |车牌颜色 |指号牌底色
    char PlateNo[GAT1400_PLATE_NO_TYPE];            //R |PlateNoType--string(15) |车牌号
    char PlateNoAttach[GAT1400_PLATE_NO_TYPE];      //O |PlateNoType--string(15) |挂车牌号 |各类机动车挂车号牌编号
    char PlateDescribe[64+1];                       //O |string(64) |车牌描述
    bool IsDecked;                                  //O |boolean |是否套牌
    bool IsAltered;                                 //O |boolean |是否涂改
    bool IsCovered;                                 //O |boolean |是否遮挡
    double Speed;                                   //O |SpeedType--double |行驶速度 |单位千米/每小时
    int DrivingStatusCode;                          //O |int |行驶状态代码
    int UsingPropertiesCode;                        //O ||车辆使用性质代码
    char VehicleBrand[32];                          //O |车辆品牌
    char VehicleType[64];                           //O |车辆型号
    int VehicleLength;                              //O int |车辆长度
    int VehicleWidth;                               //O int |车辆宽度
    int VehicleHeight;                              //O int |车辆高度
    int VehicleColor;                               //R 枚举类型 ColorType--int |车辆颜色 |必需字段    
    char VehicleHood[64+1];                         //O string(64) |车前盖
    char VehicleTrunk[64+1];                        //O string(64) |车后盖
    char VehicleWheel[64+1];                        //O string(64) |车轮
    char WheelPrintedPattern[GAT1400_WHEEL_PRINTED_PATTERN_TYPE];          //O string(2) |车轮印花纹
    char VehicleWindow[64+1];                       //O string(64) |车窗
    char VehicleRoof[64+1];                         //O string(64) |车顶
    char VehicleDoor[64+1];                         //O string(64) |车门
    char SideOfVehicle[64+1];                       //O stirng(64) |车侧
    char CarOfVehicle[64+1];                        //O string(64) |车厢
    char RearviewMirror[64+1];                      //O string(64) |后视镜
    char VehicleChassis[64+1];                      //O string(64) |地盘
    char VehicleShielding[64+1];                    //O string(64) |遮挡
    int FilmColor;                                  //O int |车身贴膜颜色(0、深色, 1、浅色, 2、无)
    int IsModified;                                 //O boolean |改装标志
    std::list<GAT_1400_SubImage> SubImageList;  

	tagGat1400NonMotor()
	{
		memset(NonMotorVehicleID, 0, sizeof(NonMotorVehicleID));
		InfoKind = 0;
		memset(SourceID, 0, sizeof(SourceID));
		memset(DeviceID, 0, sizeof(DeviceID));
		LeftTopX = 0;
		LeftTopY = 0;
		RightBtmX = 0;
		RightBtmY = 0;
		memset(MarkTime, 0, sizeof(MarkTime));
		memset(AppearTime, 0, sizeof(AppearTime));
		memset(DisappearTime, 0, sizeof(DisappearTime));
		HasPlate = false;
		PlateClass = 0;
		PlateColor = 0;
		memset(PlateNo, 0, sizeof(PlateNo));
		memset(PlateNoAttach, 0, sizeof(PlateNoAttach));
		memset(PlateDescribe, 0, sizeof(PlateDescribe));
		IsDecked = false;
		IsAltered = false;
		IsCovered = false;
		Speed = 0.0f;
		DrivingStatusCode = 0;
		UsingPropertiesCode = 0;
		memset(VehicleBrand, 0, sizeof(VehicleBrand));
		memset(VehicleType, 0, sizeof(VehicleType));
		VehicleLength = 0;
		VehicleWidth = 0;
		VehicleHeight = 0;
		VehicleColor = 0;
		memset(VehicleHood, 0, sizeof(VehicleHood));
		memset(VehicleTrunk, 0, sizeof(VehicleTrunk));
		memset(VehicleWheel, 0, sizeof(VehicleWheel));
		memset(WheelPrintedPattern, 0, sizeof(WheelPrintedPattern));
		memset(VehicleWindow, 0, sizeof(VehicleWindow));
		memset(VehicleRoof, 0, sizeof(VehicleRoof));
		memset(VehicleDoor, 0, sizeof(VehicleDoor));
		memset(SideOfVehicle, 0, sizeof(SideOfVehicle));
		memset(CarOfVehicle, 0, sizeof(CarOfVehicle));
		memset(RearviewMirror, 0, sizeof(RearviewMirror));
		memset(VehicleChassis, 0, sizeof(VehicleChassis));
		memset(VehicleShielding, 0, sizeof(VehicleShielding));
		FilmColor = 0;
		IsModified = 0;
	}
}GAT_1400_NonMotor;

//文件对象
typedef struct tagGat1400File
{
    char FileID[GAT1400_BASIC_OBJECT_ID_TYPE];      //R |BasicObiectIDType--string(41) 固定长度41|图像标识
    int InfoKind;                                   //R |枚举类型 InfoType--int |信息分类 |自动采集/人工采集 视频图像信息类型
    int Source;                                     //R |枚举类型 DataSourceType--int |图像来源，（需要从string转换为int）
    char FileName[GAT1400_PATH_TYPE];
    char StoragePath[GAT1400_PATH_TYPE];            //R/O |string |存储路径
    char FileHash[32];                              //R |string |文件哈希值 |采用MD5加密
    char FileFormat[32];                            //R |string |文件格式 |
    char Title[128];                                //R |string |题名
    int SecurityLevel;                              //R |枚举类型 SecurityLevleType |密级代码 |自动采集为5
    char SubmiterName[GAT1400_NAME_TYPE];           //R/O |NameType--string(50) |入库人 
    char SubmiterOrg[GAT1400_ORG_TYPE];	            //R/O |OrgType--string(100) |入库单位名称 
    char EntryTime[GAT1400_DATETIME_TYPE];          //R/O |dataTime| 保温不需要此字段，视图库自动生成
    int FileSize;                                   //O |int |文件大小 | 单位byte
    //std::string Data;                               //O |string|图像数据

	tagGat1400File()
	{
		memset(FileID, 0, sizeof(FileID));
		InfoKind = 0;
		Source = 0;
		memset(FileName, 0, sizeof(FileName));
		memset(StoragePath, 0, sizeof(StoragePath));
		memset(FileHash, 0, sizeof(FileHash));
		memset(FileFormat, 0, sizeof(FileFormat));
		memset(Title, 0, sizeof(Title));
		SecurityLevel = 0;
		memset(SubmiterName, 0, sizeof(SubmiterName));
		memset(SubmiterOrg, 0, sizeof(SubmiterOrg));
		memset(EntryTime, 0, sizeof(EntryTime));
		FileSize = 0;
	}
}GAT_1400_File;

//视频片段对象
typedef struct tagGat1400VideoSliceInfo
{
    char VideoID[GAT1400_BASIC_OBJECT_ID_TYPE];      //R/O |BasicObjectIDType--string(41) 固定长度48|视频表示 |人工采集必填
    int InfoKind;                                    //R |枚举类型 InfoType--int |信息分类 |自动采集/人工采集
    std::string VideoSource;                         //R |枚举类型 DataSourceType--string |视频来源
    char IsAbstranceVideo[2];                        //O |Boolean--string[2] |摘要视频标志
    char OriginVideoID[GAT1400_BASIC_OBJECT_ID_TYPE];//R/O |BasicObjectIDType--string(41) 固定长度48|原始视频ID |原始视频ID/原始视频URL两项必选其一
    std::string OriginVideoURL;	                     //R/O |string |原始视频URL
    int  EventSort;                                  //R/O |EventType--int |事件分类|自动分析事件类型，设备采集必选
    char DeviceID[GAT1400_DEVICE_ID_TYPE];           //O |DeviceIDType--string(20) |设备编码 |采集设备ID
    char StoragePath[GAT1400_PATH_TYPE];             //O |存储路径 |视频文件的存储路径，采用URI命名规则
    char ThumbnailStoragePath[GAT1400_PATH_TYPE];    //O |缩略图存储路径
    char FileHash[GAT1400_FILE_HASH_TYPE];           //O |视频文件哈希值 |采用MD5算法
    char FileFormat[GAT1400_VIDEO_FORMAT_TYPE];      //R |VideoFormatType--string(6) |视频文件格式	
    char CodedFormat[GAT1400_VIDEO_CODE_FORMAT_TYPE];//R |VideoCodeFormatType--string |视频编码格式
    int AudioFlag;                                   //R |int |音频标志 |0：无音频、1：含音频
    char AudioCodedFormat[GAT1400_AUDIO_CODE_FORMAT_TYPE];//O |AudioCodeFormatType--string |音频编码格式
    char Title[128+1];                  //R |题名 |图像资料名称的汉语描述
    char TitleNote[128+1];              //O |题名补充 |题名的补充和备注信息
    char SpecialName[128+1];            //O |专项名 |图像资料所属的专项名称
    char Keyword[GAT1400_KEYWORD_TYPE]; //O |KeywordType--string(0..200) |关键字 |能够正确表述图像资料主要内容、具有检索意义的词
    char ContentDescription[1024+1];    //R |内容描述 |对图像内容的简要描述
    char MainCharacter[256+1];          //O |主题人物 |图像资料中出现的主要人物的中文姓名全称，有多个用英文半角符号;分隔
    char ShotPlaceCode[GAT1400_PLACE_CODE_TYPE];    //R |PlaceCodeType--string(6) |拍摄地点行政区划代码 |人工采集图像需要
    char ShotPlaceFullAdress[GAT1400_PLACE_FULL_ADDRESS_TYPE];//R |PlaceFullAddressType--string(100) |拍摄地点区划内详细地址 |具体到街道门牌号
    double ShotPlaceLongitude;                      //O |LongitudeType--double |拍摄地点经度
    double ShotPlaceLatitude;                       //O |LatitudeType--double |拍摄地点纬度
    std::string HorizontalShotDirection;            //O |HorizontalShotType --string |水平拍摄方向
    std::string VerticalShotDirection;              //O |VerticalShotType --string |垂直拍摄方向
    int SecurityLevel;                              //R/O |枚举类型 SecurityLevleType |密级代码 |自动采集为5
    double VideoLen;	                            //R |double |视频长度 |单位为秒                       
    char BeginTime[GAT1400_DATETIME_TYPE];          //R |视频标识开始时间
    char EndTime[GAT1400_DATETIME_TYPE];            //R |视频标识结束时间
    int TimeErr;                                    //R |视频标识时间减去实际北京时间的值，单位为秒
    int Width;                                      //R |int |宽度 |水平像素值
    int Height;                                     //R |int |高度 |垂直像素值
    int QualityGrade;                               //O |枚举类型 QualityGradeType--int |质量等级
    char CollectorName[GAT1400_NAME_TYPE];          //R/O |NameType--string(50) |采集人 |采集人名称或者采集系统名称，人工采集必选
    char CollectorOrg[GAT1400_ORG_TYPE];            //R/O |OrgType--string(100) |采集单位名称 |人工采集必选
    int CollectorIDType;                            //O |枚举类型 IDType--int |采集人证件类型 |
    char CollectorID[GAT1400_ID_NUMBER_TYPE];       //O |IDNumberType--string(30) |采集人证件号码 |
    char EntryClerk[GAT1400_NAME_TYPE];             //R/O |NameType--string(50) |入库人 |入库人名称或者入库系统名称，人工采集必选
    char EntryClerkOrg[GAT1400_ORG_TYPE];           //R/O |OrgType--string(100) |入库单位名称 |人工采集必选
    int EntryClerkIDType;                           //O |枚举类型 IDType--int |入库人证件类型 |
    char EntryClerkID[GAT1400_ID_NUMBER_TYPE];      //O |IDNumberType--string(30) |入库人证件号码 |
    char EntryTime[GAT1400_DATETIME_TYPE];          //R/O |dataTime| 保温不需要此字段，视图库自动生成
    int VideoProcFlag;                              //O |int |图像处理标志 | 0：未处理， 1：图像经过处理
    int FileSize;                                   //O |int |图像文件大小 | 单位byte  
	//std::string Data;

	tagGat1400VideoSliceInfo()
	{
		memset(VideoID, 0, sizeof(VideoID));
		InfoKind = 0;
		memset(IsAbstranceVideo, 0, sizeof(IsAbstranceVideo));
		memset(OriginVideoID, 0, sizeof(OriginVideoID));
		EventSort = 0;
		memset(DeviceID, 0, sizeof(DeviceID));
		memset(StoragePath, 0, sizeof(StoragePath));
		memset(ThumbnailStoragePath, 0, sizeof(ThumbnailStoragePath));
		memset(FileHash, 0, sizeof(FileHash));
		memset(FileFormat, 0, sizeof(FileFormat));
		memset(CodedFormat, 0, sizeof(CodedFormat));
		AudioFlag = 0;
		memset(AudioCodedFormat, 0, sizeof(AudioCodedFormat));
		memset(Title, 0, sizeof(Title));
		memset(TitleNote, 0, sizeof(TitleNote));
		memset(SpecialName, 0, sizeof(SpecialName));
		memset(Keyword, 0, sizeof(Keyword));
		memset(ContentDescription, 0, sizeof(ContentDescription));
		memset(MainCharacter, 0, sizeof(MainCharacter));
		memset(ShotPlaceCode, 0, sizeof(ShotPlaceCode));
		memset(ShotPlaceFullAdress, 0, sizeof(ShotPlaceFullAdress));
		ShotPlaceLongitude = 0.0f;
		ShotPlaceLatitude = 0.0f;
		SecurityLevel = 0;
		VideoLen = 0.0f;
		memset(BeginTime, 0, sizeof(BeginTime));
		memset(EndTime, 0, sizeof(EndTime));
		TimeErr = 0;
		Width = 0;
		Height = 0;
		QualityGrade = 0;
		memset(CollectorName, 0, sizeof(CollectorName));
		memset(CollectorOrg, 0, sizeof(CollectorOrg));
		CollectorIDType = 0;
		memset(CollectorID, 0, sizeof(CollectorID));
		memset(EntryClerk, 0, sizeof(EntryClerk));
		memset(EntryClerkOrg, 0, sizeof(EntryClerkOrg));
		EntryClerkIDType = 0;
		memset(EntryClerkID, 0, sizeof(EntryClerkID));
		memset(EntryTime, 0, sizeof(EntryTime));
		VideoProcFlag = 0;
		FileSize = 0;
	}
}GAT_1400_VideoSliceInfo;

//物品对象
typedef struct tagGat1400Thing
{
    char ThingID[GAT1400_IMAGE_CNT_OBJECT_ID_TYPE]; //R |ImageCntObjectIDtype--string(48) 固定长度48|车辆标识 
    int InfoKind;                                   //R |枚举类型 InfoType--int |信息分类 |自动采集/人工采集
    char SourceID[GAT1400_BASIC_OBJECT_ID_TYPE];    //R |BasicObjectIDType--string(41) |来源标识 |来源图像信息标识 
    char DeviceID[GAT1400_DEVICE_ID_TYPE];          //R/O |DeviceIDType--string(20) |设备编码 |采集设备ID 自动采集必选
    int LeftTopX;                                   //R/O |int |左上角X坐标 | 自动采集必选
    int LeftTopY;                                   //R/O |int |左上角Y坐标 | 自动采集必选
    int RightBtmX;                                  //R/O |int |右下角X坐标 | 自动采集必选
    int RightBtmY;                                  //R/O |int |右下角Y坐标 | 自动采集必选  
    char LocationMarkTime[GAT1400_DATETIME_TYPE];   //O |dateTime--格式YYYYMMDDhhmmssMMM   |日期时间 |位置标记时间 人工采集有效
    char AppearTime[GAT1400_DATETIME_TYPE];         //O |dateTime--格式YYYYMMDDhhmmssMMM   |日期时间 |车辆出现时间 人工采集有效
    char DisAppearTime[GAT1400_DATETIME_TYPE];      //O |dateTime--格式YYYYMMDDhhmmssMMM   |日期时间 |车辆消失时间 人工采集有效
    std::string Name;                               //O |string |被标注物品名称
    std::string Shape;                              //O |string |被标注物品形状
    std::string Color;                              //O |ColorType |被标注物品颜色
    std::string Size;                               //O |string |被标注物品大小
    std::string Materiacl;                          //O |string |被标注物品材质
    std::string Characteristic;                     //O |string |被标注物品特征
    char Propertiy[GAT1400_THING_PROPERTY_TYPE];    //O |ThingPropertyType--string(2)|物品性质
    char InvolvedObjType[GAT1400_INVOLVED_OBJ_TYPE];//O |InvolvedObjType--string(5) |涉案物品类型
    char FirearmsAmmunitionType[GAT1400_FIREARMS_AMMUNITION_TYPE];//O |FirearmsAmmunitionType--string(2) |枪支弹药类别
    char ToolTranceType[GAT1400_TOOL_TRACE_TYPE];   //O |ToolTraceType--string(2) |工具痕迹代码
    char EvidenceType[GAT1400_EVIDENCE_TYPE];       //O |EvidenceType--string(5) |物品类型
    char CaseEvidenceType[GAT1400_CASE_EVIDENCE_TYPE];//O |CaseEvidenceType--string(2) |案件物证
	std::list<GAT_1400_SubImage> SubImageList; 

	tagGat1400Thing()
	{
		memset(ThingID, 0, sizeof(ThingID));
		InfoKind = 0;
		memset(SourceID, 0, sizeof(SourceID));
		memset(DeviceID, 0, sizeof(DeviceID));
		LeftTopX = 0;
		LeftTopY = 0;
		RightBtmX = 0;
		RightBtmY = 0;
		memset(LocationMarkTime, 0, sizeof(LocationMarkTime));
		memset(AppearTime, 0, sizeof(AppearTime));
		memset(DisAppearTime, 0, sizeof(DisAppearTime));
		memset(Propertiy, 0, sizeof(Propertiy));
		memset(InvolvedObjType, 0, sizeof(InvolvedObjType));
		memset(FirearmsAmmunitionType, 0, sizeof(FirearmsAmmunitionType));
		memset(ToolTranceType, 0, sizeof(ToolTranceType));
		memset(EvidenceType, 0, sizeof(EvidenceType));
		memset(CaseEvidenceType, 0, sizeof(CaseEvidenceType));
	}
}GAT_1400_Thing;

//场景对象
typedef struct tagGat1400Scene
{
    char SceneID[GAT1400_IMAGE_CNT_OBJECT_ID_TYPE]; //R |ImageCntObjectIDtype--string(48) 固定长度48|车辆标识 
    int InfoKind;                                   //R |枚举类型 InfoType--int |信息分类 |自动采集/人工采集
    char SourceID[GAT1400_BASIC_OBJECT_ID_TYPE];    //R |BasicObjectIDType--string(41) |来源标识 |来源图像信息标识 
    char DeviceID[GAT1400_DEVICE_ID_TYPE];          //R/O |DeviceIDType--string(20) |设备编码 |采集设备ID 自动采集必选					
    char BeginTime[GAT1400_DATETIME_TYPE];		    //O	|出现时间	
    char PlaceType[GAT1400_PLACE_TYPE];             //O	|PlaceType--string(4)|选择处所代码		
    char WeatherType[GAT1400_WEATHER_TYPE];         //O |WeatherType--string(2) |天气情况
    char SceneDescribe[256+1];			            //O |场景描述
    char SceneType[GAT1400_SCENE_TYPE];             //O |SceneType--string(2) |道路类型代码
    char RoadAlignmentType[GAT1400_ROAD_ALIGNMENT_TYPE];//O |RoadAlignmentType--string(2)|道路路线代码
    int  RoadTerraintype;                           //O |道路地形分类
    char RoadSurfaceType[GAT1400_ROAD_SURFACE_TYPE];//O |RoadSurfaceType--string(1)|道路路面类型代码
    char RoadCoditionType[GAT1400_ROAD_CONDITION_TYPE];//O |RoadConditionType--string(1)|道路路面状况代码
    char RoadJunctionSectionType[GAT1400_ROAD_JUNCTION_SECTION_TYPE];//O |RoadJunctionSectionType--string(2)|道路路口路段类型代码
    char RoadLightingType[GAT1400_ROAD_LIGHTING_TYPE];//O |RoadLightingType--string(1)|道路照明代码
    char Illustration[GAT1400_ILLUSTRATION_TYPE];   //O |Illustration--string(2)|现场图示
    char WindDirection[GAT1400_WIND_DIRECTION_TYPE];//O |WindDirectionType--string(2)|现场风向
    std::string Illumination;                       //O |IlluminationType--string|现场光线
    std::string FieldCondition;                     //O |FieldConditionType--string|现场条件
    double Temperature;                             //O |double |现场温度
    std::string Humidity;                           //O |HumidityType--string |现场湿度
    std::string PopulationDensity;                  //O |DenseDegreeType--string|人群密集程度
    std::string DenseDegree;                        //O |DenseDegreeType--string|物品密度
    int Importance;                                 //O |场景重要程度 |1~5数值越大表示越重要
	std::list<GAT_1400_SubImage> SubImageList; 

	tagGat1400Scene()
	{
		memset(SceneID, 0, sizeof(SceneID));
		InfoKind = 0;
		memset(SourceID, 0, sizeof(SourceID));
		memset(DeviceID, 0, sizeof(DeviceID));
		memset(BeginTime, 0, sizeof(BeginTime));
		memset(PlaceType, 0, sizeof(PlaceType));
		memset(WeatherType, 0, sizeof(WeatherType));
		memset(SceneDescribe, 0, sizeof(SceneDescribe));
		memset(SceneType, 0, sizeof(SceneType));
		memset(RoadAlignmentType, 0, sizeof(RoadAlignmentType));
		memset(RoadSurfaceType, 0, sizeof(RoadSurfaceType));
		memset(RoadCoditionType, 0, sizeof(RoadCoditionType));
		memset(RoadJunctionSectionType, 0, sizeof(RoadJunctionSectionType));
		memset(RoadLightingType, 0, sizeof(RoadLightingType));
		memset(Illustration, 0, sizeof(Illustration));
		memset(WindDirection, 0, sizeof(WindDirection));
		Temperature = 0.0f;
		Importance = 0;
	}
}GAT_1400_Scene;

//视频片段集合对象
typedef struct tagGat1400VideoSliceSet
{
	GAT_1400_VideoSliceInfo VideoSliceInfo;
	std::list<GAT_1400_Person> PersonList;
	std::list<GAT_1400_Face> FaceList;
	std::list<GAT_1400_Motor> MotorVehicleList;
	std::list<GAT_1400_NonMotor> NonMotorVehicleList;
	std::list<GAT_1400_Thing> ThingList;
	std::list<GAT_1400_Scene> SceneList;
	std::string Data;
}GAT_1400_VideoSliceSet;

//图像集合对象
typedef struct tagGat1400ImageSet
{
	GAT_1400_Image ImageInfo;
	std::list<GAT_1400_Person> PersonList;
	std::list<GAT_1400_Face> FaceList;
	std::list<GAT_1400_Motor> MotorVehicleList;
	std::list<GAT_1400_NonMotor> NonMotorVehicleList;
	std::list<GAT_1400_Thing> ThingList;
	std::list<GAT_1400_Scene> SceneList;
	std::string Data;
}GAT_1400_ImageSet;

//文件集合对象
typedef struct tagGat1400FileSet
{
	GAT_1400_File FileInfo;
	std::list<GAT_1400_Person> PersonList;
	std::list<GAT_1400_Face> FaceList;
	std::list<GAT_1400_Motor> MotorVehicleList;
	std::list<GAT_1400_NonMotor> NonMotorVehicleList;
	std::list<GAT_1400_Thing> ThingList;
	std::list<GAT_1400_Scene> SceneList;
	std::string Data;
}GAT_1400_FileSet;

//采集设备对象
typedef struct tagGat1400Ape
{
    char ApeID[GAT1400_DEVICE_ID_TYPE];             //R |DeviceIDType--string(20) |设备编码 
    char Name[GAT1400_DEVICE_NAME_TYPE];            //R |DeviceNameType--strign(100) |设备名称
    char Model[GAT1400_MODEL_TYPE];                 //R |ModelType--string(0..100) |型号 |
    char IPAddr[GAT1400_IP_ADDR_TYPE];              //R |IPAddrType |IP地址
    char IPV6Addr[GAT1400_IPV6_ADDR_TYPE];          //O |IPV6AddrType |Ipv6地址
    int Port;                                       //R 网络端口号
    double Longitude;                               //R |LongitudeType--double |拍摄地点经度
    double Latitude;                                //R |LatitudeType--double |拍摄地点纬度
    char PlaceCode[GAT1400_PLACE_CODE_TYPE];        //R |PlaceCodeType--string(6) |安装地点行政区划代码
    char Place[256+1];                              //O |string(256) |位置名
    char OrgCode[GAT1400_ORG_CODE_TYPE];            //O |OrgCodeType |管辖单位代码
    int CapDirection;                               //O |车辆抓拍方向 |0：拍车头、1：拍车尾
    std::string MonitorDirection;                   //O |枚举 HDirectionType --string |行驶方向
    char MonitorAreaDcsc[256+1];                    //O |string(256) |监视区域说明
    StatusType IsOnline;                            //R |是否在线
    char OwnerApsID[GAT1400_DEVICE_ID_TYPE];        //O |DeviceIDType--string(20) |所属采集系统
    char UserId[64+1];                              //O |string(64) |用户账号
    char Password[GAT1400_PASSWORD_TYPE];           //O |PasswordType--string(32) |口令
	char FunctionType[30+1];                        //R |string(30) |功能类型,多选各参数以"/"分隔

	tagGat1400Ape()
	{
		memset(ApeID, 0, sizeof(ApeID));
		memset(Name, 0, sizeof(Name));
		memset(Model, 0, sizeof(Model));
		memset(IPAddr, 0, sizeof(IPAddr));
		memset(IPV6Addr, 0, sizeof(IPV6Addr));
		Port = 0;
		Longitude = 0.0f;
		Latitude = 0.0f;
		memset(PlaceCode, 0, sizeof(PlaceCode));
		memset(Place, 0, sizeof(Place));
		memset(OrgCode, 0, sizeof(OrgCode));
		CapDirection = 0;
		memset(MonitorAreaDcsc, 0, sizeof(MonitorAreaDcsc));
		IsOnline = STATUS_ON_LINE;
		memset(OwnerApsID, 0, sizeof(OwnerApsID));
		memset(UserId, 0, sizeof(UserId));
		memset(Password, 0, sizeof(Password));
		memset(FunctionType, 0, sizeof(FunctionType));
	}
}GAT_1400_Ape;

//采集设备状态
typedef struct tagAPEStatus 
{
	char ApeID[GAT1400_DEVICE_ID_TYPE];//R 设备ID
	StatusType IsOnline;//R 是否在线 1在线，2离线，9其他
	char CurrentTime[GAT1400_DATETIME_TYPE];//R 当前时间

	tagAPEStatus()
	{
		memset(ApeID, 0, sizeof(ApeID));
		IsOnline = STATUS_ON_LINE;
		memset(CurrentTime, 0, sizeof(CurrentTime));
	}
}GAT_1400_Ape_Status;

//视频卡口对象
typedef struct tagGat1400Tollgate
{
    char TollgateID[GAT1400_DEVICE_ID_TYPE];        //R |DeviceIDType--string(20) |卡口id
    char Name[GAT1400_DEVICE_NAME_TYPE];            //R |DeviceNameType--strign(100) |名称
    double Longitude;                               //R |LongitudeType--double |经度
    double Latitude;                                //R |LatitudeType--double |纬度
    char PlaceCode[GAT1400_PLACE_CODE_TYPE];        //R |PlaceCodeType--string(6) |安装地点行政区划代码
    char Place[256];                                //O |位置名
    StatusType Status;                              //R |StatusType--string(1) |1:正常、2：停用、9：其他
    char TollgateCat[GAT1400_TOLLGATE_TYPE];        //R |TollgateType--string(2) |卡口类型 |10：国际、20：省际、30：市际、31：市区、40：县际、41：县区、99 其他
    int TollgateUsage;                              //R |int |卡口用途 |80：治安卡口、81：交通卡口、82其他
    int LaneNum;                                    //O |int |卡口车道数
    char OrgCode[GAT1400_ORG_CODE_TYPE];            //O |OrgCodeType--string(12) |管辖单位代码
    char ActiveTime[GAT1400_DATETIME_TYPE];         //O |dateTime |卡口启用时间

	tagGat1400Tollgate()
	{
		memset(TollgateID, 0, sizeof(TollgateID));
		memset(Name, 0, sizeof(Name));
		Longitude = 0;
		Latitude = 0;
		memset(PlaceCode, 0, sizeof(PlaceCode));
		memset(Place, 0, sizeof(Place));
		Status = STATUS_ON_LINE;
		memset(TollgateCat, 0, sizeof(TollgateCat));
		TollgateUsage = 0;
		LaneNum = 0;
		memset(OrgCode, 0, sizeof(OrgCode));
		memset(ActiveTime, 0, sizeof(ActiveTime));
	}
}GAT_1400_Tollgate;

//车道对象
typedef struct tagGat1400TollgateLane
{
    char TollgateID[GAT1400_DEVICE_ID_TYPE];        //R |DeviceIDType--string(20) |卡口编号
    int LaneID;                                     //R |车道ID
    int LaneNo;                                     //R |车道编号
    char Name[GAT1400_DEVICE_NAME_TYPE];            //R |DeviceNameType--strign(100) |名称
    std::string Direction;                          //O |枚举 HDirectionType --string |行驶方向
    char Desc[256+1];                               //O |string(256) |车道描述
    int MaxSpeed;                                   //O |int |限速（千米/小时）
    int CityPass;                                   //O |int |车道出入城 |1 进城、 2 出城、3 非进出城、4 进出城混合
    char ApeID[GAT1400_DEVICE_ID_TYPE];             //R |DeviceIDType--string(20) |设备ID

	tagGat1400TollgateLane()
	{
		memset(TollgateID, 0, sizeof(TollgateID));
		LaneID = 0;
		LaneNo = 0;
		memset(Name, 0, sizeof(Name));
		memset(Desc, 0, sizeof(Desc));
		MaxSpeed = 0;
		CityPass = 0;
		memset(ApeID, 0, sizeof(ApeID));
	}
}GAT_1400_Tollgate_Lane;

typedef struct tagColorArea 
{
	int AreaPosX;                                   //R/O 主色区域X坐标
	int AreaPosY;                                   //R/O 主色区域Y坐标
	int AreaWidth;                                  //R/O 主色区域宽度
	int AreaHeight;                                 //R/O 主色区域高度

	tagColorArea()
	{
		memset(this, 0, sizeof(tagColorArea));
	}
}GAT_1400_Color_Area;

typedef struct tagTarget 
{
	int PosX;                                       //O 运动目标X坐标
	int PosY;                                       //O 运动目标Y坐标
	int Width;                                      //O 运动目标宽度
	int Height;                                     //O 运动目标高度
	int Status;                                     //O 运动目标状态
	int SpeedVal;                                   //O 运动目标速度
	int SpeedRad;                                   //O 运动目标方向

	tagTarget()
	{
		memset(this, 0, sizeof(tagTarget));
	}
}GAT_1400_Target;

typedef struct tagBehaviorAnalysis 
{
	int EventLevel;                                  //O 报警事件级别
	char BehaviorBeginTime[GAT1400_DATETIME_TYPE];   //R/O |dateTime |行为开始绝对时间
	char BehaviorBeginTimeRlt[GAT1400_DATETIME_TYPE];//R/O |dateTime |行为开始相对时间
	int BehaviorBeginFrameNoRlt;                     //O int |行为开始相对帧号
	char BehaviorEndTime[GAT1400_DATETIME_TYPE];     //R/O |dateTime |行为结束绝对时间
	char BehaviorEndTimeRlt[GAT1400_DATETIME_TYPE];  //R/O |dateTime |行为结束相对时间
	int BehaviorEndFrameNoRlt;                       //O int |行为结束相对帧号
	
	tagBehaviorAnalysis()
	{
		memset(this, 0, sizeof(tagBehaviorAnalysis));
	}
}GAT_1400_Behavior_Analysis;

//视频图像标签对象
typedef struct tagGat1400VideoLabel
{
    char VideoLabelID[GAT1400_IMAGE_CNT_OBJECT_ID_TYPE];    //R |ImageCntObjectIDtype--string(48) 固定长度48|视频图像标签ID
    int EventSort;                                  //R |int |视频图像分析类型
    char EventRuleID[40+1];                         //O |string（40）|视频图像分析规则ID
    char VideoImageID[GAT1400_BASIC_OBJECT_ID_TYPE];//O |BasicObjectIDType--string(41) |来源标识 |来源图像信息标识 
    char VideoImageUrl[URL_LEN];                    //O |string(256) |视频图像来源URL
    char CameraID[GAT1400_DEVICE_ID_TYPE];          //O |DeviceIDType--string(20) |在线视频摄像机ID
    char IVADeviceID[GAT1400_DEVICE_ID_TYPE];       //R |DeviceIDType--string(20) |分析系统ID
    char CreateTimeAbs[GAT1400_DATETIME_TYPE];      //R/O |dateTime |标签生成绝对时间 |在线视频为北京时间，必选
    int CreateTimeRlt;                              //R/O |int |标签生成相对时间 |离线视频是相对时间，必选
    long CreateFrameNoRlt;                          //R/O |long |标签生成相对帧号 |离线视频必选
    char PersonID[512+1];                           //O |string(512) |人员ID
    char FaceID[512+1];                             //O |string(512) |人脸ID
    char MotorVehicleID[512+1];                     //O |string(512) |机动车ID
    char NonMotorVehicleID[512+1];                  //O |string(512) |非机动车ID
    char ThingID[512+1];                            //O |string(512) |物品ID
    char SceneID[512+1];                            //O |string(512) |场景ID
    std::string TargetColor;                        //O |枚举类型--ColorType |目标主色
    int ColorCount;                                 //R/O 目标主色个数
	std::list<GAT_1400_Color_Area> ColorAreaSet;
    int MoveObjectNum;                              //O 运动目标总数
    std::list<GAT_1400_Target> MoveObjectSet;       //O
    GAT_1400_Behavior_Analysis BehaviorAnalysisObject;
    char ImagePath[GAT1400_PATH_TYPE];              //O string(256) |抓拍图像存储路径
    char Desc[256+1];                               //O string(256) 补充描述
    int TargetNum;                                  //O int |目标总数
    int PersonNum;                                  //O int |人员数量
    int FaceNum;                                    //O int |人脸数量
    int VehicleNum;                                 //O int |车辆数量
    int ThingNum;                                   //O int |物品数量
    int TargetDensityAbs;                           //O int |所有目标绝对密度
    int PersonDensityAbs;                           //O int |人员绝对密度
    int FacenDensityAbs;                            //O int |人脸绝对密度
    int VehicleDensityAbs;                          //O int |车辆绝对密度
    int ThingDensityAbs;                            //O int |物品绝对密度
    int TargetDensityRlt;                           //O int |所有目标相对密度
    int PersonDensityRlt;                           //O int |人员相对密度
    int FacenDensityRlt;                            //O int |人脸相对密度
    int VehicleDensityRlt;                          //O int |车辆相对密度
    int ThingDensityRlt;                            //O int |物品相对密度
    int TotalTargetFlowRate;                        //O int |所有目标流量
    int PersonFlowRate;                             //O int |人员流量
    int VehicleFlowRate;                            //O int |车辆流量
    int FlowDirection;                              //O int |流量方向

	tagGat1400VideoLabel()
	{
		memset(VideoLabelID, 0, sizeof(VideoLabelID));
		EventSort = 0;
		memset(EventRuleID, 0, sizeof(EventRuleID));
		memset(VideoImageID, 0, sizeof(VideoImageID));
		memset(VideoImageUrl, 0, sizeof(VideoImageUrl));
		memset(CameraID, 0, sizeof(CameraID));
		memset(IVADeviceID, 0, sizeof(IVADeviceID));
		memset(CreateTimeAbs, 0, sizeof(CreateTimeAbs));
		CreateTimeRlt = 0;
		CreateFrameNoRlt = 0;
		memset(PersonID, 0, sizeof(PersonID));
		memset(FaceID, 0, sizeof(FaceID));
		memset(MotorVehicleID, 0, sizeof(MotorVehicleID));
		memset(NonMotorVehicleID, 0, sizeof(NonMotorVehicleID));
		memset(ThingID, 0, sizeof(ThingID));
		memset(SceneID, 0, sizeof(SceneID));
		ColorCount = 0;
		MoveObjectNum = 0;
		memset(ImagePath, 0, sizeof(ImagePath));
		memset(Desc, 0, sizeof(Desc));
		TargetNum = 0;
		PersonNum = 0;
		FaceNum = 0;
		VehicleNum = 0;
		ThingNum = 0;
		TargetDensityAbs = 0;
		PersonDensityAbs = 0;
		FacenDensityAbs = 0;
		VehicleDensityAbs = 0;
		ThingDensityAbs = 0;
		TargetDensityRlt = 0;
		PersonDensityRlt = 0;
		FacenDensityRlt = 0;
		VehicleDensityRlt = 0;
		ThingDensityRlt = 0;
		TotalTargetFlowRate = 0;
		PersonFlowRate = 0;
		VehicleFlowRate = 0;
		FlowDirection = 0;
	}
}GAT_1400_VideoLabel;

//点位对象
typedef struct tagGat1400Point 
{
	int PointX;                                     //O int |端点X坐标
	int PointY;                                     //O int |端点Y坐标

	tagGat1400Point()
	{
		memset(this, 0, sizeof(tagGat1400Point));
	}
}GAT_1400_Point;

//绊线对象
typedef struct tagGat1400Line 
{
	int Direction;
	std::list<GAT_1400_Point> PointObjectList;
	
	tagGat1400Line()
	{
		Direction = 0;
	}
}GAT_1400_Line;

//方向对象
typedef struct tagDirectionType 
{
	int Direction; //流量方向
	int TargetType;//目标类型 1人员，2人脸，3机动车，4非机动车，5物品，6场景

	tagDirectionType()
	{
		memset(this, 0, sizeof(tagDirectionType));
	}
}GAT_1400_Direction_Type;

//视频图像分析规则对象
typedef struct tagGat1400AnalysisRule
{
    char AnalysisRuleID[40+1];                      //R |string(40) |分析规则ID
    char VideoImageID[GAT1400_BASIC_OBJECT_ID_TYPE];//O |BasicObjectIDType--string(41) |来源标识 |视频来源信息标识
    char VideoImageUrl[URL_LEN];                    //R/O string(256) |视频图像来源URL
    char CameraID[GAT1400_DEVICE_ID_TYPE];          //R/O |DeviceIDType--string(20) |在线视频摄像机ID
    int Width;                                      //R/O int |目标最小宽度 |分析规则取值为X~Y之间必选
    int Height;                                     //R/O int |目标最小高度 |分析规则取值为X~Y之间必选
    int MinDuration;                                //R/O int |行为最小持续时间 |单位为秒
    int LineMinDuration;                            //O int |絆线检测最小时间间隔
    int LineMaxDuration;                            //O int |絆线检测最大时间间隔
    int LineNum;                                    //O int |包含絆线条数
	std::list<GAT_1400_Line> LineSet;
    int PointNum;                                   //O int |包含端点数量
    int PointID;                                    //O int |端点序号
    std::list<GAT_1400_Point> PointSet;
    int Direction;                                  //O int |触发方向
    int ActionType;                                 //O int |入侵行为类型
    int BeginPointX;                                //O int |起点X坐标
    int BeginPointY;                                //O int |起点Y坐标
    int EndPointX;                                  //O int |终点X坐标
    int EndPointY;                                  //O int |终点Y坐标
    int DirectionNum;                               //O int |流量方向个数
	std::list<GAT_1400_Direction_Type> DirectSet;
    int DensityUnit;                                //O int |密度检测数值单位

	tagGat1400AnalysisRule()
	{
		memset(AnalysisRuleID, 0, sizeof(AnalysisRuleID));
		memset(VideoImageID, 0, sizeof(VideoImageID));
		memset(VideoImageUrl, 0, sizeof(VideoImageUrl));
		memset(CameraID, 0, sizeof(CameraID));
		Width = 0;
		Height = 0;
		MinDuration = 0;
		LineMinDuration = 0;
		LineMaxDuration = 0;
		LineNum = 0;
		PointNum = 0;
		PointID = 0;
		Direction=  0;
		ActionType = 0;
		BeginPointX=  0;
		BeginPointY = 0;
		EndPointX = 0;
		EndPointY = 0;
		DirectionNum = 0;
		DensityUnit = 0;
	}
}GAT_1400_AnalysisRule;  

//布控对象
typedef struct tagDisposition 
{
	char DispositionID[GAT1400_BUSINESS_OBJECT_ID_TYPE];  //R|O BusinessObjectIDType 布控标识
	char Title[256+1]; //R|O string(256) |布控主题
	DispositionCategoryType DispositionCategory; //R|O 布控类别
	char TargetFeature[256+1]; //R|O string(256) |布控对象特征
	char TargetImageURI[256+1];//O string(256+1) |布控对象图像
	int PriorityLevel; //O 布控优先级，取值1~3，1表示最高
	char ApplicantName[GAT1400_NAME_TYPE];//R|O NameType |申请人
	char ApplicantInfo[256+1];//O string(256) |申请人联系方式
	char ApplicantOrg[GAT1400_ORG_TYPE]; //R|O OrgType |申请单位
	char BeginTime[GAT1400_DATETIME_TYPE]; //R|O dateTime |开始时间
	char EndTime[GAT1400_DATETIME_TYPE]; //R|O dateTime |结束时间
	char CreateTime[GAT1400_DATETIME_TYPE]; //O dateTime |创建时间
	int OperateType; //R 布控操作类型， 0布控，1撤控
	int DispositionStatus; //R|O 布控执行状态，0布控中，1已撤控，2布控到期，9未布控，该字段只读
	int DispositionRange; //R|O 布控范围，1卡口，2区域布控
	char TollgateList[256+1];//O string(256) |布控卡口，卡口时使用；可带多个卡口ID，多个以“;”间隔
	char DispositionArea[256+1];//O string(256) |布控行政区代码
	char ReceiveAddr[256+1];//O string(256) |告警信息接收地址
	char ReceiveMobile[256+1];// O string(256) |告警信息接收手机号
	char Reason[256+1];//O string(256) |理由
	char DispositionRemoveOrg[GAT1400_ORG_TYPE]; //O OrgType |撤控单位名称
	char DispositionRemovePerson[32+1];//O stirng(32) |撤控人
	char DispositionRemoveTime[GAT1400_DATETIME_TYPE]; //O dateTime |撤控时间
	char DispositionRemoveReason[64+1];// O stirng(64) |撤控原因
	std::list<GAT_1400_SubImage> SubImageList; //O 图像列表

	tagDisposition()
	{
		memset(DispositionID, 0, sizeof(DispositionID));
		memset(Title, 0, sizeof(Title));
		DispositionCategory = DISPOSITION_PERSON;
		memset(TargetFeature, 0, sizeof(TargetFeature));
		memset(TargetImageURI, 0, sizeof(TargetImageURI));
		PriorityLevel = 1;
		memset(ApplicantName, 0, sizeof(ApplicantName));
		memset(ApplicantInfo, 0, sizeof(ApplicantInfo));
		memset(ApplicantOrg, 0, sizeof(ApplicantOrg));
		memset(BeginTime, 0, sizeof(BeginTime));
		memset(EndTime, 0, sizeof(EndTime));
		memset(CreateTime, 0, sizeof(CreateTime));
		OperateType = 0;
		DispositionStatus = 0;
		DispositionRange = 1;
		memset(TollgateList, 0, sizeof(TollgateList));
		memset(DispositionArea, 0, sizeof(DispositionArea));
		memset(ReceiveAddr, 0, sizeof(ReceiveAddr));
		memset(ReceiveMobile, 0, sizeof(ReceiveMobile));
		memset(Reason, 0, sizeof(Reason));
		memset(DispositionRemoveOrg, 0, sizeof(DispositionRemoveOrg));
		memset(DispositionRemovePerson, 0, sizeof(DispositionRemovePerson));
		memset(DispositionRemoveTime, 0, sizeof(DispositionRemoveTime));
		memset(DispositionRemoveReason, 0, sizeof(DispositionRemoveReason));
	}
}GAT_1400_Disposition;

//告警对象
typedef struct tagDispositionNotification 
{
	char NotificationID[GAT1400_BUSINESS_OBJECT_ID_TYPE]; //R BusinessObjectIDType |告警标识
	char DispositionID[GAT1400_BUSINESS_OBJECT_ID_TYPE]; //R BusinessObjectIDType |布控标识
	char Title[256+1];//R string(256) |布控标题
	char TriggerTime[GAT1400_DATETIME_TYPE];//R dateTime |触发时间
	char CntObjectID[GAT1400_IMAGE_CNT_OBJECT_ID_TYPE];//R ImageCntObjectIDType |信息标识
	GAT_1400_Person PersonObject; //O 人员
	GAT_1400_Motor MotorVehicleObject;//R|O 车辆

	tagDispositionNotification()
	{
		memset(NotificationID, 0, sizeof(NotificationID));
		memset(DispositionID, 0, sizeof(DispositionID));
		memset(Title, 0, sizeof(Title));
		memset(TriggerTime, 0, sizeof(TriggerTime));
		memset(CntObjectID, 0, sizeof(CntObjectID));
	}

}GAT_1400_Disposition_Notification;

//订阅对象
typedef struct tagSubscribe 
{
	char SubscribeID[GAT1400_BUSINESS_OBJECT_ID_TYPE]; //R|O BusinessObjectIDType |订阅标识符
	char Title[256+1]; //R|O string(256) |订阅标题
	std::string SubscribeDetail; //R|O 订阅类别,可同时带多个类型，用逗号分隔
	char ResourceURI[URL_LEN]; //R|O string(256) |订阅资源路径
	char ApplicantName[GAT1400_NAME_TYPE];//R|O NameType |申请人
	char ApplicantOrg[GAT1400_ORG_TYPE]; //R|O OrgType |申请单位
	char BeginTime[GAT1400_DATETIME_TYPE]; //R|O dateTime |开始时间
	char EndTime[GAT1400_DATETIME_TYPE]; //R|O dateTime |结束时间
	char ReceiveAddr[256+1]; //O string(256) |信息接收地址
	int ReportInterval; //O 信息上报间隔 单位秒
	char Reason[256+1]; //O string(256) |理由
	int OperateType; //R 操作类型，0订阅，1取消订阅
	int SubscribeStatus; //R|O 订阅执行状态，0订阅中，1已取消订阅，2订阅到期，9未订阅，该字段只读
	char SubscribeCancelOrg[GAT1400_ORG_TYPE]; //O OrgType |订阅取消单位
	char SubscribeCancelPerson[32+1];//O string(32) |订阅取消人
	char CancelTime[GAT1400_DATETIME_TYPE]; //O dateTime |取消时间
	char CancelReason[64+1]; //O string(64) |取消原因

	tagSubscribe()
	{
		memset(SubscribeID, 0, sizeof(SubscribeID));
		memset(Title, 0, sizeof(Title));
		memset(ResourceURI, 0, sizeof(ResourceURI));
		memset(ApplicantName, 0, sizeof(ApplicantName));
		memset(ApplicantOrg, 0, sizeof(ApplicantOrg));
		memset(BeginTime, 0, sizeof(BeginTime));
		memset(EndTime, 0, sizeof(EndTime));
		memset(ReceiveAddr, 0, sizeof(ReceiveAddr));
		ReportInterval = 0;
		memset(Reason, 0, sizeof(Reason));
		OperateType = 0;
		SubscribeStatus = 0;
		memset(SubscribeCancelOrg, 0, sizeof(SubscribeCancelOrg));
		memset(SubscribeCancelPerson, 0, sizeof(SubscribeCancelPerson));
		memset(CancelTime, 0, sizeof(CancelTime));
		memset(CancelReason, 0, sizeof(CancelReason));
	}
}GAT_1400_Subscribe;

//通知对象
typedef struct tagSubscribeNotification 
{
	char NotificationID[GAT1400_BUSINESS_OBJECT_ID_TYPE];//R BusinessObjectIDType |通知标识
	char SubscribeID[GAT1400_BUSINESS_OBJECT_ID_TYPE];//R BusinessObjectIDType |订阅标识
	char Title[256+1];//R strng(256) |订阅标题
	char TriggerTime[GAT1400_DATETIME_TYPE];//R dateTime触发时间
	std::string InfoIDs;//R 信息标识
	//视频案件暂时没有
	std::list<GAT_1400_Tollgate> TollgateObjectList;//视频卡口
	std::list<GAT_1400_Tollgate_Lane> LaneObjectList;//车道
	std::list<GAT_1400_Ape> DeviceList; //设备
	std::list<GAT_1400_Ape_Status> DeviceStatusList;//设备状态
	//采集系统暂时没有
	std::list<GAT_1400_Person> PersonObjectList;//人员信息
	std::list<GAT_1400_Face> FaceObjectList;//人脸信息
	std::list<GAT_1400_Motor> MotorVehicleObjectList;//机动车信息
	std::list<GAT_1400_NonMotor> NonMotorVehicleObjectList;//非机动车信息
	std::list<GAT_1400_Thing> ThingObjectList;//物品信息
	std::list<GAT_1400_Scene> SceneObjectList;//场景信息

	tagSubscribeNotification()
	{
		memset(NotificationID, 0, sizeof(NotificationID));
		memset(SubscribeID, 0, sizeof(SubscribeID));
		memset(Title, 0, sizeof(Title));
		memset(TriggerTime, 0, sizeof(TriggerTime));
	}
}GAT_1400_Subscribe_Notification;

//视频案事件对象
typedef struct tagCase
{
	char CaseID[GAT1400_CASE_OBJECT_ID_TYPE];//R CaseObjectIDType |案事件标识
	char CaseLinkMark[GAT1400_CASE_LINK_MARK_TYPE];//O CaseLinkMarkType |案件关联标识
	char CaseName[GAT1400_CASE_NAME_TYPE];//R CaseNameType |案件名称
	char CaseAbstract[GAT1400_CASE_ABSTRACT_TYPE];//R CaseAbstractType |简要案情
	std::string ClueID;//R string(1024) |线索标识
	char TimeUpLimit[GAT1400_DATETIME_TYPE];//R dateTime |事发时间上限
	char TimeLowLimit[GAT1400_DATETIME_TYPE];//O dateTime |事发时间下限
	char CreateTime[GAT1400_DATETIME_TYPE];//O dateTime |创建时间
	char PlaceCode[GAT1400_PLACE_CODE_TYPE];//R PlaceCodeType |事发地点行政区划代码
	char PlaceFullAddress[GAT1400_PLACE_FULL_ADDRESS_TYPE];//R PlaceFullAddressType |事发地点区划内详细地址
	int SuspectNumber;//O int |可疑人数量
	std::string WitnessIDs;//O string(1024) |发现人标识符
	char CreatorName[GAT1400_NAME_TYPE];//O NameType |创建人姓名
	char CreatorIDType[GAT1400_ID_TYPE];//O IDType |创建人证件类型
	char CreatorID[GAT1400_ID_NUMBER_TYPE];//O IDNumberType |创建人证件号码
	char CreatorOrg[GAT1400_ORG_TYPE];//O OrgType |创建人单位名称
	double Longitude;//O LongitudeType |事发地经度
	double Latitude;//O LatitudeType |事发地纬度
	std::string MotorVehicleIDs;//O string(1024) |机动车标识符
	std::string NonMotorVehicleIDs;//O string(1024) |非机动车标识符
	std::string PersonIDs;//O string(1024) |人员标识符
	std::string FaceIDs;//O string(1024) |人脸标识符
	std::string ThingIDs;//O string(1024) |物品标识符
	std::string FileIDs;//O string(1024) |文件标识符
	std::string SceneIDs;//O string(1024) |场景标识符
	std::string RelateCaseIdList;//O string(1024) |相关案件标识
	char ParentCaseId[GAT1400_CASE_OBJECT_ID_TYPE];//O CaseObjectIDType |父案件标识
	int State;//R int |案件状态

	tagCase()
	{
		memset(CaseID, 0, sizeof(CaseID));
		memset(CaseLinkMark, 0, sizeof(CaseLinkMark));
		memset(CaseName, 0, sizeof(CaseName));
		memset(CaseAbstract, 0, sizeof(CaseAbstract));
		memset(TimeUpLimit, 0, sizeof(TimeUpLimit));
		memset(TimeLowLimit, 0, sizeof(TimeLowLimit));
		memset(CreateTime, 0, sizeof(CreateTime));
		memset(PlaceCode, 0, sizeof(PlaceCode));
		memset(PlaceFullAddress, 0, sizeof(PlaceFullAddress));
		SuspectNumber = 0;
		memset(CreatorName, 0, sizeof(CreatorName));
		memset(CreatorIDType, 0, sizeof(CreatorIDType));
		memset(CreatorID, 0, sizeof(CreatorID));
		memset(CreatorOrg, 0, sizeof(CreatorOrg));
		Longitude = 0.0f;
		Latitude = 0.0f;
		memset(ParentCaseId, 0, sizeof(ParentCaseId));
		State = 0;
	}
}GAT_1400_Case;

//案件集合对象
typedef struct tagGat1400CaseSet
{
	GAT_1400_Case CaseInfo;
	std::list<GAT_1400_VideoSliceSet> VideoSliceList;
	std::list<GAT_1400_ImageSet> ImageList;
	std::list<GAT_1400_FileSet> FileList;
	std::list<GAT_1400_Person> PersonList;
	std::list<GAT_1400_Face> FaceList;
	std::list<GAT_1400_Motor> MotorVehicleList;
	std::list<GAT_1400_NonMotor> NonMotorVehicleList;
	std::list<GAT_1400_Thing> ThingList;
	std::list<GAT_1400_Scene> SceneList;
}GAT_1400_CaseSet;


typedef struct _tagGATClientMsg 
{
	HTTP_URI_TYPE_H type;
	std::string strURL;
	std::string strParam;//GET方法的参数如/url/get?param1=1&param2=2
	void* pMsgBuf;//POST方法的消息内容
	int nMsgLen;//POST方法的消息长度
	_tagGATClientMsg()
	{
		type = OTHER;
		pMsgBuf = NULL;
		nMsgLen = 0;
	}
}tagGATClientMsg;

#endif