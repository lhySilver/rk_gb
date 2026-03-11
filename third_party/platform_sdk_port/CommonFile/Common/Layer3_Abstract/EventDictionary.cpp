
#include "EventDictionary.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
CEventDictionary::CEventDictionary()
{
	/*报警字典begin*/
	std::list< std::pair<tuint32, string> > alarmtype_idcs;
	//监控点相关报警
	{
		alarmtype_idcs.clear();
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_MOTION,"IDCS_ALARM_MOTION"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_VLOSS,"IDCS_ALARM_VLOSS"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_OSC,"IDCS_TYPE_ALARM_OSC"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_AVD,"IDCS_TYPE_ALARM_AVD"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_PEA_TRIPWIRE,"IDCS_TYPE_ALARM_PEA_TRIPWIRE"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_PEA_PERIMETER,"IDCS_TYPE_ALARM_PEA_PERIMETER"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ENTER_AREA_OVER_LINE,"IDCS_TYPE_ALARM_ENTER_AREA_OVER_LINE"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_LEAVE_AREA_OVER_LINE,"IDCS_TYPE_ALARM_LEAVE_AREA_OVER_LINE"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_STAY_AREA_OVER_LINE,"IDCS_TYPE_ALARM_STAY_AREA_OVER_LINE"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_CPC,"IDCS_TYPE_ALARM_CPC"));//笼统的（设备暂时上报的没有做区分，所以只能采用笼统这个,下面3种先屏蔽掉）
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_VFD,"IDCS_TYPE_ALARM_VFD"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_CDD,"IDCS_TYPE_ALARM_CDD"));//人群密度检测报警
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_IPD,"IDCS_TYPE_ALARM_IPD"));//人员入侵侦测报警
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_TARGET_IN_BLACKLIST,"IDCS_TYPE_ALARM_TARGET_IN_BLACKLIST"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_TARGET_UNDEFINED,"IDCS_TYPE_ALARM_UNDEFINED"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_SENCE_CHANGE, "IDCS_TYPE_ALARM_VQD_SCENE"));//场景变更 （视频遮挡）
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_CHANNEL_OFFLINE,"IDCS_TYPE_ALARM_CHANNEL_OFFLINE"));
		if (IsV2_0()==false)
		{
			alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_FACE_MATCH,"IDCS_TYPE_ALARM_FACE_MATCH"));
			alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_BLACKLIST_CAR_PASS,"IDCS_TYPE_ALARM_PMS_BLACKLIST_CAR"));//黑名单车辆通行报警
			if (IsHigher2_1_2() == true)
			{
				alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_CAR_PASS_NOBLACKLIST,"IDCS_TYPE_ALARM_PMS_WHITELIST_CAR"));//白名单车辆通行报警
				alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_CAR_PASS_TEMPCAR,"IDCS_TYPE_ALARM_PMS_TEMP_CAR"));//临时车辆通行报警

				if (ENABLE_INDUSTRY_THERMALEX)
				{
					alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_INDUSTRY_THERMALEX_HIGH_WARNING,"IDCS_TYPE_ALARM_INDUSTRY_THERMALEX_HIGH_WARNING"));//工业相机高温预警
					alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_INDUSTRY_THERMALEX_HIGH_ALARM,"IDCS_TYPE_ALARM_INDUSTRY_THERMALEX_HIGH_ALARM"));//工业相机高温报警
					//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_INDUSTRY_THERMALEX_LOW_WARNING,"IDCS_TYPE_ALARM_INDUSTRY_THERMALEX_LOW_WARNING"));//工业相机低温预警
					//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_INDUSTRY_THERMALEX_LOW_ALARM,"IDCS_TYPE_ALARM_INDUSTRY_THERMALEX_LOW_ALARM"));//工业相机低温报警
				}
			}
			
			alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_AOIENTRY,"IDCS_TYPE_ALARM_AOIENTRY"));//进入区域报警
			alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_AOILEAVE,"IDCS_TYPE_ALARM_AOILEAVE"));//离开区域报警
		}

		if ( (IsV2_0() == false) && (IsV2_1_1() == false) )
		{
			alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_SUSPECTED_FEVER,"IDCS_TYPE_ALARM_SUSPECTED_FEVER"));//疑似发热
			alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_UNMASK,"IDCS_TYPE_ALARM_NO_MASK"));//未戴口罩
			alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_STRANGER_MATCHING,"IDCS_ALARM_MOTION_STRANGER"));//陌生人报警
			alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_BLACKLIST_MATCHING,"IDCS_ALARM_MOTION_BLACK"));//黑名单报警
		}

		//以下是智能告警--智诺
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_VQD_DARK,"IDCS_TYPE_ALARM_VQD_DARK"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_VQD_LIGHT,"IDCS_TYPE_ALARM_VQD_LIGHT"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_VQD_BLUR,"IDCS_TYPE_ALARM_VQD_BLUR"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_VQD_NOISE,"IDCS_TYPE_ALARM_VQD_NOISE"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_VQD_STRIATION,"IDCS_TYPE_ALARM_VQD_STRIATION"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_VQD_JITTER,"IDCS_TYPE_ALARM_VQD_JITTER"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_VQD_FROZEN,"IDCS_TYPE_ALARM_VQD_FROZEN"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_VQD_COVER,"IDCS_TYPE_ALARM_VQD_COVER"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_VQD_LOSS,"IDCS_TYPE_ALARM_VQD_LOSS"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_VQD_SCENE,"IDCS_TYPE_ALARM_VQD_SCENE"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_VQD_CAMMOVE,"IDCS_TYPE_ALARM_VQD_CAMMOVE"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_VQD_PTZERR,"IDCS_TYPE_ALARM_VQD_PTZERR"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_VQD_UNBALANCE,"IDCS_TYPE_ALARM_VQD_UNBALANCE"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_SSB_TRAIL,"IDCS_TYPE_ALARM_SSB_TRAIL"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_SSB_HOVER,"IDCS_TYPE_ALARM_SSB_HOVER"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_SSB_FELL,"IDCS_TYPE_ALARM_SSB_FELL"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_SSB_GATHER,"IDCS_TYPE_ALARM_SSB_GATHER"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_SSB_REMNANTS,"IDCS_TYPE_ALARM_SSB_REMNANTS"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_SSB_FIGHT,"IDCS_TYPE_ALARM_SSB_FIGHT"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_SSB_INVADE,"IDCS_TYPE_ALARM_SSB_INVADE"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_SSB_SUSPICIOUS_TRAIL,"IDCS_TYPE_ALARM_SSB_SUSPICIOUS_TRAIL"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_SSB_HIJACK,"IDCS_TYPE_ALARM_SSB_HIJACK"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_SSB_HUMAN_NUMBER,"IDCS_TYPE_ALARM_SSB_HUMAN_NUMBER"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ATM_PANELPASTE,"IDCS_TYPE_ALARM_ATM_PANELPASTE"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ATM_KEYBOARD,"IDCS_TYPE_ALARM_ATM_KEYBOARD"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ATM_CARDPORT,"IDCS_TYPE_ALARM_ATM_CARDPORT"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ATM_KEYMASK,"IDCS_TYPE_ALARM_ATM_KEYMASK"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ATM_ENTER,"IDCS_TYPE_ALARM_ATM_ENTER"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ATM_LEAVE,"IDCS_TYPE_ALARM_ATM_LEAVE"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ATM_STAY,"IDCS_TYPE_ALARM_ATM_STAY"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ATM_FACECAPTURE,"IDCS_TYPE_ALARM_ATM_FACECAPTURE"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ATM_FACEABNORMAL,"IDCS_TYPE_ALARM_ATM_FACEABNORMAL"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ATM_SMASH,"IDCS_TYPE_ALARM_ATM_SMASH"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ATM_TANKDOWN,"IDCS_TYPE_ALARM_ATM_TANKDOWN"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ATM_BAGGAGE,"IDCS_TYPE_ALARM_ATM_BAGGAGE"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ATM_GATHER,"IDCS_TYPE_ALARM_ATM_GATHER"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ATM_MULTIFACE,"IDCS_TYPE_ALARM_ATM_MULTIFACE"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ATM_FIGHT,"IDCS_TYPE_ALARM_ATM_FIGHT"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ATM_HIJACK,"IDCS_TYPE_ALARM_ATM_HIJACK"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ATM_INVADE,"IDCS_TYPE_ALARM_ATM_INVADE"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ATM_FIRE,"IDCS_TYPE_ALARM_ATM_FIRE"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ATM_FAKEKEYBOARD,"IDCS_TYPE_ALARM_ATM_FAKEKEYBOARD"));
		//alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ATM_FAKECARDREADER,"IDCS_TYPE_ALARM_ATM_FAKECARDREADER"));
        //alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ATM_PHONEFRAUD,"IDCS_TYPE_ALARM_ATM_PHONEFRAUD"));

		m_mapNodeType_AlarmType[CTypeCheck::GetChnnTypeValue()] = alarmtype_idcs;
	}
	//传感器报警
	{
		alarmtype_idcs.clear();
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_SENSOR,"IDCS_ALARM_SENSOR"));
		m_mapNodeType_AlarmType[CTypeCheck::GetAlarmInAbstractTypeValue()] = alarmtype_idcs;
		
	}
	//子系统报警
	{
		alarmtype_idcs.clear();
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ARM,"IDCS_TYPE_ALARM_ARM"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_DISARM,"IDCS_TYPE_ALARM_DISARM"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_CLEAR_ALARM,"IDCS_TYPE_ALARM_CLEARALARM"));
		m_mapNodeType_AlarmType[CTypeCheck::GetSubSystemTypeValue()] = alarmtype_idcs;
	}
	//防区报警
	{
		alarmtype_idcs.clear();
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_BYPASS,"IDCS_TYPE_ALARM_BYPASS"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_BYPASSRES,"IDCS_TYPE_ALARM_BYPASSRES"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ZONE_ALARM,"IDCS_TYPE_ALARM_ZONEALARM"));
		//界面不需要显示报警恢复alarmtype_idcs[NVMS_DEFINE::ECMS_ALARM_TYPE_ZONE_ALARMRES,"IDCS_TYPE_ALARM_ZONEALARMRES";
		m_mapNodeType_AlarmType[CTypeCheck::GetZoneTypeValue()] = alarmtype_idcs;
	}
	//门禁事件报警
	{
		alarmtype_idcs.clear();
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ACSDOOR_ALARM,"IDCS_TYPE_ALARM_ACSDOORALARM"));
		//新增泰科KANTECH门禁报警
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_SECOND_ENTRANCE,"IDCS_TYPE_ALARM_KANTECHDOORALARM_SECOND_ENTRANCE"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_UNLOCK,"IDCS_TYPE_ALARM_KANTECHDOORALARM_UNLOCK"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_GRANTED,"IDCS_TYPE_ALARM_KANTECHDOORALARM_GRANTED"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_CARD_UNKNOWN,"IDCS_TYPE_ALARM_KANTECHDOORALARM_CARD_UNKNOWN"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_BADCARD,"IDCS_TYPE_ALARM_KANTECHDOORALARM_BADCARD"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_CARD_LOSTSTOLEN,"IDCS_TYPE_ALARM_KANTECHDOORALARM_CARD_LOSTSTOLEN"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_CARD_EXPIRED,"IDCS_TYPE_ALARM_KANTECHDOORALARM_CARD_EXPIRED"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_CARD_PENDING,"IDCS_TYPE_ALARM_KANTECHDOORALARM_CARD_PENDING"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_CARD_COUNT,"IDCS_TYPE_ALARM_KANTECHDOORALARM_CARD_COUNT"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_SCHEDULE_EXCEPTION,"IDCS_TYPE_ALARM_KANTECHDOORALARM_SCHEDULE_EXCEPTION"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_BAD_ACCESS,"IDCS_TYPE_ALARM_KANTECHDOORALARM_BAD_ACCESS"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_SUPLEVEL_REQUIRED,"IDCS_TYPE_ALARM_KANTECHDOORALARM_SUPLEVEL_REQUIRED"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_PASSBACKBAD_LOCATION,"IDCS_TYPE_ALARM_KANTECHDOORALARM_PASSBACKBAD_LOCATION"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_DUAL_CUSTODY,"IDCS_TYPE_ALARM_KANTECHDOORALARM_DUAL_CUSTODY"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_NUMBER_AREA,"IDCS_TYPE_ALARM_KANTECHDOORALARM_NUMBER_AREA"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_ALARMSYSTEM_ARMED,"IDCS_TYPE_ALARM_KANTECHDOORALARM_ALARMSYSTEM_ARMED"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_AREA_NOTCLEAR,"IDCS_TYPE_ALARM_KANTECHDOORALARM_AREA_NOTCLEAR"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_INTERLOCK_ACTIVE,"IDCS_TYPE_ALARM_KANTECHDOORALARM_INTERLOCK_ACTIVE"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_DOORMANUALLY_DISABLED,"IDCS_TYPE_ALARM_KANTECHDOORALARM_DOORMANUALLY_DISABLED"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_NOTENOUGH_CARDS,"IDCS_TYPE_ALARM_KANTECHDOORALARM_NOTENOUGH_CARDS"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_CARD_BUSY,"IDCS_TYPE_ALARM_KANTECHDOORALARM_CARD_BUSY"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_MINPASSBACK_DELAY,"IDCS_TYPE_ALARM_KANTECHDOORALARM_MINPASSBACK_DELAY"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_PASSBACK_BADLOCATION,"IDCS_TYPE_ALARM_KANTECHDOORALARM_PASSBACK_BADLOCATION"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_AREA,"IDCS_TYPE_ALARM_KANTECHDOORALARM_AREA"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_WAIT_APPROVAL,"IDCS_TYPE_ALARM_KANTECHDOORALARM_WAIT_APPROVAL"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_READER_LOCKED,"IDCS_TYPE_ALARM_KANTECHDOORALARM_READER_LOCKED"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_SECOND_CARD,"IDCS_TYPE_ALARM_KANTECHDOORALARM_SECOND_CARD"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_DOOR_ARMED,"IDCS_TYPE_ALARM_KANTECHDOORALARM_DOOR_ARMED"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_ACCESS_DENIED,"IDCS_TYPE_ALARM_KANTECHDOORALARM_ACCESS_DENIED"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_DOOR_FORCED_OPEN,"IDCS_TYPE_ALARM_KANTECHDOORALARM_DOOR_FORCED_OPEN"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_DOOR_FORCED_OPEN_RESTORED,"IDCS_TYPE_ALARM_KANTECHDOORALARM_DOOR_FORCED_OPEN_RESTORED"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_PREALARM_DOOR_OPEN_TOOLONG,"IDCS_TYPE_ALARM_KANTECHDOORALARM_PREALARM_DOOR_OPEN_TOOLONG"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_DOOR_OPEN_TOOLONG,"IDCS_TYPE_ALARM_KANTECHDOORALARM_DOOR_OPEN_TOOLONG"));
		m_mapNodeType_AlarmType[CTypeCheck::GetAcsDoorTypeValue()] = alarmtype_idcs;
	}
	//考勤事件报警
	{
		alarmtype_idcs.clear();
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ACSDEVICE_ATTEND_ALARM,"IDCS_TYPE_ALARM_ACSDEVICEATTENDALARM"));
		m_mapNodeType_AlarmType[CTypeCheck::GetAcsDeviceTypeValue()] = alarmtype_idcs;
	}
	//服务器报警
 	{
 		alarmtype_idcs.clear();
 		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_SERVER_OFFLINE,"IDCS_TYPE_ALARM_SERVER_OFFLINE"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_RAID_NOUSE,"IDCS_TYPE_ALARM_SERVER_RAID_NOUSE"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_RAID_EXCEPTION,"IDCS_TYPE_ALARM_SERVER_RAID_EXCEPTION"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_RAID_REBUILD,"IDCS_TYPE_ALARM_SERVER_RAID_REBUILD"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_RAID_DEGRADE,"IDCS_TYPE_ALARM_SERVER_RAID_DEGRADE"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_NO_DISK,"IDCS_TYPE_ALARM_SERVER_NO_DISK"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_NO_PARTION,"IDCS_TYPE_ALARM_SERVER_NO_PARTION"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_OPENFILE_ERR,"IDCS_TYPE_ALARM_SERVER_OPENFILE_ERR"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ROUTING_CHECK_OFF_LINE,"IDCS_TYPE_ALARM_SERVER_ROUTING_CHECK_OFF_LINE"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_RAID_DISKBAD,"IDCS_TYPE_ALARM_SERVER_RAIDDISK_BAD"));
 		m_mapNodeType_AlarmType[CTypeCheck::GetServerTypeBeginValue()] = alarmtype_idcs;
 	}
	//编码设备报警
	{
		alarmtype_idcs.clear();
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::NVMS_ALARM_TYPE_IP_CONFLICT,"IDCS_TYPE_ALARM_IP_CONFLICT"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::NVMS_ALARM_TYPE_DISK_IO_ERROR,"IDCS_TYPE_ALARM_DISK_IO_ERROR"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::NVMS_ALARM_TYPE_DISK_FULL,"IDCS_TYPE_ALARM_DISK_FULL"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::NVMS_ALARM_TYPE_RAID_SUBHEALTH,"IDCS_TYPE_ALARM_RAID_SUBHEALTH"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::NVMS_ALARM_TYPE_RAID_UNAVAILABLE,"IDCS_TYPE_ALARM_RAID_UNAVAILABLE"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::NVMS_ALARM_TYPE_ILLEIGAL_ACCESS,"IDCS_TYPE_ALARM_ILLEIGAL_ACCESS"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::NVMS_ALARM_TYPE_NET_DISCONNECT,"IDCS_TYPE_ALARM_NET_DISCONNECT"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::NVMS_ALARM_TYPE_NO_DISK,"IDCS_TYPE_ALARM_NO_DISK"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::NVMS_ALARM_TYPE_SIGNAL_SHELTER,"IDCS_TYPE_ALARM_SIGNAL_SHELTER"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::NVMS_ALARM_TYPE_HDD_PULL_OUT,"IDCS_TYPE_ALARM_HDD_PULL_OUT"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_DEVICE_OFFLINE,"IDCS_TYPE_ALARM_PU_OFFLINE"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::NVMS_ALARM_TYPE_RAID_HOT_ERROR,"IDCS_TYPE_ALARM_RAID_HOT_ERROR"));
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::NVMS_ALARM_TYPE_RAID_EXCEPTION,"IDCS_TYPE_ALARM_RAID_EXCEPTION"));
		m_mapNodeType_AlarmType[CTypeCheck::GetPUAbstractTypeValue()] = alarmtype_idcs;
	}
	//报警主机报警
	{
		alarmtype_idcs.clear();
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ALARMHOST_OFFLINE,"IDCS_TYPE_ALARM_ALARMHOST_OFFLINE"));//报警主机下线报警
		m_mapNodeType_AlarmType[CTypeCheck::GetAlarmHostAbstractTypeValue()] = alarmtype_idcs;
	}
	//门禁系统报警
	{
		alarmtype_idcs.clear();
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ACSSYSTEM_OFFLINE,"IDCS_TYPE_ALARM_ACSSYSTEM_OFFLINE"));//门禁系统下线报警
		m_mapNodeType_AlarmType[CTypeCheck::GetAcsSystemAbstractTypeValue()] = alarmtype_idcs;
	}
	//解码设备报警
	{
		alarmtype_idcs.clear();
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_DECODER_OFFLINE,"IDCS_TYPE_ALARM_DECODER_OFFLINE"));//解码设备下线报警
		m_mapNodeType_AlarmType[CTypeCheck::GetDECODERAbstractTypeValue()] = alarmtype_idcs;
	}

    //报警任务
	if ( (IsV2_0() == false) && (IsV2_1_1() == false) )
    {
        alarmtype_idcs.clear();
        alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ALARM_TASK_LEAVE_POST,"IDCS_ALARM_TASK_LEAVE_POST"));//离岗报警
		alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_ALARM_TASK_ENTER_HIGH_COUNT,"IDCS_ALARM_TYPE_ALARM_TASK_ENTER_HIGH_COUNT"));//阈值报警    threshold value
        m_mapNodeType_AlarmType[CTypeCheck::GetAlarmTaskTypeValue()] = alarmtype_idcs;
    }
    //停车场报警
	if (IsHigher2_1_2() == true)
    {
        alarmtype_idcs.clear();
        alarmtype_idcs.push_back(std::make_pair(NVMS_DEFINE::ECMS_ALARM_TYPE_TEMP_CAR_PARKING_OVERTIME,"IDCS_TYPE_ALARM_TEMP_CAR_PARKING_OVERTIME"));//临时车辆超时停放报警
        m_mapNodeType_AlarmType[CTypeCheck::GetPmsParkTypeValue()] = alarmtype_idcs;
    }
	/*报警字典end*/

	/*任务字典begin*/
	std::map<tuint32 , string > tasktype_idcs;
	{
		tasktype_idcs.clear();
		tasktype_idcs[NVMS_DEFINE::ECMS_TASK_TYPE_VFD],"IDCS_TASK_VFD";//人脸检测任务
		tasktype_idcs[NVMS_DEFINE::ECMS_TASK_TYPE_CPC],"IDCS_TASK_CPC";//人数统计任务
		m_mapNodeType_TaskType[CTypeCheck::GetChnnTypeValue()] = tasktype_idcs;
	}
	/*任务字典end*/


	/*日志类型begin*/
	{
		//报警日志
		std::map< tuint32,std::list< std::pair<tuint32, string> > >::iterator iterNodeType = m_mapNodeType_AlarmType.begin();
		for (;iterNodeType != m_mapNodeType_AlarmType.end();iterNodeType++)
		{
			std::list< std::pair<tuint32, string> >::iterator iterEvent = iterNodeType->second.begin();
			for (;iterEvent!=iterNodeType->second.end();iterEvent++)
			{
				m_mapAllLogTypeAndIDCS.push_back(std::make_pair((*iterEvent).first,(*iterEvent).second));
			}
		}

		//操作日志
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_OPERATE_TYPE_RECORD_SPB,"IDCS_OPERATE_RECORD_SPB"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_OPERATE_TYPE_MANUAL_RECORD,"IDCS_OPERATE_MANUAL_RECORD"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_OPERATE_TYPE_MANUAL_ALARM,"IDCS_OPERATE_MANUAL_ALARM"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_OPERATE_TYPE_SYSTEM_MAINTENANCE,"IDCS_OPERATE_SYSTEM_MAINTENANCE"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_OPERATE_TYPE_PTZ_CONTROL,"IDCS_OPERATE_PTZ_CONTROL"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_OPERATE_TYPE_AUDIO_TALK,"IDCS_OPERATE_AUDIO_TALK"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_OPERATE_TYPE_SYSTEM_SCR,"IDCS_OPERATE_SYSTEM_SCR"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_OPERATE_TYPE_LOGIN_LOGOUT,"IDCS_OPERATE_LOGIN_LOGOUT"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_OPERATE_TYPE_SNAPSHOT_MSPB,"IDCS_OPERATE_SNAPSHOT_MSPB"));
		if (IsV2_0() == false)
		{
			m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_OPERATE_TYPE_CAPTURE_MACHINE,"IDCS_OPERATE_CAPTURE_MACHINE"));
			m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_OPERATE_TYPE_LED,"IDCS_OPERATE_LED"));
			m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_OPERATE_TYPE_BARRIER_GATE,"IDCS_OPERATE_BARRIER_GATE"));
		}
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_OPERATE_TYPE_OPENDOOR_BYHAND,"IDCS_OPENDOOR_BYHAND"));
		//m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_OPERATE_TYPE_ACCESS_RECORD,"IDCS_OPERATE_ACCESS_RECORD"));
		if ( IsHigher2_1_2() )
		{
			m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_OPERATE_TYPE_PREVIEW, "IDCS_OPERATE_LIVE_PREVIEW"));
			m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_OPERATE_TYPE_PLAYBACK, "IDCS_OPERATE_PLAYBACK"));
		}

		//设置日志
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_CONFIG_TYPE_CHANNEL,"IDCS_CONFIG_CHANNEL"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_CONFIG_TYPE_RECORD,"IDCS_CONFIG_RECORD"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_CONFIG_TYPE_ALARM,"IDCS_CONFIG_ALARM"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_CONFIG_TYPE_DISK,"IDCS_CONFIG_DISK"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_CONFIG_TYPE_NETWORK,"IDCS_CONFIG_NETWORK"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_CONFIG_TYPE_SCHEDULE,"IDCS_CONFIG_SCHEDULE"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_CONFIG_TYPE_USER,"IDCS_CONFIG_USER"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_CONFIG_TYPE_BASIC,"IDCS_CONFIG_BASIC"));

		//信息日志
