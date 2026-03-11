
#ifndef __EROORCODE_DEF__
#define __EROORCODE_DEF__
/*
1、错误码定义的标准为一个DWORD类型的值，第29位必须为1以保证与MS定义的错误码做区分。
2、所有用户函数返回错误的地方都可以通过GetLastError获取最终错误码。


这样做的好处：可以随时获取函数返回错误的原因，不需要错误码在函数中层层上传。

*/
 

#define USER_ERROR_FLAG             0x20000000 ///////////////用户错误都需要这个位为1


//
// MessageId: USER_ERROR_NODE_ID_EXISTS
//
// MessageText:节点ID已经存在
//
#define USER_ERROR_NODE_ID_EXISTS              USER_ERROR_FLAG+0x01
#define USER_ERROR_NODE_EXISTS                 USER_ERROR_NODE_ID_EXISTS
//
// MessageId: USER_ERROR_UNKNOWN
//
// MessageText:未知错误
//
#define USER_ERROR_UNKNOWN              USER_ERROR_FLAG+0x02


//
// MessageId: USER_ERROR_DISK_SPACE_NO_ENOUGH
//
// MessageText:磁盘空间不足
//
#define USER_ERROR_DISK_SPACE_NO_ENOUGH              USER_ERROR_FLAG+0x03


//
// MessageId: USER_ERROR_NETNODE_ID_CONFLICT
//
// MessageText:网络节点ID冲突 一般在加入网络节点时出现
//
#define USER_ERROR_NETNODE_ID_CONFLICT            USER_ERROR_FLAG+0x04

//
// MessageId: USER_ERROR_NETNODE_INITIAL_ERROR
//
// MessageText:网络节点初始化错误
//
#define USER_ERROR_NETNODE_INITIAL_ERROR            USER_ERROR_FLAG+0x05

//
// MessageId: USER_ERROR__CREATE_MSU_CHAL_TABLE_ERROR
//
// MessageText:创建MSU服务器所存储通道的表不成功！
//
#define USER_ERROR__CREATE_MSU_CHAL_TABLE_ERROR            USER_ERROR_FLAG+0x06

//
// MessageId: USER_ERROR__DELETE_MSU_CHAL_TABLE_ERROR
//
// MessageText:删除MSU服务器所存储通道的表不成功！
//
#define USER_ERROR__DELETE_MSU_CHAL_TABLE_ERROR            USER_ERROR_FLAG+0x07

//
// MessageId: USER_ERROR__CREATE_MDU_DEVICE_TABLE_ERROR
//
// MessageText:创建MDU服务器所存转发设备的表不成功！
//
#define USER_ERROR__CREATE_MDU_DEVICE_TABLE_ERROR            USER_ERROR_FLAG+0x08

//
// MessageId: USER_ERROR__DELETE_MDU_DEVICE_TABLE_ERROR
//
// MessageText删除MDU服务器所存转发设备的表不成功！
//
#define USER_ERROR__DELETE_MDU_DEVICE_TABLE_ERROR            USER_ERROR_FLAG+0x09


//
// MessageId: USER_ERROR__GET_INFO_ITEMID_ERROR
//
// 获取信息时ItemID错误
//
#define USER_ERROR__GET_INFO_ITEMID_ERROR            USER_ERROR_FLAG+0x0A

//
// MessageId: USER_ERROR__CANNOT_FIND_NODE_ERROR
//
// 不能找到节点
//
#define USER_ERROR__CANNOT_FIND_NODE_ERROR            USER_ERROR_FLAG+0x0B

//
// MessageId: USER_ERROR__CANNOT_FIND_NODE_ERROR
//
// 此节点没有子节点
//
#define USER_ERROR__NO_CHILD_NODE_ERROR            USER_ERROR_FLAG+0x0C


//
// MessageId: USER_ERROR__NO_PARENT_NODE_ERROR
//
// 此节点没有父节点
//
#define USER_ERROR__NO_PARENT_NODE_ERROR			USER_ERROR_FLAG+0x0D

