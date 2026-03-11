#include "xml_get_data.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
/*
 * UserAuth
 */
int getUserAuthData(XMLN *xmlHeader, AUTH_DATA *authData)
{
	size_t len;
	XMLN *sercurity = NULL;
	XMLN *token = NULL;
	XMLN *name = NULL;
	XMLN *password = NULL;
	XMLN *noce = NULL;
	XMLN *created = NULL;

	sercurity = xml_node_soap_get(xmlHeader, "Security");
	if (sercurity == NULL) return -1;

	token = xml_node_soap_get(sercurity, "UsernameToken");
	if (token == NULL) return -1;

	name = xml_node_soap_get(token, "Username");
	if (name == NULL || name->data == NULL) return -1;
	authData->userName = name->data;	

	password = xml_node_soap_get(token, "Password");
	if (password == NULL || password->data == NULL) return -1;
	authData->password = password->data;

	noce = xml_node_soap_get(token, "Nonce");
	if (noce == NULL || noce->data == NULL) return -1;
	authData->noce = noce->data;

	created = xml_node_soap_get(token, "Created");
	if (created == NULL || created->data == NULL) return -1;
	authData->created = created->data;

	return 0;
}

/*
 * GetCapabilities
 */
CATEGORY_TYPE getCapabilitiesCategory(XMLN *xmlBody)
{
	XMLN *category;
	if (xmlBody == NULL) return CAP_CATEGORY_INVALID;

	category = xml_node_soap_get(xmlBody, "Category");
	if (category == NULL || category->data == NULL) return CAP_CATEGORY_INVALID;

	if (strcasecmp(category->data, "Media") == 0)
    {
        return CAP_CATEGORY_MEDIA;
    }
    else if (strcasecmp(category->data, "Device") == 0)
    {
        return CAP_CATEGORY_DEVICE;
    }
    else if (strcasecmp(category->data, "Analytics") == 0)
    {
        return CAP_CATEGORY_ANALYTICS;
    }
    else if (strcasecmp(category->data, "Events") == 0)
    {
        return CAP_CATEGORY_EVENTS;
    }
    else if (strcasecmp(category->data, "Imaging") == 0)
    {
        return CAP_CATEGORY_IMAGE;
    }
    else if (strcasecmp(category->data, "PTZ") == 0)
    {
        return CAP_CATEGORY_PTZ;
    }
    else if (strcasecmp(category->data, "All") == 0)
    {
        return CAP_CATEGORY_ALL;
    }

    return CAP_CATEGORY_INVALID;
}

/*
 * GetStreamUri
 */
int getStreamUriData(XMLN *xmlBody, STREAMURI_DATA *data)
{
	XMLN *token = NULL;
	XMLN *streamSetup = NULL;
	XMLN *transport = NULL;
	XMLN *protocol = NULL;
	int tokenLen;

	token = xml_node_soap_get(xmlBody, "ProfileToken");
	if (token == NULL || token->data == NULL) return -1;

	tokenLen = strlen(token->data);
	data->index = token->data[tokenLen-1] - '0';

	streamSetup = xml_node_soap_get(xmlBody, "StreamSetup");
	if (streamSetup == NULL) return -1;

	transport = xml_node_soap_get(streamSetup, "Transport");
	if (transport == NULL) return -1;

	protocol = xml_node_soap_get(transport, "Protocol");
	if (protocol == NULL || protocol->data == NULL) return -1;

	strcpy(data->Protocol, protocol->data);
	
	return 0;
}

/*
 * GetPtz CFG
 */