// 		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_INFO_TYPE_SCHEDULE_RECORD,""));
// 		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_INFO_TYPE_SCHEDULE_SNAP,""));
// 		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_INFO_TYPE_DISK,""));
// 		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_INFO_TYPE_NETWORK,""));
// 		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_INFO_TYPE_SYSTEM_BASE,""));
// 		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_INFO_TYPE_SYSTEM_RUN,""));
// 		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_INFO_TYPE_CHANNEL_STATE,""));
// 		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_INFO_TYPE_ALARM_STATE,""));
// 		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_INFO_TYPE_RECORD_STATE,""));

		//异常日志
		//m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_EXCEPTION_TYPE_DISK_FULL,"IDCS_EXCEPTION_DISK_FULL"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_EXCEPTION_TYPE_RAID_UNAVAILABLE,"IDCS_TYPE_ALARM_SERVER_RAID_NOUSE"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_EXCEPTION_TYPE_NOT_DISK,"IDCS_TYPE_ALARM_SERVER_NO_DISK"));
        m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_EXCEPTION_TYPE_NOT_PARTION,"IDCS_TYPE_ALARM_SERVER_NO_PARTION"));
        m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_EXCEPTION_TYPE_RAID_EXCEPTION,"IDCS_TYPE_ALARM_SERVER_RAID_EXCEPTION"));
        m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_EXCEPTION_TYPE_RAID_REBUILD,"IDCS_TYPE_ALARM_SERVER_RAID_REBUILD"));
        m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_EXCEPTION_TYPE_RAID_DEGRADE,"IDCS_TYPE_ALARM_SERVER_RAID_DEGRADE"));
        m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_EXCEPTION_TYPE_RAID_NORMAL,"IDCS_TYPE_ALARM_SERVER_RAID_NORMAL"));
        m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_EXCEPTION_TYPE_RAID_CREATE,"IDCS_TYPE_ALARM_SERVER_RAID_CREAT"));
        m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_EXCEPTION_TYPE_RAID_DEL,"IDCS_TYPE_ALARM_SERVER_RAID_DEL"));
        m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_EXCEPTION_TYPE_RAIDDISK_ADD,"IDCS_TYPE_ALARM_SERVER_RAIDDISK_ADD"));
        m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_EXCEPTION_TYPE_RAIDDISK_REMOVED,"IDCS_TYPE_ALARM_SERVER_RAIDDISK_REMOVED"));
        m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_EXCEPTION_TYPE_RAIDDISK_SUBHEALTH,"IDCS_TYPE_ALARM_SERVER_RAIDDISK_SUBHEALTH"));
        m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_EXCEPTION_TYPE_RAIDDISK_BAD,"IDCS_TYPE_ALARM_SERVER_RAIDDISK_BAD"));   //wangkai1117

		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_INFO_STATUS_SERVER_ONLINE,"IDCS_LOG_STATUS_SERVER_ONLINE"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_INFO_STATUS_SERVER_OFFLINE,"IDCS_LOG_STATUS_SERVER_OFFLINE"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_INFO_STATUS_PU_ONLINE,"IDCS_LOG_STATUS_PU_ONLINE"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_INFO_STATUS_PU_OFFLINE,"IDCS_LOG_STATUS_PU_OFFLINE"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_INFO_STATUS_DECODER_ONLINE,"IDCS_LOG_STATUS_DECODER_ONLINE"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_INFO_STATUS_DECODER_OFFLINE,"IDCS_LOG_STATUS_DECODER_OFFLINE"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_INFO_STATUS_ALARMHOST_ONLINE,"IDCS_LOG_STATUS_ALARMHOST_OFFLINE"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_INFO_STATUS_ALARMHOST_OFFLINE,"IDCS_LOG_STATUS_ALARMHOST_ONLINE"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_INFO_STATUS_RECORD_ON,"IDCS_LOG_STATUS_RECORD_ON"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_INFO_STATUS_RECORD_OFF,"IDCS_LOG_STATUS_RECORD_OFF"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_INFO_STATUS_CHANNEL_ONLINE,"IDCS_LOG_STATUS_CHANNEL_ONLINE"));
		m_mapAllLogTypeAndIDCS.push_back(std::make_pair(NVMS_DEFINE::ECMS_INFO_STATUS_CHANNEL_OFFLINE,"IDCS_LOG_STATUS_CHANNEL_OFFLINE"));
	}

	/*日志类型end*/

}

