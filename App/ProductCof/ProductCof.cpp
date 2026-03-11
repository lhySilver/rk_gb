#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ProductCof.h"
#include "Inifile.h"

PATTERN_SINGLETON_IMPLEMENT(CProductCof);

CProductCof::CProductCof()
{
	memset(&m_productCof,0,sizeof(PRODUCT_COF_S));

	m_productCof.enable = 0;
	m_productCof.language = 0;
	m_productCof.ircut_flip = 0;
	m_productCof.image_flip = 0;
	m_productCof.manual_ircut = 0;
	m_productCof.ptz_opposite_run = 0; // #云台控制方向反转状态 -- 0:默认双反（画面方向一致，拖地图） ; 1:电机方向一致 ；2.水平与电机方向一致； 3垂直电机方向一致
	m_productCof.play_vol = 100;
	m_productCof.capture_vol = 31;
	m_productCof.lamp_board = 0;
	m_productCof.lamp_board_value = 900;
	m_productCof.ud_len = 100;
	m_productCof.lr_len = 500;
	m_productCof.ud_dir = 0;
	m_productCof.lr_dir = 0;
	m_productCof.ud_notuse = 0;
	m_productCof.check_speed = 1300000;
	m_productCof.run_speed = 4000000;
	m_productCof.run_ud_speed = 4000000;
	m_productCof.track_speed = 1300000;
	m_productCof.tracker_step_multiple = 5;
	m_productCof.tracker_stop_overtime = 30;
	m_productCof.up_len = -50;
	m_productCof.tracker_lr_dir = 0;
	m_productCof.tracker_ud_dir = 0;
	m_productCof.Night2Day = 35; // 640;
	m_productCof.Day2Night = 14; // 16;//900;
	m_productCof.light_ctrl = 0;
	m_productCof.smartir_en = 1;
	m_productCof.ir_led_ctrl = 1;
	m_productCof.pwmfrequency = 1;
	m_productCof.hardtype = 0;
	m_productCof.speaker_reversed = 1;
	m_productCof.auto_light_off = 0;
	m_productCof.auto_light_off_time = 12;
	m_productCof.telnet = 0;
	m_productCof.forceBurnLicense = 0;
	m_productCof.private_motorstatus = 0;
	m_productCof.md_threshold_low = 235;
	m_productCof.md_threshold_mid = 245;
	m_productCof.md_threshold_high = 248;
	m_productCof.stitch_distance = 5.0;
}

CProductCof::~CProductCof()
{

}

int CProductCof::Init()
{
	return LoadFile();
}

int  CProductCof::GetProductMode()
{
	return m_productMode; 
}

PRODUCT_COF_S & CProductCof::GetProductCof()
{
	return m_productCof;
}

int CProductCof::LoadFile()
{
	// 出现主配置丢失的情况,用备份配置恢复主配置
	if ((access(DEV_CONFIG_FILE, F_OK) != 0) &&
		(access(DEV_CONFIG_FILE_BK, F_OK) == 0))
	{
		START_PROCESS("sh", "sh", "-c", "cp -a " DEV_CONFIG_FILE_BK " " DEV_CONFIG_FILE, NULL);
		sync();
	}

	GetProductCof(&m_productCof,DEV_CONFIG_FILE, 1);
	m_productMode = CheckProductConf();
	return 0;
}

int CProductCof::CheckProductConf()
{
	int ret;
	int mode;
	PRODUCT_COF_S conf = {0};

	START_PROCESS("sh", "sh", "-c", "mount " MMCP1" " MOUNT_DIR, NULL);

	//判断是否有audio目录存在
	if(access(AUDIO_SD_DIR, F_OK) == 0)
	{
		START_PROCESS("sh", "sh", "-c", "rm -rf " AUDIO_CONFIG_DIR, NULL);
		START_PROCESS("sh", "sh", "-c", "cp -rf " AUDIO_SD_DIR " " AUDIO_CONFIG_PATH, NULL);
		sync();
	}

	// 判断生产配置文件是否存在
	if (access(MMC_DEV_CONFIG_FILE, F_OK) == 0)
	{
		if (true == IsAgingTest(MMC_DEV_CONFIG_FILE))
		{
			mode = PRODUCT_AGING_TEST;
			goto end;
		}
		memcpy(&conf, &m_productCof, sizeof(PRODUCT_COF_S));
		ret = GetProductCof(&conf, MMC_DEV_CONFIG_FILE, 1); // 配置立即生效
		if (memcmp(&conf, &m_productCof, sizeof(PRODUCT_COF_S)) != 0)
		{
			memcpy(&m_productCof, &conf, sizeof(PRODUCT_COF_S));
			// 保存配置
			SetProductCof(&m_productCof, DEV_CONFIG_FILE);
			START_PROCESS("sh", "sh", "-c", "cp -a " DEV_CONFIG_FILE " " DEV_CONFIG_FILE_BK, NULL);
		}

		if (0 == ret) // 仅修改配置
		{
			mode = PRODUCT_CONFIG;
			goto end;
		}
		else if (1 == ret) // 有WiFi ssid 和 pwd, 进入生产模式
		{
			int CalMode = GetCalTest(MMC_DEV_CONFIG_FILE);
			if (1 == CalMode)
			{
				mode = PRODUCT_CAL_TEST;
			}
			else if (2 == CalMode)
			{
				mode = PRODUCT_CAL_AWB_TEST;
			}
			else
			{
				mode = PRODUCT_PRO_TEST;
			}
			
			goto end;
		}
	}

	mode = PRODUCT_NONE;

end:
	START_PROCESS("sh", "sh", "-c", "umount -l " MOUNT_DIR, NULL);

	return mode;
}

