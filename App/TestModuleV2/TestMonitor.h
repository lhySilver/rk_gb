/*
 * Copyright (c) 2023 Fancy Code.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef TEST_MONITOR_H
#define TEST_MONITOR_H
#include "ILog.h"
#include "ITestModuleV2.h"

//===============================????
#define PRODUCT_AUDIO_REC_BUFF_SIZE (8000*5) 	//8K 16bit G711 ???1?8000???5?????8000*5
#define AUDIO_FILE_PRODUCT_RECORD         "/tmp/product_audio.g711u" 

typedef struct
{
    unsigned int size;
    unsigned char audio_data[PRODUCT_AUDIO_REC_BUFF_SIZE]; //8K 16bit G711 ???1?8000???5?????8000*5
}ProductAudioRecord_S;

#define LED_GREEN "led_green"
#define LED_RED "led_red"
#define LED_BLUE "led_blue"
//===============================????



/**
 * @brief Used to monitor the protocol events of the production test tool. Reload and implement the relevant virtual
 * function interface as needed according to the product category.
 */
class TestMonitor : public VTestMonitor
{
public:
    TestMonitor() = default;
    virtual ~TestMonitor() = default;
    static std::shared_ptr<TestMonitor> &GetInstance(std::shared_ptr<TestMonitor> *impl = nullptr);//1218
    /**
     * @brief Only one interface is implemented here, and other interfaces can refer to this interface for implementation.
     */
    TestCode PeerGetRtspUrl(std::string &url, int &videoWidth, int &videoHeight, std::string &errorMessage) override;
    TestCode PeerGetRtspUrlV2(std::string &url, int &videoWidth, int &videoHeight,
                                       std::string &urlSub, int &videoWidthSub, int &videoHeightSub,
                                       std::string &errorMessage);//添加urlSub字段 add on 2025.05.15
	
    TestCode PeerRebootDevice(std::string &errorMessage) override;
    TestCode PeerResetDevice(std::string &errorMessage) override;
    TestCode PeerSetConfigParam(void) override;
    TestCode PeerGetConfigParam(void) override;
	TestCode PeerSetSN(const std::string &sn, std::string &errorMessage) override;
	TestCode PeerGetSN(std::string &sn, std::string &errorMessage) override;
	TestCode PeerControlPTZ(const PTZCommand &command, const unsigned int &step, std::string &errorMessage) override;//不用,暂且保留
    TestCode PeerControlPTZ(const PTZCommand &command, const bool &keep, std::string &errorMessage) override;
	// TestCode PeerGetWifiList(std::vector<WifiInfo> &wifiList, std::string &errorMessage) override;
    TestCode PeerGetWifiList(const std::string &recvSsid, std::vector<WifiInfo> &wifiList, std::string &errorMessage) override;
    TestCode PeerTestInfraredLamp(const bool &isOpen, std::string &errorMessage) override;
	TestCode PeerTestMicAndHorn(std::shared_ptr<VTestReply> reply, const bool &isOpen,
                                std::string &errorMessage) override;
	TestCode PeerGetSDCartInfo(unsigned int &totalSize_MB, unsigned int &usedSize_MB,
	                           std::string &errorMessage) override;
	TestCode PeerTestPWMLamp(const unsigned short &brightness, std::string &errorMessage) override;
	TestCode PeerSetDayNightMode(const DayNightMode &mode, std::string &errorMessage) override;
    TestCode PeerIrcutSwitch(const bool &isOpen, std::string &errorMessage)override;
	TestCode PeerTestIndicatorLed(const std::string &switchName, const LEDTest &test,
                                          std::string &errorMessage) override;
	TestCode PeerGetTemperatureHumidity(float &temperature, float &humidity, std::string &errorMessage) override;
	TestCode PeerPirAlarm(const unsigned short &pir, std::string &errorMessage) override;
	TestCode PeerHornTest(const bool &open, std::string &errorMessage) override;
    TestCode PeerSendLicense(const char *license, const unsigned int &length, const std::string &pid,
                                     std::string &errorMessage) override;
	TestCode PeerEraseLicense(std::string &errorMessage) override;
    TestCode PeerChangePid(const std::string &pid, std::string &errorMessage) override;
    TestCode PeerGetDeviceInfo(DeviceInfo &info, std::string &errorMessage) override;
    TestCode PeerSendTimeForCheck(const CheckTime &time, std::string &errorMessage) override;//add on 2025.02.11 ????

public:
    void SendPirReportEvent(const std::string &pirValue);//add on 2024.12.28 
    void SendKeyReportEvent(const std::string &keyName, const KeyTest &event); //add on 2024.12.28 
    void SendDeviceInfoToPeer(const DeviceInfo &info);//add on 2025.01.02 //????????
    static void AskCheckTimeFromPeer(void);             //add on 2025.02.10
};
#endif