void CEventDictionary::GetAlarmEventDictionary(std::map< tuint32,std::list< std::pair<tuint32, string> > >& mapNodeType_AlarmType)
{
	mapNodeType_AlarmType = m_mapNodeType_AlarmType;
}

void CEventDictionary::GetTaskEventDictionary(std::map<  tuint32,std::map<tuint32 , string >  >& mapNodeType_TaskType)
{
	mapNodeType_TaskType = m_mapNodeType_TaskType;
}

void CEventDictionary::GetAllLogTypeAndIDCS(std::list< std::pair<tuint32, string> >& mapAllLogTypeAndIDCS)
{
	mapAllLogTypeAndIDCS = m_mapAllLogTypeAndIDCS;
}

//根据日志类型获取日志翻译
string CEventDictionary::GetIDCSByLogType(tuint32 nLogType)
{
	std::list< std::pair<tuint32, string> > ::iterator iterLogType = m_mapAllLogTypeAndIDCS.begin();
	for (;iterLogType!=m_mapAllLogTypeAndIDCS.end();iterLogType++)
	{
		if ((*iterLogType).first == nLogType)
		{
			return (*iterLogType).second;
		}
	}
	return "";
}

//是否为报警日志
bool CEventDictionary::IsAlarmLogType(tuint32 nLogType)
{
	if ( (nLogType >= NVMS_DEFINE::ECMS_ALARM_TYPE_RANGE_BEGIN)
		&&(nLogType <= NVMS_DEFINE::ECMS_ALARM_TYPE_RANGE_END))
	{
		return true;
	}
	return false;
}
//是否为操作日志
bool CEventDictionary::IsOperateLogType(tuint32 nLogType)
{
	if ( (nLogType >= NVMS_DEFINE::ECMS_OPERATE_TYPE_RANGE_BEGIN)
		&&(nLogType <= NVMS_DEFINE::ECMS_OPERATE_TYPE_RANGE_END))
	{
		return true;
	}
	return false;
}
//是否为配置日志
bool CEventDictionary::IsConfigLogType(tuint32 nLogType)
{
	if ( (nLogType >= NVMS_DEFINE::ECMS_CONFIG_TYPE_RANGE_BEGIN)
		&&(nLogType <= NVMS_DEFINE::ECMS_CONFIG_TYPE_RANGE_END))
	{
		return true;
	}
	return false;
}
//是否为信息日志
bool CEventDictionary::IsInfoLogType(tuint32 nLogType)
{
	if ( (nLogType >= NVMS_DEFINE::ECMS_INFO_TYPE_RANGE_BEGIN)
		&&(nLogType <= NVMS_DEFINE::ECMS_INFO_TYPE_RANGE_END))
	{
		return true;
	}
	return false;
}

