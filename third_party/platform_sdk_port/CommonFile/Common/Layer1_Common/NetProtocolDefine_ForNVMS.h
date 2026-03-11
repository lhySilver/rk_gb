/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       :
** Date         : 2013-9-5
** Name         :
** Version      : 3.0
** Description  :网络协议，以5000为基准，包含网络命令处理对象的标识和需要解析的结构体定义
** Modify Record:

1、登陆过程
    客户端                               服务器

    连接成功后                        检查协议版本、用户名、密码返回登陆结果
    发送命令头、登陆信息

**************************************************************************/

#ifndef NET_PROTOCOL_DEFINE_FORNVMS_H
#define NET_PROTOCOL_DEFINE_FORNVMS_H

#include "base_type_define.h"
#include "ConfigPack_Struct_Def.h"
#include "NVMS_NetProtocolVer.h"
#include <string>
#include<list>

using namespace NVMS_NET_PROTOCOL_VER;
namespace NVMS_NET_PROTOCOL
{
    //const unsigned short int ECMS_NET_PROTOCOL_VER = 0x04;//////最新的网络协议的版本(单独写到NVMS_NetProtocolVer.h中)

    //修改了报警联动结构体的协议版本号，如果等于这个版本号，就用新修改的结构体处理数据
    const unsigned short int ECMS_NET_PROTOCOL_VER_FOR_ALARMTRIGGER = 0x05;

#pragma pack(push, 4)           //结构体定义为4字节对齐,在gcc里只能是1,2,4

    //const tuint32 INVALID_DEVICE_ID = (DWORD)(~0);    //无效设备ID
    //无效的节点ID的定义在头文件ECMS_Define.h里面

    //////////所有命令的头
    typedef struct _ecms_pack_cmd
    {
        unsigned short int cmdProtocolVer;//命令版本，可以直接根据这个版本确定命令的格式
        unsigned char  byExtendInfo; //扩充信息类型，0为无扩充信息
        unsigned char  byHasReply; ///此命令是否需要应答

        tuint32 dwCmdType;//命令类型，对应ECMS_PROTOCOL_DEF中的值
        tuint32 dwCmdID;//指令序号
        tuint32 dwDataLen;//数据的长度,此长度不包括ECMS_PACKCMD_EXTEND 或 ECMS_PACKCMD_EXTEND_TASK结构的长度
        //////////只是后面命令数据的长度
    } ECMS_PACKCMD;

    const unsigned int ECMS_PACKCMD_LENGTH = sizeof(ECMS_PACKCMD);

    //扩充信息类型
    enum EXTEND_CMD_TYPE
    {
        EXTEND_CMD_NULL = 0, //无扩充信息
        EXTEND_CMD_BASE, //基本扩充信息
        EXTEND_CMD_TASK, //任务型扩充信息
    };

    //命令头扩充 附路由信息
    typedef struct _ecms_pack_cmd_extend
    {
        GUID guidLink;////////////路由唯一ID  ，全局唯一，由GUID产生的
        GUID sourceID;////源节点的ID
        GUID destID;////目的节点的ID

        unsigned char byDirect;//路由方向，对应的类型  ROUTE_DIRECT_DEF
        unsigned char byReserver1[3]; ///保留

        //unsigned char  byReserver2[4]; ///保留
    } ECMS_PACKCMD_EXTEND;

    //任务型命令头扩充信息 附路由信息和任务GUID
    typedef struct _ecms_pack_cmd_extend_task
    {
        ECMS_PACKCMD_EXTEND cmdextend;
        GUID taskguid;//全局唯一，由GUID产生的
        /////////////////////
        unsigned char isStart; //TASKSTATE_TYPE  是开始任务还是结束任务，由于在任务传输过程中不一定有具体命令处理对象，所以需要在执行端直接确定
        unsigned char byReserver1[3]; ///保留
    } ECMS_PACKCMD_EXTEND_TASK;


    const unsigned int ECMS_PACKCMD_EXTEND_LENGTH = sizeof(ECMS_PACKCMD_EXTEND);

    /************************************************************************/
    /* 发送请求的时候目的->源，回复请求的时候目的和源节点的位置不对调
    因为回复的时候目的和源节点的信息在程序中都没有用到*/
    /************************************************************************/
    enum ROUTE_DIRECT_DEF
    {
        ROUTE_DIRECT_DEF_RANGE_BEGIN,
        DEST_REPLY_SOURCE,//目的节点回复源节点的信息
        FROM_SOURCE_TO_DEST,//源节点对目的节点发起的请求
        ROUTE_DIRECT_DEF_RANGE_END
    };

    /************************************************************************/
    /*   网络协议数据格式
    --PACKCMD--
    路由信息
    -----------
    数据描述
    -----------
    数据
    */
    /************************************************************************/
    ////////////////////命令ID的定义
    typedef enum _ecms_net_cmd_id_
    {
        NET_CMD_ID_IGNORE,

        NET_CMD_ID_ONE_TO_ONE_MIN = (0xFF + 0x01),//////CMD ID的最小值
        NET_CMD_ID_ONE_TO_ONE_MAX = (0xFFFFFFFF - 0x01),//////////CMD ID的最大值

        NET_CMD_ID_BROADCAST = 0xFFFFFFFF,//////广播数据采用此ID
    } ECMS_NET_CMD_ID;

    //NET_CMD_ID_MULTICASE = 0x0,////////多播数据采用此ID

    enum TASKSTATE_TYPE
    {
        TASKSTATE_TYPE_NULL,//此命令没状态
        TASKSTATE_TYPE_START,//此命令为启动状态
        TASKSTATE_TYPE_STOP,//此命令为停止状态
        TASKSTATE_TYPE_START_REPLY,//此命令为启动应答
        TASKSTATE_TYPE_STOP_REPLY, //此命令为停止应答
        TASKSTATE_TYPE_DATA,  //任务数据
        TASKSTATE_TYPE_DATA_COMPLETE,//数据完成
    };

    //所有命令的应答成功采用请求偏移ECMS_CMD_REPLY_SUCCESS_BASE方式
    //所有命令的应答失败采用请求偏移ECMS_CMD_REPLY_FAIL_BASE的方式
    //不再定义命令应答失败和成功的类型值了

    const  tuint32 ECMS_CMD_REPLY_SUCCESS_BASE  = 0x10000000;
    const  tuint32 ECMS_CMD_REPLY_FAIL_BASE     = 0x20000000;
    const  tuint32 ECMS_CMD_REPLY_NULL_BASE     = 0x30000000;

    inline tuint32 GetReplySuccessCmd(tuint32 requestcmd) {
        return requestcmd + ECMS_CMD_REPLY_SUCCESS_BASE;
    };
    inline tuint32 GetReplyFailCmd(tuint32 requestcmd) {
        return requestcmd + ECMS_CMD_REPLY_FAIL_BASE;
    };

    //类型定义 命令处理类型
    enum CMDPROC_TYPE
    {
        CMDPROC_TYPE_NULL,//无类型
        CMDPROC_TYPE_REQUEST,//请求命令
        CMDPROC_TYPE_REPLY_SUCCESS,//回复成功
        CMDPROC_TYPE_REPLY_FAIL,//回复失败
    };
    //得到命令处理类型
    inline CMDPROC_TYPE GetCmdProcType(tuint32 dwCmd)
    {
        if (dwCmd < ECMS_CMD_REPLY_SUCCESS_BASE) {
            return CMDPROC_TYPE_REQUEST;
        }
        else if (dwCmd < ECMS_CMD_REPLY_FAIL_BASE) {
            return CMDPROC_TYPE_REPLY_SUCCESS;
        }
        else if (dwCmd < ECMS_CMD_REPLY_NULL_BASE) {
            return CMDPROC_TYPE_REPLY_FAIL;
        }
        else {
            return CMDPROC_TYPE_NULL;
        }
    };


    enum ECMS_PTZ_MOVE
    {
        ECMS_PTZ_MOVE_TOP = 1, ////向上走
        ECMS_PTZ_MOVE_BOTTOM, ////向下走
        ECMS_PTZ_MOVE_LEFT, ////向左
        ECMS_PTZ_MOVE_RIGHT, ////向右
        ECMS_PTZ_MOVE_LEFT_TOP, //左上
        ECMS_PTZ_MOVE_LEFT_BOTTOM,//左下
        ECMS_PTZ_MOVE_RIGHT_TOP,//右上
        ECMS_PTZ_MOVE_RIGHT_BOTTOM,//右下
        ECMS_PTZ_MOVE_FOCUSNEAR, ////调焦近
        ECMS_PTZ_MOVE_FOCUSFAR, ////调焦远
        ECMS_PTZ_MOVE_ZOOMIN, ///放大
        ECMS_PTZ_MOVE_ZOOMOUT, ////缩小
        ECMS_PTZ_MOVE_IRISOPEN, ////聚焦
        ECMS_PTZ_MOVE_IRISCLOSE, ////聚焦
    };
    /**/
    enum ECMS_PTZ_CONTROL
    {
        ECMS_PTZ_CONTROL_STOP = 100, /////停止
        ECMS_PTZ_CONTROL_PRESETGO,  ////到某一预置点
        ECMS_PTZ_CONTROL_CRUISEGO,   ////到某一巡航线
        ECMS_PTZ_CONTROL_PRESETSET,  ////设置某一预置点
        ECMS_PTZ_CONTROL_CRUISESTOP, //停止巡航
        ECMS_PTZ_CONTROL_TRACKSTART,    //开始保存轨迹
        ECMS_PTZ_CONTROL_TRACKSTOP,     //停止保存轨迹
        ECMS_PTZ_CONTROL_TRACKSET,
        ECMS_PTZ_CONTROL_TRACK_SCAN_START,  //开始轨迹
        ECMS_PTZ_CONTROL_TRACK_SCAN_STOP,   //停止轨迹
        ECMS_PTZ_CONTROL_TRACK_SCAN_STOP_NOSAVE,    //取消轨迹
        ECMS_PTZ_CONTROL_AUTO_SCAN_START,   //开始自动扫描
        ECMS_PTZ_CONTROL_AUTO_SCAN_STOP,    //停止自动扫描
        ECMS_PTZ_CONTROL_OPEN_LAMP,//接通灯光
        ECMS_PTZ_CONTROL_CLOSE_LAMP,//关闭灯光
        ECMS_PTZ_CONTROL_OPEN_WIPER,//接通雨刷
        ECMS_PTZ_CONTROL_CLOSE_WIPER,//关闭雨刷
        ECMS_PTZ_CONTROL_CRUISEGO_ONECE,   //巡航一次
    };

    enum ECMS_1400_HTTP_METHOD
    {
        ECMS_1400_GET,   //http的get方法
        ECMS_1400_PUT,   //http的put方法
        ECMS_1400_POST,  //http的post方法
        ECMS_1400_DELETE //http的delete方法
    };

    enum ECMS_RAW_FORMAT
    {
        ECMS_JSON_FORMAT,  //json格式
        ECMS_XML_FORMAT    //xml格式
    };