//
// MessageId: USER_ERROR_ADD_FRAME_TYPE_INEXISTENT
//
// 加入错误的帧数据到列表,说明加入的帧类型不存在
//
#define USER_ERROR_ADD_FRAME_TYPE_INEXISTENT			USER_ERROR_FLAG+0x0E

//
// MessageId: USER_ERROR_SEND_OVERTIME
//
// 发送数据超时
//
#define USER_ERROR_SEND_OVERTIME			            USER_ERROR_FLAG+0x0F

//
// MessageId: USER_ERROR_MODULE_NO_INITIAL
//
// 初始化失败
//
#define USER_ERROR_MODULE_NO_INITIAL			            USER_ERROR_FLAG+0x10

//
// MessageId: USER_ERROR_INVALID_POINT
//
// 无效的坐标值 
//
#define USER_ERROR_INVALID_POINT			            USER_ERROR_FLAG+0x11

//
// MessageId: USER_ERROR_CANNOT_FIND_CHMDU
//
// 找不到转发通道的转发服务器 
//
#define USER_ERROR_CANNOT_FIND_CHMDU			            USER_ERROR_FLAG+0x12

//
// MessageId: USER_ERROR_NODE_NET_DISCONNECT
//
// 节点网络连接断开
//
#define USER_ERROR_NODE_NET_DISCONNECT			            USER_ERROR_FLAG+0x13

//
// MessageId: USER_ERROR_CHANNEL_NO_OPEN_VIDEO
//
// 通道没有打开视频
//
#define USER_ERROR_CHANNEL_NO_OPEN_VIDEO   USER_ERROR_FLAG+0x14


//
// MessageId: USER_ERROR_STREAM_PENDING
//
// 此流的请求稍后会被完成
//
#define USER_ERROR_STREAM_PENDING		            USER_ERROR_FLAG+0x15

//
// MessageId: USER_ERROR_FAIL
//
// 失败
//
#define USER_ERROR_FAIL			                            USER_ERROR_FLAG+0x16



//
// MessageId: USER_ERROR_NODE_NET_DISCONNECT
//
// 节点不在线,当不能与这个节点链接时,这个节点不在线 
//
#define USER_ERROR_NODE_NET_OFFLINE			    USER_ERROR_FLAG+0x17

//
// MessageId: USER_ERROR_UNSUPPORTED_NODE
//
// 不支持的节点
//
#define USER_ERROR_UNSUPPORTED_NODE			    USER_ERROR_FLAG+0x18

//路由错误，注册服务器不在线或者注册服务器找不到相关路由信息
#define USER_ERROR_ROUTE_ERROR						    USER_ERROR_FLAG+0x19

//使用的节点ID是无效的
#define USER_ERROR_INVLID_NODE						    USER_ERROR_FLAG+0x1A
	
//服务还没准备好
#define USER_ERROR_NO_READY								USER_ERROR_FLAG+0x1C

//任务不存在
#define USER_ERROR_TASK_NO_EXISTS						USER_ERROR_FLAG+0x1D

//无录像数据
#define USER_ERROR_NO_RECORDDATA					USER_ERROR_FLAG+0x1E

//无效参数,在命令传输的参数里出现没法处理的参数时返回此值
#define USER_ERROR_INVALID_PARAM						USER_ERROR_FLAG+0x1F

//不支持的命令
#define USER_ERROR_UNSUPPORTED_CMD			    USER_ERROR_FLAG+0x20

//设备忙,不能请求
#define USER_ERROR_DEVICE_BUSY				            USER_ERROR_FLAG+0x21

//端口监听失败
#define USER_ERROR_LISTEN_FAIL				            USER_ERROR_FLAG+0x22

//此用户不存在
#define USER_ERROR_NO_USER								USER_ERROR_FLAG+0x23

//密码错误
#define USER_ERROR_PWD_ERR								USER_ERROR_FLAG+0x24

//用户已经登陆
#define USER_ERROR_USER_ALREDAY_LOGIN			USER_ERROR_FLAG+0x25

//用户被限制在特有的电脑上MAC、IP
#define USER_ERROR_USER_LIMITED							USER_ERROR_FLAG+0x26

//用户被锁定，暂时无法使用
#define USER_ERROR_USER_LOCKED							USER_ERROR_FLAG+0x27