//是否为异常日志
bool CEventDictionary::IsExceptionLogType(tuint32 nLogType)
{
	if ( (nLogType >= NVMS_DEFINE::ECMS_EXCEPTION_TYPE_RANGE_BEGIN)
		&&(nLogType <= NVMS_DEFINE::ECMS_EXCEPTION_TYPE_RANGE_END))
	{
		return true;
	}
	return false;
}
//是否为状态日志
bool CEventDictionary::IsStatuLogType(tuint32 nLogType)
{
	if ( (nLogType >= NVMS_DEFINE::ECMS_INFO_STATUS_RANGE_BEGIN)
		&&(nLogType <= NVMS_DEFINE::ECMS_INFO_STATUS_RANGE_END))
	{
		return true;
	}
	return false;
}

//是否为报警开始和停止为非成对出现的(模拟的报警结束) 报警服务器联动用到
bool CEventDictionary::IsNoEndAlarmType(tuint32 nAlarmType)
{
	if ( (nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_ARM)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_DISARM)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_CLEAR_ALARM)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_BYPASS)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_BYPASSRES)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_ACSDOOR_ALARM)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_SECOND_ENTRANCE)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_UNLOCK)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_GRANTED)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_CARD_UNKNOWN)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_BADCARD)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_CARD_LOSTSTOLEN)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_CARD_EXPIRED)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_CARD_PENDING)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_CARD_COUNT)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_SCHEDULE_EXCEPTION)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_BAD_ACCESS)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_SUPLEVEL_REQUIRED)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_PASSBACKBAD_LOCATION)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_DUAL_CUSTODY)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_NUMBER_AREA)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_ALARMSYSTEM_ARMED)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_AREA_NOTCLEAR)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_INTERLOCK_ACTIVE)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_DOORMANUALLY_DISABLED)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_NOTENOUGH_CARDS)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_CARD_BUSY)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_MINPASSBACK_DELAY)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_PASSBACK_BADLOCATION)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_AREA)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_WAIT_APPROVAL)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_READER_LOCKED)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_SECOND_CARD)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_DOOR_ARMED)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_ACCESS_DENIED)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_DOOR_FORCED_OPEN)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_DOOR_FORCED_OPEN_RESTORED)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_PREALARM_DOOR_OPEN_TOOLONG)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_DOOR_OPEN_TOOLONG)||\
        (nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_ACSDEVICE_ATTEND_ALARM) ||\
        (nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_TARGET_IN_BLACKLIST) ||\
        (nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_TARGET_UNDEFINED) ||\
        (nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_CHANNEL_OFFLINE) ||\
        (nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_DEVICE_OFFLINE) ||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_ROUTING_CHECK_OFF_LINE) ||\
        (nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_SERVER_OFFLINE) ||\
