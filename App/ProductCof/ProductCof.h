#ifndef __PRODUCTCOF_H__
#define __PRODUCTCOF_H__

#include "Common.h"

/**
#不需要的选项可以(1)不填写 或 (2)行前加# 或 (3)整行删除，避免误操作

[STATION]
#需要连接路由的 名称(等号后面直接填写，不要加其他无关字符/符号，空代表不连接)
stationssid=TP-LINK_DG
#需要连接路由的 密码(等号后面直接填写，不要加其他无关字符/符号)
stationpwd=dgiot0202

[CONST_PARAM]
ircut_flip=0    	   # ircut状态 与flash默认状态 是否相反 -- 1:相反 , 0:正常
image_flip             # image 与flash默认状态 是否相反 -- 1:相反 , 0:正常
play_vol=100
capture_vol=100
ptz_opposite_run=0     #云台控制方向反转状态 -- 0:默认（画面方向一致，拖地图） ; 1:双反 ；2.水平反转； 3垂直翻转
lamp_board=0           #灯板输入的逻辑 0-正， 1-反 默认0
[DEFAULT_SETTING]
telnet=0               #telnet 0禁止 1使能 
language=1000          # 提示语言类型 -- 1000:中文 , 1100:英文 （其他数值无效
**/

typedef struct _PRODUCT_COF {
	unsigned int language; 			// 提示语言类型 -- 1000:中文 , 1100:英文 （其他数值无效）
	unsigned char enable;   		// 0:无效,1:有效
	unsigned char ircut_flip; 		// ircut状态 与flash默认状态 是否相反 -- 1:相反 , 0:正常
	unsigned char image_flip; 		// image 与flash默认状态 是否相反 -- 1:相反 , 0:正常
	unsigned char manual_ircut; 	// 0:自动切换ircut, 1:手动切换    仅在测试模式时有效
	unsigned char ptz_opposite_run; // 云台控制方向反转状态 -- 0:默认（画面方向一致，拖地图） ; 1:双反 ；2.水平反转； 3垂直翻转
	unsigned char play_vol;      	//播放音量
    unsigned char capture_vol;   	//采集音量
	unsigned char lamp_board;       //灯板输入的逻辑 0-正， 1-反  默认0
	unsigned short lamp_board_value; //灯板输入值 默认 900
	unsigned short lamp_board_value_method; //


//	unsigned char exterior_type;    //外观类型 0-浩方 1-宝锋 2-腾安达 3-海视泰80a
	unsigned int ud_len;            //上下行程步长
	unsigned int lr_len;            //左右行程步长
	unsigned int ud_dir;            //上下方向
	unsigned int lr_dir;            //左右方向
//	unsigned char check_ptz;        //是否自检  0-不自检 1-自检
	unsigned int check_speed;       //自检速度 [100, 900]
	unsigned int run_speed;         //运行速度 [100, 900]
	unsigned int run_ud_speed;      //电机上下运行速度 [100, 900]
	unsigned int track_speed;       //移动追踪转速配置
	unsigned int tracker_step_multiple;
	unsigned int tracker_stop_overtime;
	int up_len;            			//向上步长
	
	unsigned char tracker_lr_dir; 	//移动追踪左右方向配置
	unsigned char tracker_ud_dir; 	//移动追踪上下方向配置
	unsigned char ud_notuse;        //不使用上下追踪
	unsigned char light_ctrl;       //白光灯控制逻辑 0-正 高电平亮灯，低电平关灯，1-反 高电平关灯，低电平亮灯
	unsigned char ir_led_ctrl;      //红外灯控制逻辑 0-正 高电平亮灯，低电平关灯，1-反 高电平关灯，低电平亮灯
	unsigned char smartir_en;       //是否使用软光敏 0-硬光敏 1-软光敏

	unsigned short Day2Night;       //白天切到夜晚阈值  max 1024
	unsigned short Night2Day;       //夜晚切到白天阈值 max 1536
	
	unsigned char pwmfrequency;
	unsigned char md_threshold_low;  //移动侦低测灵敏度
	unsigned char md_threshold_mid;  //移动侦中测灵敏度
	unsigned char md_threshold_high;  //移动侦高测灵敏度
	
	unsigned char speaker_reversed; //喇叭反向使能，0-高电平有效 1-低电平有效
	unsigned char hardtype;         //硬件类型 0-摇头机 1-安保灯 2-球机 3-枪机
	unsigned char auto_light_off;   //是否超时自动关灯，默认关
	unsigned char auto_light_off_time; //超时自动关灯时间 单位：小时
	unsigned char private_motorstatus; //隐私模式电机状态，0-保持不动，1-转到最上 2-转到最下
	unsigned char telnet;
	unsigned char forceBurnLicense; 	//是否强制烧录License
	float stitch_distance;   //标定距离设置
	//
	unsigned char ispmode;         //isp mode 为1支持从tf卡读取图像效果文件
	char ssid[64];                  // 需要连接路由的 名称
	char pwd[64];                   // 需要连接路由的 密码
	char ip[20];
	char gw[20];
	char mask[20];
}PRODUCT_COF_S;

typedef enum {
	PRODUCT_NONE, 			//NONE
	PRODUCT_CONFIG,     	//仅设置配置
	PRODUCT_PRO_TEST,   	//进入生产测试
	PRODUCT_AGING_TEST,  	//进入老化测试
	PRODUCT_CAL_TEST, 		//进入拼接标定测试
	PRODUCT_CAL_AWB_TEST    //进入白平衡标定测试
}PRODUCT_ENUM;

class CProductCof
{
public:
	
	PATTERN_SINGLETON_DECLARE(CProductCof);

	int Init();

	int  GetProductMode();
	PRODUCT_COF_S & GetProductCof();

private:
	/// 加载配置文件
	int  LoadFile();
	/// 检查是否存在生产配置文件
	int  CheckProductConf();
	bool IsAgingTest(const char *path);
	int  GetCalTest(const char *path);
	int	 GetProductCof(PRODUCT_COF_S *pCof, const char *path, int type);
	int  SetProductCof(PRODUCT_COF_S *pCof, const char *path);

private:
	PRODUCT_COF_S m_productCof;
	int m_productMode;
};

#define g_ProductCofHandle (*CProductCof::instance())
#define ProductCof_g CProductCof::instance()->GetProductCof()

#endif// __PRODUCTCOF_H__