//自己登陆自己
#define USER_ERROR_LOGIN_SELF							USER_ERROR_FLAG+0x28

//权限不够
#define USER_ERROR_NO_AUTH								USER_ERROR_FLAG+0x29

//系统忙,不能请求
#define USER_ERROR_SYSTEM_BUSY				            USER_ERROR_FLAG+0x30

//文件流被完成，不是用户主动结束的，而是由于网络原因或者文件结束后自动完成的
#define USER_ERROR_FILE_STREAM_COMPLETED				USER_ERROR_FLAG+0x31

//获取配置信息失败
#define USER_ERROR_GET_CONFIG_INFO_FAIL					USER_ERROR_FLAG+0x32

//正在被另外一个用户配置，请等待退出后再进入
#define USER_ERROR_ANOTHER_USER_HASENTER					USER_ERROR_FLAG+0x33

//登录超时错误
#define USER_ERROR_LOGIN_OVERTIME					USER_ERROR_FLAG+0x34

//用户音频打开失败，由于视频没有打开导致的 
#define USER_ERROR_CHANNEL_AUDIO_OPEN_FAIL					USER_ERROR_FLAG+0x35

//未登陆成功的情况下发送了指令，返回此值
#define USER_ERROR_NOLOGIN					USER_ERROR_FLAG+0x36

//找不到地图
#define USER_ERROR_CANNOT_FIND_MAP_ERROR			USER_ERROR_FLAG+0x37

//没有父地图
#define USER_ERROR_NO_PARENT_MAP_ERROR				USER_ERROR_FLAG+0x38

//没有子地图
#define USER_ERROR_NO_CHILD_MAP_ERROR				USER_ERROR_FLAG+0x39

//名称已经存在
#define USER_ERROR_NAME_EXISTED					    USER_ERROR_FLAG + 0x3A

//保存地图文件失败
#define USER_ERROR_MAP_SAVE_ERROR		            USER_ERROR_FLAG + 0x3B

//没有电子地图的信息
#define USER_ERROR_EMAP_NO_INFO		                USER_ERROR_FLAG + 0x3C


//
// MessageId: USER_ERROR_NOSUPPORT_DEV_VERSION
//
// 不支持的前端设备的版本
//
#define USER_ERROR_NOSUPPORT_DEV_VERSION		            USER_ERROR_FLAG+0x3D

//设备回放请求才用此状,等待上一个请求返回
#define USER_ERROR_STREAM_WAITING							USER_ERROR_FLAG+0x3E

//设备不支持此功能
#define USER_ERROR_UNSUPPORTED_FUNC							USER_ERROR_FLAG+0x3F

//设备类型错误
#define USER_ERROR_DEVICE_TYPE_ERROR						USER_ERROR_FLAG+0x40

//文件类型错误
#define USER_ERROR_FILE_TYPE_ERROR						   USER_ERROR_FLAG+0x41

//文件已经存在
#define USER_ERROR_FILE_EXISTED								USER_ERROR_FLAG + 0x42

//文件不存在
#define USER_ERROR_FILE_NO_EXISTED							USER_ERROR_FLAG + 0x43

//文件打开(创建)错误
#define USER_ERROR_OPEN_FILE_ERROR							USER_ERROR_FLAG + 0x44


//
// MessageId: USER_ERROR_EXISTED_CHILD_NODE
//
// 此节点有子节点
//
#define USER_ERROR_EXISTED_CHILD_NODE            USER_ERROR_FLAG+0x45


//
// MessageId: USER_ERROR_DEV_RESOURCE_LIMITED
//
// 设备资源限制
//
#define USER_ERROR_DEV_RESOURCE_LIMITED            USER_ERROR_FLAG+0x46

//
// MessageId: USER_ERROR_DECODE_RESOURCE_LACK
//
// 没有解码资源,没有解码引擎
//
#define USER_ERROR_DECODE_RESOURCE_LACK				USER_ERROR_FLAG+0x47

//
// MessageId: USER_ERROR_DECODE_RESOURCE_LIMITED
//
// 解码资源限制,丢帧解码
//
#define USER_ERROR_DECODE_RESOURCE_LIMITED			USER_ERROR_FLAG+0x48