    /////////////网络协议的相关定义
    typedef enum _net_protocol_cmd_def_
    {
        /************************************************************************/
        /*         客户端登陆                                                   */
        /************************************************************************/
        /////////客户端登陆
        ECMS_CMD_BASENUM_LOGIN = 0x100,
        ECMS_CMD_REQUEST_LOGIN,  ///////请求登陆，权限验证
        ECMS_CMD_REQUEST_READY,  //其它节点登陆注册服务器时使用，告诉RSU自己已经准备就绪，发送Listen端口，PC唯一序号，获得配置信息
        ECMS_CMD_REQUEST_HOST_SET_TIME,//主机之间校时 add by wangxin
        ECMS_CMD_REQUEST_REPORT_CONFIG_PORT,//上报配置服务器的端口
        ECMS_CMD_REQUEST_LOCKACCESS_TIME,//加密狗信息
        ECMS_CMD_REQUEST_OUTLICENSEFORADDCHANNEL,//总通道数量大于授权数量，部分设备添加失败或离线
        ECMS_CMD_REQUEST_TEST_LOGIN_DEV,//客户端发送给转发测试登录设备
        ECMS_CMD_REQUEST_SERVER_STATE_WITH_RSU,//服务器与管理服务器状态通知给客户端
        ECMS_CMD_REQUEST_DEV_CAPACITY,//获取设备能力集 通道\输入\输出数量
        ECMS_CMD_REQUEST_IP_INFO,//上报本地IP地址列表（主要用于管理服务器电脑改完IP地址后将自己的IP发送给电视墙服务器，电视墙服务器校验一个能连上的IP发送给解码器）
        ECMS_CMD_SET_RSU_CONFIG_FILE,//修改认证服务器配置文件，比如停车超时时间（小时）
        ECMS_CMD_REQUEST_SYN_PLATFORM_TIME,//手动同步平台服务器和设备时间,客户端给管理服务器发送的命令
        ECMS_CMD_REQUEST_SYN_SERVER_TIME,//服务器和设备时间,管理服务器给各个服务器发送的命令
        ECMS_CMD_REQUEST_SET_CHL_ALARMIN_ALARMOUT_GUID,//设置通道\输入\输出在平台中的GUID
        ECMS_CMD_REQUEST_GET_CUSTOM_PRODUCT,//获取客户定制数据
        ECMS_CMD_REQUEST_VISITOR_TIMEOUT,   //访客超时提醒
        ECMS_CMD_REQUEST_FORCED_DISCONNECTION,   //强制断开设备的网络连接
        ECMS_CMD_REQUEST_REQ_SUB_SMART_STREAM,//订阅智能流
        ECMS_CMD_LOGIN_END,
        /************************************************************************/
        /*              /////////操作权限检查                                                          */
        /************************************************************************/
        ECMS_CMD_BASENUM_RIGHT_CHECK = 0x200,
        ECMS_CMD_REQUEST_RIGHT_CHECK,//////
        ECMS_CMD_RIGHT_CHECK_END,

        /************************************************************************/
        /*      ////////路由表的相关操作                                                                 */
        /************************************************************************/
        ECMS_CMD_BASENUM_ROUTE = 0x300,
        ECMS_CMD_REQUEST_ROUTE_INFO,//请求路由信息
        ECMS_CMD_REFRESH_ROUTE_INFO,//请求刷新路由信息
        ECMS_CMD_REQUEST_ROUTE_END,

        /************************************************************************/
        /*           日志相关的操作                                                           */
        /************************************************************************/
        ECMS_CMD_BASENUM_LOG = 0x400,////日志相关命令
        ECMS_CMD_REQUEST_ABNORMAL_LOG,//搜索程序异常日志
        ECMS_CMD_REQUEST_SERRCH_LOG,///搜索日志
        ECMS_CMD_REQUEST_SEARCH_DEVICE_LOG, ///搜索设备的日志信息
        ECMS_CMD_REQUEST_SERRCH_LOG_CREATEQUERYRECORD,///创建查询日志句柄
        ECMS_CMD_REQUEST_SERRCH_LOG_GETRECORDTOTALCOUNT,///获取日志总条数
        ECMS_CMD_REQUEST_SERRCH_LOG_GETRECORD,///获取日志
        ECMS_CMD_REQUEST_SERRCH_LOG_DESTROYQUERYRECORD,///销毁查询句柄
        ECMS_CMD_REQUEST_WRITE_ALARM_LOG,//写报警日志
        ECMS_CMD_REQUEST_WRITE_OPERATE_LOG,//写操作日志
        ECMS_CMD_REQUEST_WRITE_CONFIG_LOG,//写配置日志
        ECMS_CMD_REQUEST_WRITE_EXCEPTION_LOG,//写异常日志
        ECMS_CMD_REQUEST_WRITE_INFO_LOG,//写信息日志
        ECMS_CMD_REQUEST_WRITE_ABNORMAL_LOG,//写程序内部异常日志
        ECMS_CMD_REQUEST_REMOVE_HISTORY_LOG,//删除历史日志
        ECMS_CMD_REQUEST_SEARCH_PMS_PASSRECORD,//查询车辆通行记录 用于平台SDK查询车辆通行记录
        ECMS_CMD_REQUEST_SEARCH_PMS_PASSRECORD_DESTROY,//销毁查询句柄 和ECMS_CMD_REQUEST_SEARCH_PMS_PASSRECORD对应，必须成对出现
		ECMS_CMD_REQUEST_XML_QUERY_PMSPASSRECORD,//xml查询车辆通行记录 用于客户端收到通行记录时查询车辆通行记录,之前是通过走配置服务器实现，数据量大的时候并发很大，有性能瓶颈
		ECMS_CMD_REQUEST_XML_EDIT_PMSPASSRECORD,//xml编辑车辆通行记录 用于客户端收到通行记录时查询车辆通行记录,之前是通过走配置服务器实现，数据量大的时候并发很大，有性能瓶颈
        ECMS_CMD_REQUEST_XML_EDIT_PMS_PARK_INFO,//xml修改停车场信息
		/************************************************************************/
        /*               /////////现场数据控制                                                       */
        /************************************************************************/
        ECMS_CMD_BASENUM_REAL_STREAM = 0x500,
        ECMS_CMD_REQUEST_LIVE_STREAM_START,///////请求现场流的传输
        ECMS_CMD_REQUEST_LIVE_STREAM_STOP,///////关闭现场流的传输
        ECMS_CMD_REQUEST_REAL_STREAM_PREVIEW_OPEN,//////////控制对象打开现场预览
        ECMS_CMD_REQUEST_REAL_STREAM_PREVIEW_CLOSE,/////////控制对象关闭现场预览
        ECMS_CMD_REQUEST_REAL_STREAM_TVWALL_OPEN,/////////控制电视墙打开现场预览
        ECMS_CMD_REQUEST_REAL_STREAM_TVWALL_CLOSE,/////////控制电视墙关闭现场预览
        ECMS_CMD_REQUEST_KEY_FRAME,//预览请求关键帧 add by wangxin
        ECMS_CMD_REAL_STREAM_END,


        /************************************************************************/
        /*                                    /////////////////////录像控制                                 */
        /************************************************************************/
        ECMS_CMD_BASENUM_REQUEST_CTRL_REC = 0x600,
        ECMS_CMD_REQUEST_ALARM_NET_CTRL_REC_START,//////////////报警服务器发送的控制录像开始的指令
        ECMS_CMD_REQUEST_ALARM_NET_CTRL_REC_STOP,//////////////报警服务器发送的控制录像结束的指令
        ECMS_CMD_REQUEST_NET_CTRL_REC_START,//////////////网络控制手动录像
        ECMS_CMD_REQUEST_NET_CTRL_REC_STOP,//////////////网络控制手动录像
        ECMS_CMD_REQUEST_CTRL_REC_END,


        /************************************************************************/
        /*      ////////////////对讲和广播控制                                                                */
        /************************************************************************/
        ECMS_CMD_BASENUM_TALK = 0x700,
        ECMS_CMD_REQUEST_TALKBACK_BEGIN,///////////请求对讲开始
        ECMS_CMD_REQUEST_TALKBACK_END,///////////请求对讲结束
        ECMS_CMD_REQUEST_BROADCAST_BEGIN,   //请求广播开始
        ECMS_CMD_REQUEST_BROADCAST_END, //请求广播开始
        ECMS_CMD_REQUEST_ALARM_TRIGGER_AUDIO_BEGIN,
        ECMS_CMD_REQUEST_ALARM_TRIGGER_AUDIO_END,
        ECMS_CMD_TALK_END,

        /************************************************************************/
        /*              录像数据播放控制请求部分                                  */
        /************************************************************************/
        ECMS_CMD_BASENUM_REC_STREAM_CTRL = 0x800,
        ECMS_CMD_REQUEST_REC_EVENT_SEARCH,//////搜索事件
        ECMS_CMD_REQUEST_REC_SECTION_SEARCH,/////搜索时间段
        ECMS_CMD_REQUEST_REC_DATA_PLAY,//////播放
        ECMS_CMD_REQUEST_REC_DATA_PAUSE,/////暂停
        ECMS_CMD_REQUEST_REC_DATA_RESUME,//////恢复播放
        ECMS_CMD_REQUEST_REC_DATA_FF, //////关键帧快进
        ECMS_CMD_REQUEST_REC_DATA_STOP,//////停止
        ECMS_CMD_REQUEST_REC_INDEX,/////当前帧的索引号
        ECMS_CMD_REQUEST_REC_DATA_BACKUP,//////播放
        ECMS_CMD_REQUEST_REC_DATA_STOP_BACKUP,//////停止
        ECMS_CMD_REQUEST_REC_OPEN_ONLY_KEY_FRAME,   //打开关键帧快进
        ECMS_CMD_REQUEST_REC_CLOSE_ONLY_KEY_FRAME,  //关闭关键帧快进
        ECMS_CMD_REQUEST_REC_REWIND,                //倒放
        ECMS_CMD_REQUEST_REC_PREVIOUS,              //上一帧
        ECMS_CMD_REQUEST_REC_RESUME,                //恢复回放
        ECMS_CMD_REQUEST_REC_CANCEL_REWIND,         //取消倒放
        ECMS_CMD_REQUEST_REC_DATA_LEN,              //获取数据长度
        ECMS_CMD_REQUEST_REC_ADD_TAG,   //添加录像标签
        ECMS_CMD_REQUEST_REC_DEL_TAG,   //删除录像标签
        ECMS_CMD_REQUEST_REC_TAG_SEARCH,//搜索录像标签
        ECMS_CMD_REQUEST_PIC_SEARCH,//图片列表搜索
        ECMS_CMD_REQUEST_PIC_DATA,//图片数据请求
        ECMS_CMD_REQUEST_REC_EVENT_DATE_SEARCH,//搜索含有录像数据的日期
        ECMS_CMD_REQUEST_REC_EVENT_TYPE_SEARCH,//请求录像类型
        // 2018-6-13 添加国标倍速播放命令
        ECMS_CMD_REQUEST_REC_PLAY_SPEED,    // 播放倍速控制字段  数据为1、2、3、4、5 代表（1/4、1/2、 1、 2、 4倍速）
        ECMS_CMD_REQUEST_PMS_PASSRECORD_PIC_DATA,//客户端找存储请求通行记录图片
        ECMS_CMD_REQUEST_STORAGE_INFO,//存储服务信息，事件发生后推送或查询，如未配置分区，raid状态变化等。//ECMS_STORAGE_INFO
        ECMS_CMD_REQUEST_MANUAL_STORAGE,
        ECMS_CMD_REQUEST_PLATEMATCH_LIST,   //搜索车牌
        ECMS_CMD_REQUEST_PLATEMATCH_PIC_DATA,   //车牌图片数据
        ECMS_CMD_END_REC_STREAM_CTRL,

        /************************************************************************/
        /*      /////////                                      */
        /************************************************************************/
        ECMS_CMD_BASENUM_CONFIG = 0x900,
        ECMS_CMD_REQUEST_CONFIG_START,///////进入配置
        ECMS_CMD_REQUEST_CONFIG_END,///////退出配置
        ECMS_CMD_REQUEST_CONFIG_INFO,///////请求配置信息
        ECMS_CMD_REQUEST_SAVE_INFO,///////保存配置信息
        ECMS_CMD_REQUEST_NEW_NODE_ID,////分配新的节点ID
        ECMS_CMD_REQUEST_CONFIG_INFO_UPDATE, /////配置信息更新，附带被更新的配置数据
        ECMS_CMD_REQUEST_UPDATE_PARTITION_INFO,//存储服务器更新分区信息
        ECMS_CMD_REQUEST_CONFIG_REBOOT,//重启设备
        ECMS_CMD_REQUEST_CONFIG_RESTORE,//恢复出厂设置
        ECMS_CMD_REQUEST_BEGIN_IMPORT_SYS_CONFIG,/////请求导入系统配置
        ECMS_CMD_REQUEST_IMPORT_SYS_CONFIG_DATA,
        ECMS_CMD_REQUEST_END_IMPORT_SYS_CONFIG,///导入系统配置
        ECMS_CMD_REQUEST_DISK_INFO, //请求磁盘信息
        ECMS_CMD_REQUEST_MODIFY_PW,///修改密码
        ECMS_CMD_REQUEST_CH_NAME_UPDATE_INFO,////通知更新通道名称
        ECMS_CMD_REQUEST_EMAP_USER_RIGHT_UPDATE,        //认证收到配置客户端电子地图权限发送给电子地图
        ECMS_CMD_REQUEST_DOWNLOAD_EMAPFILE,     //下载电子地图文件，参数只有一个ULONG代表电子地图ID
        ECMS_CMD_REQUEST_CONFIG_INFO_LOGIN,     //登陆发送过来的配置信息,目前只用于电子地图
        ECMS_CMD_REQUEST_SETCHANNLE_COLOR,      //设置通道颜色值ECMS_SET_CHANNEL_COLOR
        ECMS_CMD_REQUEST_CHANGE_TIME, //修改的时间，根据指定的时间参数修改
        //ECMS_CMD_REQUEST_CH_ENCODE_INFO_UPDATE,////更新通道编码信息D1、CIF等   涵义有误而且太过具体而直接废除了,由下面的一项代替
        ECMS_CMD_REQUEST_NODE_APPEND_INFO_UPDATE, //////节点的附加信息, 包括设备和通道支持的功能, 码流信息等
        ECMS_CMD_REQUEST_WIFI_LIST,//请求可用WIFI列表
        ECMS_CMD_REQUEST_ONLINE_USERS, //在线用户查询
        NVMS_CMD_REQUEST_TRANSPARENT_TRANSPORT,  ///请求透明传输请求数据 TransparentTransport
        NVMS_CMD_REQUEST_SNAP_PICTURE,//联动抓图
        ECMS_CMD_REQUEST_CHANGE_TIME_BY_OSTIME, //修改的时间，根据系统当前时间修改
        ECMS_CMD_API_CONFIG_FOR_MOBILE,
        NVMS_CMD_AF_CONTROL,
        ECMS_CMD_REQUEST_START_SEARCH_DEVICE,//搜索本地设备
        ECMS_CMD_REQUEST_STOP_SEARCH_DEVICE,
        ECMS_CMD_REQUEST_SEND_SEARCH_DEVICE,
        ECMS_CMD_REQUEST_NET_BAND,//管理服务器向存储服务器查询网络带宽信息
        ECMS_CMD_REQUEST_DEVICE_NAME_UPDATE_INFO,////通知更新设备名称
        ECMS_CMD_REQUEST_SERVER_RESTART,//转发和存储服务器内部重启
        ECMS_CMD_BARRIER_CONTROL,//道闸控制
        ECMS_CMD_REQUEST_LED_TEXT_SET,//设置LED屏显示
        ECMS_CMD_REQUEST_SERIAL_LED_TEXT_SET,//设置华安外接485屏显示
        ECMS_CMD_TRANSPORT,// 透传命令
        ECMS_CMD_REQUEST_DEVICE_INFO,//请求设备信息
        ECMS_CMD_REQUEST_DEVICE_DETAIL,//请求设备详情
        ECMS_CMD_REQUEST_CH_GB28181ID_UPDATE_INFO,//更新通道国标ID
        ECMS_CMD_REQUEST_UPDATE_CASCADE_SERVER_INFO,//上级网关国标服务器更新信息到管理时使用
        ECMS_CMD_REQUEST_SYNC_LOWERCASCADEPLATFORM_INFO,//上级平台同步下级平台的设备资源
        ECMS_CMD_REQUEST_BATCH_CONFIG,//批量设置设备参数，现在是用于客户端发送给转发，传输数据采用xml格式，利于扩展
        ECMS_CMD_REQUEST_SENSOR_NAME_UPDATE_INFO,//通知更新sensor名称
        ECMS_CMD_REQUEST_ALARMOUT_NAME_UPDATE_INFO,//通知更新alarmout名称
        ECMS_CMD_REQUEST_FORMAT_DISK,    ////请求格式化磁盘
        ECMS_CMD_CREATE_REC_BACKUP_TASK, //请求创建备份任务
        ECMS_CMD_QUERY_REC_BACKUP_TASKLIST,   //请求查询录像备份任务列表
        ECMS_CMD_CTRL_REC_BACKUP_TASK,   //请求操作录像备份
        ECMS_CMD_REQUEST_QUERY_EXTERNALDISKS,   //请求查询外接盘信息
        ECMS_CMD_REQUEST_NVR_ALARM_CHANNELINFO, //NVR关联录像通道查询
        ECMS_CMD_REQUEST_DONGHUAN_RS485_INFO, //查询RS485数量
        ECMS_CMD_REQUEST_DONGHUAN_SENSOR_INFO, //查询动环的传感器参数信息
        ECMS_CMD_REQUEST_SYN_RSU_TIME,    //请求校时管理服务器时间
        ECMS_CMD_REQUEST_YITU_HEARTBEAT,
        ECMS_CMD_MODIFY_DEVICE_IP,//设置设备的IP地址
        ECMS_CMD_REQUEST_PASSLINE_COUNT,    //请求IPC的过线统计数
        ECMS_CMD_REQUEST_CHANGE_TIME_EXCEPT_TIMEZONE, //修改的时间，与ECMS_CMD_REQUEST_CHANGE_TIME_BY_OSTIME同作用但不同步时区
        ECMS_CMD_REQUEST_STORAGEDEV_INFO,//查询设备录像时间，多少天
        ECMS_CMD_REQUEST_NVRSTORAGE_INFO,
        ECMS_CMD_REQUEST_CH_GAT1400ID_UPDATE_INFO,//更新通道GAT1400ID
        ECMS_CMD_REQUEST_EDGE_BOX_HEARTBEAT,//边缘盒子心跳
        ECMS_CMD_REQUEST_XCP10_LED_TEXT_SET,//设置XCP10LED屏显示
        ECMS_CMD_CONFIG_END,

        /************************************************************************/
        /*  //////////状态数据，状态数据下面可以根据具体情况细分
        状态数据包括，节点连接断开、报警信息、通道名称修改
        、前段设备的通道录像状态、
        云台是否可用*/
        /************************************************************************/
        ECMS_CMD_BASENUM_REQUEST_STATE  = 0xA00,
        ECMS_CMD_REQUEST_ALARM_STATE_INFO,////请求报警的状态信息
        ECMS_CMD_REQUEST_CONNECT_STATE_INFO,///请求连接状态信息
        ECMS_CMD_REQUEST_RECORD_STATE_INFO,////请求录像状态信息
        ECMS_CMD_REQUEST_DEVRECORD_STATE_INFO,////请求设备录像状态信息
        ECMS_CMD_REQUEST_PASS_EVENT,//车辆通过事件
        ECMS_CMD_REQUEST_PASS_EVENT_WITH_NOPICTURE,//车辆通过事件（不带图片的）
        ECMS_CMD_REQUEST_DEV_LOGIN_ERROR_INFO,//针对所有编码设备连接失败错误码
        ECMS_CMD_REQUEST_PR_PASS_EVENT,
        ECMS_CMD_REQUEST_TRAJECT_RECT_EVENT,//目标框事件（包括所有目标框）
        ECMS_CMD_REQUEST_NEW_ONLINE_UPDATE_EVENT,//新的适配库上传过来中途上线的监控点，用作NVR在线时，有监控点上线的情况
        ECMS_CMD_REQUEST_STATE_END,


        /************************************************************************/
        /*  /////////通过网络对云台进行控制                                                                    */
        /************************************************************************/
        ECMS_CMD_BASENUM_PTZ_CONTROL = 0xB00,
        ECMS_CMD_PTZ_MOVE,      /////移动操作   NET_PTZ_CTRL_INFO结构体里面的dwCmdType子段为ECMS_PTZ_MOVE的值之一
        ECMS_CMD_PTZ_CONTROL,   /////云台控制  NET_PTZ_CTRL_INFO结构体里面的dwCmdType子段为ECMS_PTZ_CONTROL的值之一
        ECMS_CMD_GET_PTZ_PRESET,        //获取预置点
        ECMS_CMD_GET_PTZ_CRUISE,        //获取巡航线
        ECMS_CMD_PTZ_3DCONTROL,         //3D定位
        ECMS_CMD_SET_PTZ_PRESETNAME,    //修改预置点名称
        ECMS_CMD_GET_PTZ_TRACE,     //获取轨迹
        ECMS_CMD_SET_PTZ_GUARDINFO, //设置云台的看守卫
        ECMS_CMD_TVT_UPDATEINFO,    //升级4GIPC
        ECMS_CMD_TVT_UPDATEPROCESS,     //获取进度
        ECMS_CMD_TVT_UPDATECHECK,       //验证IPC是否符合规则
        ECMS_CMD_GET_PTZ_STEP,      // 获取云台位置、zoom位置
        ECMS_CMD_GET_PTZ_GUARDINFO, //获取云台的看守卫
        ECMS_CMD_TVT_UPDATESTATUS,      //获取此时的IPC升级状态
        ECMS_CMD_REQUEST_DOWNUPLOAD_FILE,
        ECMS_CMD_PTZ_CONTROL_END,


        /************************************************************************/
        /*      ///////////////////其它未分类的系统操作控制                                                                */
        /************************************************************************/
        ECMS_CMD_BASENUM_OTHER_SYS_CTRL = 0xC00,
        ECMS_CMD_REQUEST_DOWN_FILE,     //下载文件,1.0 ver,no used,don't del
        ECMS_CMD_DOWN_FILE_DATA,        //下载文件数据
        ECMS_CMD_DOWN_FILE_COMPLETE,    //下载文件完成
        ECMS_CMD_DOWN_FILE_STOP,        //下载文件停止
        ECMS_CMD_REQUEST_UPLOAD_FILE,   //请求上传文件,1.0 ver,no used,don't del
        ECMS_CMD_UPLOAD_FILE_DATA,      //上传文件数据
        ECMS_CMD_UPLOAD_FILE_COMPLETE,  //上传文件完成
        ECMS_CMD_UPLOAD_FILE_STOP,      //上传文件停止
        ECMS_CMD_VOICE_BROADCAST_FILE_START,    //管理服务给转发服务发语音播报音频文件开始
        ECMS_CMD_VOICE_BROADCAST_FILE_DATA,     //管理服务给转发服务发语音播报音频文件数据
        ECMS_CMD_VOICE_BROADCAST_FILE_COMPLETE, //管理服务给转发服务发语音播报音频文件完成
        ECMS_CMD_OTHER_SYS_CTRL_END,

        ECMS_CMD_BASENUM_ALARMOUT_CTRL = 0xD00,
        ECMS_CMD_REQUEST_SETALARMOUT,//控制设备的报警输出
        ECMS_CMD_REQUEST_CLEARALARMOUT,//清除设备的报警输出
        ECMS_CMD_BASENUM_ALARMOUT_CTRL_END,


        ////现场流数据
        ECMS_CMD_BASENUM_REAL_STREAM_DATA = 0x10000,
        ECMS_CMD_LIVE_STREAM_DATA,///////现场流数据
        ECMS_CMD_LIVE_STREAM_DATA_COMPLETE,/////现场数据流完成
        ECMS_CMD_REAL_STREAM_DATA_END,

        ////回放流数据
        ECMS_CMD_BASENUM_PLAY_STREAM_DATA = 0x20000,
        ECMS_CMD_PLAY_STREAM_DATA,//////回放流数据
        ECMS_CMD_PLAY_STREAM_DATA_COMPLETE,/////回放数据流完成
        ECMS_CMD_PLAY_STREAM_DATA_END,

        //对讲数据
        ECMS_CMD_BASENUM_TALKBACK_STREAM_DATA = 0x30000,
        ECMS_CMD_TALKBACK_STREAM_DATA,              //对讲流数据
        ECMS_CMD_TALKBACK_STREAM_DATA_COMPLETE,     //对讲流数据完成
        ECMS_CMD_TALKBACK_STREAM_DATA_END,

        //监控客户端与电视墙服务器交互
        ECMS_CMD_TVWALL_GET_CONFIG = 0x40000,           //客户端获取配置
        ECMS_CMD_TVWALL_SAVE_CONFIG,                    //客户端保存配置
        ECMS_CMD_TVWALL_OPTION,                         //客户端操作命令
        ECMS_CMD_TVWALL_UPDATE,                         //电视墙主动通知客户端
        ECMD_CMD_TVWALL_KEYBOARD,                       //键盘操作命令
        ECMS_CMD_TVWALL_SET_WALL_LAYOUT,                //设置墙布局
        ECMS_CMD_TVWALL_GET_WALL_LAYOUT,                //获取墙布局
        ECMS_CMD_TVWALL_DO_BIND_WALL,                   //绑定墙
        ECMS_CMD_TVWALL_SETWALL,                        //设置电视墙
        ECMS_CMD_TVWALL_GETTVWALLSDATA,
        ECMS_CMD_TVWALL_SETTVWALLSDATA,
        ECMS_CMD_TVWALL_NOTIFY_CONFIG_END,


        //电视墙服务器与解码器交互命令定义
        ECMS_CMD_TVWALL_DECODE_GET_CONFIG = 0x50000,            //解码器参数获取
        ECMS_CMD_TVWALL_DECODE_SAVE_CONFIG,                     //解码器保存配置
        ECMS_CMD_TVWALL_DECODE_OPTION,                          //解码器操作
        ECMS_CMD_TVWALL_DECODE_UPDATE,                          //解码器数据上报
        ECMS_CMD_TVWALL_DECODE_OTHER,                           //解码器其他功能
        ECMS_CMD_TVWALL_DECODE_SET_WALL_LAYOUT,                 //解码器设置墙布局
        ECMS_CMD_TVWALL_DECODE_GET_WALL_LAYOUT,                 //解码器获取墙布局
        ECMS_CMD_TVWALL_DECODE_DO_BIND_WALL,                    //解码器绑定墙
        ECMS_CMD_TVWALL_DECODE_CONFIG_END,

        //接入服务器相关交互
        ECMS_CMD_REQUEST_ALARM_HOST_REVERSE_CONTROL = 0x60000,          //反控告警主机
        ECMS_CMD_REQUEST_ALARM_HOST_REVERSE_CONTROL_ARM,                //布防
        ECMS_CMD_REQUEST_ALARM_HOST_REVERSE_CONTROL_DISARM,             //撤防
        ECMS_CMD_REQUEST_ALARM_HOST_REVERSE_CONTROL_CLEAR_ALARM,        //消警
        ECMS_CMD_REQUEST_ALARM_HOST_REVERSE_CONTROL_BYPASS,             //旁路
        ECMS_CMD_REQUEST_ALARM_HOST_REVERSE_CONTROL_BYPASSRES,          //旁路恢复
        ECMS_CMD_REQUEST_ALARM_HOST_STATE_INFO,                         //查询告警主机状态
        ECMS_CMD_REQUEST_UPLOADACSDEVICE_INFO,                          //上报门禁系统的设备集合
        ECMS_CMD_REQUEST_UPLOADACSDOOR_INFO,                            //上报门禁系统的设备集合
        ECMS_CMD_REQUEST_UPLOADACSREADER_INFO,                          //上报门禁系统的设备集合
        ECMS_CMD_REQUEST_ACS_STATE_INFO,                                //查询门禁系统状态
        ECMS_CMD_REQUEST_ACS_REVERSE_CONTROL,                           //反控门禁系统
        ECMS_CMD_REQUEST_ACS_REVERSE_CONTROL_OPEN_DOOR,                                    //开门
        ECMS_CMD_REQUEST_ACS_REVERSE_CONTROL_CLOSE_DOOR,                                   //关门
        ECMS_CMD_REQUEST_ACS_REVERSE_CONTROL_ALWAYS_OPEN_DOOR,                 //常开
        ECMS_CMD_REQUEST_ACS_REVERSE_CONTROL_ALWAYS_CLOSE_DOOR,                 //常关
        ECMS_CMD_REQUEST_ACS_REVERSE_CONTROL_EXPLODE_TEAR_BOMB,             //引爆催泪弹
        ECMS_CMD_REQUEST_ACS_REVERSE_CONTROL_CANCEL_TEAR_BOMB,                  //取消引爆催泪弹
        ECMS_CMD_REQUEST_ACS_REVERSE_CONTROL_REFRESH_RESOURCE,  //门禁系统资源更新
        ECMS_CMD_REQUEST_FK_SYNC_EMPLOYEE,
        ECMS_CMD_REQUEST_FK_SYNC_BLACKLIST,
        ECMS_CMD_REQUEST_KP_UPLOAD_VISITORS,//访客信息上报
        ECMS_CMD_REQUEST_FK_DELETE_VISITORS,//访客注销
        ECMS_CMD_REQUEST_ACS_RECORD,                           //门禁记录
        ECMS_CMD_REQUEST_ACS_RECORD_ATTENDANCE,                      //获取门禁刷卡记录
        ECMS_CMD_REQUEST_FK_SYNC_DEPARTMENT,
        ECMS_CMD_REQUEST_WIFIPROBE_UPLOAD_INFO,
        ECMS_CMD_REQUEST_WIFIPROBE_UPDATE_INFO,
        ECMS_CMD_REQUEST_WIFIPROBE_DELETE_INFO,
        ECMS_CMD_REQUEST_ACS_STATE_INFO_KANTECH,
        ECMS_CMD_REQUEST_KANTECH_DOOR_STATE,    //泰科门状态上报给客户端的

        //////////////////////////////////////////////////////////////////////////
        // 智能分析服务器与客户端相关交互
        ECMS_CMD_IA_GET_CONFIG = 0x70000,           // 获取智能分析服务的配置参数
        ECMS_CMD_IA_SET_CONFIG,                     // 设置智能分析服务的配置参数
        ECMS_CMD_IA_OPERATION,                      // 智能分析服务操作相关
        ECMS_CMD_IA_UPDATE,                         // 智能分析服务状态更新

        //////////////////////////////////////////////////////////////////////////
        //监控客户端与报警服务器交互
        ECMS_CMD_ALARM_TEST_SMTP = 0x71000,  //测试邮箱

        //////////////////////////////////////////////////////////////////////////
        // 智能分析服务器与IPC相关交互
        NVMS_CMD_REQUEST_SMART_EVENT_STATE = 0x80000,
        NVMS_CMD_REQUEST_SUB_SMART_TOPIC,
        NVMS_CMD_REQUEST_UNSUB_SMART_TOPIC,
        NVMS_CMD_PUSH_SMART_TOPIC_DATA,         // 人脸比对IPC上报抓图信息， A2会增加抓图及告警信息
        NVMS_CMD_NVR_PUSH_SMART_TOPIC_DATA,     // 人脸比对NVR上报告警信息

        // 车牌是识别，订阅相关
        NVMS_CMD_REQUEST_SUB_VEHICLE_TOPIC,         // 车牌识别上报信息
        NVMS_CMD_REQUEST_UNSUB_VEHICLE_TOPIC,
        NVMS_CMD_REQUEST_CLIENT_WITH_MDU_CONFIG,
        //区域入侵相关订阅
        //区域入侵抓图订阅相关
        NVMS_CMD_REQUEST_SUB_PERIMETER,
        NVMS_CMD_REQUEST_UNSUB_PERIMETER,
        NVMS_CMD_MSU_WITH_MDU_PERIMETER,
        NVMS_CMD_MDU_TO_MSU_PIC_PERMITER,
        NVMS_CMD_MDU_NOTIFY_TO_MSUSUBSTATU,
        //云台预置点和巡航线，兼容配置命令
        NVMS_CMD_ADAPT_REQUEST_BEGIN = 0x90000,
        NVMS_CMD_ADAPT_REQUEST_SAVE_PRESET = NVMS_CMD_ADAPT_REQUEST_BEGIN,  //保存预置点
        NVMS_CMD_ADAPT_REQUEST_CONFIG_CRUISE,           //保存巡航线
        NVMS_CMD_REQUEST_DELETE_PRESET,           //删除预置点

        //////////////////////////////////////////////////////////////////////////
        // 智能分析服务器与转发服务器关于人脸比对NVR相关交互
        // 由于命令较多，故采用一个命令处理类，根据命令类型处理不同的命令
        NVMS_CMD_ANS_WITH_MDU_CONFIG = 0x100000,
        NVMS_CMD_MDU_NOTIFY_ALARM_TO_ANS,       // 转发服务器向智能服务器推送比对报警、抓拍报警等信息
        NVMS_CMD_ANS_NOTIFY_CONFIG_TO_RSU,      // 智能服务器向管理服务器推送目标库信息
        NVMS_CMD_MOVE_VISITOR_RECORD,          //管理向智能 推送访客记录数据

        //////////////////////////////////////////////////////////////////////////
        // 智能服务器与1400服务器交互
        NVMS_CMD_1400_IN_NOTIFY_TO_ANS = 0x101000,
        //////////////////////////////////////////////////////////////////////////
        // 1400上级与下级交互命令
        NVMS_CMD_1400_IN_WITH_OUT_HTTP = 0x110000,

        // 转发服务器向NVR请求命令， 目标库管理， useless
        NVMS_CMD_MDU_WITH_NVR_CONFIG = 0x200000,

        //应用服务器通过推送数据给app
        NVMS_CMD_NOTIFY_DATA_TO_APP = 0x210000,
        //专门用于应用服务器去请求字节头数据
        NVMS_CMD_REQUEST_HEAD_DATA,
        NVMS_CMD_APPSERVER_TRANSPORT,
        //////////////////////////////////////////////////////////////////////////

        //无效命令
        ECMS_CMD_NULL = 0xFFFFFFFF,
    } NET_PROTOCOL_CMD_DEF;