//PTZ Configuration
#define			MAX_PRESET_NUM					4
PTZConfig  ptz_cfg = {
	"PTZConf1", //token
	"user0",	//name
	4,			//usecnt
	"PTZ1", 	//nodetoken
	"http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace",		//abs_pant_space
	"http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace",			//abs_zoom_space
	"http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace",	//rel_pant_space
	"http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationGenericSpace",		//rel_zoom_space
	"http://www.onvif.org/ver10/tptz/PanTiltSpaces/VelocityGenericSpace",		//cont_pant_space
	"http://www.onvif.org/ver10/tptz/ZoomSpaces/VelocityGenericSpace",			//cont_zoom_space
	"http://www.onvif.org/ver10/tptz/PanTiltSpaces/GenericSpeedSpace",			//speed_pant_space
	50.0,		 //speed_pant_x
	50.0,		 //speed_pant_y
	"http://www.onvif.org/ver10/tptz/ZoomSpaces/ZoomGenericSpeedSpace", 		//speed_zoom_space
	0.0,		//speed_zoom_x
	60000,		 //timeout
	"http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace",		//limit_pant_space
	{-1.0,1.0}, //limit_pant_x
	{-1.0,1.0}, //limit_pant_y
	"http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace",			//limit_zoom_space
	{-1.0,1.0}, //limit_zoom_x
	{-1.0,1.0}, //abs_pant_x
	{-1.0,1.0}, //abs_pant_y
	{-1.0,1.0}, //abs_zoom_x
	{-1.0,1.0}, //rel_pant_x
	{-1.0,1.0}, //rel_pant_y
	{-1.0,1.0}, //rel_zoom_x
	{-1.0,1.0}, //cont_pant_x
	{-1.0,1.0}, //cont_pant_y
	{-1.0,1.0}, //cont_zoom_x
	{0.0,100.0},  //pant_speed_x
	{0.0,100.0},  //zoom_speed_x
	MAX_PRESET_NUM, 		//max_presets_num
	60000,		//max_timeout
	{0,-1.0,0.0},	 // MOTOR_SPACE_INFO pan
	{0,-1.0,0.0},	 // MOTOR_SPACE_INFO tilt
	{0,0.0,0.0},	 // MOTOR_SPACE_INFO zoom
};
PTZConfig * getPtzCfg(void)
{
	
	return &ptz_cfg;
}

/*
 * GetProfile
 * GetProfiles
 */

//metaDatacongration
MetaConfig meta_cfg={
    "MetaDataConf1",
    "MetaDataConf1",
    4,
    "true",
    "true",
    "http://www.onvif.org/ver10/tev/topicExpression/ConcreteSet",
    "http://www.onvif.org/ver10/tev/messageContentFilter/ItemFilter",
    "false",
    "",
    "0.0.0.0",
    "0000:0000:0000:0000:0000:0000:0000:0000",
    60000,
    64,
    "false",
    "PT0H1M0S"
};

MetaConfig * getMetaCfg(void)
{
	return &meta_cfg;
}

/*
 * GetContinuesMoveData
 */

int GetContinuousMoveData(XMLN *xmlBody, float *x, float *y)
{	
	XMLN *panilt = NULL;
	XMLN *Velocity = NULL;
	const char *numX = NULL;
	const char *numY = NULL;
	
	Velocity = xml_node_soap_get(xmlBody, "Velocity");
	if (Velocity == NULL) return -1;
	
	panilt = xml_node_soap_get(Velocity, "PanTilt");
	if (panilt == NULL) return -1;
	
	numX = xml_attr_get(panilt, "x");
	printf("x:%s\n",numX);
	if (numX) *x = (float)atof(numX);
	else return -1;
	numY = xml_attr_get(panilt, "y");
	printf("y:%s\n",numY);
	if (numY) *y = (float)atof(numY);
	else return -1;
	
	return 0;
}

/*
 *	UserControl
 *	在报文中获取到的userLevel通过下面的table转换为userRole
 */