//无对应的录像日志
#define USER_ERROR_NO_RECORD_LOG								USER_ERROR_FLAG+0x49

//录像读取任务过多
#define USER_ERROR_READ_TASK_TOO_MUCH						USER_ERROR_FLAG+0x4A

//IP地址格式错误
#define USER_ERROR_INVALID_IP           USER_ERROR_FLAG+0x50
//子网掩码格式错误
#define USER_ERROR_INVALID_SUBMASK      USER_ERROR_FLAG+0x51
//无效IP地址和子网掩码合并。IP地址的主机地址部分里的所有字节都被设置为1
#define USER_ERROR_IP_MASK_ALL1      USER_ERROR_FLAG+0x52
//无效IP地址和子网掩码合并。IP地址的主机地址部分里的所有字节都被设置为0
#define USER_ERROR_IP_MASK_ALL0      USER_ERROR_FLAG+0x53
//无效网关地址和子网掩码合并。网关的主机地址部分里的所有字节都被设置为1
#define USER_ERROR_ROUTE_MASK_ALL1      USER_ERROR_FLAG+0x54
//无效网关地址和子网掩码合并。网关的主机地址部分里的所有字节都被设置为0
#define USER_ERROR_ROUTE_MASK_ALL0      USER_ERROR_FLAG+0x55
//以127起头的IP地址无效，因为它们保留用作环回地址。请指定一个介于 1 和 223 之间的数值
#define USER_ERROR_USE_LOOPBACK      USER_ERROR_FLAG+0x56
//IP地址或网关不是以一个有效的值开头。请指定一个介于 1 和 223 之间的数值
#define USER_ERROR_IP_ROUTE_INVALID      USER_ERROR_FLAG+0x57
//输入一个无效的子网掩码，子网掩码必须是相邻的
#define USER_ERROR_MASK_NOT_CONTINE      USER_ERROR_FLAG+0x58
//网关不在由IP地址和子网掩码定义的同一网段上
#define USER_ERROR_DIFFERENT_SEGMENT      USER_ERROR_FLAG+0x59
//网关地址格式错误
#define USER_ERROR_INVALID_GATEWAY           USER_ERROR_FLAG+0x5A
//域名格式错误
#define USER_ERROR_INVALID_DOMAIN_NAME    USER_ERROR_FLAG+0x5B

//超出数量限制
#define USER_ERROR_OVER_LIMIT    USER_ERROR_FLAG+0x5C

//超出流量限制
#define USER_ERROR_OVER_BANDWIDTH_LIMIT		USER_ERROR_FLAG+0x5D

//区域下有相关资源
#define USER_ERROR_AREA_EXISTED_CHILD_NODE		USER_ERROR_FLAG+0x5E

//配置中含有特殊字符
#define USER_ERROR_SPECIAL_CHAR	 USER_ERROR_FLAG+0x5F

//配置中含有特殊字符@
#define USER_ERROR_SPECIAL_CHAR_2	 USER_ERROR_FLAG+0x60

//文件校验失败
#define USER_ERROR_CHECK_FILE_ERROR		 USER_ERROR_FLAG+0x61

//服务不在线 
#define USER_ERROR_SERVER_OFF_LINE		 USER_ERROR_FLAG+0x62

//服务不存在
#define USER_ERROR_SERVER_NO_EXISTS		 USER_ERROR_FLAG+0x63

//键盘编号已存在
#define USER_ERROR_KEYBOARDINDEX_EXISTS		 USER_ERROR_FLAG+0x64

//键盘编号有误
#define USER_ERROR_KEYBOARDINDEX_ERROR		 USER_ERROR_FLAG+0x65

//角色下面存在用户
#define USER_ERROR_EXISTED_USER_NODE            USER_ERROR_FLAG+0x66

//角色不存在
#define USER_ERROR_USERGOURP_NO_EXISTS	           USER_ERROR_FLAG+0x67

//仅超级管理员能新增主区域
#define USER_ERROR_ONLY_ADMIN_CAN_ADDROOTAREA								USER_ERROR_FLAG+0x68