    inline NET_PROTOCOL_CMD_DEF GetRequestCmd(const tuint32 dwCmd)
    {
        return (NET_PROTOCOL_CMD_DEF)(dwCmd & 0x0fffffff);
    }

    //登陆时用的结构体
    typedef struct _ecms_net_login_info
    {
        tuint32   nodeType;  //节点类型
        GUID      nodeID;      //节点ID
        GUID      destNodeID;////连接的目标节点的ID

        char            username[64];//用户姓名
        char            password[64];//用户密码
        //以下两项在过滤客户端时使用
        unsigned char   IP[24];      //客户端用来连接的IP地址  , 从第0个开始是sockaddr_in的结构体

        unsigned char   szMAC[307];  //网卡的MAC地址修改长度,由于电脑可能多网卡,字符串表示,逗号隔开，比如"00:00:00:00:00:00,00:00:00:00:00:00,"
        char   szServerVersion[16];  //add by zcy 20190730
        unsigned char   sz4GHuananSn[32];//4Ghuaan
        unsigned char   szDecrInfo[28];//机器序列号DecrInfo硬件标识
        char            szIsOtherType;//是否为其他类型采用MCU类型登录  1是解码器 2是平台SDK //384
        unsigned char   byRtspUrl[96];//原来szMAC地址512中拆分出来的
        tuint64  huanan4GdeviceHandle;//4Ghuaan
        unsigned char  byReserve1[24];//保留
        unsigned char   byTestLogin;////是否是测试连接的登陆  正常登录为0， 测试连接的登录为非0
        char            szPlatformType;      // Enterprise_Type=0企业版,Standard_Type=1标准版, Lite_Type=2单机版
        char            szEncrypt;//是否是加密 add by hwl ,0表示不加密，1表示普天 2表示数据进行AES加密
        char            isNeedCheckTime;////是否需要向设备同步时间，登录时用。0：不需要，1：需要，不同步时区, 3:同步时间与时区
        tuint32         softwarePackID;//软件包ID，样可以保证软件包之间不能相互连接，必须整体升级 采用NVMS_Define.h中的宏NVMS_VERSION 第一个数字左移24位，第二个数字左移16位，第三个数字左移8位
        tuint32         customerID;//客户ID，这样可以保证软件包之间不能相互连接，必须整体升级

        tuint32         clientNonce; //客户端认证服务器的随机值 ---wangyu 20180913 接入NVR1.3.5
        tuint32         reserve[7];  //保留
        //tuint32        reserve[8];
    } ECMS_NET_LOGIN_INFO, *PECMS_NET_LOGIN_INFO;
    //登陆时用的结构体

    //注意服务器登陆时如果没有配置内外网IP，就获取本地全部IP排序后存储InIP，OutIP全部为0
    //在到了注册服务器后如果发现OutIP为0，就将客户端连接过来的IP地址记录为A，客户端发过来的连接IP计为B
    //      如果A为127.0.0.1，OutIP全部使用InIP，否则
    //      如果InIP数量只有一个，OutIP就取A（可能和In一样）否则
    //      A和InIP比较，如果发现有相同的就将OutIP全部使用InIP，否则
    //      就试图找B，如果找到了，OutIP对应的IP取A，其它全部取InIP对应值，否则
    //      就OutIP全部使用InIP（这种情况下一般不可能发生）
    typedef struct _ecms_net_ready_info
    {
        GUID            uniqueIdentifier;//唯一标识GUID
        char             bNeedCfgPack;////是否需要配置数据包
        char                rev[3];//保留字节
    } ECMS_NET_READY_INFO, *PECMS_NET_READY_INFO;
    //内外网IP通过XML附加在此结构体后面传输

    //
    //  typedef struct _ecms_net_set_time
    //  {
    //      char szData[256];
    //      char                rev[3];//保留字节
    //  }ECMS_NET_SET_TIME, *PECMS_NET_SET_TIME;

    typedef struct _ecms_net_modify_password
    {
        GUID userID;//更改密码时暂时不用，直接根据当前登录用户来确定，以后可能会用到
        char oldPwd[64];//原来的密码
        char newPwd[64];//新密码
    } ECMS_NET_MODIFY_PASSWORD;

    //////////统一失败回复结构体
    typedef struct _ecms_net_reply
    {
        tuint32   dwResult;//错误码
        GUID      nodeID;//在哪个节点上应答回来的错误
        unsigned short  wDescriptionLen;//后面描述数据的实际有效长度
        char    szDescription[246];//一般情况下定义为错误描述，但也可以由具体协议自定义具体内容
    } ECMS_NET_REPLY_RESULT;

#define ENABLE_INDUSTRY_THERMALEX 0 //工业测温模块开关
    /////////////登陆成功的回复信息
    typedef struct _net_login_success_info
    {
        GUID    remoteNodeID;////网络节点的ID
        tuint32 remoteNodeType;////网络节点的类型
        char        strRemoteNodeName[24];////////网络节点的名称
        char IsEnableIndustryThermalEx;//1-启用工业测温模块 0-不启用工业测温模块
        char typeBySpecialVersionControl;//特殊版本控制-NVMS_SpecialVersionControl.h
        char IsEnableUploadLicense;//管理服务器是否允许上传license 0不允许 1允许  2020-08-24从strRemoteNodeName分出 Ubuntu实现客户端模式，客户端不能单纯通过是否是Ubuntu来判断是否显示上传license的按钮
        char StandardClientUseThermalImage;//;//目前仅对clients分支有效 ,2021年1月5日后AI和trunk默认都带测温模块，0-标准版客户端不允许显示测温模块 1-忽略该字段,用客户端自己的判断逻辑 (TSS版本传1)2020-07-29从strRemoteNodeName分出 防止TSS客户端登录A300-MTC后有测温功能
        char  clientDigest[20];              // 客户端返回的digest值

        // 传输加密秘钥=AES(md5(随机数)，key)，加密使用的key是登录用户密码的md5值
        char  transportEncryptKey[16];       // 服务端传输加密秘钥
        GUID    nodeID;//////////发送登录信息节点的ID,如果发送登录信息的节点不知道自己的ID就填GUID_NULL
        GUID            guidPCSerial;                   //返回应用程序上次成功登陆RSU的  用做系统节点之间的认证
        ////////////////////////////
        GUID            sysRSUSerial; /////系统认证服务器的序列号（GUID）只有登录RSU的时候此字段才有效
    } ECMS_LOGIN_SUCCESS_INFO, NETDEVICE_LOGIN_SUCCESS_INFO;
    //Device Caps   XML附加在此结构体后面传输

    const unsigned int ECMS_LOGIN_SUCCESS_INFO_LENGTH = sizeof(ECMS_LOGIN_SUCCESS_INFO);

    /////////////以后可能可以发一个设备内的多个通道联合在一起的这玩意
    typedef struct _net_stream_ctrl_info
    {
        union
        {
            struct {
                tuint8 szIP[64];
                tuint16  uPort;
                tuint16 chNumIndex;
            } nodeID_IP_Port;////second辅助的，二号
            GUID    nodeID_GUID;///通道节点和设备节点的ID  请求对讲的时候是设备节点的ID
        } nodeID;
        tuint32 streamType; ////主码流 或 子码流ECMS_STREAM_TYPE
        unsigned char   chNO;////通道号
        unsigned char   byPlayAudio;//是否请求声音数据，此变量现在没有实际含义，请慎用
        unsigned char   byReserve[2];////保留 byReserve[0] = 1;//1表示语音播报;byReserve[1]=1表示国标录像备份
    } NET_STREAM_CTRL_INFO;

    //命令处理参数
    //////路由信息Check
    typedef struct _route_check_info
    {
        GUID  curNodeID;////当前应用程序的节点ID
        GUID  first; //////从源到目的在此应用程序中的源标识
        GUID  second;/////从源到目的在此应用程序中的目的标识

        GUID  sourceNodeID;////////源节点的ID
        GUID  destNodeID;/////目的节点的ID
        tuint32  indicate;//标志符，区分路由信息
        tuint32  routeDirect;//路由方向 是否为正方向；非0正方向 sourceNodeID -> destNodeID; 0表示反方向 destNodeID -> sourceNodeID
    } ROUTE_CHECK_INFO;

    //////////////请求通道的录像数据
    /************************************************************************
    REC_DATA_SEARCH+NET_STREAM_CTRL_INFO(多个)
    ************************************************************************/
    typedef struct _rec_data_search
    {
        tuint32      dwStartTime;////开始时间
        tuint32      dwEndTime; /////结束时间
        tuint32      uEventTypeMASK;
        unsigned char         byChannelCount;///////通道数量
        unsigned char         byReserve; //保留，0表示正常请求回放，1表示请求当前时间向前偏移，dwStartTime填0，dwEndTime填偏移秒，2表示只回放关键帧。
        unsigned char         byReserve1[2]; //保留，对齐
    } REC_DATA_SEARCH;
    typedef enum _ecms_play_type_
    {
        ECMS_PLAYBACK_TYPE_DEFAULT = 0,
        ECMS_PLAYBACK_TYPE_ATONCE = 1,
        ECMS_PLAYBACK_TYPE_KEYFRAME = 2,
    } ECMS_PLAYBACK_TYPE;
    /************************************************************************
    ////////////////请求通道的录像事件的搜索
    REC_DATA_SEARCH+NET_REC_EVENT_CTRL_INFO(多个)
    ************************************************************************/
    typedef struct _net_rec_event_ctrl_info
    {
        union
        {
            struct {
                tuint8 szIP[64];
                tuint16  uPort;
                tuint16 chNumIndex;
            } nodeID_IP_Port;////second辅助的，二号
            GUID    nodeID_GUID;///通道节点和设备节点的ID  请求对讲的时候是设备节点的ID
        } nodeID;
        unsigned char      byChannelNum; //////通道号
        unsigned char      byWithDataLen;   //是否需要统计数据长度
        unsigned char      byReserve[2]; ////保留
    } NET_REC_EVENT_CTRL_INFO;


    //////////////请求通道的录像标签
    /************************************************************************
    REC_TAG_SEARCH+NET_REC_TAG_CTRL_INFO(0或多个)
    ************************************************************************/
    typedef struct _rec_tag_search
    {
        tuint32      dwStartTime;//开始时间
        tuint32      dwEndTime; //结束时间
        unsigned char     byChannelCount;//通道数量，目前只实现了填0全部通道和填1一个通道
        unsigned char     byReserve[3]; ///保留
    } REC_TAG_SEARCH;

    /************************************************************************
    ////////////////请求通道的录像标签的搜索
    REC_TAG_SEARCH+NET_REC_TAG_CTRL_INFO(0或1个，全部通道时为0个，1个通道时指明哪个通道)
    ************************************************************************/
    typedef struct _net_rec_tag_ctrl_info
    {
        GUID    nodeID_GUID;//通道节点和设备节点的ID
        unsigned char      byChannelNum; //通道号
        unsigned char      byReserve[3]; ////保留
    } NET_REC_TAG_CTRL_INFO;

    const int RECORD_TAG_MAXLEN = 127;
    //录像标签
    typedef struct _record_tag_log
    {
        GUID nodeID_GUID;   ////通道号
        tuint32 dwTime;     //时间   从1970 1 1号开始绝对时间的秒数
        unsigned char      byChannelNum; //通道号
        unsigned char      tagStr[RECORD_TAG_MAXLEN];//描述
    } REC_TAG_LOG;

    /************************************************************************
    //////////////////删除录像标签
    REC_TAG_LOG_DEL+NET_REC_TAG_DEL_INFO(1个或多个)
    ************************************************************************/
    typedef struct _record_tag_log_del
    {
        tuint32     byTagCount;//数量
    } REC_TAG_LOG_DEL;
    typedef struct _net_rec_tag_del_info
    {
        GUID    nodeID_GUID;//通道节点和设备节点的ID
        tuint32 dwTime;     //时间   从1970 1 1号开始绝对时间的秒数
        unsigned char      byChannelNum; //通道号
        unsigned char      byReserve[3]; ////保留
    } NET_REC_TAG_DEL_INFO;

    //
    // 请求通道的图片搜索
    // 请求NET_PIC_SEARCH_INFO，回复REC_PIC_LIST_LOG
    //
    typedef struct _net_pic_search_info
    {
        tuint32     dwStartTime;//开始时间
        tuint32     dwEndTime; //结束时间
        GUID        nodeID_GUID;//通道节点的ID
        tuint32     dwEventMask;//报警抓图类型
        tuint32     dwPageIndex;   //请求分页号
        tuint32     dwPageSize;    //页大小
        unsigned char      byReserve[128]; ////保留
    } NET_PIC_SEARCH_INFO;
    //图片
    typedef struct _net_pic_log
    {
        GUID nodeID_GUID;   ////通道号
        tuint32 dwTime;     //时间   从1970 1 1号开始绝对时间的秒数
        unsigned char      byChannelNum; //通道号
        unsigned char      byReserveCN[3]; ////保留
        unsigned char      byPicName[64];
        tuint32     dwPageIndex;   //请求分页号
        tuint32     dwPageSize;    //页大小
        tuint32     dwImageIndex;   //图片序号
        tuint32     dwEventMask;//报警抓图类型
        tuint32     dwPicTotals; //请求时间段图片总数
        unsigned char      byReserve[124]; ////保留
    } NET_PIC_LOG;

    //请求通行记录图片使用
    typedef struct _net_pms_passrecord_pic
    {
        tuint32 nPmsPassRecordIndex;//通行记录索引
        unsigned char byReserve[128]; ////保留
    } NET_PMS_PASSRECORD_PIC;

    typedef struct _net_PlateMatch_search
    {
        char szXML[4096];
        tuint32 dwLen;
    } NET_PLATEMATCH_SEARCH;

    typedef struct _net_PlateMatch_image
    {
        GUID chlId;//通道guid，到时候查询图片时直接传过来即可
        GUID pathGUID;
        tuint32 dwTime; //时间   从1970 1 1号开始绝对时间的秒数
        tuint32 Millisecond; //毫秒
        tuint32 recStartTime;   //录像开始时间
        tuint32 recEndTime; //录像结束时间
        tuint32 imgId;
        tuint16 sectionNo;
        tuint16 fileIndex;
        tuint16 blockNo;
        tuint16 offset;
        tuint16 eventType; //事件类型
        unsigned char isPanorama;//请求图片数据时用，原图1，车牌0
        unsigned char byReserve; ////保留
    } NET_PLATEMATCH_IMAGE;

    typedef struct _net_PlateMatch_image_data
    {
        char eventType[32];
        char targetType[64];
        char plateNumber[16];
        int imageDateLen;
        tuint64 imageDate;
    } NET_PLATEMATCH_IMAGE_DATA;
    /************************************************************************
    回复任务的索引
    ************************************************************************/
    typedef struct _reply_index_info
    {
        tuint32 dwIndexValue ;///////////对应的索引值
    } REPLY_INDEX_INFO;

    typedef struct _play_control_info
    {
        FILETIME ftFrameTime;           //回放操作时的帧时间
        tuint32 dwSectionTime;          //对应操作的时间段时间
    } PLAY_CONTROL_INFO;

    //TWIPC车牌订阅/退订与转发交互使用
    typedef struct cmdVehcleTopic
    {
        GUID nDevGuid;
        tuint32 typeCmd;
        tuint32 dataLen;
    } CMD_VEHCLETOPIC;
    ///////////报警状态信息
    typedef struct _net_alarm_state_info
    {
        tuint32 dwNodeType;//节点类型
        GUID nodeID;////节点ID
        tuint32      alarmType;//报警类型
        unsigned char byReserve[3];//保留
        unsigned char byisAlarm;///非0是报警状态，0是非报警状态
    } NET_ALARM_STATE_INFO;

    //人脸比对报警状态信息
    struct ALARM_STATE_FACE_MATCH
    {
        FILETIME  frameTime;   //帧时间
        tuint32 dwRealFaceID;  //抓拍人脸ID
        tuint32 dwGrpID;       //特征组ID
        tuint32 dwLibFaceID;   //特征库人脸ID
        tuint32 dwHandle;      //抓拍人脸数据图片长度
        tuint32 dwSimilar;     //相似度
        unsigned char byName[32]; //姓名
        GUID ChannelID;         //抓拍通道ID
        unsigned char bHasData;     //是否真有数据
        union
        {
            char* pPicData;             //传送报警给上层时，附上图片
            tuint64 FaultTolerance; //容错，上面一个指针!!!!!
        };

        ALARM_STATE_FACE_MATCH()
        {
            pPicData = NULL;
            bHasData = 0;
        }
        ALARM_STATE_FACE_MATCH& operator=(const ALARM_STATE_FACE_MATCH& faceMatchInfo)
        {
            if (this != &faceMatchInfo)
            {
                bHasData = faceMatchInfo.bHasData;
                frameTime = faceMatchInfo.frameTime;
                dwRealFaceID = faceMatchInfo.dwRealFaceID;
                dwGrpID = faceMatchInfo.dwGrpID;
                dwLibFaceID = faceMatchInfo.dwLibFaceID;
                dwHandle = faceMatchInfo.dwHandle;
                dwSimilar = faceMatchInfo.dwSimilar;
                memcpy(byName, faceMatchInfo.byName, sizeof(byName));
                ChannelID = faceMatchInfo.ChannelID;
                if (pPicData && faceMatchInfo.pPicData && faceMatchInfo.dwHandle > 0 && faceMatchInfo.dwHandle < 2000000)
                {
                    memcpy(pPicData, faceMatchInfo.pPicData, faceMatchInfo.dwHandle);
                }
            }
            return *this;
        }
    };

    ///////////存储服务器通道录像状态信息
    typedef struct _net_record_state_info
    {
        GUID storageID;//存储服务器的节点ID
        GUID nodeID;////通道节点ID
        tuint32 recordState;//录像状态
    } NET_RECORD_STATE_INFO;

    ///////////设备录像状态
    typedef struct _net_devrecord_state_info
    {
        GUID nodeID;////通道节点ID
        tuint32 recordState;//录像状态
    } NET_DEVRECORD_STATE_INFO;