#define MAX_USE_NUM 8
static const char * userTable[] = {"Administrator", "Operator", "User"};
int getUserInfoFromxml(XMLN *xmlBody, char *name, ONVIF_USER_INFO *userInfo)
{
	
	XMLN *userTag = NULL;
	int i = 0;
	XMLN *user = NULL;

	if (strcmp(name, "DeleteUsers") != 0) {
		userTag = xml_node_soap_get(xmlBody, name);
		if (userTag == NULL) return -1;
	} else {
		userTag = xmlBody;
	}

	user = userTag->f_child;

	while (1)
	{
		if (user == NULL || user->data == NULL) return -1;

		if (soap_strcmp(user->name, "Username") == 0) {
			if (strlen(user->data) > MAX_USER_NAME_LEN) return -1;
			strcpy(userInfo[i].userName, user->data);
		} else return -1;

		user= user->next;

		if (strcmp(name, "DeleteUsers") == 0) {
			i++;
			if (user == NULL) break;
			continue;
		}
		
		if (user == NULL || user->data == NULL) return -1;

		if (soap_strcmp(user->name, "Password") == 0) {
			if (strlen(user->data) > MAX_USER_PASSWD_LEN) return -1;
			strcpy(userInfo[i].password, user->data);
		} else return -1;
			
		user= user->next;

		if (user == NULL || user->data == NULL) return -1;

		if (soap_strcmp(user->name, "UserLevel") != 0) return -1;

		if (strcmp(user->data, userTable[0]) == 0) userInfo[i].userRole = 1;
		else if (strcmp(user->data, userTable[1]) == 0) userInfo[i].userRole = 2;
		else if (strcmp(user->data, userTable[2]) == 0) userInfo[i].userRole = 3;
		else userInfo[i].userRole = 3;

		user= user->next;
		i++;
		if (user == NULL) break;
	}

	return i;
}

int getVideoEncParamFromXml(XMLN *xmlBody, unsigned short *nFps, unsigned short *nKBps, int *nChn)
{
	XMLN *configuration = NULL;
	XMLN *name = NULL;
	XMLN *rateControl = NULL;
	XMLN *frameRateLimit = NULL;
	XMLN *bitrateLimit = NULL;
	int nLen = 0;

	configuration = xml_node_soap_get(xmlBody, "Configuration");
	printf("---------------%d,%s,%s, %d, %d\n",__LINE__,configuration->name, configuration->f_child->name, (configuration == NULL), (configuration->data == NULL));
	if (configuration == NULL) return -1;

	name = xml_node_soap_get(configuration, "Name");
	if (name == NULL ||  name->data == NULL) return -1;

	nLen = strlen(name->data);
	*nChn = name->data[nLen-1] - '0';

	rateControl = xml_node_soap_get(configuration, "RateControl");
	if (rateControl == NULL) return -1;

	frameRateLimit = xml_node_soap_get(rateControl, "FrameRateLimit");
	if (frameRateLimit == NULL || frameRateLimit->data == NULL) return -1;

	*nFps = (unsigned short)atoi(frameRateLimit->data);
	printf("---------------%d\n",__LINE__);

	bitrateLimit = xml_node_soap_get(rateControl, "BitrateLimit");
	if (bitrateLimit == NULL || bitrateLimit->data == NULL) return -1;
	printf("---------------%d\n",__LINE__);

	*nKBps = (unsigned short)atoi(bitrateLimit->data);
	printf("---------------%d\n",__LINE__);
	return 0;
}