//无父区域权限
#define USER_ERROR_NO_PARENT_AREA_AUTH								USER_ERROR_FLAG+0x69

//必须保留一个区域
#define USER_ERROR_MUST_SAVE_ONE_AREA								USER_ERROR_FLAG+0x6A

//无此区域权限
#define USER_ERROR_NO_CUR_AREA_AUTH								USER_ERROR_FLAG+0x6B

//IP段间隔过大
#define USER_ERROR_IP_SEGMENT_TO_LONG								USER_ERROR_FLAG+0x6C

//序列号不能为空
#define USER_ERROR_SERIAL_NOTNULL								USER_ERROR_FLAG+0x6D

//序列号已经存在
#define USER_ERROR_SERIAL_EXISTS								USER_ERROR_FLAG+0x6E

//当前协议不能用IP段方式
#define USER_ERROR_IP_SEGMENT_NO_PROTOCAL								USER_ERROR_FLAG+0x6F

//同一个角色下的用户不能修改
#define USER_ERROR_NO_AUTH_EDIT_SAMERIGHT								USER_ERROR_FLAG+0x70

//转发服务器下挂有设备,不能删除
#define USER_ERROR_MDU_HAVEDEVICE								USER_ERROR_FLAG+0x71

//电视墙下挂有解码设备,不能删除
#define USER_ERROR_WALL_HAVEDECODER								USER_ERROR_FLAG+0x72

//接入服务器下挂有报警主机,不能删除
#define USER_ERROR_JSU_HAVEALARMHOST								USER_ERROR_FLAG+0x73

// 整理后 2017-08-28后新增
//热点已存在
#define USER_ERROR_HOT_POINT_EXISTS								USER_ERROR_FLAG+0x74

//导入数据中通道数超出数量限制
#define USER_ERROR_IMPORTCONFIG_OVER_LIMIT    USER_ERROR_FLAG+0x75

//文件不匹配
#define USER_ERROR_FILE_MISMATCHING USER_ERROR_FLAG+0x76

//admin不允许删除
#define USER_ERROR_CANNOT_DEL_ADMIN USER_ERROR_FLAG+0x77

//当前登录用户不允许删除
#define USER_ERROR_CANNOT_DEL_CUR_USER USER_ERROR_FLAG+0x78

//存储服务器下挂有设备,不能删除
#define USER_ERROR_MSU_HAVEDEVICE								USER_ERROR_FLAG+0x79

//报警服务器必须有且只允许存在一个
#define USER_ERROR_ONLY_ONE_ADU_EXISTED USER_ERROR_FLAG+0x7A

//IP端口与已存在同类型节点重复
#define USER_ERROR_IP_PORT_SERVER_EXISTS USER_ERROR_FLAG+0x7B

//电视墙服务器下挂有墙,不能删除
#define USER_ERROR_TVWALLSERVER_HAVEWALL								USER_ERROR_FLAG+0x7C

//2017-12-25新增
//子系统数量范围为1-32
#define USER_ERROR_LIMIT_MAX_SUBSYSTEM_NUM								USER_ERROR_FLAG+0x7D
//防区数量范围为1-512
#define USER_ERROR_LIMIT_MAX_ZONE_NUM						USER_ERROR_FLAG+0x7E
//2018-01-08新增
//接入服务器下挂有门禁系统,不能删除
#define USER_ERROR_JSU_HAVEACSSYSTEM								USER_ERROR_FLAG+0x7F

//2018-01-23新增
//连接管理服务器失败
#define USER_ERROR_CONNECT_RSU_FAILED			                            USER_ERROR_FLAG+0x80

//2018-01-29新增
//热区已存在
#define USER_ERROR_HOT_AREA_EXISTS								USER_ERROR_FLAG+0x81

//2018-01-29新增
//传入时间范围不正确
#define USER_ERROR_TIME_RANGE_ERROR							USER_ERROR_FLAG+0x82

//2018-04-19新增
//重新打开视频流
#define USER_ERROR_LIVE_RECONNECT							USER_ERROR_FLAG+0x83

//PMS 2018-04-24新增
//车牌已存在
#define USER_ERROR_LICENSEPLATE_EXISTS				USER_ERROR_FLAG+0x84