/*		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_BLACKLIST_CAR_PASS) ||\*/
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_SUSPECTED_FEVER)
		)
	{
		return true;
	}
	return false;
}

//是否为传感器报警
bool CEventDictionary::IsSensorAlarmType(tuint32 nAlarmType)
{
	std::map< tuint32,std::list< std::pair<tuint32, string> > >::iterator it = m_mapNodeType_AlarmType.find(CTypeCheck::GetAlarmInAbstractTypeValue());
	if (it!=m_mapNodeType_AlarmType.end())
	{
		std::list< std::pair<tuint32, string> >::iterator itAlarmType = it->second.begin();
		for (;itAlarmType!=it->second.end();itAlarmType++)
		{
			if ((*itAlarmType).first == nAlarmType)
			{
				return true;
			}
		}
	}
	return false;
}

//是否为监控点报警
bool CEventDictionary::IsChannelAlarmType(tuint32 nAlarmType)
{
	std::map< tuint32,std::list< std::pair<tuint32, string> > >::iterator it = m_mapNodeType_AlarmType.find(CTypeCheck::GetChnnTypeValue());
	if (it!=m_mapNodeType_AlarmType.end())
	{
		std::list< std::pair<tuint32, string> >::iterator itAlarmType = it->second.begin();
		for (;itAlarmType!=it->second.end();itAlarmType++)
		{
			if ((*itAlarmType).first == nAlarmType)
			{
				return true;
			}
		}
	}
	return false;
}