int getSystemDateAndTime(XMLN *xmlBody, SYSTEM_DATE_TIME *systemDateAndTime)
{
	XMLN *dateTimeType;
	XMLN *temp;
	XMLN *timeZone;
	XMLN *setTime;
	XMLN *setDate;
	struct tm timeValue;
	int nRes;

	memset(&timeValue, 0, sizeof(timeValue));
    memset(systemDateAndTime, 0, sizeof(SYSTEM_DATE_TIME));

	dateTimeType = xml_node_soap_get(xmlBody, "DateTimeType");
	if (dateTimeType == NULL) return -1;
	printf("name:%s\n",dateTimeType->name);
	do {
		nRes = strcmp(dateTimeType->data, "Manual");
		if (nRes == 0) {
			systemDateAndTime->type = MANUAL;
			break;
		}

		nRes = strcmp(dateTimeType->data, "NTP");
		if (nRes == 0) {
			systemDateAndTime->type = NTP;
			break;
		}

		printf(" get SystemDateAndTime type error!\n");
		return -1;
	} while(0);

	if (systemDateAndTime->type == NTP) {
		temp = xml_node_soap_get(xmlBody, "TimeZone");
		printf("name:%s\n",temp->name);
		if (temp == NULL) return -1;

		timeZone = temp = xml_node_soap_get(temp, "TZ");
		printf("name:%s\n",timeZone->name);
		if (timeZone == NULL || timeZone->data == NULL) return -1;

		strcpy(systemDateAndTime->timeZone, timeZone->data);		

	} else {
		temp = xml_node_soap_get(xmlBody, "UTCDateTime");
		if (temp == NULL) return -1;

		setTime = xml_node_soap_get(temp, "Time");
		if (setTime == NULL) return -1;

		setDate = xml_node_soap_get(temp, "Date");
		if (setDate == NULL) return -1;

		// 获取时间
		temp = xml_node_soap_get(setTime, "Second");
		if (temp == NULL || temp->data == NULL) return -1;
		timeValue.tm_sec = atoi(temp->data);

		temp = xml_node_soap_get(setTime, "Minute");
		if (temp == NULL || temp->data == NULL) return -1;
		timeValue.tm_min = atoi(temp->data);

		temp = xml_node_soap_get(setTime, "Hour");
		if (temp == NULL || temp->data == NULL) return -1;
		timeValue.tm_hour = atoi(temp->data);

		temp = xml_node_soap_get(setDate, "Day");
		if (temp == NULL || temp->data == NULL) return -1;
		timeValue.tm_mday = atoi(temp->data);
	
		temp = xml_node_soap_get(setDate, "Month");
		if (temp == NULL || temp->data == NULL) return -1;
		timeValue.tm_mon = atoi(temp->data) - 1;

		temp = xml_node_soap_get(setDate, "Year");
		if (temp == NULL || temp->data == NULL) return -1;
		timeValue.tm_year = atoi(temp->data) - 1900;

		systemDateAndTime->timeValue.tv_sec = mktime(&timeValue);
		systemDateAndTime->timeValue.tv_usec = 0;
		
	}
	
	return 0;
}
time_t getCreatePullPointTime(XMLN *xmlBody)
{
	XMLN *terminationTime = NULL;
	time_t timeGet;
	time_t durationTime = 0;
	char tempStr[32];
	char timeType;

	terminationTime = xml_node_soap_get(xmlBody, "InitialTerminationTime");
	if (terminationTime == NULL || terminationTime->data == NULL) return -1;

	sscanf(terminationTime->data, "%32[^0-9]%d%c",tempStr,&timeGet, &timeType);

	switch (timeType) {
		case 'd' :
		case 'D' :
			durationTime = timeGet*60*60*24;
			break;
		case 'h' :
		case 'H' :
			durationTime = timeGet*60*60;
			break;
		case 'm' :
		case 'M' :
			durationTime = timeGet*60;
			break;
		case 's' :
		case 'S' :
			durationTime = timeGet;
			break;
		default :
			printf("the time of Create Pull Point is error!\n");
			break;
	}

	return durationTime;
}

time_t getRenewTime(XMLN *xmlBody)
{
	XMLN *terminationTime = NULL;
	time_t timeGet;
	time_t durationTime = 0;
	char tempStr[32];
	char timeType;

	terminationTime = xml_node_soap_get(xmlBody, "TerminationTime");
	if (terminationTime == NULL || terminationTime->data == NULL) return -1;
	sscanf(terminationTime->data, "%32[^0-9]%d%c",tempStr,&timeGet, &timeType);

	switch (timeType) {
		case 'd' :
		case 'D' :
			durationTime = timeGet*60*60*24;
			break;
		case 'h' :
		case 'H' :
			durationTime = timeGet*60*60;	
			break;
		case 'm' :
		case 'M' :
			durationTime = timeGet*60;
			break;
		case 's' :
		case 'S' :
			durationTime = timeGet;	
			break;
		default :
			printf("the time of Create Pull Point is error!\n");
			break;
	}
	return durationTime;
}