    namespace ECMS_NET_PROTOCOL
    {
        //////////////////////////报警联动的信息
        typedef struct _to_ptz
        {
            GUID channelID;/////通道ID
            unsigned char byType;////类型
            unsigned char byPreset;////预置点
            unsigned char byCurise;////寻航线
            unsigned char byReserved; ////保留
        } TO_PTZ;

        typedef struct _to_alarm
        {
            GUID alarmOutID;/////传感器输出ID
            unsigned char byReserved[4]; ////保留
        } TO_ALARM_OUT;

        typedef struct _to_tvwall
        {
            GUID wallID;//墙ID，不是服务器
            unsigned char byReserved[4]; ////保留
        } TO_TVWALL;

    };//////end   namespace ECMS_NET_PROTOCOL

    //////////////////////////////////////////////////////////////////////////
    ////磁盘分区信息
    const int MAX_PARTITION_CNT = 100;
    typedef struct _net_partition_info
    {
        //  tuint8      byPartitionNO;      //分区序号
        tuint8      isOSUsed;           //操作系统所使用,0:未使用，1:使用
        tuint8      partState;          //分区状态，0：未初始化， 1：已初始化
        tuint8      mediaType;          //PARTITION_TYPE
        tuint8      noused;             //保留字节，对齐用
        GUID        partGUID;           //磁盘分区节点的GUID
        char        szPath[16];
        char        szName[32];
        tuint32     totalSpace;         //单位M，最大支持到4096T，足够
        tuint32     freeSpace;          //单位M，最大支持到4096T，足够
        tuint32     avaSpace;           //单位M，最大支持到4096T，可用空间
        GUID        msuGUID;            //存储服务器节点的GUID
    } NET_PARTITION_INFO, *LPNET_PARTITION_INFO;
    //////////////////////////////////////////////////////////////////////////
    ////国标级联网关服务信息
    typedef struct _net_cascade_info
    {
        GUID cguGUID;               //国标级联服务器节点的GUID
        char szGB28181ID[32];       //国标级联服务本地国标ID
        char szGBPwd[64];           //国标级联服务密码
        char reserve[64];           //保留字段
    } NET_CASCADE_INFO, *LPNET_CASCADE_INFO;

    /////////////////////////////////////////////////////////
    typedef enum _to_ptz_type
    {
        TO_PTZ_NULL,
        TO_PTZ_PRESET,
        TO_PTZ_CURISE
    } TO_PTZ_TYPE;

    /////////////////////////////////////////////////////////
    typedef struct _net_alarm_trigger_info : public NET_ALARM_STATE_INFO
    {
        NET_STREAM_CTRL_INFO      toStreamInfo; ///联动预览   联动录像  联动电视墙时的监控点 ID为无效的节点ID的时候
        ECMS_NET_PROTOCOL::TO_PTZ          toPTZ; ////联动云台
        ECMS_NET_PROTOCOL::TO_ALARM_OUT          toAlarmOut; ////联动传感器输出
        ECMS_NET_PROTOCOL::TO_TVWALL              toTVWall;//联动到电视墙
    } NET_ALARM_TRIGGER_INFO, *LPNET_ALARM_TRIGGER_INFO;

    //////////////////////////////////////////////////////////////////////////
    typedef struct _net_device_config_info
    {
        GUID    nodeID;             //设备节点的ID
    } NET_DEVICE_CONFIG_INFO;

    //控制云台时用
    typedef struct _net_PTZ_ctrl_info
    {
        tuint32  dwParam;       //控制参数：预置点序号，巡航线序号，轨迹序号等
        tuint32  dwSpeed;       //速度
        GUID     chID;          //通道ID
        tuint32   dwSubCmdType; /////子命令类型
        unsigned char   chNO;           //通道号
        unsigned char   isUsed;         //预置点是否已启用（1 启用  0 未启用）
        unsigned char   byReserved[2];  //保留
    } NET_PTZ_CTRL_INFO;

    //云台3D放大
    typedef struct _net_PTZ_3dctrl_info
    {
        tint32        dx;         // 范围: -1000到1000
        tint32        dy;         // 范围: -1000到1000
        GUID          chID;       // 通道ID
        tint16        zoom;       // 变倍倍数（缩小*-10 或 放大*10）
        unsigned char chNO;       // 通道号
        unsigned char byReserved; // 保留
    } NET_PTZ_3DCTRL_INFO;

    //获取预置点和巡航线时用
    typedef struct _net_get_PTZ_config_info
    {
        GUID        chID;           //通道ID
        unsigned char   chNO;           //通道号
        unsigned char   byReserved[3];  //保留
    } NET_GET_PTZ_CONFIG_INFO;

    /************************************************************************/
    /*云台预置点配置信息                                                    */
    /************************************************************************/
    typedef struct _ecms_ptz_preset_info
    {
        GUID chID;                  //通道ID
        tuint16 index;          //预置点序号 index原来是tuint32拆成tuint16+tuint16
        tuint16 nNoExist;       //0表示当前预置点已经配置了 1表示没有配置
        char szName[36];                //预置点名称
    } ECMS_PTZ_PRESET_INFO, *LPECMS_PTZ_PRESET_INFO;

    /************************************************************************/
    /*云台看守卫                                                  */
    /************************************************************************/
    typedef struct _ecms_ptz_guard_info
    {
        GUID chID;                  //通道ID
        tuint32 bSwitch;
        char strLocation[16];       //看守卫的位置
        tuint32 nNumber;            //编号
        tuint32 nWaitTime;          //等待时间
    } ECMS_PTZ_GUARD_INFO, *LPECMS_PTZ_GUARD_INFO;

    /************************************************************************/
    /*云台巡航线配置信息                                                    */
    /************************************************************************/
    typedef struct _ecms_ptz_cruise_info
    {
        GUID chID;                  //通道ID
        tuint32 index;          //巡航线序号
        char szName[36];                //巡航线名称
    } ECMS_PTZ_CRUISE_INFO, *LPECMS_PTZ_CRUISE_INFO;

    typedef struct _net_barrier_control_info
    {
        GUID chnGUID;                   //通道Guid
        tuint32 operateType;            //道闸操作 1-open, 2-close, 3-stop
        unsigned char   byReserve[128];//预留字段
    } NET_BARRIER_CONTROL_INFO;

    /************************************************************************/
    /* 设备更新通道名称信息                                                                     */
    /************************************************************************/
    typedef struct _ecms_net_update_ch_name
    {
        GUID  chNodeID;//////通道节点的ID
        unsigned short  chnn;////
        unsigned short  nameLen;
        char            name [64];//IPC的监控点名称超过36，加长结构体长度
    } ECMS_NET_UPDATE_CH_NAME, *LPECMS_NET_UPDATE_CH_NAME;

    /* 设备更新Sensor名称信息                                                                     */
    /************************************************************************/
    typedef struct _ecms_net_update_sensor_name
    {
        GUID  sensorNodeID;//////通道节点的ID
        unsigned short  sensornn;////
        unsigned short  nameLen;
        char            name [64];//sensor名称
    } ECMS_NET_UPDATE_SENSOR_NAME, *LPECMS_NET_UPDATE_SENSOR_NAME;

    /************************************************************************/
    /* 设备更新alarmOut名称信息                                                                     */
    /************************************************************************/
    typedef struct _ecms_net_update_alarmout_name
    {
        GUID  alrmOutNodeID;//////通道节点的ID
        unsigned short  alarmOutnn;////
        unsigned short  nameLen;
        char            name [64];//alrmOut名称
    } ECMS_NET_UPDATE_ALARMOUT_NAME, *LPECMS_NET_UPDATE_ALARMOUT_NAME;

    /************************************************************************/
    /* 设备更新通道国标ID    added by huwulin 20180718                                                                */
    /************************************************************************/
    typedef struct _ecms_net_update_ch_gb28181id
    {
        GUID  chNodeID;//////通道节点的ID
        unsigned short  chnn;////
        unsigned short  nodeGB28181IDLen;
        char            nodeGB28181ID [64];
    } ECMS_NET_UPDATE_CH_GB28181ID, *LPECMS_NET_UPDATE_CH_GB28181ID;

    typedef struct _ecms_net_update_ch_gat1400id
    {
        GUID  chNodeID;//////通道节点的ID
        unsigned short  chnn;////
        unsigned short  nodeGAT1400IDLen;
        char            nodeGAT1400ID [64];
    } ECMS_NET_UPDATE_CH_GAT1400ID, *LPECMS_NET_UPDATE_CH_GAT1400ID;

    //设备名称更新信息
    typedef struct _ecms_net_update_device_name
    {
        GUID  deviceNodeID;//////设备节点的ID
        unsigned short  nameLen;
        char            name [64];
        unsigned char   byReserve[2];////保留,对齐
    } ECMS_NET_UPDATE_DEVICE_NAME, *LPECMS_NET_UPDATE_DEVICE_NAME;

    //获取jpeg图片
    typedef struct _net_get_JPEG_picture_info
    {
        GUID        chID;           //通道ID
        unsigned char   chNO;           //通道号
        unsigned char   byReserved[3];  //保留
    } NET_GET_JPEG_PICTURE_INFO;

    //获取NVR报警关联录像通道
    typedef struct _net_get_NVR_alarmchl_info
    {
        GUID        chID;           //通道ID
        tuint32 nAlarmType;
        unsigned char   chNO;           //通道号
        unsigned char   triggerType; //0表示查询联动录像 1表示查询联动图片
        unsigned char   byReserved[2];  //
    } NET_GET_NVR_ALARMCHL_INFO;

    //测试登录设备
    typedef struct _net_test_login_dev_info
    {
        tuint32 nDevType;
        char szIP[128];
        tuint32 nPort;
        char szLoginUser[128];
        char szLoginPwd[128];
        char szUrl[512];
        unsigned char   byReserved[4];  //保留
    } NET_TEST_LOGIN_DEV_INFO;
    /*add by wangxin 用于传输到管理服务器 设备或通道附加信息更新                                                                     */
    typedef struct _ecms_net_update_node_append_info
    {
        //因为GUID在数据库中没有做限制，设备表和通道表中是可以重复的所以不用GUID
        GUID  chNodeID;//节点的ID
        char szAppendInfo[CFGPACK_DEFINE::APPEND_INFO_BUF_LENGTH];//#include "ConfigPack_Struct_Def.h" 根据此头文件中的宏定义的长度
    } ECMS_NET_UPDATE_NODE_APPEND_INFO, *LPECMS_NET_UPDATE_NODE_APPEND_INFO;

    //
    // 从服务器下载文件的文件类型
    //
    typedef enum _ECMS_DOWN_FILE_TYPE
    {
        ECMS_DOWN_FILE_TYPE_RANGE_BEGIN,
        ECMS_DOWN_FILE_TYPE_SERVER_INI,//服务器INI，目前只用到了注册服务器的用于配置的备份
        ECMS_DOWN_FILE_TYPE_RANGE_END
    } ECMS_DOWN_FILE_TYPE;

    /************************************************************************/
    /* UPLOAD文件的文件类型*/
    /************************************************************************/
    typedef enum _ECMS_UPLOAD_FILE_TYPE_
    {
        ECMS_UPLOAD_FILE_TYPE_RANGE_BEGIN = 0x2000,
        ECMS_UPLOAD_FILE_TYPE_CONFIG_FILE,
        ECMS_UPLOAD_FILE_TYPE_CERT_FILE,
        ECMS_UPLOAD_FILE_TYPE_UPGRAD_FILE,
        ECMS_UPLOAD_FILE_TYPE_ALARM_AUDIO_FILE,
        ECMS_UPLOAD_FILE_TYPE_ALARM_IPCAUDIO_FILE,
        ECMS_UPLOAD_FILE_TYPE_RANGE_END
    } ECMS_UPLOAD_FILE_TYPE;

    typedef struct _ecms_net_down_file
    {
        unsigned char bFileType;//文件类型
        unsigned char Reserve[3];///保留
    } ECMS_NET_DOWN_FILE, *LPECMS_NET_DOWN_FILE;

    //上传文件信息
    //上传文件信息
    typedef struct _ecms_net_upload_file_info_v2
    {
        tuint32 szFileType; //ECMS_UPLOAD_FILE_TYPE
        tuint32 dwFileLen;
        char            username[64];//用户姓名
        char            password[64];//用户密码
        char    szUpgradeAuthInfo[4096];
    } ECMS_NET_UPLOAD_FILE_INFO_V2;

    //上传文件信息
    typedef struct _ecms_net_down_upload_file_info
    {
        char szFileTypeIdentify[128];   //"SYSTEM_UPGRAD_FILE", "IMPORT_CONFIG_FILE"  "EXPORT_CONFIG_FILE" "VOICE_BROADCAST_FILE"
        tuint32 dwFileLen;
        char szFileAuthInfo[64];
    } ECMS_NET_DOWN_UPLOAD_FILE_INFO;

    //上传文件信息
    typedef struct _ecms_net_down_upload_file_info_ex
    {
        char szFileTypeIdentify[128];   //"SYSTEM_UPGRAD_FILE", "IMPORT_CONFIG_FILE"  "EXPORT_CONFIG_FILE" "VOICE_BROADCAST_FILE"
        tuint32 dwFileLen;
        char szFileAuthInfo[64];
        char szUpgradeAuthInfo[4096];
    } ECMS_NET_DOWN_UPLOAD_FILE_INFO_EX;

    /************************************************************************/
    /* 搜索日志信息                 BEING                                               */
    /************************************************************************/
    typedef struct _ecms_search_log_info
    {
        tuint32 dwLogTypeMask;////一般为所有
        tuint32 dwStartTime;////开始时间     单位为绝对的秒数
        tuint32 dwEndTime;//结束时间    单位为绝对的秒数
        GUID    userID; ///如果用户ID为0表示用户ID无效
        unsigned char      byState;/////状态
        unsigned char      byReserve[2];////保留
        unsigned char      byContentNum;////为0表示所有的警情
        unsigned char dwInputSrcNum;//////为0表示所有输入源
        unsigned char   byReserve1[3];////保留,对齐
    } ECMS_SEARCH_LOG_INFO, *LPECMS_SEARCH_LOG_INFO;

    ///////////警情的关键字
    typedef struct _ecms_search_log_key_content
    {
        char szContent[36];////内容信息
    } ECMS_SEARCH_LOG_KEY_CONTENT, *LPECMS_SEARCH_LOG_KEY_CONTENT;

    ///////////报警源的关键字
    typedef struct _ecms_search_log_key_input_src
    {
        tuint32  dwNodeType;/////节点类型
        GUID    nodeID;//节点ID
        unsigned char      bySubIndexNum;//子索引信息  0xFF表示忽略子索引
        unsigned char      byReserve[3];
    } ECMS_SEARCH_LOG_KEY_INPUT_SRC, *LPECMS_SEARCH_LOG_KEY_INPUT_SRC;

    //add
    typedef struct _ecms_search_log_info_create_query_record
    {
        tuint32 dwLogTypeMask;////一般为所有
        tuint32 dwStartTime;////开始时间     单位为绝对的秒数
        tuint32 dwEndTime;//结束时间    单位为绝对的秒数
        GUID    userID; ///如果用户ID为0表示用户ID无效
        unsigned char      byReserve[4];////保留
    } ECMS_SEARCH_LOG_INFO_CREATE_QUERY_RECORD, *LPECMS_SEARCH_LOG_INFO_CREATE_QUERY_RECORD;

    //创建查询成功后的返回结构体
    typedef struct _ecms_search_log_info_create_query_record_success
    {
        tuint32 handle;////查询句柄
        unsigned char      byReserve[4];////保留
    } ECMS_SEARCH_LOG_INFO_CREATE_QUERY_RECORD_SUCCESS, *LPECMS_SEARCH_LOG_INFO_CREATE_QUERY_RECORD_SUCCESS;

    typedef struct _ecms_search_log_info_get_record_total_count
    {
        tuint32 handle;//查询句柄
        unsigned char      byReserve[4];////保留
    } ECMS_SEARCH_LOG_INFO_GET_RECORD_TOTAL_COUNT, *LPECMS_SEARCH_LOG_INFO_GET_RECORD_TOTAL_COUNT;

    //查询总条数成功后的返回结构体
    typedef struct _ecms_search_log_info_get_record_total_count_success
    {
        tuint32 handle;//查询句柄
        tuint32 total_count;////总条数
        unsigned char      byReserve[4];////保留
    } ECMS_SEARCH_LOG_INFO_GET_RECORD_TOTAL_COUNT_SUCCESS, *LPECMS_SEARCH_LOG_INFO_GET_RECORD_TOTAL_COUNT_SUCCESS;


    typedef struct _ecms_search_log_info_get_record
    {
        tuint32 handle;//查询句柄
        tuint32 startIndex;
        tuint32 count;
        unsigned char      byReserve[4];////保留
    } ECMS_SEARCH_LOG_INFO_GET_RECORD, *LPECMS_SEARCH_LOG_INFO_GET_RECORD;

    typedef struct _ecms_search_log_info_destroy_query_record
    {
        tuint32 handle;//查询句柄
        unsigned char      byReserve[4];////保留
    } ECMS_SEARCH_LOG_INFO_DESTROY_QUERY_RECORD, *LPECMS_SEARCH_LOG_INFO_DESTROY_QUERY_RECORD;

    typedef struct _ecms_search_pms_pass_record
    {
        CFGPACK_DEFINE::PMS_LOG_PASS_RECORD logPassRecord;
        tuint32 dwStartTime;
        tuint32 dwEndTime;
        tuint32 pageIndex;//第几页，从1开始
        tuint32 pageSize;//每页条数
        tuint32 totalCount;
        unsigned char byReserve[4];//保留
    } ECMS_SEARCH_PMS_PASS_RECORD, *LPECMS_SEARCH_PMS_PASS_RECORD;

    /************************************************************************/
    /* 搜索日志信息            END                                                */
    /************************************************************************/

    /************************************************************************/
    /* 写日志信息            BEGIN                                                */
    /************************************************************************/
#define  MAX_LOG_ST_LEN 1024*12

#define MAX_LOG_CONTENT_LEN 1024*3

#define MAX_LOG_ALARMTRIGGER_LEN 1024*8

    typedef struct _ecms_write_alarm_log
    {
        tuint64 dwLogID;//日志类型
        tuint32 tTime;//接收到的报警utc时间
        tuint64 dwLogKey;//Logkey
        GUID NodeGUID;
        char szNodeName[256];
        tuint32 dwNodeType;
        char szContent[MAX_LOG_CONTENT_LEN];
        char szAlarmTrigger[MAX_LOG_ALARMTRIGGER_LEN];   //new add
        BYTE AlarmState;
        unsigned char      byReserve[3];////保留，对齐

    } ECMS_WRITE_ALARM_LOG, *LPECMS_WRITE_ALARM_LOG;

    typedef struct _ecms_write_operate_log
    {
        tuint64 dwLogID;
        GUID UserGUID;
        char szUserName[64];
        tuint32 clientType;
        GUID OperateNodeGUID;//用户操作的目标节点
        tuint32 dwOcrTime;  //发生时间
        GUID LinkGUID[1024 / sizeof(GUID)];
        int nGUIDNumber;
        char szContent[MAX_LOG_CONTENT_LEN];
        unsigned char      byReserve[4];////保留
    } ECMS_WRITE_OPERATE_LOG, *LPECMS_WRITE_OPERATE_LOG;

    typedef struct _ecms_write_config_log
    {
        tuint64 dwLogID;
        GUID UserGUID;
        char szUserName[64];
        tuint32 clientType;
        GUID OperateNodeGUID;
        char szContent[1024];
        unsigned char      byReserve[4];////保留，对齐
    } ECMS_WRITE_CONFIG_LOG, *LPECMS_WRITE_CONFIG_LOG;

    typedef struct _ecms_write_exception_log
    {
        tuint64 dwLogID;
        GUID NodeGUID;
        char szContent[1024];
        tuint32 time;//异常时间，如果其它组件有传过来就用，没有就生成时间。
    } ECMS_WRITE_EXCEPTION_LOG, *LPECMS_WRITE_EXCEPTION_LOG;

    typedef struct _ecms_write_info_log
    {
        tuint64 dwLogID;
        GUID UserGUID;
        char szUserName[64];
        tuint32 clientType;
        GUID OperateNodeGUID;
        char szContent[1024];
        unsigned char      byReserve[4];////保留，对齐
    } ECMS_WRITE_INFO_LOG, *LPECMS_WRITE_INFO_LOG;

    typedef struct _ecms_write_abnormal_log
    {
        char szText[255];
        char szFileName[255];
        unsigned char byReserve1[2];////保留，对齐
        tuint32 line;
        tuint32 version;
        unsigned char      byReserve[4];////保留，对齐
    } ECMS_WRITE_ABNORMAL_LOG, *LPECMS_WRITE_ABNORMAL_LOG;
    /************************************************************************/
    /* 写日志信息            END                                                */
    /************************************************************************/

    /************************************************************************/
    /* 清除日志信息            END                                                */
    /************************************************************************/
    typedef struct _ecms_remove_history_log
    {
        tuint32 dwTime;
        unsigned char      byReserve[4];////保留
    } ECMS_REMOVE_HISTORY_LOG, *LPECMS_REMOVE_HISTORY_LOG;
    /************************************************************************/
    /* 清除日志信息            END                                                */
    /************************************************************************/