//国标平台 2018-04-27新增
//国标ID已存在
#define USER_ERROR_GBID_EXISTS USER_ERROR_FLAG+0x85

//国标平台 2018-05-02新增
//级联网关服务器下挂有下级平台或国标设备,不能删除
#define USER_ERROR_CGU_HAVEDEVICE								USER_ERROR_FLAG+0x86

//2018-05-07新增
//回放备份路数超限
#define USER_ERROR_PLAYBACK_BACKUP_OVER_LIMIT							USER_ERROR_FLAG+0x87

//2018-05-28新增
//服务器为单机版，禁止非本机客户端登录
#define USER_ERROR_CLIENT_LIMITED_BY_LITE_TYPE						USER_ERROR_FLAG+0x88

//2018-06-29新增
//客户端平台类型与管理平台不匹配
#define USER_ERROR_CLIENT_LIMITED_PLATFORM_TYPE_MISMATCH				USER_ERROR_FLAG+0x89

//2018-07-04新增
//备份文件平台类型与管理平台不匹配
#define USER_ERROR_LIMITED_PLATFORM_TYPE_MISMATCH				USER_ERROR_FLAG+0x8A
//备份文件版本号与管理平台不匹配
#define USER_ERROR_LIMITED_PLATFORM_VERSION_MISMATCH				USER_ERROR_FLAG+0x8B

//2018-07-06新增
//非本平台序列号授权文件
#define USER_ERROR_PC_LICENSE_MISMATCH				USER_ERROR_FLAG+0x8C

//2018-07-12新增
//请求码流失败
#define USER_ERROR_NODE_LIVE_STREAM_FAIL				USER_ERROR_FLAG+0x8D

//2018-10-26新增
//平台缺省服务器禁止删除
#define USER_ERROR_DEFAULT_SERVER_FORBID_DELETE				USER_ERROR_FLAG+0x8E

//2019-04-11新增
//通道组下有子通道组,禁止删除
#define USER_ERROR_CHANNELGROUP_EXISTED_CHILD_NODE		USER_ERROR_FLAG+0x8F


//2019-07-05新增 设备表用
//表中没有此sopGUID
#define USER_ERROR_NO_SOP_GUID		USER_ERROR_FLAG+0x90
//名称不能为空
#define USER_ERROR_NAME_IS_NULL     USER_ERROR_FLAG+0x91

//客户端与平台版本号不匹配,登录失败
#define USER_ERROR_CLIENT_LIMITED_PLATFORM_VERSION_MISMATCH		USER_ERROR_FLAG+0x92

//搜索NVR图片列表失败
#define USER_ERROR_SEARCH_PIC_LIST      USER_ERROR_FLAG+0x93

//请求NVR图片数据失败
#define USER_ERROR_REQUEST_PIC_DATA     USER_ERROR_FLAG+0x94

//此报警信息正在被打开
#define USER_ERROR_THE_ALARM_INFO_BE_USING     USER_ERROR_FLAG+0x95

//日志查询和导出的日期不能跨月
#define USER_ERROR_LOG_QUERY_BE_IN_A_MONTH      USER_ERROR_FLAG+0x96

//看守位设置  传入的配置参数不在范围内
#define USER_ERROR_ARG_RANGE_ERROR							USER_ERROR_FLAG+0x97

//手动录像已打开
#define USER_ERROR_MANUAL_RECORD_OPENED						USER_ERROR_FLAG+0x98

//手动录像已关闭
#define USER_ERROR_MANUAL_RECORD_STOPPED						USER_ERROR_FLAG+0x99

//导入配置输入密码与导出不一致
#define USER_ERROR_MANUAL_PASSWORD_FROM_EXPORT                 USER_ERROR_FLAG+0x9A

//导入数据中设备数超出数量限制
#define USER_ERROR_IMPORTCONFIG_OVER_DEV_LIMIT    USER_ERROR_FLAG+0x9B

//导入数据中通道数和设备数都超出限制
#define USER_ERROR_IMPORTCONFIG_OVER_CHN_AND_DEV_LIMIT    USER_ERROR_FLAG+0x9C