//是否为设备报警(编码器)
bool CEventDictionary::IsDeviceAlarmType(tuint32 nAlarmType)
{
	std::map< tuint32,std::list< std::pair<tuint32, string> > >::iterator it = m_mapNodeType_AlarmType.find(CTypeCheck::GetPUAbstractTypeValue());
	if (it!=m_mapNodeType_AlarmType.end())
	{
		std::list< std::pair<tuint32, string> >::iterator itAlarmType = it->second.begin();
		for (;itAlarmType!=it->second.end();itAlarmType++)
		{
			if ((*itAlarmType).first == nAlarmType)
			{
				return true;
			}
		}
	}
	return false;
}

//当前报警类型是否需要配置联动信息
bool CEventDictionary::IsNeedConfigTriggerInfo(tuint32 nAlarmType)
{
// 	if ((nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_CHANNEL_OFFLINE)||\
// 		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_DEVICE_OFFLINE)||\
// 		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_ALARMHOST_OFFLINE)||\
// 		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_ACSSYSTEM_OFFLINE)||\
// 		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_DECODER_OFFLINE)||\
// 		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_ROUTING_CHECK_OFF_LINE)||\
// 		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_SERVER_OFFLINE))
// 	{
// 		return false;
// 	}
	return true;
}

//是否是节点下线报警类型
bool CEventDictionary::IsNodeOfflineAlarmType(tuint32 nAlarmType)
{
	if ((nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_CHANNEL_OFFLINE)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_DEVICE_OFFLINE)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_ALARMHOST_OFFLINE)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_ACSSYSTEM_OFFLINE)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_DECODER_OFFLINE)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_SERVER_OFFLINE))
	{
		return true;
	}
	return false;
}