    typedef struct _ecms_right_check_info
    {
        tuint32 opTyoe;//操作类型
        GUID  destID;//操作对象ID
        tuint32 opParam1;
        tuint32 opParam2;
    } ECMS_RIGHT_CHECK_INFO, *LPECMS_RIGHT_CHECK_INFO;

    typedef struct _ecms_set_channel_color
    {
        GUID   chID;            //通道ID
        unsigned char   chNO;           //通道号
        unsigned char   byReserved[3];  //保留
        tuint32 brightness;
        tuint32 contrast;
        tuint32 saturation;
        tuint32 hue;
    } ECMS_SET_CHANNEL_COLOR;

    typedef struct _ecms_set_time
    {
        tuint32 tTime;//utc时间
    } ECMS_SET_TIME;

    typedef struct _ecms_host_set_time
    {
        ECMS_SET_TIME setTime;
        char szHostFlag[1024];//主机标识，主机标识相同时无需校时
    } ECMS_HOST_SET_TIME;

    typedef struct _ecms_manual_storage
    {
        tint32 tManualCmd;
        GUID chlGuid;
    } ECMS_MANUAL_STORAGE;


    typedef struct _ecms_set_ip_info
    {
        char szIpList[1024];//多IP地址逗号分隔
    } ECMS_SET_IP_INFO;

    typedef struct _ecms_req_sub_smart_stream_info
    {
        GUID chlGuid;
    } ECMS_REQ_SUB_SMART_STREAM;

    enum NVMS_AF_CTRL_TYPE
    {
        NVMS_AF_CTRL_TYPE_RANGE_BEGIN,
        ONEKEY_FOCUS,
        ZOOM,
        MANUAL_FOCUS,
        AUTO_FOCUS,
        NVMS_AF_CTRL_TYPE_RANGE_END,
    };

    typedef struct _nvms_af_control_
    {
        tuint32          afType;
        tuint32          nValue;

        GUID     chGuid;            //通道GUID
        unsigned char   chNO;           //通道号
        unsigned char   byReserve[3];////保留，对齐
    } NVMS_AF_CONTROL;

    //现场权限请求细分类型,只在监控客户端使用和解析,注意不能大于255
    enum LIVE_RIGHT_TYPE
    {
        LIVE_RIGHT_TYPE_LIVE_PREVIEW = 1,
        LIVE_RIGHT_TYPE_RECORD,
        LIVE_RIGHT_TYPE_DWELL_GROUP,
        LIVE_RIGHT_TYPE_DWELL_CHANNEL,
        LIVE_RIGHT_TYPE_CHANNEL_SETTING,//通道配置时请求
        LIVE_RIGHT_TYPE_EMAP_PREVIEW,//电子地图自动弹出画面
        LIVE_RIGHT_TYPE_ALARM_POP,//报警大画面预览
        LIVE_RIGHT_TYPE_START_RESUME,//程序启动时恢复显示
    };

    typedef struct
    {
        tuint32     degree;     //度表示dddmm
        tuint32     minute;     //分表示sscc
    } ECMS_DEGREE;

    //GPS信息，固定为64个字节
    typedef struct _ecms_gps_info_
    {
        tuint32 curTime;                                //当前时间； 4个字节

        /*GPS信息开始 26个字节*/
        ECMS_DEGREE         longitude;                  //经度；8个字节
        ECMS_DEGREE         latitude;                   //纬度；8个字节
        unsigned short          speedhight;                 //速度高位单元，小数点前数字,2字节
        unsigned short          speedlow;                   //速度低位单元，小数点后数字,2字节
        unsigned short          directionhigh;              //方向高位单元，小数点前数字,2字节
        unsigned short          directionlow;               //方向低位单元，小数点后数字,2字节
        unsigned char           lo_aspect;                  //经度方向；1个字节
        unsigned char           la_aspect;                  //纬度方向；1个字节

        /*g-sensor信息 */                         //共6个字节
        unsigned short          xpos;                       //x坐标
        unsigned short          ypos;                       //y坐标
        unsigned short          zpos;                       //z坐标
        ///
        unsigned short          McuSpeedHigh;               //速度 整数部分 2个字节
        unsigned char           McuSpeedLow;                //速度 小数部分 1个字节
        unsigned char           temperture;                 //温度 1个字节

        //////////////////////////////
        /*********************************************************************************************************************************
        0000    |0      |0      |0  |0  |0000   |0000
        预留4位  |倒车 |刹车 |右    |左    |预留4位 |AlarmIn
        *********************************************************************************************************************************/
        unsigned short          SensorAlarm;        //前8位为C-sensor报警(前8位从低位开始的四位依次为左，
        //右，刹车，倒车),后4位为AlarmIn报警；2个字节
        /*********************************************************************************************************************************
        0000    |0          |   0               |      0            |         0
        保留4位  |速度来源   |温度单位           |     速度单位      |    加热状态
                |0:GPS 1:CAR    |0:摄氏度/1:华氏度    |0：公里 /1：海里 |1:正在加热,0:未加热
        *********************************************************************************************************************************/
        unsigned char           MoreInfo;   //具体信息见上注释  1个字节

        /* add in 2012.10.9 begin*/  //这一段暂时还未填充数据
        unsigned char           AdvSpeed;   //每分钟平均速度accis码
        /*********************************************************************************************************************************
        0   |0   |0       |0      |0       |0       |0         |0   |0   |0     |0     |0     |0     |0  |00
        经度|纬度|紧急报警|车辆控制超时报警|震动报警|主电源断电|备用|刹车|门边线|左转灯|右转灯|远光灯|acc|备用
        *********************************************************************************************************************************/
        unsigned short          CarState;                    //每分钟车辆状态
        unsigned char           HightCarMill;                //当前时间里程高字节bcd码
        unsigned char           MidCarMill;                  //当前时间里程中字节bcd码
        unsigned char           MidLowMill;                  //当前时间里程低字节bcd码
        /* add in 2012.10.9 end*/

        // add 2012.11.22
        unsigned char           InCount;        //上车人数
        unsigned char           OutCount;       //下车人数

        unsigned char reserve[13];      //预留字节

        _ecms_gps_info_()
        {
            curTime = 0;
            memset(&longitude, 0, sizeof(ECMS_DEGREE));
            memset(&latitude, 0, sizeof(ECMS_DEGREE));
            speedhight = 0;                 //速度高位单元，小数点前数字,2字节
            speedlow = 0;               //速度低位单元，小数点后数字,2字节
            directionhigh = 0;              //方向高位单元，小数点前数字,2字节
            directionlow = 0;           //方向低位单元，小数点后数字,2字节
            lo_aspect = 0;              //经度方向；1个字节
            la_aspect = 0;                  //纬度方向；1个字节
            xpos = 0;                       //x坐标
            ypos = 0;                   //y坐标
            zpos = 0;                   //z坐标
            McuSpeedHigh = 0;           //速度 整数部分 2个字节
            McuSpeedLow = 0;                //速度 小数部分 1个字节
            temperture = '\0';              //温度 1个字节
            SensorAlarm = 0;        //前8位为C-sensor报警(前8位从低位开始的四位依次为左，
            MoreInfo = '\0'; //具体信息见上注释  1个字节
            AdvSpeed = '\0'; //每分钟平均速度accis码
            CarState = 0;                    //每分钟车辆状态
            HightCarMill = '\0';             //当前时间里程高字节bcd码
            MidCarMill = '\0';               //当前时间里程中字节bcd码
            MidLowMill = '\0';               //当前时间里程低字节bcd码
            InCount = '\0';     //上车人数
            OutCount = '\0';    //下车人数
            memset(reserve, 0, sizeof(reserve));
        }
    } ECMS_GPS_INFO, *PECMS_GPS_INFO;


    ////////透明传输
    typedef struct _ecms_transparent_transport
    {
        char szXML[4096];
        tuint32 dwLen;
    } ECMS_TRANSPARENT_TRANSPORT;


    typedef struct _api_config_info
    {
        char            username[64];
        char            strCMD[64];
    } API_CONFIG_INFO;

    //开始搜索设备结构体
    typedef struct _ecms_iptool_start_search_info
    {
        tuint32 SearchTypeMask;/*=SEARCH_TVT_STANDARD_DEV*/
        int nMaxRecordCount;/*=200*/
        unsigned char reserve[3];       //预留字节
        _ecms_iptool_start_search_info()
        {
            /*
            typedef enum _search_type_
            {
            SEARCH_TVT_STANDARD_DEV =0x020,
            SEARCH_ONVIF_DEV        =0x040,
            SEARCH_UPNP_DEV         =0x080
            }SEARCH_TYPE;
            */
            SearchTypeMask = 0x020/*SEARCH_TVT_STANDARD_DEV*/;
            nMaxRecordCount = 2000;
            memset(reserve, 0, sizeof(reserve));
        }
    } ECMS_IPTOOL_START_SEARCH_INFO, *LPECMS_IPTOOL_START_SEARCH_INFO;
    //搜索结果上报结构体
    typedef struct _ecms_iptool_send_dev_info
    {
        GUID Server_GUID;//表示属于哪个转发服务或者电视墙服务查找出来的
        int opt;//表示当前是1上线，或2下线，或3更新 主要用于管理服务器管理list用
        int nAutoReport;//0表示被动设备，1表示主动设备
        char    devName[32];
        char    szIpcIp[20];
        char    szMask[20];
        char    szIpcIp2[20];
        char    szRoute[20];
        char    szDns1[20];
        char    szDns2[20];
        int     nPort;
        int     nHttpPort;
        char    szStrMac[64];
        char    szDevType[16];//设备类型
        char    szSoftware[16];
        char    szBuildDate[16];
        char    szHardware[16];//硬件版本
        char    szKernel[16];
        char    szProduct[24];//productModel型号
        char    szSysRunTime[64];
        char    szSysStartTime[20];
        char    szManufacturer[64];//厂家
        tuint32 nAutoReportDevID;//主动上报设备的设备ID
        tuint64 tReportTime;//记录主动设备上报到转发时的时间
        _ecms_iptool_send_dev_info()
        {
            Server_GUID = GUID_NULL;
            opt = 1;
            nAutoReport = 0;
            memset(devName, 0, 32);
            memset(szIpcIp, 0, 20);
            memset(szMask, 0, 20);
            memset(szIpcIp2, 0, 20);
            memset(szRoute, 0, 20);
            memset(szDns1, 0, 20);
            memset(szDns2, 0, 20);
            nPort = 0;
            nHttpPort = 80;
            memset(szStrMac, 0, 64);
            memset(szDevType, 0, 16);
            memset(szSoftware, 0, 16);
            memset(szBuildDate, 0, 16);
            memset(szHardware, 0, 16);
            memset(szKernel, 0, 16);
            memset(szProduct, 0, 24);
            memset(szSysRunTime, 0, 64);
            memset(szSysStartTime, 0, 20);
            memset(szManufacturer, 0, 64);
            nAutoReportDevID = 0;
            tReportTime = 0;
        }
    } ECMS_IPTOOL_SEND_DEV_INFO, *LPECMS_IPTOOL_SEND_DEV_INFO;

    struct LAN_SERVER
    {
        GUID    SearchGUID;//服务临时生成的用于搜索唯一标识
        bool    bUsed;//是否被使用
        char    szIP[64];
        char    szMac[64];
        char    byReserve[1];////保留，对齐
        unsigned short  nPort;
        tuint32 nServerType;
        char    szVersion[64];
    };

    typedef struct _ecms_net_band_info
    {
        tuint32 nAllBandWidth;//总带宽
        tuint32 nResBandWidth;//剩余带宽
        _ecms_net_band_info()
        {
            nAllBandWidth = 0;
            nResBandWidth = 0;
        }
    } ECMS_NET_BAND_INFO, *LPECMS_NET_BAND_INFO;

    typedef struct ECMS_ALARM_INFO
    {
        tuint32 tTime;//转发或者接入接收到报警的utc时间
        char szCID[5];//接入服务器接入报警主机用到
        char szRes[3];//保留
        char szAlarmContent[MAX_LOG_CONTENT_LEN - sizeof(ALARM_STATE_FACE_MATCH) - sizeof(tuint64)];
        tuint64 nLogKey;//数据库LogKey从szAlarmContent分出来的空间
        ALARM_STATE_FACE_MATCH faceMatchInfo;
        ECMS_ALARM_INFO()
        {
            tTime = 0;
            nLogKey = 0;
            memset(szCID, 0, sizeof(szCID));
            memset(szRes, 0, sizeof(szRes));
            memset(szAlarmContent, 0, sizeof(szAlarmContent));
        }
    }* PECMS_ALARM_INFO;

    typedef struct ECMS_STATE_INFO_
    {
        char    byReserve[4];////保留
    } ECMS_STATE_INFO;

    /************************************************************************/
    /*                   电视墙服务器相关结构定义                           */
    /************************************************************************/

    //客户端和电视墙服务器通信协议
    typedef struct _ecms_tvwall_opt_info
    {
        tuint32 nOptType;       //操作的类型,参考E_MTVW_TYPE类型定义
        tuint32 nLen;           //协议长度
    } TVWALL_OPT_INFO, *PTVWALL_OPT_INFO;


    /************************************************************************/
    /*                  智能分析服务相关结构定义                             */
    /************************************************************************/
    // 监控客户端与智能分析服务器的协议头
    typedef struct _ecms_iasrv_cmd_info
    {
        tuint32 nCmdType;   // 命令类型
        tuint32 nDataLen;   // 协议长度, 不包含协议头
        tuint32 nSenderId;  // 发送数据端的nodeId， 服务端主动更新时有效
    } IASrv_CMD_INFO, *P_IASrv_CMD_INFO;

    // 智能分析服务器与 转发服务器之间数据交互协议头
    typedef struct _ecms_ans_msu_cmd_info
    {
        tuint32 nCmdType;   // 命令类型
        tuint32 nDataLen;   // 协议长度
        tuint32 nSenderId;  // 发送数据端的nodeId， 转发主动推送给智能分析服务器时有效
        GUID    nDevGuid;
    } Ans_Mdu_CMD_INFO, *P_Ans_Msu_CMD_INFO;

    typedef Ans_Mdu_CMD_INFO        Mdu_FrNvr_CMD_INFO;
    typedef Ans_Mdu_CMD_INFO        Ans_Update_Cmd_Info;
    // 智能分析服务器与 转发服务器之间数据交互协议头
    typedef struct _process_info
    {
        tuint32 m_process;
        tuint32 m_bFinish;
        tuint32 updateStatu;
        GUID    nDevGuid;
    } PROCESS_INFO, *P_PROCESS_INFO;

    typedef Ans_Mdu_CMD_INFO        Mdu_FrNvr_CMD_INFO;

    // 存储服务与 转发服务器之间数据交互协议头
    typedef struct _ecms_mdu_to_msu_cmd_info
    {
        tuint32 nCmdType;   // 命令类型
        tuint32 nDataLen;   // 协议长度
        tuint32 nSenderId;  // 发送数据端的nodeId， 转发主动推送给存储服务时有效
        GUID    nDevGuid;
    } Mdu_To_Msu_CMD_INFO, *P_Mdu_To_Msu_CMD_INFO;

    // 智能分析服务器与 1400In服务器之间数据交互协议头
    typedef struct _ecms_ans_1400_cmd_info
    {
        tuint32 nCmdType;   // 命令类型
        tuint32 nDataLen;   // 协议长度
        GUID    nChlGuid;   // 通道GUID
        char byReserve[128];//预留
    } Ans_1400_CMD_INFO, *P_Ans_1400_CMD_INFO;
    /************************************************************************/

    //测试邮箱
    typedef struct _test_smtp_para
    {
        unsigned short dwSSL;   //SSL_AUTH_MODE_NONE SSL_AUTH_MODE_SSL SSL_AUTH_MODE_TLS
        unsigned short nPort;   //端口
        char pFromaddr[64];
        char pSenderName[64];  //发件人姓名（别名）
        char pUserName[32];
        char pPassword[32];
        char pServAddr[32];
        char pToAddr[64];
    } TEST_SMTP_PARA;

    /*                   系统运行状态相关结构定义                           */
    /************************************************************************/
    typedef struct _ecms_system_runstate_info
    {
        tuint32 nState;     //状态类型 0-离线 1-在线
        tuint32 nLastOnLineTime;            //最后上线时间/开始录像时间
        tuint32 nLastOffLineTime;           //最后下线时间/停止录像时间

        char szUser[64];//记录用户名,用于CS客户端上下线
        GUID msuGUID;//存储服务器GUID 通道录像状态用到
        char szMsuName[256];//存储服务器名称 通道录像状态用到
        char szIPAddr[400];//登录节点地址，用于写入客户端登录登出日志
        tuint32 nNodeType;//节点类型
        char szIsOtherType;//是否为其他类型采用MCU类型登录  1是解码器 2是平台SDK
        _ecms_system_runstate_info()
        {
            nState = 0;
            nLastOnLineTime = 0;
            nLastOffLineTime = 0;

            memset(szUser, 0, sizeof(szUser));
            msuGUID = GUID_NULL;
            memset(szMsuName, 0, sizeof(szMsuName));
            memset(szIPAddr, 0, sizeof(szIPAddr));
            nNodeType = 0;
            szIsOtherType = 0;
        }
    } SYSTEM_RUNSTATE_INFO, *PSYSTEM_RUNSTATE_INFO;

    //上报配置服务器端口的结构体
    typedef struct _ecms_report_config_port
    {
        tuint32 nConfigPort;
        tuint32 nPmsParkOverHour;   //PMS超时停车时长（小时）
        tuint32 nPictureServerPort; //图片服务器端口
        tuint32 nPictureServer;     //0:存储服务器 1:图片服务器
        tuint32 nHttpsStatus;       //0:https 关闭 1:https 已启动
        tuint32 nHttpServerPort;    //apache-tomcat Http服务器端口号
        _ecms_report_config_port()
        {
            nConfigPort = 7002;
            nPmsParkOverHour = 12;
            nPictureServerPort = 8002;
            nPictureServer = 0;
            nHttpsStatus = 0;
            nHttpServerPort = 8080;
        }
    } ECMS_REPORT_CONFIG_PORT;

    //反控报警主机
    typedef struct _net_alarmhost_reverse_control
    {
        tuint32 dwNodeType;//节点类型（子系统或防区）
        GUID nodeID;////节点ID（平台编号）
        tuint32 dwReverseControlType;////反控指令类型
        tuint16 u16Channel;//催泪弹道，dwReverseControlType是ECMS_CMD_REQUEST_ACS_REVERSE_CONTROL_EXPLODE_TEAR_BOMB时使用
        tint32 szRes[16];//保留
    } NET_ALARM_HOST_REVERSE_CONTROL;

    //状态查询报警主机
    typedef struct _net_alarm_host_state_requst
    {
        GUID nodeID;////节点ID
    } NET_ALARM_HOST_STATE_REQUST;

    typedef struct _net_alarm_host_state_reply
    {
        GUID nodeID;////节点ID
        tuint32 dwBufferLen;//结构体后面跟的Buffer长度
    } NET_ALARM_HOST_STATE_REPLY;

    //狗类型
    enum DOG_TYPE
    {
        DOG_TYPE_NULL = 0, //演示版（识别不到加密狗和License）
        DOG_TYPE_DOG = 1, //加密狗授权
        DOG_TYPE_LICENSE_FORMAL = 2, //正式License授权
        DOG_TYPE_LICENSE_DEMO = 3 //试用License授权 数值3是无效定义，忽略，没有用到
    };
    //授权状态
    enum AUTH_LICENSE_TYPE
    {
        AUTH_TYPE_NORMAL = 0, //正常
        AUTH_TYPE_LIMITCHANNELCOUNT = 1, //通道数量超限
        AUTH_TYPE_TIMEOVER = 2, //授权到期
        AUTH_TYPE_READINFOFAILED = 3, //授权信息读取失败
        AUTH_TYPE_LIMITDEVCOUNT = 4, //设备数量超限
        AUTH_TYPE_LIMITCHANNEL_AND_DEV_COUNT = 5 //通道和设备数量都超限
    };
    //上报加密狗的时间信息
    typedef struct _ecms_report_dog_time
    {
        DOG_TYPE nDogType;//参见enum DOG_TYPE
        tuint32 dwInvalidTime;      //到期时间CTime::GetTime() UTC时间，客户端显示的时候要根据当前时区进行转换
        tuint32 nMaxChlCount;//最大通道数限制
        tuint32 nCurChlCount;//当前系统中的通道数量
        AUTH_LICENSE_TYPE nAuthLicenseOrDog;//授权状态
        char szVersion[512];//版本号
        char szSerialNumber[512 - 8]; //序列号
        tuint32 nMaxDevCount;//最大设备数限制
        tuint32 nCurDevCount;//当前系统中的设备数量
        _ecms_report_dog_time()
        {
            nDogType = DOG_TYPE_NULL;
            dwInvalidTime = 2082672000;//CPFTime t(2035,12,31,0,0,0,true);//utc时间
            nMaxChlCount = 16;
            nCurChlCount = 0;
            nAuthLicenseOrDog = AUTH_TYPE_NORMAL;
            memset(szVersion, 0, sizeof(szVersion));
            memset(szSerialNumber, 0, sizeof(szSerialNumber));
            nMaxDevCount = 0xFFFFFFF;//默认不限制,x86nvr ubuntu18.04.3的4G内存是32,8G内存的是64
            nCurDevCount = 0;
        }
    } ECMS_REPORT_DOG_TIME;