//单机设备，拒绝登录
#define USER_ERROR_STAND_ALONE_LIMIT    USER_ERROR_FLAG+0x9D

//解码器设备模式，拒绝CMS登录
#define USER_DECODER_ERR_REFUSED_CMS_LOGIN    USER_ERROR_FLAG+0x9E

//访客记录添加失败
#define USER_ERROR_VISITOR_RECORD_ADDFAIL						USER_ERROR_FLAG+0x9F   

//访客记录修改失败
#define USER_ERROR_VISITOR_RECORD_EDITFAIL						USER_ERROR_FLAG+0xA0  

//访客记录删除失败
#define USER_ERROR_VISITOR_RECORD_DELFAIL						USER_ERROR_FLAG+0xA1 

//http会话超时
#define USER_SESSION_TIMEOUT                                    USER_ERROR_FLAG+0xA2

//http会话找不到
#define USER_SESSION_NOTFOUND                                   USER_ERROR_FLAG+0xA3

//通道对应的任务已存在
#define USER_ERROR_CHANNEL_TASK_EXIST                           USER_ERROR_FLAG+0xA4

//从管理智能服务 移动访客记录失败
#define USER_ERROR_MOVE_VISITOR_RECORD_FROM_RSU2IASU			USER_ERROR_FLAG+0xA5

//GAT1400ID已存在 2020-07-27新增
#define USER_ERROR_GAT1400ID_EXISTS                             USER_ERROR_FLAG+0xA6

//报警输出组下有子报警输出组,禁止删除
#define USER_ERROR_ALARMOUTGROUP_EXISTED_CHILD_NODE				USER_ERROR_FLAG+0xA7
//收件人邮件地址已存在
#define USER_ERROR_EMAIL_EXISTED								USER_ERROR_FLAG+0xA8
//收件人邮件地址不能为空
#define USER_ERROR_EMAIL_ADDR_IS_NULL     USER_ERROR_FLAG+0xA9
//发件人已存在(发件人只一个)
#define USER_ERROR_EMAIL_SENDER_EXISTED								USER_ERROR_FLAG+0xAA

//已有关联热备关系的服务不可以修改为热备服务
#define USER_ERROR_DONT_MODIFY_ISBACKUPSERVER					USER_ERROR_FLAG+0xAB

//没找到cert证书
#define USER_ERROR_HTTPS_CERT_NOT_FOUND					        USER_ERROR_FLAG+0xAC

//cert 已存在
#define USER_ERROR_HTTPS_CERT_EXIST					            USER_ERROR_FLAG+0xAD

//https 已启动
#define USER_ERROR_HTTPS_ACTIVED					            USER_ERROR_FLAG+0xAE

//创建PKCS12失败
#define USER_ERROR_HTTPS_PKCS12_CREATE_FAILED					USER_ERROR_FLAG+0xAF

//加载PKCS12失败
#define USER_ERROR_HTTPS_PKCS12_LOAD_FAILED					    USER_ERROR_FLAG+0xB0
//拒绝登录
#define USER_ERROR_DECODER_REFUSED								USER_ERROR_FLAG+0xB1
//无效命令
#define USER_ERROR_DECODER_INVLIDCMD							USER_ERROR_FLAG+0xB2
//达到最大用户登录数限制
#define USER_ERROR_DECODER_MAXUSER								USER_ERROR_FLAG+0xB3
//达到最大连接数
#define USER_ERROR_DECODER_MAXCONNECTION						USER_ERROR_FLAG+0xB4
//无效端口号
#define USER_ERROR_DECODER_INVALIDPORT							USER_ERROR_FLAG+0xB5
//重复端口号
#define USER_ERROR_DECODER_REPEATPORT							USER_ERROR_FLAG+0xB6
//用户名or密码过长
#define USER_ERROR_NAME_OR_PWD_OVER_LENGTH						USER_ERROR_FLAG+0xB7
//没有磁盘
#define USER_ERROR_NO_DISK										USER_ERROR_FLAG+0xB8
//相同版本
#define USER_ERROR_SAME_VERSION									USER_ERROR_FLAG+0xB9
#endif