bool CProductCof::IsAgingTest(const char *path)
{
	bool bRet = false;
	char ConfigPath[256];
	int ret = -1;
	int value = 0;
	CInifile ini;
	char valstr [64] = {'\0'};
	
	ret = ini.read_profile_string("AGING_SETTING", "aging_test",valstr, sizeof(valstr),path);
	if (ret==0) {
		value = (int)strtol(valstr, NULL, 0);
		if (1 == value) {
			return true;
		}
	}
	return bRet;
}

int CProductCof::GetCalTest(const char *path)
{
	bool bRet = false;
	char ConfigPath[256];
	int ret = -1;
	int value = 0;
	CInifile ini;
	char valstr [64] = {'\0'};
	
	ret = ini.read_profile_string("CAL_SETTING", "cal_test",valstr, sizeof(valstr),path);
	if (ret==0) {
		value = (int)strtol(valstr, NULL, 0);
	}
	return value;
}

int CProductCof::GetProductCof(PRODUCT_COF_S *pCof, const char *path, int type)
{
	int result = 0;
	char pSSID[64] = {'\0'};
	char pPWD[64] = {'\0'};
	char pIP[20] = {'\0'};
	char pGW[64] = {'\0'};
	char pMASK[64] = {'\0'};

	pCof->enable = 1;
	int ret = -1;
	CInifile ini;
	char valstr [64] = {'\0'};
	
	ret = ini.read_profile_string("CONST_PARAM", "ircut_flip",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->ircut_flip = (int)strtol(valstr, NULL, 0);

    ret = ini.read_profile_string("CONST_PARAM", "image_flip",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->image_flip = (int)strtol(valstr, NULL, 0);
	
	ret = ini.read_profile_string("CONST_PARAM", "manual_ircut",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->manual_ircut = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "ptz_opposite_run",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->ptz_opposite_run = (int)strtol(valstr, NULL, 0);

 ////////

	ret = ini.read_profile_string("CONST_PARAM", "play_vol",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->play_vol = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "capture_vol",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->capture_vol = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "lamp_board",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->lamp_board = (int)strtol(valstr, NULL, 0);
	
	ret = ini.read_profile_string("CONST_PARAM", "lamp_board_value",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->lamp_board_value = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "ud_len",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->ud_len = (int)strtol(valstr, NULL, 0);
	
	ret = ini.read_profile_string("CONST_PARAM", "lr_len",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->lr_len = (int)strtol(valstr, NULL, 0);
	
	ret = ini.read_profile_string("CONST_PARAM", "ud_dir",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->ud_dir = (int)strtol(valstr, NULL, 0);
	
	ret = ini.read_profile_string("CONST_PARAM", "lr_dir",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->lr_dir = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "ud_notuse",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->ud_notuse = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "check_speed",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->check_speed = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "Day2Night",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->Day2Night = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "Night2Day",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->Night2Day = (int)strtol(valstr, NULL, 0);
	
	ret = ini.read_profile_string("CONST_PARAM", "run_speed",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->run_speed = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "run_ud_speed",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->run_ud_speed = (int)strtol(valstr, NULL, 0);
	
	ret = ini.read_profile_string("CONST_PARAM", "track_speed",valstr, sizeof(valstr),path);
		if (ret==0)
			pCof->track_speed = (int)strtol(valstr, NULL, 0);
		
	ret = ini.read_profile_string("CONST_PARAM", "up_len",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->up_len = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "tracker_step_multiple",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->tracker_step_multiple = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "tracker_stop_overtime",valstr, sizeof(valstr),path);
		if (ret==0)
			pCof->tracker_stop_overtime = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "tracker_lr_dir",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->tracker_lr_dir = (int)strtol(valstr, NULL, 0);
	
	ret = ini.read_profile_string("CONST_PARAM", "tracker_ud_dir",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->tracker_ud_dir = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "light_ctrl",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->light_ctrl = (int)strtol(valstr, NULL, 0);
	
	ret = ini.read_profile_string("CONST_PARAM", "ir_led_ctrl",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->ir_led_ctrl = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "smartir_en",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->smartir_en = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "pwmfrequency",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->pwmfrequency = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "hardtype",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->hardtype = (int)strtol(valstr, NULL, 0);
	
	ret = ini.read_profile_string("CONST_PARAM", "speaker_reversed",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->speaker_reversed = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "auto_light_off",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->auto_light_off = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "auto_light_off_time",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->auto_light_off_time = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "private_motorstatus",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->private_motorstatus = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "md_threshold_low",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->md_threshold_low = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "md_threshold_mid",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->md_threshold_mid = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "md_threshold_high",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->md_threshold_high = (int)strtol(valstr, NULL, 0);

	ret = ini.read_profile_string("CONST_PARAM", "stitch_distance",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->stitch_distance = (float)strtof(valstr, NULL);

 ///////

	ret = ini.read_profile_string("DEFAULT_SETTING", "language",valstr, sizeof(valstr),path);
	if (ret==0)
		pCof->language = (int)strtol(valstr, NULL, 0);

	//读取外部配置
	if( 1 == type )
	{
		ret = ini.read_profile_string("DEFAULT_SETTING", "telnet",valstr, sizeof(valstr),path);
		if (ret==0)
			pCof->telnet = (int)strtol(valstr, NULL, 0);
		
		ret = ini.read_profile_string("DEFAULT_SETTING", "forceBurnLicense",valstr, sizeof(valstr),path);
		if (ret==0)
			pCof->forceBurnLicense = (int)strtol(valstr, NULL, 0);

		ret = ini.read_profile_string("DEFAULT_SETTING", "ispmode",valstr, sizeof(valstr),path);
		if (ret==0)
			pCof->ispmode = (int)strtol(valstr, NULL, 0);
		
		ret = ini.read_profile_string( "STATION", "stationssid",pSSID, sizeof(pSSID),path);
		if (ret==0) {
			strncpy(pCof->ssid,pSSID,sizeof(pCof->ssid));
		}

		ret = ini.read_profile_string( "STATION", "stationpwd",pPWD, sizeof(pPWD),path);
		if (ret==0) {
			strncpy(pCof->pwd,pPWD,sizeof(pCof->pwd));
		}

		ret = ini.read_profile_string( "STATION", "ipaddr",pIP, sizeof(pIP),path);
		if (ret==0) {
			strncpy(pCof->ip,pIP,sizeof(pCof->ip));
		}

		ret = ini.read_profile_string( "STATION", "netmask",pMASK, sizeof(pMASK),path);
		if (ret==0) {
			strncpy(pCof->mask,pMASK,sizeof(pCof->mask));
		}

		ret = ini.read_profile_string( "STATION", "gateway",pGW, sizeof(pGW),path);
		if (ret==0) {
			strncpy(pCof->gw,pGW,sizeof(pCof->gw));
		}

		if( strlen(pSSID) != 0 )
			result = 1;
	}
	
	pCof->enable = 1;

	return result;
}
int CProductCof::SetProductCof(PRODUCT_COF_S *pCof, const char *path)
{
	CInifile ini;
	char ircut_flip[64] = {'\0'};
    char image_flip[64] = {'\0'};
	char ptz_opposite_run[64] = {'\0'};
    char play_vol[64] = {'\0'};
    char capture_vol[64] = {'\0'};
	char lamp_board[64] = {'\0'};
	char lamp_board_value[64] = {'\0'};
	char ud_len[64] = {'\0'};
	char lr_len[64] = {'\0'};
	char ud_dir[64] = {'\0'};
	char lr_dir[64] = {'\0'};
	char Night2Day[64] = {'\0'};
	char Day2Night[64] = {'\0'};
	char ud_notuse[64] = {'\0'};
	char check_speed[64] = {'\0'};
	char tracker_step_multiple[64] = {'\0'};
	char tracker_stop_overtime[64] = {'\0'};
	char run_speed[64] = {'\0'};
	char run_ud_speed[64] = {'\0'};
	char track_speed[64] = {'\0'};
	char up_len[64] = {'\0'};
	char tracker_lr_dir[64] = {'\0'};
	char tracker_ud_dir[64] = {'\0'};
	char light_ctrl[64] = {'\0'};
	char ir_led_ctrl[64] = {'\0'};
	char pwmfrequency[64] = {'\0'};
	char speaker_reversed[64] = {'\0'};
	char private_motorstatus[64] = {'\0'};
	char smartir_en[64] = {'\0'};
	char auto_light_off[64] = {'\0'};
	char auto_light_off_time[64] = {'\0'};
	char md_threshold_low[64] = {'\0'};
	char md_threshold_mid[64] = {'\0'};
	char md_threshold_high[64] = {'\0'};
	char stitch_distance[64] = {'\0'};
	char language[64] = {'\0'};
	int ret = -1;

	FILE *file;
	if ( access(path, F_OK) != 0){
		
		file = fopen(path, "w+");//可读写，文件不存在则创建
		if (file == NULL) 
			return -1;
		else
			fclose(file);
	}
	
	sprintf(ircut_flip, "%d", pCof->ircut_flip);
    sprintf(image_flip, "%d", pCof->image_flip);
	sprintf(ptz_opposite_run, "%d", pCof->ptz_opposite_run);
    sprintf(play_vol, "%d", pCof->play_vol);
    sprintf(capture_vol, "%d", pCof->capture_vol);
    sprintf(lamp_board, "%d", pCof->lamp_board);
    sprintf(lamp_board_value, "%d", pCof->lamp_board_value);
    sprintf(ud_len, "%d", pCof->ud_len);
    sprintf(lr_len, "%d", pCof->lr_len);
    sprintf(ud_dir, "%d", pCof->ud_dir);
    sprintf(lr_dir, "%d", pCof->lr_dir);	
	sprintf(ud_notuse, "%d", pCof->ud_notuse);
    sprintf(check_speed, "%d", pCof->check_speed);	
	sprintf(Night2Day, "%d", pCof->Night2Day);
	sprintf(Day2Night, "%d", pCof->Day2Night);
    sprintf(run_speed, "%d", pCof->run_speed);	
	sprintf(run_ud_speed, "%d", pCof->run_ud_speed);
	sprintf(track_speed, "%d", pCof->track_speed);
	sprintf(tracker_step_multiple, "%d", pCof->tracker_step_multiple);
	sprintf(tracker_stop_overtime, "%d", pCof->tracker_stop_overtime);
    sprintf(up_len, "%d", pCof->up_len);	
    sprintf(tracker_lr_dir, "%d", pCof->tracker_lr_dir);
    sprintf(tracker_ud_dir, "%d", pCof->tracker_ud_dir);
    sprintf(light_ctrl, "%d", pCof->light_ctrl);
    sprintf(ir_led_ctrl, "%d", pCof->ir_led_ctrl);
	sprintf(pwmfrequency, "%d", pCof->pwmfrequency);
	sprintf(speaker_reversed, "%d", pCof->speaker_reversed);
	sprintf(private_motorstatus, "%d", pCof->private_motorstatus);
	sprintf(smartir_en, "%d", pCof->smartir_en);
	sprintf(auto_light_off, "%d", pCof->auto_light_off);
	sprintf(auto_light_off_time, "%d", pCof->auto_light_off_time);
	sprintf(md_threshold_low, "%d", pCof->md_threshold_low);
	sprintf(md_threshold_mid, "%d", pCof->md_threshold_mid);
	sprintf(md_threshold_high, "%d", pCof->md_threshold_high);
	sprintf(stitch_distance, "%f", pCof->stitch_distance);
	sprintf(language, "%d", pCof->language);
	
	ret = ini.write_profile_string("CONST_PARAM","ircut_flip", ircut_flip, path);
	if (ret!=0) {
		AppErr("write ini ircut_flip error\n");
		return -1;
	}
    ret = ini.write_profile_string("CONST_PARAM","image_flip", image_flip, path);
	if (ret!=0) {
		AppErr("write ini image_flip error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","ptz_opposite_run", ptz_opposite_run, path);
	if (ret!=0) {
		AppErr("write ini ptz_opposite_run error\n");
		return -1;
	}
	ret = ini.write_profile_string("DEFAULT_SETTING","language", language, path);
	if (ret!=0) {
		AppErr("write ini language error\n");
		return -1;
	}

    /////
	ret = ini.write_profile_string("CONST_PARAM","play_vol", play_vol, path);
	if (ret!=0) {
		AppErr("write ini play_vol error\n");
		return -1;
	}

	ret = ini.write_profile_string("CONST_PARAM","capture_vol", capture_vol, path);
	if (ret!=0) {
		AppErr("write ini capture_vol error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","lamp_board", lamp_board, path);
	if (ret!=0) {
		AppErr("write ini lamp_board error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","lamp_board_value", lamp_board_value, path);
	if (ret!=0) {
		AppErr("write ini lamp_board_value error\n");
		return -1;
	}

	ret = ini.write_profile_string("CONST_PARAM","ud_len", ud_len, path);
	if (ret!=0) {
		AppErr("write ini ud_len error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","lr_len", lr_len, path);
	if (ret!=0) {
		AppErr("write ini lr_len error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","ud_dir", ud_dir, path);
	if (ret!=0) {
		AppErr("write ini ud_dir error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","lr_dir", lr_dir, path);
	if (ret!=0) {
		AppErr("write ini lr_dir error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","ud_notuse", ud_notuse, path);
	if (ret!=0) {
		AppErr("write ini ud_notuse error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","check_speed", check_speed, path);
	if (ret!=0) {
		AppErr("write ini check_speed error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","run_speed", run_speed, path);
	if (ret!=0) {
		AppErr("write ini run_speed error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","Night2Day", Night2Day, path);
	if (ret!=0) {
		AppErr("write ini run_speed error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","Day2Night", Day2Night, path);
	if (ret!=0) {
		AppErr("write ini run_speed error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","run_ud_speed", run_ud_speed, path);
	if (ret!=0) {
		AppErr("write ini run_speed error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","track_speed", track_speed, path);
	if (ret!=0) {
		AppErr("write ini track_speed error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","tracker_step_multiple", tracker_step_multiple, path);
	if (ret!=0) {
		AppErr("write ini tracker_step_multiple error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","tracker_stop_overtime", tracker_stop_overtime, path);
	if (ret!=0) {
		AppErr("write ini tracker_stop_overtime error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","up_len", up_len, path);
	if (ret!=0) {
		AppErr("write ini up_len error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","tracker_lr_dir", tracker_lr_dir, path);
	if (ret!=0) {
		AppErr("write ini tracker_lr_dir error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","tracker_ud_dir", tracker_ud_dir, path);
	if (ret!=0) {
		AppErr("write ini tracker_ud_dir error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","light_ctrl", light_ctrl, path);
	if (ret!=0) {
		AppErr("write ini light_ctrl error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","ir_led_ctrl", ir_led_ctrl, path);
	if (ret!=0) {
		AppErr("write ini ir_led_ctrl error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","smartir_en", smartir_en, path);
	if (ret!=0) {
		AppErr("write ini smartir_en error\n");
		return -1;
	}
	ret = ini.write_profile_string("CONST_PARAM","pwmfrequency", pwmfrequency, path);
	if (ret!=0) {
		AppErr("write ini pwmfrequency error\n");
		return -1;
	}
	
	ret = ini.write_profile_string("CONST_PARAM","speaker_reversed", speaker_reversed, path);
	if (ret!=0) {
		AppErr("write ini hardtype error\n");
		return -1;
	}

	ret = ini.write_profile_string("CONST_PARAM","private_motorstatus", private_motorstatus, path);
	if (ret!=0) {
		AppErr("write ini hardtype error\n");
		return -1;
	}
	
	ret = ini.write_profile_string("CONST_PARAM","auto_light_off", auto_light_off, path);
	if (ret!=0) {
		AppErr("write ini auto_light_off error\n");
		return -1;
	}

	ret = ini.write_profile_string("CONST_PARAM","auto_light_off_time", auto_light_off_time, path);
	if (ret!=0) {
		AppErr("write ini auto_light_off_time error\n");
		return -1;
	}

	ret = ini.write_profile_string("CONST_PARAM","md_threshold_low", md_threshold_low, path);
	if (ret!=0) {
		AppErr("write ini auto_light_off_time error\n");
		return -1;
	}

	ret = ini.write_profile_string("CONST_PARAM","md_threshold_mid", md_threshold_mid, path);
	if (ret!=0) {
		AppErr("write ini auto_light_off_time error\n");
		return -1;
	}

	ret = ini.write_profile_string("CONST_PARAM","md_threshold_high", md_threshold_high, path);
	if (ret!=0) {
		AppErr("write ini auto_light_off_time error\n");
		return -1;
	}

	ret = ini.write_profile_string("CONST_PARAM","stitch_distance", stitch_distance, path);
	if (ret!=0) {
		AppErr("write ini stitch_distance error\n");
		return -1;
	}
	
    /////
	return 0;
}