    //上报访客超时提醒的信息
    typedef struct _ecms_report_visitor_timeout
    {
        GUID     Log_Index;         //访客记录的唯一标识,实际是businessGUID
        tuint32  nTimeoutCount;     //超时未离开人数
    } ECMS_REPORT_VISITOR_TIMEOUT;

    //上报服务器下线的信息
    typedef struct _ecms_report_server_state
    {
        GUID nodeGuid;//服务器节点GUID
        bool bState;//true上线 false下线 服务器与管理的连接状态
        char byReserve[3];////保留，对齐
    } ECMS_REPORT_SERVER_STATE;

    //license头结构体
    struct LICENSE_FILE_HEAD_INFO
    {
        tuint32    dwFileFlag;
        tuint32    dwFileVersion;
        tuint32    FHISize;////文件头信息的大小
        tuint32    dwRegisterFlag ;
        tuint32    dwDataInfoLen;
    };

    //上报门禁系统中门禁设备结构体
    typedef struct _ecms_net_upload_acs_device
    {
        char szIdEx[64];//设备在第三方系统里面的id  json：id
        char szSerialNo[64];//设备在第三方系统里面的序列号  json：sn
        char szName[256];//名称 json：name
        tuint32 nStatus;//设备状态0禁用 1启用 json：statu
        char szBelongModule[64];//所属模块，acc门禁，att考勤，ele梯控 json：module
        char szDevModelName[256];//设备型号名称； json：type
        GUID acsSystemGUID;//门禁系统的GUID
        _ecms_net_upload_acs_device()
        {
            memset(szIdEx, 0, sizeof(szIdEx));
            memset(szSerialNo, 0, sizeof(szSerialNo));
            memset(szName, 0, sizeof(szName));
            nStatus = 1;
            memset(szBelongModule, 0, sizeof(szBelongModule));
            memset(szDevModelName, 0, sizeof(szDevModelName));
            acsSystemGUID = GUID_NULL;
        }
    } ECMS_NET_UPLOAD_ACS_DEVICE, *LPECMS_NET_UPLOAD_ACS_DEVICE;

    //上报门禁系统中门结构体
    typedef struct _ecms_net_upload_acs_door
    {
        char szIdEx[64];//门在第三方系统里面的id  json：id
        char szName[232];//名称 json：name 256
        char szSerialNo[20];//控制器序列号
        tuint32  address;//门地址
        char szDeviceIdEx[64];//门所属设备在第三方系统里面的id  json：deviceId
        GUID acsSystemGUID;//门禁系统的GUID
    } ECMS_NET_UPLOAD_ACS_DOOR, *LPECMS_NET_UPLOAD_ACS_DOOR;

    //上报门禁系统中读头结构体
    typedef struct _ecms_net_upload_acs_reader
    {
        char szIdEx[64];//读头在第三方系统里面的id  json：id
        char szName[256];//名称 json：name
        char szDoorIdEx[64];//读头所属门在第三方系统里面的id  json：doorId
        char szReaderNo[64];//读头编号                      json：readerNo
        tuint32 nReaderState;//读头进出入状态，0入，1出    json：readerState
        GUID acsSystemGUID;//门禁系统的GUID
    } ECMS_NET_UPLOAD_ACS_READER, *LPECMS_NET_UPLOAD_ACS_READER;

    // 门禁系统反控
    typedef struct _ecms_net_acs_reverse_control
    {
        GUID nodeID;                  // 节点ID（平台编号）
        tuint32 dwReverseControlType; // 反控指令类型
    } ECMS_NET_ACS_REVERSE_CONTROL, *LPECMS_NET_ACS_REVERSE_CONTROL;

    // 门禁系统记录
    typedef struct _ecms_net_acs_record
    {
        GUID acsID;                  // 门禁系统ID
        tuint32 nRecordType; // 记录指令类型
        tuint32 nBeginTime;//开始时间
        tuint32 nEndTime;//结束时间
    } ECMS_NET_ACS_RECORD, *LPECMS_NET_ACS_RECORD;


    // 门禁系统获取门状态信息
    typedef struct _ecms_net_acs_door_state_requst
    {
        GUID acsID;                // 门禁系统ID
    } ECMS_NET_ACS_DOOR_STATE_REQUST, *LPECMS_NET_ACS_DOOR_STATE_REQUST;

    // 门禁系统获取门状态信息
    typedef struct _ecms_net_acs_door_state_requst_kantech
    {
        GUID acsID;                // 门禁系统ID
        tuint32 doorID[64];        // 门ID
        tuint32 Reserve[64];    //保留
    } ECMS_NET_ACS_DOOR_STATE_REQUST_KANTECH, *LPECMS_NET_ACS_DOOR_STATE_REQUST_KANTECH;

    // 访客机同步部门
    typedef struct _ecms_net_fk_sync_department
    {
        GUID guidFKDev;//访客机GUID
        tint32 nDepartmentStatus;/*0：自动识别（新增或更新） 1：新增 2：更新 3：删除*/
        tint32 nDepartmentSyncID;
        tint32 nDepartmentSyncParentID;
        char pCompanyNumber[256];
        char pDepartmentName[256];
    } ECMS_NET_FK_SYNC_DEPARTMENT, *LPECMS_NET_FK_SYNC_DEPARTMENT;

    // 访客机添加员工
    typedef struct _ecms_net_fk_sync_employee
    {
        GUID guidFKDev;//访客机GUID
        GUID guidAlbum;
        tint32 nDepartmentStatus;/*0：自动识别（新增或更新） 1：新增 2：更新 3：删除*/
        tint32 nDepartmentSyncID;
        tint32 nEmployeeStatus;/*0：自动识别（新增或更新） 1：新增 2：更新 3：删除*/
        tint32 nEmployeeSyncID;
        tint32 nSex;
        char pDepartmentName[256];
        char pCardNum[256];
        char pNickname[256];
        char pPhoneNo[256];
        char pNation[256];
    } ECMS_NET_FK_SYNC_EMPLOYEE, *LPECMS_NET_FK_SYNC_EMPLOYEE;

    typedef struct _ecms_net_fk_sync_blacklist
    {
        GUID guidFKDev;//访客机GUID
        GUID guidTarget;//访客库GUID
        tint32 nTargetID;//目标ID
        tint32 nSuccessID;//操作成功返回ID
        tint32 nStatus;/*0：自动识别（新增或更新） 1：新增 2：更新 3：删除*/
        char id[64]; //证件号
    } ECMS_NET_FK_SYNC_BLACKLIST, *LPECMS_NET_FK_SYNC_BLACKLIST;

    enum DOOR_CONNECT_STATE
    {
        DCS_OFFLINE = 0,//离线
        DCS_ONLINE, //在线
        DCS_DISABLE //禁用
    } ;
    enum DOOR_SENSOR_STATE
    {
        DSS_UNKNOW = 0,//未知
        DSS_NONE,//无门磁
        DDS_CLOSE,//关闭
        DDS_OPEN//打开
    } ;
    enum DOOR_ALARM_STATE
    {
        DAS_NONE = 0,//无
        DAS_ALARM,//报警
        DAS_OPENTIMEOUT,//门开超时
        DAS_OPENABNORMAL,//门被意外打开
        DAS_TAMPER,//防拆
        DAS_OPENSTRESSPWD,//胁迫密码开门
        DAS_OPENSTRESSFINGERPRINT,//胁迫指纹开门
        DAS_OPENSTRESS,//胁迫开门
        DAS_NORMALLYOPENALARM,//常开报警
        DAS_BATTERYLOW,//电池电压过低
        DAS_BATTERYREPLACE,//立即更换电池
        DAS_ILLEGALOPERATION,//非法操作
        DAS_BACKUPPOWER,//后备电源
        DAS_UNKNOW//未知
    } ;

    typedef struct _ecms_net_acs_door_state_replay
    {
        tuint32 doorIdEx; // 第三方系统门ID
        tuint32 doorConnectState ;//门连接状态，对应enum DOOR_CONNECT_STATE
        tuint32 doorSensorState;//门磁状态,对应enum DOOR_SENSOR_STATE
        tuint32 doorAlarmState;//门报警状态,对应enum DOOR_ALARM_STATE
    } ECMS_NET_ACS_DOOR_STATE_REPLAY, *LPECMS_NET_ACS_DOOR_STATE_REPLAY;

    typedef struct _ecms_net_kantech_door_state_replay
    {
        GUID    acsGUID;  //门禁GUID
        tuint32 doorIdEx; // 第三方系统门ID
        tuint32 doorConnectState ;//门连接状态，对应enum DOOR_CONNECT_STATE
        tuint32 doorSensorState;//门磁状态,对应enum DOOR_SENSOR_STATE
        tuint32 doorAlarmState;//门报警状态,对应enum DOOR_ALARM_STATE
    } ECMS_NET_KANTECH_DOOR_STATE_REPLAY, *LPECMS_NET_KANTECH_DOOR_STATE_REPLAY;

    typedef struct SubSysStat
    {
        tuchar ArmStat;   // 布撤防状态
#define STAT_ARM            0x0                                     // 普通布防
#define STAT_ARM_INS        0x1                                 // 即时布防
#define STAT_ARM_STAY       0x2                             // 留守布防
#define STAT_DISARM         0x3                                 // 撤防
        char    byReserve[3];////保留，对齐
        tint32 SysTrigger;   // 系统触发 1-触发 0-未触发
        tint32 Alarm;           //系统报警 1-报警 0-未报警
        bool IsUsed;           // 子系统是否启用 1-启用 0-未启用
        bool IsWalkTest;      // 子系统是否进入步测 1-进入 0-未进入
        char    byReserve2[2];////保留，对齐
    } SubSysStat; //子系统

    typedef struct ZonerStat
    {
        tint32 SubSys;                       //所属子系统
        tint32 ZoneAlarm;                 //防区报警 1-报警 0-未报警
        tint32 ZoneAlarmMemory;   //防区报警记忆  1-报警记忆 0-无报警记忆
        tint32 ZoneBypass;               //防区旁路 1-旁路 0-旁路恢复
        tint32 ZoneFail;                      //防区失效 1-失效 0-未失效
        tint32 ZoneFault;                     //防区故障 1-故障 0-未故障
        tint32 ZoneTrigger;               //防区触发 1-触发 0-未触发
        bool UnReg;                        // 未注册
        char   byReserve[3];////保留，对齐
    } ZonerStat; //防区

    /*艾礼安 begin*/
#define MAX_ALEAN_ZONE_NUM 2048//艾礼安主机最大防区数量,由于艾礼安报警主机查询不到各个防区的状态,不对状态结构体做修改
    /*艾礼安 end*/

#define MAX_ZONE_NUM 2048//最大防区数量
#define MAX_SUBSYS_NUM 256//最大子系统数量
    //报警主机通用状态结构体
    typedef struct CommnStat
    {
        char         ModuleId[129];      //模块id
        char    byReserve[3];////保留，对齐
        tuint32  ModuleType;           //模块类型
        tuint32  PanelType;               //主机类型
        SubSysStat SysStat[MAX_SUBSYS_NUM];      //子系统
        ZonerStat ZoneStat[MAX_ZONE_NUM];     //防区

        bool BatteryLow;                    //true-电池电压偏低
        bool ACFail;                           // true-交流掉电
        bool SirenFail;                        //true-警号保险失效
        bool kill;                                 //true-死机
        bool Condition;                      //true-条件
        bool TelCommunicationFail; //true-电话线通信失败
        bool WatchDogReset;            //true-看门狗复位
        bool Bell;                               //true-响铃
        bool Chime;                           //true-钟声
        bool Instant;                           //true-紧急
        bool ACLed;                          //true-自适应控制LED
        bool ProgMode;                     //true-编程模式
        bool TestReport;                    //true-测试报告
        bool BatteryTest;                   //true-电池测试
        bool WalkTest;                       //true-步测
        bool MedicalZone;                //true-紧急防区
        bool FireZone;                       //true-火警
        bool PoliceZone;                   //true-盗警
        bool LedFail;                         //true-指示灯故障
        bool DataLineStat ;               //true-数据线状态
        bool HostageAlarm;              //true-挟持报警

        bool DemolishAlarm;           // true-防拆报警
        bool MainKeyboardFail;        // true-主键盘掉线
        bool NetworkFail;                 // true-网络故障
        bool GPRSFail;                    // true-GPRS异常
        bool MBUSFail;                    // true-MBUS异常
        char TearBombState;         //8路催泪弹状态，一个位代表一路，1 = 发射，0 = 准备
        char    byReserve2[1];////保留，对齐
    } AlarmHostCommnStat; //报警主机通用状态

    //状态变化结构体，提示哪些防区、子系统发生变化
    typedef struct CommnStatVary
    {
        int ZoneNum;
        int ZoneVary[MAX_ZONE_NUM];
        int SubSysNum;
        int SysVary[MAX_SUBSYS_NUM];
    } CommnStatVary;

    //状态报告
    typedef struct State_Report
    {
        GUID guidAlarmHost;                      //报警主机GUID
        CommnStatVary TriggerMsg;         //报警主机状态变化结构体
        AlarmHostCommnStat PanelStat;   //报警主机通用状态结构体
    } ECMS_NET_ALARM_HOST_STATE_REPORT;

    typedef struct OutLicenseForAddChannel
    {
        GUID nodeGUID;                      //设备GUID
        bool bIsCreate;//是否是新增设备操作
        char    byReserve[3];////保留，对齐
        OutLicenseForAddChannel()
        {
            nodeGUID = GUID_NULL;
            bIsCreate = false;
        }
    } NET_OUT_LICENSE_FOR_ADDCHANNEL;

    //连接状态
    typedef struct __node_connect_state
    {
        GUID    nodeID;
        tuint32 nodeType;
        tuint32 nodeConnectState;
        tuint32 byReserve1;
        tuint32 byReserve2;
        tuint32 byReserve3;
        tuint32 byReserve4;
    } NODE_CONNECT_STATE;

    typedef struct _ecms_pms_param
    {
        char szXML[10 * 1024];
        char strCMD[64];
        tuint32 dwLen;
        tuint32 dwPage;//from 0
    } ECMS_PMS_PARAM;

    // 红门IP-LED屏_命令消息_数据结构
    typedef struct _ecms_led_text_param {
        GUID ledGUID;
        tuint32 ledType;  //当命令为5，的时候，是进行触发心跳
        char szIP[32];
        tuint16 port;
        tuint16 nLine;
        char szSend[1024];
        tuint32 sendLen;
        bool bAudio;
    } ECMS_LED_TEXT_PARAM, *PECMS_LED_TEXT_PARAM;

    //  华安外接485屏_命令消息_数据结构
    typedef struct _ecms_serial_led_text_param {
        GUID camGUID;
        tuint32 ledType;
        char szSend[1024];
        tuint32 sendLen;
        tuint16 nLine;
        bool bAudio;
    } ECMS_SERIAL_LED_TEXT_PARAM, *PECMS_SERIAL_LED_TEXT_PARAM;

    // XCP10LED屏_命令消息_数据结构   //TODO:要修改
    typedef struct _ecms_XCP10_led_text_param {
        GUID ledGUID;           // LED 的GUID
        tuint32 nL1DisplayMode; // 第一行的显示方式    默认第1行立现  1
        tuint32 nL2DisplayMode; // 第二行的显示方式    默认第2行左移  3
        char szL1SendTxt[1024]; // 第一行发送的内容
        char szL2SendTxt[1024]; // 第二行发送的内容

    } ECMS_XCP10_LED_TEXT_PARAM, *PECMS_XCP10_LED_TEXT_PARAM;

    //  海康动环主机 查询参数
    typedef struct _hk_donghuan_sensor_param {
        GUID devGUID;
        tuint32 chlID;
        tuint32 slotID;
    } HK_DONGHUAN_SENSOR_PARAM, *PHK_DONGHUAN_SENSOR_PARAM;
    //  //picture jpeg-----used by PMS
    //  typedef struct _net_picture_info
    //  {
    //      GUID nodeID;//通道节点ID
    //      //char szFileName[256];
    //      char szBuf[256*1024];
    //      tuint32 bufLen;
    //      tuint32 passTime;
    //      BYTE alarmState;
    //  }NET_PICTURE_INFO;

    //////////PMS通行记录的节点信息
    typedef struct _net_pms_passevent_node_info
    {
        tuint32 dwNodeType;//节点类型
        GUID nodeID;////节点ID
        unsigned char byReserve[4];//保留
    } NET_PMS_PASSEVENT_NODE_INFO;

    typedef struct _net_updateprocess_node_info
    {
        tuint32 dwNodeType;//节点类型
        GUID nodeID;////节点ID
        unsigned char byReserve[4];//保留
    } NET_UPDATEPROCESS_NODE_INFO;

    typedef struct _net_permiter_node_info
    {
        tuint32 dwNodeType;//节点类型
        GUID nodeID;////节点ID
        unsigned char byReserve[4];//保留
    } NET_PERMITER_NODE_INFO;

    typedef struct _ecms_storage_info
    {
        GUID    nodeID;
        tuint32 time;
        char    byReserve[8];//保留
        char    szDetailXml[1536];//
    } ECMS_STORAGE_INFO;

    typedef struct _login_error_type_
    {
        GUID    nodeID;
        tuint32 errorType;
        char    byReserve[8];//保留
    } LOGIN_ERROR_TYPE;

    /// ECMS_CMD_GET_PTZ_STEP 命令的返回值
    typedef struct _ptz_step_config
    {
        GUID    m_nodeGuid;
        tuint32 m_xStep;
        tuint32 m_yStep;
        tuint32 m_zStep;
        // 预留32字节备用
        tuint8  m_spare[32];
    } PTZ_STEP_CONFIG;

    //访客信息公共字段
    typedef struct _visitor_public_info_
    {
        char recordID[64];             //这条记录的GUID
        char devID[64];                //设备ID
        char devAddr[64];              //设备名
        char phone[20];                //访客电话号码
        char carNo[16];                //车牌号码
        char respondentDepartment[64]; //被访人部门
        char respondentStaffno[32];    //被访人工号
        char respondentName[32];       //被访人
        char respondentAddr[64];       //被访人地址
        tuint8 isblackUser;            //是否是重点关注人员
        char szRes[3];                 //预留
        char startTime[32];            //访客起始时间
        char endTime[32];              //访客结束时间
        char createTime[32];           //创建时间
    } VISITOR_PUBLIC_INFO;

    //身份证信息
    typedef struct _id_card_info_
    {
        char name[32];       //姓名
        tint32 sex;          //性别
        char birthday[32];   //出生日期
        char address[64];    //家庭住址
        char nation[16];     //民族
        char id[64];         //身份证号码
        char validPeriod[32];//身份证有效期
        char issue[64];      //签发机关
        tuint32 idImgLen;     //证件照大小
        tuint64 idImage;     //证件照
        tuint32 camImgLen;    //现场照大小
        tuint64 cameraImage; //现场照
    } ID_CARD_INFO;

    //驾驶证信息
    typedef struct _driver_license_info_
    {
        char name[32];         //姓名
        tint32 sex;            //性别
        char birthday[32];     //出生日期
        char nationality[32];  //国籍
        char usage[32];        //驾驶证有效期
        char address[64];      //家庭住址
        char driveID[64];      //驾驶证ID
        char issueDate[32];    //初次领证日期
        char issue[64];        //签发机关
        tuint32 idImgLen;       //证件照大小
        tuint64 idImage;       //证件照
        tuint32 camImgLen;      //现场照大小
        tuint64 cameraImage;   //现场照
    } DRIVER_LICENSE_INFO;

    //社保卡
    typedef struct _social_security_card_
    {
        char name[32];         //姓名
        char id[64];           //社会保障号码
        char cardID[64];       //卡号
        char issueDate[32];    //发行日期
        tuint32 idImgLen;       //证件照大小
        tuint64 idImage;       //证件照
        tuint32 camImgLen;      //现场照大小
        tuint64 cameraImage;   //现场照
    } SOCIAL_SECURITY_CARD;

    //护照信息
    typedef struct _passport_info_
    {
        char name[32];          //姓名
        tint32 sex;             //性别
        char birthday[32];      //出生日期
        char address[64];       //家庭住址
        char nationality[32];   //国籍
        char passportID[64];    //护照号码
        char passportType[32];  //护照类型
        char countryCode[64];   //国家码
        char issue[64];         //签发机关
        char issueDate[32];     //发行日期
        char invalidityDate[32];//有效期至
        tuint32 idImgLen;       //证件照大小
        tuint64 idImage;        //证件照
        tuint32 camImgLen;      //现场照大小
        tuint64 cameraImage;    //现场照
    } PASSPORT_INFO;