//是否是智能分析服务器上报过来的报警
bool CEventDictionary::IsAnsReportAlarm(tuint32 nAlarmType)
{
	if ((nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_TARGET_IN_BLACKLIST)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_TARGET_UNDEFINED)||\
		(nAlarmType == NVMS_DEFINE::ECMS_ALARM_TYPE_SUSPECTED_FEVER))
	{
		return true;
	}
	return false;
}

//是否为门禁
bool CEventDictionary::IsDoorAlarmType(tuint32 nAlarmType)
{
	std::map< tuint32,std::list< std::pair<tuint32, string> > >::iterator it = m_mapNodeType_AlarmType.find(CTypeCheck::GetAcsDoorTypeValue());
	if (it!=m_mapNodeType_AlarmType.end())
	{
		std::list< std::pair<tuint32, string> >::iterator itAlarmType = it->second.begin();
		for (;itAlarmType!=it->second.end();itAlarmType++)
		{
			if ((*itAlarmType).first == nAlarmType)
			{
				return true;
			}
		}
	}
	return false;
}
//是否为考勤
bool CEventDictionary::IsAcsAttAlarmType(tuint32 nAlarmType)
{
	std::map< tuint32,std::list< std::pair<tuint32, string> > >::iterator it = m_mapNodeType_AlarmType.find(CTypeCheck::GetAcsDeviceTypeValue());
	if (it!=m_mapNodeType_AlarmType.end())
	{
		std::list< std::pair<tuint32, string> >::iterator itAlarmType = it->second.begin();
		for (;itAlarmType!=it->second.end();itAlarmType++)
		{
			if ((*itAlarmType).first == nAlarmType)
			{
				return true;
			}
		}
	}
	return false;
}
	//是否是服务器报警