    typedef struct _visitor_info
    {
        GUID   nodeID;
        tuint32 messageType;   // 1:身份证2:驾驶证3:社保卡4:护照
        VISITOR_PUBLIC_INFO visitorPublicInfo;
        union {
            ID_CARD_INFO idCard;
            DRIVER_LICENSE_INFO driverLicense;
            SOCIAL_SECURITY_CARD socialSecurityCard;
            PASSPORT_INFO passPort;
        } card;
    } VISITOR_INFO;

    typedef struct _delete_visitor_info
    {
        GUID nodeID;
        tuint32 messageType;    //1:身份证2:驾驶证3:社保卡4:护照
        char recordID[64];     //这条记录的GUID
        char devID[64];        //设备ID
        char devAddr[64];      //设备名
        char name[32];         //姓名
        char id[64];           //证件号码
        char createTime[32];   //创建时间
    } DEL_VISITOR_INFO;

    typedef struct _custom_product
    {
        char useP2P; //客户是否定制p2p
        char useProtocol; //客户是否定制接入设备协议
		char useP2PTrans; //客户是否定制P2P穿透业务
        char rev;//保留
        char p2pServer[128];//p2p服务器地址
        tuint32 p2pPort;//p2p服务端口
        char protocols[64];//支持接入设备协议
        char tvtManufacturer[32];//客户定制tvt设备的生产厂商，非定制tvt设备显示onvif,默认为空
        char tvtManufacturer_nvr[16];//客户定制tvt设备的生产厂商，非定制tvt设备显示onvif,默认为空
        char p2pServerSub[64];//p2p服务器地址
        tuint32 p2pPortSub;//p2p服务端口
        char p2pServer20[64];//p2p2.0服务器地址
        tuint32 p2pPort20;//p2p2.0服务端口
		char p2pServerTransIp[64];
		tuint32 p2pServerTransPort;
        char rev1[772];//保留

    } CUSTOM_PRODUCT;

    // 1400上级与1400下级之间数据交互协议头
    typedef struct _ecms_1400_http_cmd_info {
        GUID chlGUID;       //通道GUID
        tuint32 nHttpMethod;// ECMS_1400_HTTP_METHOD, Http命令类型
        tuint32 nHttpRaw;   // ECMS_RAW_FORMAT, 内容格式
        char url[256];      // URL
        tuint32 nDataLen;   // 协议长度，协议内容为JSON
        char byReserve[128];//预留
    } ECMS_1400_HTTP_CMD_INFO, *P_ECMS_1400_HTTP_CMD_INFO;

    struct QUERY_ALBUM
    {
        GUID      nodeID;//////
        char      name[256];////
        char      szRes1[64];////
        char      szRes2[64];////
        QUERY_ALBUM()
        {
            nodeID = GUID_NULL;
            memset(name, 0, sizeof(name));
            memset(szRes1, 0, sizeof(szRes1));
            memset(szRes2, 0, sizeof(szRes2));
        }
    };

    typedef struct _WifiProbe_addinfo
    {
        GUID nodeID;
        char cRecordID[64];
        char cDevName[256];
        char cAddr[256];
        char cDevID[64];//设备编码
        char cWifiMAc[64];//wifi 的mac地址
        char cTime[64];//时间
        char cLon[64];
        char cLat[64];
        char cSSID[64];
        char cLeaveTime[64];
        _WifiProbe_addinfo()
        {
            memset(this, 0, sizeof(_WifiProbe_addinfo));
        }
    } WIFIPROBE_ADDINFO;

    typedef struct _WifiProbe_Updateinfo
    {
        GUID nodeID;
        char cRecordID[64];
        char cWifiMAc[64];//wifi 的mac地址
        char cLeaveTime[64];
        _WifiProbe_Updateinfo()
        {
            memset(this, 0, sizeof(_WifiProbe_Updateinfo));
        }
    } WIFIPROBE_UPDATEINFO;

    //停车场PMS业务相关结构体
    ///////////////////////////////
    struct PMS_LANE_CNT
    {
        std::string laneStrGUID;    //车道的guid
        tuint32 inCnt;              //进入
        tuint32 outCnt;             //出去
        tuint32 totalCnt;           //总计

        PMS_LANE_CNT()
        {
            inCnt = 0;
            outCnt = 0;
            totalCnt = 0;
        }
    };

    struct PMS_TIMR_INFO
    {
        tuint32 startTime;
        tuint32 endTime;
        tuint32 sumInCnt;               //该时间段内，所有通道的总计
        tuint32 sumOutCnt;
        tuint32 sumTotalCnt;
        std::list<PMS_LANE_CNT> laneInfoList;

        PMS_TIMR_INFO()
        {
            startTime = 0;
            endTime = 0;
            sumInCnt = 0;
            sumOutCnt = 0;
            sumTotalCnt = 0;
        }
    };

    struct PMS_INFO
    {
        std::list<PMS_TIMR_INFO> timeInfoList;
    };

    ///////////////////////////////

}/////////namespace NVMS_NET_PROTOCOL
namespace NVMS2_IA
{
    // 命令字
    enum {

        //////////////////////////////////////////////////////////////////////////
        /** 图片数据推流
        *   客户端请求/设置图像样本数据， 存在批量处理业务
        *   当批量处理时，若样本图像较多，不能将图像打包成整体发送
        *   多次发送目标数据进行处理
        *   UPDATE 类命令
        **/
        IA_CMD_BEGIN = 0,
        IA_PUSH_TARGET_IMAGE_DATA_deprecated,// deprecated, should be never used // server推送目标库中的样本图像数据, client接收                 (done)
        IA_PUSH_SNAP_IMAGE_DATA_deprecated,// deprecated, should be never used   // server推送智能分析服务人脸抓图图像数据， client接收          (done)
        IA_PUSH_SNAP_RAW_IMAGE_DATA_deprecated,// deprecated, should be never used // server推送智能分析服务抓拍人脸图的原图数据， client接收  (done)
        IA_PUSH_SEARCH_IMAGE_DATA_deprecated,// deprecated, should be never used  // server推送以图缩图的结果， client接收，多个目标库有同一目标 (done)
        IA_PUSH_VERIFIED_IMAGE_DATA_deprecated,// deprecated, should be never used // server推送一段时间内检测出的目标图像                       (done)
        IA_PUSH_CREATE_EVENT_FAILED, // deprecated, should be never used// 智能分析服务器在向IPC订阅事件失败时，向客户端发送该消息        (done)

        //////////////////////////////////////////////////////////////////////////
        //                              智能分析事件                          //
        //////////////////////////////////////////////////////////////////////////
        // 创建、删除、获取、被动通知智能分析服务器向IPC订阅事件(人数统计、人脸检测)
        IA_SET_CREATE_EVENT = 0x00000010,     // 创建智能分析事件                   (done)
        IA_SET_DELETE_EVENT,     // 删除智能分析事件                    (done)
        IA_GET_EVENT,            // 主动获取有智能分析任务的通道      (done)
        IA_UPDATE_EVENT,         //  // deprecated, should be never used 自动更新有智能分析任务的通道     (done)
        IA_GET_FR_EVENT_ALBUM,   // 获取人脸比对事件的样本库                (done) // 人脸匹配 过程中所需目标库的 设置、获取命令
        IA_SET_FR_EVENT_ALBUM,   // 设置人脸比对事件的样本库 可指定多个    (done)
        IA_SET_GLOBAL_VFD_CONF,  // 设置人脸检测的置信度 0---1000         (done)
        IA_GET_GLOBAL_VFD_CONF,  // 获取人脸检测的置信度                  (done)
        IA_GET_SERVER_PARAM,     // 获取服务器配置
        IA_SET_SERVER_PARAM,     // 配置服务器
        IA_UPDATE_SERVER_PARAM,  // 更新结果推送
        IA_SET_FR_ALARM_TYPE,    // 设置人脸比对告警类型
        IA_GET_FR_ALARM_TYPE,    // 获取人脸比对告警类型
        IA_PUSH_FR_ALARM_TYPE,   // 推送人脸比对告警类型
        //////////////////////////////////////////////////////////////////////////
        //             目标库管理, 目前仅支持  黑名单 白名单                        //
        //////////////////////////////////////////////////////////////////////////
        IA_SET_CREATE_ALBUM = 0x00000100,       // 创建目标库 基本信息             (done)
        IA_SET_DELETE_ALBUM,                    // 删除目标库 基本信息             (done)
        IA_SET_MODIFY_ALBUM,                    // 修改目标库 基本信息             (done)
        IA_GET_ALBUM_LIST,                      // 获取目标库列表                    (done)
        IA_SET_ASYNC_ALBUM_TO_DEV,              // 复制album中的目标信息至另外一个设备中，并将其同步
        IA_SET_UN_ASYNC_ALBUM_TO_DEV,           // 将目标库中某一些目标解除绑定关系
        IA_SET_IMPORT_ALBUM_PACK,               // album 打包导入（当前没有需求）（2.1版本不实现）
        IA_SET_ADD_ALBUM_TARGET_LIST,   // 批量向目标库中添加目标            (done) 目标库中的样本数据管理
        IA_SET_MODIFY_ALBUM_TARGET_LIST,// 批量修改目标库中的目标信息      (2.1.0版本不实现)
        IA_SET_DEL_ALBUM_TARGET_LIST,   // 批量从目标库中删除目标            (done)
        IA_SET_ADD_ALBUM_TARGET,        // 向目标库中添加目标              (done)
        IA_SET_MODIFY_ALBUM_TARGET,     // 修改目标库中目标             (done)
        IA_SET_DEL_ALBUM_TARGET,        // 从目标库中删除目标              (done)
        IA_SET_CLEAR_ALBUM_TARGET,      // 清空目标库中的目标              (done)
        IA_SET_COPY_ALBUM_TO_ANOTHER_ALBUM, // 将当前目标库的信息拷贝至另外一个目标库， ie：两个目标库名称, 2018-11-5, for 华安主机
        IA_SET_MOVE_ALBUM_TO_ANOTHER_ALBUM, // 将当前目标库的信息转移到另外一个目标库， ie：复制并删除自己, 2018-11-5, for 华安主机
        IA_SET_SYNC_ADD_ALBUM_TARGET,  // 目标库添加、修改、删除
        IA_SET_SYNC_MODIFY_ALBUM_TARGET,
        IA_SET_SYNC_DELETE_ALBUM_TARGET,
        IA_SET_SYNC_CLEAR_ALBUM_TARGET,
        IA_SET_ADD_AUTHORITY_TO_DEVICE, //将目标库中的目标绑定至相应的设备
        IA_SET_DEL_AUTHORITY_TO_DEVICE, //删除设备中目标库中指定目标
        IA_SET_MODIFY_AUTHORITY_TO_DEVICE,//配置结果通过异步回调接口通知给客户端， 返回信息为目标在设备上的绑定关系， 如果存在绑定失败的，给出失败原因及失败的设备列表
        IA_GET_AUTHORITY_INFO, // 获取目标的绑定信息, 指定需要获取的目标列表，返回各目标在设备上的绑定关系
        IA_SET_STOP_ADD_TARGET, //停止添加目标
        IA_GET_PROC_TARGET_FAILED_REASON,  // 获取目标库中目标 添加 & 修改 & 删除失败的原因
        IA_IMPORT_TARGET_LIST,              // 导入目标列表
        IA_GET_TARGET_FAIL_LIST,            // 获取目标失败列表
        IA_GET_TARGET_FAIL_COUNT,           // 获取目标失败列表总数
        IA_DEL_TARGET_FAIL_LIST,            // 删除目标失败记录
        IA_ADD_ORG_NODE_LIST,               // 添加组织节点列表
        IA_GET_IMPORT_TARGET_STATE,         // 获取目标导入状态
        IA_CLEAR_TARGET_FAIL_LIST,          // 清除目标失败记录



        /** 获取指定目标库中的所有用户属性、用户图像列表信息，（仅包含userId和userDataId）
        *   命令参数中附加pullImage则会触发 IA_PUSH_TARGET_IMAGE_DATA 返回实际图像信息
        *   如果指定需要获取图像  则服务通过IA_PUSH_TARGET_IMAGE_DATA发送数据
        **/
        IA_GET_ALBUM_TARGET_COUNT = 0x00000200,         // 获取目标库中目标的数量
        IA_GET_ALBUM_TARGET_SLICE,                      // 获取目标库中目标信息分页， 用于代替 IA_GET_ALBUM_TARGET_SLICE_INFO
        IA_GET_ALBUM_TARGET,                            // 获取目标库中 指定目标的 (属性 + 图像) 用于替代IA_GET_ALBUM_TARGET_INFO
        IA_GET_ALBUM_TARGET_SLICE_INFO_deprecated, // deprecated, never used，获取指定目标库中指定序列目标的信息（分页获取), 请使用 IA_GET_ALBUM_TARGET_SLICE
        IA_GET_ALBUM_TARGET_INFO_deprecated, // deprecated, never used, 获取目标信息， 直接返回目标属性，目标图像通过推流的方式返回(如果有需要)， 请使用 IA_GET_ALBUM_TARGET
        IA_GET_ALBUM_TARGET_IMAGE_STOP_deprecated,  // deprecated, never used
        IA_GET_ALBUM_TARGET_PROPERTY_deprecated,// deprecated, never used
        IA_GET_QUERY_DETECTED_FACE_COUNT,       // 查询一段时间内的人脸抓图数量(done) //查询智能分析结果VFD、抓图、告警
        IA_GET_QUERY_DETECTED_FACE_INFO,        // 查询一段时间内的人脸抓图索引(done)
        IA_GET_QUERY_DETECTED_FACE_COUNT_EX,    // 搜索人脸抓图数量扩展接口，支持从多通道搜索(done 2019-12-18)
        IA_GET_QUERY_DETECTED_FACE_INFO_EX,     // 搜索一段时间多个通道的人脸抓图索引接口， 支持多通道搜索(done 2019-12-18)
        IA_GET_DETECTED_FACE_IMAGE_deprecated, // deprecated, never used
        IA_GET_DETECTED_FACE_IMAGE_STOP_deprecated, // deprecated, never used
        IA_GET_DETECTED_FACE_ON_TIME,       // 获取具体时间的人脸抓图图像
        IA_GET_DETECTED_FACE_IMAGE_ON_TIME, // 获取具体时间的包含人脸抓图的大图
        IA_GET_ALBUM_TARGET_LIST,                       // 获取目标列表（属性+图像）  //新增批量获取目标接口/////////////////////////////////
        IA_GET_QUERY_VERIFIED_TARGET_INFO_deprecated,  // 查询一段时间内检测到的目标图像信息列表   智能分析结果 // deprecated use IA_GET_VERIFIED_TARGET_RESULT_SLICE instead
        IA_GET_VERIFIED_TARGET_RESULT_COUNT,// 获取 智能分析服务器 一段时间内的人脸比对结果数量
        IA_GET_VERIFIED_TARGET_RESULT_SLICE,// 获取 智能分析服务器 一段时间内人脸比对结果的分页详情
        IA_GET_VERIFIED_TARGET_ON_TIME,     // 获取指定的识别信息， 包含目标抓图、目标库图、目标属性
        IA_GET_ATTENDANCE_TARGET_INFO_BRIEF,    // 查询考勤数据简略信息， 最早考勤时间、最晚考勤时间
        IA_GET_ATTENDANCE_TARGET_INFO_DETAIL,   // 考勤详情，一段时间内的所有考勤信息
        IA_GET_ATTENDANCE_BATCH_TARGET_INFO_BRIEF, // 批量查询考勤数据简略信息
        IA_GET_ATTENDANCE_TARGET_INFO_STATISTIC,// 查询目标库中考勤信息的统计信息， 包含班组有多少人打卡信息
        IA_CLEAR_ATTENDANCE_TARGET_INFO,              // 清除掉今天数据库中的关于考勤信息的统计信息
        IA_PUSH_ATTENDANCE_TARGET_INFO,             // 推送清除消息
        IA_GET_VERIFIED_TARGET_RECENTLY_TIME, //查询目标最近几次的比对结果(抓图图像)
		IA_GET_VERIFIED_TARGET_RESULT_COUNT_EX, //获取 智能分析服务器 一段时间内的人脸比对结果数量,支持从多通道搜索
		IA_GET_VERIFIED_TARGET_RESULT_SLICE_EX, //获取 智能分析服务器 一段时间内人脸比对结果的分页详情,支持从多通道搜索

        //////////////////////////////////////////////////////////////////////////
        //                  以图搜图  在目标库中搜索与上传图片相似的成员
        //////////////////////////////////////////////////////////////////////////
        /** 以图搜图接口，新增 2018-8-31  */
        /** 客户端设置一张图片给服务器， 服务器从目标库中查找相似目标， 若找到将相似目标给出
        *   IA_SET_SEARCH_IMAGE_RESULT之后返回查找多多少相似结果
        *   服务端通过 IA_PUSH_SEARCH_IMAGE_DATA 发送识别结果
        *   服务端推流
        **/
        IA_SET_SEARCH_IMAGE_FROM_ALBUM = 0x00000300,        // 从目标库中搜图
        IA_SET_SEARCH_IMAGE_FROM_CAPTURE,   // 从抓拍库中搜图
        IA_SET_SEARCH_IMAGE_RESULT_deprecated,     // 设置一张图片进行查询
        IA_GET_SEARCH_IMAGE_deprecated,         // deprecated    // 开始获取与匹配图相似的样本图， 触发 IA_PUSH_SEARCH_IMAGE_DATA
        IA_GET_SEARCH_IMAGE_STOP_deprecated,    // deprecated    // 停止获取， 用于结束 IA_PUSH_SEARCH_IMAGE_DATA

        //////////////////////////////////////////////////////////////////////////
        IA_PUSH_ALBUM = 0x00000400,         // 推送增加、删除、修改目标库, 目标库变化之后，推送目标库列表
        IA_PUSH_MODIFY_ALBUM_TARGET_deprecated,// deprecated, should be never used
        IA_PUSH_ALBUM_TARGET_CHANGED_deprecated,        // 推送修改目标， 待修改为推送目标库中目标变化通知，仅提示目标库中的目标发生变化

        IA_PUSH_ADD_ALBUM_TARGET_FAILED,    // 添加目标至目标库失败 通知--AI重构复用
        IA_PUSH_MODIFY_ALBUM_TARGET_FAILED,// 修改目标库中的目标失败 通知, 对于多NVR， 部分失败，则，将填写目标id--AI重构复用
        IA_PUSH_DELETE_ALBUM_TARGET_FAILED,// 删除目标库中的目标失败 通知, 对于多NVR， 部分失败，则，将填写目标id--AI重构复用
        IA_PUSH_CLEAR_ALBUM_TARGET_FAILED,  // 清空目标库中的目标失败 通知, 对于多NVR， 部分失败，则，将填写目标id

        IA_PUSH_ADD_ALBUM_TARGET_SUCCEEDED, // 推送目标库中的目标添加成功--AI重构复用
        IA_PUSH_MODIFY_ALBUM_TARGET_SUCCEEDED,  // 推送目标库中的目标修改成功--AI重构复用
        IA_PUSH_DELETE_ALBUM_TARGET_SUCCEEDED,  // 推送目标库中的目标删除成功--AI重构复用
        IA_PUSH_CLEAR_ALBUM_TARGET_SUCCEEDED,   // 推送目标库中的目标清空成功
        IA_PUSH_ALBUM_CREATED,              // 推送目标库创建消息
        IA_PUSH_ALBUM_MODIFIED,             // 推送目标库修改消息
        IA_PUSH_ALBUM_DELETED,              // 推送目标库删除消息
        IA_PUSH_ADD_AUTHORITY_RESULT,     //添加目标权限结果
        IA_PUSH_MODIFY_AUTHORITY_RESULT,  //修改目标权限结果
        IA_PUSH_DELETE_AUTHORITY_RESULT,  //删除目标权限结果


        //////////////////////////////////////////////////////////////////////////
        //                          智能分析告警管理
        //////////////////////////////////////////////////////////////////////////
        IA_SET_VERIFIED_ALARM_PARAM = 0x00000500,    // 告警管理 针对通道
        IA_UPDATE_VERIFIED_TARGET_ALARM,// 告警（服务端给客户端）
        IA_GET_VERRIFIED_TARGET,        // 客户端主动获取检测到的目标
        IA_UPDATE_VIDEO_FACE_DETECTION, // 检测到人脸通知
        IA_UPDATE_TARGET_MATCH_deprecated,          // 废弃，匹配告警， 与 IA_UPDATE_VERIFIED_TARGET_ALARM 相同，用于替代 IA_UPDATE_VERIFIED_TARGET_ALARM
        IA_UPDATE_TARGET_MISMATCH_deprecated,       // 废弃，人脸不匹配告警
        IA_UPDATE_VERIFIED_MOBILE_ALARM,            //告警（服务端发送给手机端）
        IA_SET_MOBILE_SEMD_MESSAGE_STATUS,          //更新手机消息推送状态（手机端发送给服务端）
        IA_PUSH_VIDEO_FACE_NO_IMAGE,                // 推送人脸抓拍除抓拍之外的信息
        IA_UPDATE_VIDEO_PERSON_DETECTION,           // 检测到人员通知
        IA_UPDATE_VIDEO_MOTORVEHICLES_DETECTION,    // 检测到机动车通知
        IA_UPDATE_VIDEO_NONMOTORVEHICLES_DETECTION, // 检测到非机动车通知
        IA_PUSH_VIDEO_FACE_DETECTION_MOBILE,        // 人脸检测告警（服务端发送给手机端）

        //////////////////////////////////////////////////////////////////////////
        //                          客流统计
        //////////////////////////////////////////////////////////////////////////
        IA_GET_CPC_PASSENGER_INFO = 0x00000600,     // 查询一段时间内进入人数
        IA_SET_DELETE_PASSENGER_DATA,       // 删除一段时间内的客流统计
        IA_UPDATE_CPC_DATA,                 // 向客户端主动推送更新的客流数据
        IA_GET_CPC_CHNL_DATA,               // 客户端查询多个通道之间的进出口数据
        IA_PUSH_DELSTATUS_CHNL_DATA,        // 向客户端主动推送通道的数据库删除状态

        //////////////////////////////////////////////////////////////////////////
        // 人脸比对排程设置、排程获取、排程更新
        IA_SET_VFD_SCHD = 0x00000700,
        IA_GET_VFD_SCHD,
        IA_UPDATE_VFD_SCHD,
        IA_SET_ALBUM_THRESHOLD,// 设置、获取目标库的相似度阈值
        IA_GET_ALBUM_THRESHOLD,
        IA_UPDATE_ALBUM_THRESHOLD,
        IA_SET_SEND_VFD_VIA_FTP_CFG, // 通过FTP、HTTP、AGBOX发送人脸抓图配置
        IA_GET_SEND_VFD_VIA_FTP_CFG,
        IA_UPDATE_SEND_VFD_VIA_FTP_CFG,
        IA_SET_FEBRILE_PERSON_THRESHOLD, // 设置、获取发热温度阈值 疑似发热人员筛选
        IA_GET_FEBRILE_PERSON_THRESHOLD,
        IA_PUSH_FEBRILE_PERSON_THRESHOLD,
        IA_PUSH_FEBRILE_PERSON_DATA,               //推送发热人员缩略图及发热信息  // 发热人员数据推送、查询、确认信息修改
        IA_PUSH_FEBRILE_PERSON_CONFIRM_INFO,       //推送发热人员确认信息
        IA_GET_FEBRILE_PERSON_COUNT,               //获取发热人员条数
        IA_GET_FEBRILE_PERSON_DATA,                //获取发热人员缩略图及发热信息
        IA_GET_NEXT_FEBRILE_PERSON_DATA,           // 获取上一条或者下一条信息
        IA_GET_FEBRILE_PERSON_DETAILED_INFORMATION,//获取发热人员详细信息（原图）
        IA_UPDATE_FEBRILE_PERSON_CONFIRM_INFO,     //更新发热人员确认信息
        IA_GET_FEBRILE_COUNT_INFO,                  //获取红外人员数据条数 // 发热人员数据统计信息查询
        IA_GET_HISTORY_FEBRILE_PERSON_COUNT_INFO,   //获取发热人数和总人数
        IA_GET_STATISTIC_INFO,                      // 人脸布控温度信息展示统计信息
        IA_CLEAR_FEBRILE_DATA,                      // 清除人脸测温数据
        IA_PUSH_CLEAR_FEBRILE_DATA,                 // 推送清除人脸测温数据

        //////////////////////////////////////////////////////////////////////////
        // 转发服务器推送的数据, 人脸比对NVR 告警
        IA_SET_ALARM_FROM_MDU = 0x00000800,
        IA_SET_FEBRILE_PERSON_INFO,         // 转发发送给之智能的疑似温度告警 // 转发服务器推送的数据，疑似温度告警

        //////////////////////////////////////////////////////////////////////////
        IA_SUB_TOPIC_WITH_MDU = 0x00001000, // 智能发送给转发，智能不需要注册处理命令
        IA_UNSUB_TOPIC_WITH_MDU,            // 智能发送给转发，智能不需要注册处理命令
        IA_BATCH_CONFIG_WITH_MDU,           // 批量配置,常州项目添加
        IA_SET_ALARM_FROM_MDU_VFD,          // 人脸抓拍信息 转发向智能分析服务发送人脸抓拍IPC上报的抓抓拍信息 智能需要解析这些数据进行相关的业务，需要注册命令处理类
        IA_SET_ALARM_FROM_MDU_CPC,          // 人数统计
        IA_SET_ALARM_FROM_MDU_VFD_MATCH,    // A2人脸比对ipc 匹配告警
        IA_SET_ALARM_FROM_MDU_SUB_STATE,    // 通知订阅状态（可能涉及到多服务器， 暂时不实现）
        IA_SET_ALARM_FROM_MDU_UNSUB_STATE,  // 通知退订状态（可能涉及到多服务器， 暂时不实现）
        IA_NVR_142_VERSION_CONFIG,
        IA_SET_ALARM_FROM_MDU_PEA,          // 区域入侵和绊线告警
        IA_SET_ALARM_FROM_MDU_PLATE,        // 车牌
        IA_REPORT_STATUS_TO_MDU, //智能向管理 推送商汤平板上下线

        //////////////////////////////////////////////////////////////////////////
        IA_UPDATE_WORK_STATE = 0x00001100,  // 智能分析服务器工作状态

        ////////////////////////////////////////////考勤管理///////////////////////////////
        IA_GET_ATTENDANCE_WORK_TIME = 0x00001200,   // 获取考勤时间段
        IA_UPDATE_ATTENDANCE_WORK_TIME,             // 更新考勤时间段
        IA_PUSH_ATTENDANCE_WORK_TIME,               // 服务端推送考勤时间段
        IA_GET_ATTENDANCE_SHIFT_INFO,               // 获取考勤班次
        IA_UPDATE_ATTENDANCE_SHIFT_INFO,            // 更新考勤班次
        IA_PUSH_ATTENDANCE_SHIFT_INFO,              // 服务器推送考勤班次修改
        IA_GET_PEOPLE_ATTENDANCE_INFO,              // 获取人员/部门排班详情
        IA_UPDATE_PEOPLE_ATTENDANCE_INFO,           // 修改人员/部门排班
        IA_PUSH_PEOPLE_ATTENDANCE_INFO,             // 服务端推送人员/部门排班
        IA_GET_PEOPLE_ATTENDANCE_INFO_SPECIAL,      // 查询特定人员/部门排班(传参:PeopleToAttendanceShiftXml)
        IA_INQUIRE_ATTANDANCE_RECORD_GROUP,         // 查询考勤明细
        IA_INQUIRE_ATTANDANCE_RECORD_COUNT,         // 查询考勤明细条数
        IA_INQUIRE_ATTANDANCE_COMPARE_RECORD_GROUP, // 查询考勤比对记录
        IA_INQUIRE_ATTANDANCE_COMPARE_RECORD_COUNT, // 查询考勤比对记录条数
        IA_START_STATISTIC_ATTENDANCE_RECORD,       // 客户端触发开始考勤结果统计
        IA_UPDATE_ATTENDANCE_RECORD_TO_CLIENT,      // 智能服务统计完考勤结果时，通知客户端
        IA_GET_EXCEPTION_HANDLING_TYPE_GROUP,       // 获取异常处理类型
        IA_UPDATE_EXCEPTION_HANDLING_TYPE,          // 更改异常处理类型
        IA_PUSH_EXCEPTION_HANDLING_TYPE,            // 服务端推送更改异常处理类型
        IA_INQUIRE_EXCEPTION_HANDING_RECORD_COUNT,  // 查询异常处理记录条数
        IA_GET_EXCEPTION_HANDING_RECORD,            // 获取异常处理记录
        IA_UPDATE_EXCEPTION_HANDING_RECORD,         // 更新异常处理记录
        IA_PUSH_EXCEPTION_HANDLING_RECORD,          // 服务端推送更改异常处理记录
        IA_INQUIRE_ENDORSEMENT_RECORD_COUNT,        // 查询补签记录条数
        IA_GET_ENDORSEMENT_RECORD,                  // 获取补签记录
        IA_UPDATE_ENDORSEMENT_RECORD,               // 更新补签记录
        IA_PUSH_ENDORSEMENT_RECORD,                 // 服务端推送更改补签记录
        IA_GET_ATTANDANCE_CONFIG,                   // 获取人脸考勤配置信息
        IA_SET_ATTANDANCE_CONFIG,                   // 设置人脸考勤配置信息
        IA_PUSH_ATTANDANCE_CONFIG,                  // 服务端推送人脸考勤配置信息

        ///////////////////////////////////AI应用重构后的组织架构///////////////////////////////////////
        IA_GET_STAFF_ORGANIZATION = 0x00001300,     // 获取人员组织架构
        IA_ADD_STAFF_ORGANIZATION_POINT,            // 新增人员组织架构节点
        IA_UPDATE_STAFF_ORGANIZATION_POINT,         // 修改人员组织架构节点
        IA_DELETE_STAFF_ORGANIZATION_POINT,         // 删除人员组织架构节点
        IA_PUSH_STAFF_ORGANIZATION_MODIFY,          // 推送人员组织架构修改
        IA_DELETE_STAFF_ORGANIZATION_POINT_RECURSIVELY, // 删除人员组织架构所有空节点

        ///////////////////////////////////AI应用重构后的人员///////////////////////////////////////
        IA_UPDATE_PERSON_FATHER_NODE = 0x00001400,              // 修改人员组织--异步接口
        IA_UPDATE_PERSON_FATHER_NODE_SYNC,                      // 修改人员组织--同步接口
        IA_PUSH_PERSON_FATHER_NODE_MODIFY,                      // 推送人员组织修改

        ///////////////////////////////////AI应用重构后的人员权限///////////////////////////////////////
        IA_GET_DEVICE_AUTHORITY_BY_PERSON = 0x00001500,                             // 通过人员获取设备权限列表
        IA_GET_PERSON_AUTHORITY_BY_DEVICE,                                          // 通过设备获取人员权限列表
        IA_UPDATE_DEVICE_AUTHORITY_BY_PERSON,                                       // 通过人员更新设备权限列表--异步接口
        IA_UPDATE_DEVICE_AUTHORITY_BY_PERSON_SYNC,                                  // 通过人员更新设备权限列表--同步接口
        IA_UPDATE_DEVICE_AUTHORITY_BY_ORGANIZATIO,                                  // 通过组织更新设备权限列表--异步接口
        IA_UPDATE_DEVICE_AUTHORITY_BY_ORGANIZATIO_SYNC,                             // 通过组织更新设备权限列表--同步接口
        IA_UPDATE_PERSON_AUTHORITY_BY_DEVICE,                                       // 通过设备更新人员权限列表--异步接口
        IA_UPDATE_PERSON_AUTHORITY_BY_DEVICE_SYNC,                                  // 通过设备更新人员权限列表--同步接口
        IA_UPDATE_ORGANIZATIO_AUTHORITY_BY_DEVICE,                                  // 通过设备更新组织权限列表--异步接口
        IA_UPDATE_ORGANIZATIO_AUTHORITY_BY_DEVICE_SYNC,                             // 通过设备更新组织权限列表--同步接口
        IA_REISSUE_PERSON_TO_DEVICE,                                                // 重新下发人员到设备上--异步接口
        IA_REISSUE_PERSON_TO_DEVICE_SYNC,                                           // 重新下发人员到设备上--同步接口
        IA_DELETE_PERSON_FROM_DEVICE,                                               // 删除人员与设备的对应关系--异步接口
        IA_DELETE_PERSON_FROM_DEVICE_SYNC,                                          // 删除人员与设备的对应关系--同步接口
        IA_PUSH_PERSON_TO_DEVICE_INFO,                                              // 推送人员向设备下发的状态
        IA_GET_DEVICE_AUTHORITY_BY_ORGANIZATION,                                    // 通过组织获取设备权限列表
        IA_GET_DEVICE_LIST_BY_ORGANIZATION,                                         // 通过组织获取组织设备列表

        ///////////////////////////////////智慧社区///////////////////////////////////////
        IA_GET_SMART_COMMUNITY_VILLAGE = 0x00001600,             //获取小区
        IA_ADD_SMART_COMMUNITY_VILLAGE,                          //新增小区
        IA_UPDATE_SMART_COMMUNITY_VILLAGE,                       //修改小区
        IA_DELETE_SMART_COMMUNITY_VILLAGE,                       //删除小区
        IA_PUSH_SMART_COMMUNITY_VILLAGE,                         //推送小区修改
        IA_GET_SMART_COMMUNITY_BUILDING,                         //获取楼栋
        IA_ADD_SMART_COMMUNITY_BUILDING,                         //新增楼栋
        IA_UPDATE_SMART_COMMUNITY_BUILDING,                      //修改楼栋
        IA_DELETE_SMART_COMMUNITY_BUILDING,                      //删除楼栋
        IA_PUSH_SMART_COMMUNITY_BUILDING,                        //推送楼栋修改
        IA_GET_SMART_COMMUNITY_HOUSE,                            //获取房屋
        IA_ADD_SMART_COMMUNITY_HOUSE,                            //新增房屋
        IA_UPDATE_SMART_COMMUNITY_HOUSE,                         //修改房屋
        IA_DELETE_SMART_COMMUNITY_HOUSE,                         //删除房屋
        IA_PUSH_SMART_COMMUNITY_HOUSE,                           //推送房屋修改
        IA_GET_SMART_COMMUNITY_CAR,                              //获取车辆
        IA_ADD_SMART_COMMUNITY_CAR,                              //新增车辆
        IA_UPDATE_SMART_COMMUNITY_CAR,                           //修改车辆
        IA_DELETE_SMART_COMMUNITY_CAR,                           //删除车辆
        IA_PUSH_SMART_COMMUNITY_CAR,                             //推送车辆修改
        IA_GET_HOUSE_BY_PERSON,                                  //通过人员获取房屋
        IA_GET_CAR_BY_PERSON,                                    //通过人员获取车
        IA_GET_SMART_COMMUNITY_COMPANY,                          //获取单位
        IA_ADD_SMART_COMMUNITY_COMPANY,                          //新增单位
        IA_UPDATE_SMART_COMMUNITY_COMPANY,                       //修改单位
        IA_DELETE_SMART_COMMUNITY_COMPANY,                       //删除单位
        IA_PUSH_SMART_COMMUNITY_COMPANY,                         //推送单位修改
        IA_GET_SMART_COMMUNITY_COMPANY_PERSON,                   //获取单位人员
        IA_ADD_SMART_COMMUNITY_COMPANY_PERSON,                   //新增单位人员
        IA_UPDATE_SMART_COMMUNITY_COMPANY_PERSON,                //修改单位人员
        IA_DELETE_SMART_COMMUNITY_COMPANY_PERSON,                //删除单位人员
        IA_PUSH_SMART_COMMUNITY_COMPANY_PERSON,                  //推送单位人员修改
        IA_GET_LABEL_PERSON_COUNT,                               //获取所有标签下对应人的总数，比如老年人总数、残疾人总数
        IA_GET_VILLAGE_STATISTICS,                               //一标六实
        IA_GET_SMART_COMMUNITY_HOUSE_PERCEPTION,                 //房屋感知
        IA_GET_SMART_COMMUNITY_POWER,                            //获取实有力量
        IA_ADD_SMART_COMMUNITY_POWER,                            //新增实有力量
        IA_UPDATE_SMART_COMMUNITY_POWER,                         //修改实有力量
        IA_DELETE_SMART_COMMUNITY_POWER,                         //删除实有力量
        IA_PUSH_SMART_COMMUNITY_POWER,                           //推送实有力量修改
        IA_GET_LABEL_POWER_COUNT,                                //获取所有实有力量分类下对应人的总数,比如保安总数、物业人员总数
        IA_GET_VIL_PERSON_CNT_BY_CATEGOGY,                      //分类统计小区人数，支持按户籍/性别/年龄/省份划分统计
        IA_GET_TOPN_VILLAGE_BY_LABEL,                       	 //统计重点关注人员个数排名前N位的小区
		IA_QUICKADD_SMART_COMMUNITY_BUILDING,                    //快速添加楼栋
		IA_QUICKADD_SMART_COMMUNITY_HOUSE,                       //快速添加房屋
		IA_GET_SMART_COMMUNITY_CUSTOM_LABEL,                     //获取人员自定义标签
		IA_ADD_SMART_COMMUNITY_CUSTOM_LABEL,                     //新增人员自定义标签
		IA_UPDATE_SMART_COMMUNITY_CUSTOM_LABEL,                  //修改人员自定义标签
		IA_DELETE_SMART_COMMUNITY_CUSTOM_LABEL,                  //删除人员自定义标签
		IA_PUSH_SMART_COMMUNITY_CUSTOM_LABEL,                    //推送人员自定义标签
		IA_GET_HOUSE_STATUS_COUNT,                               //根据房屋状态统计房屋数，如自住、出租、空置的房屋数

        ///////////////////////////////////////////////////////////////////////////////
        // 以下通过手机app、微信公众号等途径预约时使用
        IA_GET_VISITOR_APPOINTMENT = 0x00001800,    // 客户端 主动获取 访客预约信息
        IA_UPDATE_VISITOR_APPOINTMENT,              // 服务端 主动推送 访客预约信息
        IA_APPLY_VISITOR_APPOINTMENT,               // 客户端 审核 访客预约信息
        //other////////////////////////////////////////////////////////////////////////
        IA_SET_JSON_CMD = 0x00002000,       // 设置Json命令 客流查询
        IA_SET_FACE_FROM_1400,              //1400发送给智能的人脸信息
        IA_SET_ADD_VISITOR_LOG,              // 新加 访客配置信息记录表
        IA_SET_MODIFY_VISITOR_LOG,           // 修改 访客配置信息记录表
        IA_FUZZY_QUERRY_VISITOR_LOG,         // 模糊查询
        IA_GET_PERSON_INFO_GUID,             // 根据（库Id，目标Id）获取Person_Info表的Guid
        IA_QUERRY_PERSON_INFO,               // 查询 个人信息表    (弃用)
        IA_MODIFY_PERSON_INFO,               // 修改 个人信息表    (弃用)使用时，需重新调试
        IA_QUERRY_CHANNEL_LOG,               // 查询通道日志（轨迹列表）
        IA_SET_DEL_VISITOR_TARGET,           // 删除 访客目标
        IA_SET_PERSON_FROM_1400,            //1400发送给智能的人员信息
        IA_SET_MOTORVEHICLES_FROM_1400,     //1400发送给智能的机动车信息
        IA_SET_NONMOTORVEHICLES_FROM_1400,  //1400发送给智能的非机动车信息
        IA_GET_1400_TOTAL,                   // 查询 1400 协议结构化数据 总条数
        IA_GET_1400_PERSON_LIST,             // 查询 1400 协议结构化数据 列表
        IA_GET_1400_MOTOR_LIST,              // 查询 1400 协议结构化数据 列表
        IA_GET_1400_NONMOTOR_LIST,           // 查询 1400 协议结构化数据 列表
        IA_GET_1400_PERSON_DETAIL,           // 查询 1400 协议结构化数据 详情
        IA_GET_1400_MOTOR_DETAIL,            // 查询 1400 协议结构化数据 详情
        IA_GET_1400_NONMOTOR_DETAIL,         // 查询 1400 协议结构化数据 详情
        IA_ADD_THIRD_PLATFORM_INFO,          // 新增 访客第三方平台配置信息
        IA_QUERY_THIRD_PLATFORM_INFO,        // 查询 访客第三方平台配置信息
        IA_PUSH_THIRD_PLATFORM_INFO,         // 推送 访客第三方平台信息
        IA_GET_APPROVAL_INFO,                // 查询 访客-审批信息表
        IA_UPDATE_VISITOR_BIND_DEV_STATUS,   // 设备绑定状态通知
        IA_UPDATE_VISITOR_PASS_STATUS,       // 通行状态通知

        //////////////////////////////////////////////////////////////////////////
        IA_SET_INDUSTRIAL_FEBRILE_INFO = 0x00002100,    // 转发推送工业测温数据
        IA_GET_INDUSTRIAL_FEBRILE_COUNT,                // 客户端获取工业测温数据个数
        IA_GET_INDUSTRIAL_FEBRILE_INFO,                 // 客户端获取工业测温数据
        IA_PUSH_INDUSTRIAL_FEBRILE_INFO,                // 智能推送工业测温数据

        ////////////////////////////////////////////////////////////////////////
        // 访客预约审核
        IA_PUSH_APPOINTMENT = 0x00002500, // 访客预约信息推送至客户端
        IA_GET_APPOINTMENT_LIST,          // 客户端拉取访客记录列表
        IA_GET_APPOINTMENT,               // 客户端获取访客预约记录
        IA_AUDIT_APPOINTMENT,             // 客户端审核访客预约状态
        IA_GET_FANS_LIST,                 // 客户端获取关注人员列表
        IA_SET_APPOINTMENT_ADMIN,         // 设置访客预约的管理者
        IA_GET_APPOINTMENT_ADMIN,         // 获取访客预约的配置（含二维码）
        IA_PUSH_APPOINTMENT_STATE,        // 智能服务状态推送
        IA_CMD_END,
    };
};
#pragma pack(pop)

#endif ///NET_PROTOCOL_DEFINE_FORNVMS_H