bool CEventDictionary::IsServerAlarmType(tuint32 nAlarmType)
{
	std::map< tuint32,std::list< std::pair<tuint32, string> > >::iterator it = m_mapNodeType_AlarmType.find(CTypeCheck::GetServerTypeBeginValue());
	if (it!=m_mapNodeType_AlarmType.end())
	{
		std::list< std::pair<tuint32, string> >::iterator itAlarmType = it->second.begin();
		for (;itAlarmType!=it->second.end();itAlarmType++)
		{
			if ((*itAlarmType).first == nAlarmType)
			{
				return true;
			}
		}
	}
	return false;
}

	//是否是报警任务
bool CEventDictionary::IsAlarmTaskType(tuint32 nAlarmType)
{
    std::map< tuint32,std::list< std::pair<tuint32, string> > >::iterator it = m_mapNodeType_AlarmType.find(CTypeCheck::GetAlarmTaskTypeValue());
	if (it!=m_mapNodeType_AlarmType.end())
	{
		std::list< std::pair<tuint32, string> >::iterator itAlarmType = it->second.begin();
		for (;itAlarmType!=it->second.end();itAlarmType++)
		{
			if ((*itAlarmType).first == nAlarmType)
			{
				return true;
			}
		}
	}
	return false;
}

//是否是防区报警
bool CEventDictionary::IsZoneAlarmType(tuint32 nAlarmType)
{
	std::map< tuint32,std::list< std::pair<tuint32, string> > >::iterator it = m_mapNodeType_AlarmType.find(CTypeCheck::GetZoneTypeValue());
	if (it!=m_mapNodeType_AlarmType.end())
	{
		std::list< std::pair<tuint32, string> >::iterator itAlarmType = it->second.begin();
		for (;itAlarmType!=it->second.end();itAlarmType++)
		{
			if ((*itAlarmType).first == nAlarmType)
			{
				return true;
			}
		}
	}
	return false;
}

//是否是子系统
bool CEventDictionary::IsSubSystemAlarmType(tuint32 nAlarmType)
{
	std::map< tuint32,std::list< std::pair<tuint32, string> > >::iterator it = m_mapNodeType_AlarmType.find(CTypeCheck::GetSubSystemTypeValue());
	if (it!=m_mapNodeType_AlarmType.end())
	{
		std::list< std::pair<tuint32, string> >::iterator itAlarmType = it->second.begin();
		for (;itAlarmType!=it->second.end();itAlarmType++)
		{
			if ((*itAlarmType).first == nAlarmType)
			{
				return true;
			}
		}
	}
	return false;
}

//是否是停车场报警
bool CEventDictionary::IsPmsParkAlarmType(tuint32 nAlarmType)
{
    std::map< tuint32,std::list< std::pair<tuint32, string> > >::iterator it = m_mapNodeType_AlarmType.find(CTypeCheck::GetPmsParkTypeValue());
    if (it!=m_mapNodeType_AlarmType.end())
    {
        std::list< std::pair<tuint32, string> >::iterator itAlarmType = it->second.begin();
        for (;itAlarmType!=it->second.end();itAlarmType++)
        {
            if ((*itAlarmType).first == nAlarmType)
            {
                return true;
            }
        }
    }
    return false;
}

bool CEventDictionary::IsV2_0()
{
	string strCurVersion = GetNvmsVersion();
	tuint32 nCurMain=0,nCurSub1=0,nCurSub2=0,nCurSub3=0;
	CSysHelper::GetIntVersionFormStr(strCurVersion,nCurMain,nCurSub1,nCurSub2,nCurSub3);
	if ( nCurMain>=2 && nCurSub1>=1)
	{
		return false;
	}
	return true;
}

bool CEventDictionary::IsV2_1()
{
	string strCurVersion = GetNvmsVersion();
	tuint32 nCurMain=0,nCurSub1=0,nCurSub2=0,nCurSub3=0;
	CSysHelper::GetIntVersionFormStr(strCurVersion,nCurMain,nCurSub1,nCurSub2,nCurSub3);
	if ( nCurMain==2 && nCurSub1==1)
	{
		return true;
	}
	return false;
}

bool CEventDictionary::IsV2_1_1()
{
	string strCurVersion = GetNvmsVersion();
	tuint32 nCurMain=0,nCurSub1=0,nCurSub2=0,nCurSub3=0;
	CSysHelper::GetIntVersionFormStr(strCurVersion,nCurMain,nCurSub1,nCurSub2,nCurSub3);
	if ( nCurMain==2 && nCurSub1==1 && nCurSub2==1)
	{
		return true;
	}
	return false;
}

bool CEventDictionary::IsHigher2_1_2()
{
	string strCurVersion = GetNvmsVersion();
	tuint32 nCurMain=0,nCurSub1=0,nCurSub2=0,nCurSub3=0;
	CSysHelper::GetIntVersionFormStr(strCurVersion,nCurMain,nCurSub1,nCurSub2,nCurSub3);
	if ( (nCurMain==2 && nCurSub1==1 && nCurSub2>=2) || (nCurMain==2 && nCurSub1>=2) || (nCurMain>2) )
	{
		return true;
	}
	return false;
}

bool CEventDictionary::IsGeV2_2()
{
	string strCurVersion = GetNvmsVersion();
	tuint32 nCurMain=0,nCurSub1=0,nCurSub2=0,nCurSub3=0;
	CSysHelper::GetIntVersionFormStr(strCurVersion,nCurMain,nCurSub1,nCurSub2,nCurSub3);
	if ( nCurMain==2 && nCurSub1>=2)
	{
		return true;
	}
	return false;
}
