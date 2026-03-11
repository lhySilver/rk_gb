/*
 * Copyright (c) 2023 DGIOT
 *
 * NOTICE: ALL RIGHTS RESERVED.
 *
 * This source code file is the exclusive property of DGIOT.
 * It is confidential and proprietary information.
 *
 * No part of this file may be copied, reproduced, transmitted, distributed,
 * sold, disclosed, or otherwise used in any form or by any means without
 * the express written permission of DGIOT.
 *
 * Violation of this copyright notice may result in severe legal consequences.
 * By accessing this source code, you agree to abide by the terms of this
 * copyright notice.
 */
#ifndef I_TEST_MODULE_V2_H
#define I_TEST_MODULE_V2_H
#include "StatusCode.h"
#include <memory>
#include <string>
#include <vector>
enum class TestCode
{
    SUCCESS = 0,
    RESPONE_ASYNCHRONOUSLY = 1,
    UN_SUPPORT_FUNCTION = -1,
    TEST_FAILED = -2,
    PARSE_DATA_FAILED = -3,
    UN_SUPPORT_PARAMETER = -4,
    DEVICE_REPORT_FAILED = -5,
    END
};
enum class PTZCommand
{
    PTZ_UP = 0,
    PTZ_DOWN,
    PTZ_LEFT,
    PTZ_RIGHT,
    PTZ_SELF_TEST,
    PTZ_SELF_LEVEL,
    PTZ_SELF_VERTICAL,
    PTZ_RESET,
    END
};
enum class DayNightMode
{
    AUTO = 0,
    DAY,
    NIGHT,
    END
};
enum class LEDTest
{
    OFF = 0,
    ON,
    BLUE,
    GREEN,
    RED,
    END
};
enum class LenFocus
{
    FRONT = 0,
    REAR,
    END
};
enum class KeyTest
{
    UP = 0,
    DOWN,
    HOLD_DOWN,
    HOLD_UP,
    CLICK,
    END
};
typedef struct wifi_info
{
    wifi_info(const std::string &ssid, const unsigned short &signal);
    const std::string mSsid;
    const unsigned short mSignal;
} WifiInfo;
typedef struct device_info
{
    device_info();
    std::string mProductModel;
    std::string mVersion;
    std::string mCloudPlatform;
    std::string mMac;
    std::string mPid;
    std::string mUid;
} DeviceInfo;
class VTestAsk
{
public:
    VTestAsk() = default;
    virtual ~VTestAsk() = default;
    virtual TestCode Blocking(void);
    virtual bool NeedReply(void);
    virtual void ReplyFinished(const TestCode &code);
    virtual bool IfTimeout(const unsigned int &integrationTime_ms);

public:
    /**
     * @brief The serial number of a single request, assigned by the TestModuleV2 module, is used for internal
     * management within the TestModuleV2 module.
     */
    unsigned int mSerialNumber;
};
template <typename T>
class TestAsk : virtual public VTestAsk
{

public:
    TestAsk() = default;
    virtual ~TestAsk() = default;

public:
    T mDataReply;
};
class VTestReply
{
public:
    VTestReply() = default;
    virtual ~VTestReply() = default;
    virtual void ReplyAsynchronously(const TestCode &code, const std::string &errorMessage = "");
};
template <typename T>
class TestReplyV2 : virtual public VTestReply
{

public:
    TestReplyV2() = default;
    virtual ~TestReplyV2() = default;

public:
    T mDataReply;
};
/**
 * @brief The test module monitor abstract class is implemented by the user and registered to the test module to obtain
 * the command instructions received by the module.
 */
class VTestMonitor
{
public:
    VTestMonitor() = default;
    virtual ~VTestMonitor() = default;
    /**
     * @brief Get the URL address of the RTSP media stream, including the video resolution (used by the peer to frame
     * the video area).
     * @param url [out] - RTSP media stream URL
     * @param videoWidth [out] - Video resolution-height
     * @param videoHeight [out] - Video resolution-width
     * @param errorMessage [out] - Error message string. When the return fails, you can fill in the error message.
     * @return TestCode
     */
    virtual TestCode PeerGetRtspUrl(std::string &url, int &videoWidth, int &videoHeight, std::string &errorMessage);
    /**
     * @brief Sets the frame area.
     * @param videoWidth [in]
     * @param videoHeight [in]
     * @param frame_X [in]
     * @param frame_Y [in]
     * @param frameWidth [in]
     * @param frameHeight [in]
     * @return TestCode
     */
    virtual TestCode PeerSetFrameArea(const int &videoWidth, const int &videoHeight, const int &frame_X,
                                      const int &frame_Y, const int &frameWidth, const int &frameHeight);
    /**
     * @brief Restart the device.
     * @return TestCode
     */
    virtual TestCode PeerRebootDevice(std::string &errorMessage);
    virtual TestCode PeerResetDevice(std::string &errorMessage);
    /**
     * @brief Set configuration parameters.
     * @warning TODO: Content to be determined.
     * @return TestCode
     */
    virtual TestCode PeerSetConfigParam(void);
    /**
     * @brief Read configuration parameters.
     * @warning TODO: Content to be determined.
     * @return TestCode
     */
    virtual TestCode PeerGetConfigParam(void);
    /**
     * @brief
     * @param sn
     * @param errorMessage [out] - Error message string. When the return fails, you can fill in the error message.
     * @return TestCode
     */
    virtual TestCode PeerSetSN(const std::string &sn, std::string &errorMessage);
    /**
     * @brief
     * @param sn
     * @param errorMessage [out] - Error message string. When the return fails, you can fill in the error message.
     * @return TestCode
     */
    virtual TestCode PeerGetSN(std::string &sn, std::string &errorMessage);
    virtual TestCode PeerControlPTZ(const PTZCommand &command, const unsigned int &step, std::string &errorMessage);
    virtual TestCode PeerControlPTZ(const PTZCommand &command, const bool &keep, std::string &errorMessage);
    /**
     * @brief Switch the ir cut.
     * @param isOpen [in] - The number of ircut switches.
     * @param errorMessage [out] - Error message string. When the return fails, you can fill in the error message.
     * @return TestCode
     */
    virtual TestCode PeerIrcutSwitch(const bool &isOpen, std::string &errorMessage);
    /**
     * @brief Wireless speed test.
     *
     * @param time_ms [in] - The time of the wireless speed test, in milliseconds.
     * @param speed [out] - Reply "speed" data.
     * @param errorMessage [out] - Error message string. When the return fails, you can fill in the error message.
     * @return TestCode
     */
    virtual TestCode PeerGetLinkingWifiSignal(unsigned int &signal, std::string &errorMessage);
    /**
     * @brief Get the WiFi list.
     * @param wifiList [out] - Reply "wifiList" data. See WifiInfo for details.
     * @param errorMessage [out] - Error message string. When the return fails, you can fill in the error message.
     * @return TestCode
     */
    virtual TestCode PeerGetWifiList(std::vector<WifiInfo> &wifiList, std::string &errorMessage);
    /**
     * @brief Test the MIC function and link it to the speaker. Record the audio and play it back to complete the test
     * process.
     * @param reply [in] - Use this parameter to complete an asynchronous reply.
     * @param isOpen
     * @param errorMessage
     * @return TestCode - If "RESPONE_ASYNCHRONOUSLY" is returned, the "reply" parameter must be used to complete the
     * asynchronous reply.
     */
    virtual TestCode PeerTestMicAndHorn(std::shared_ptr<VTestReply> reply, const bool &isOpen,
                                        std::string &errorMessage);
    virtual TestCode PeerTestInfraredLamp(const bool &isOpen, std::string &errorMessage);
    /**
     * @brief Test the mounting and unmounting of the SD card.
     * @param mount [in] - true: Mount the SD card, false: Unmount the SD card.
     * @param errorMessage [out] - Error message string. When the return fails, you can fill in the error message.
     * @return TestCode
     */
    virtual TestCode PeerMountSDCard(const bool &mount, std::string &errorMessage);
    virtual TestCode PeerGetSDCartInfo(unsigned int &totalSize_MB, unsigned int &usedSize_MB,
                                       std::string &errorMessage);
    virtual TestCode PeerFormatSDCard(std::string &errorMessage);
    /**
     * @brief
     * @param brightness [in] - The brightness of the lamp, 0-100.
     * @param errorMessage [out] - Error message string. When the return fails, you can fill in the error message.
     * @return TestCode
     */
    virtual TestCode PeerTestPWMLamp(const unsigned short &brightness, std::string &errorMessage);
    virtual TestCode PeerSetDayNightMode(const DayNightMode &mode, std::string &errorMessage);
    /**
     * @brief LED light test.
     * @param switchName [in] - The control name of the production test tool switch control is used to identify the
     * specific light name.
     * @param test [in] - See LEDTest for details.
     * @param errorMessage
     * @return TestCode
     */
    virtual TestCode PeerTestIndicatorLed(const std::string &switchName, const LEDTest &test,
                                          std::string &errorMessage);
    virtual TestCode PeerGetTemperatureHumidity(float &temperature, float &humidity, std::string &errorMessage);
    virtual TestCode PeerPirAlarm(const unsigned short &pir, std::string &errorMessage);
    virtual TestCode PeerLenFocus(const LenFocus &len, const unsigned short &step, std::string &errorMessage);
    virtual TestCode PeerHornTest(const bool &open, std::string &errorMessage);
    virtual TestCode PeerSendLicense(const char *license, const unsigned int &length, const std::string &pid,
                                     std::string &errorMessage);
    virtual TestCode PeerEraseLicense(std::string &errorMessage);
    virtual TestCode PeerChangePid(const std::string &pid, std::string &errorMessage);
    /**
     * @brief The production test tool obtains device information.
     * @warning If a field of "DeviceInfo" does not exist, do not assign a value to it.
     * @param info [out] - See DeviceInfo for details.
     * @param errorMessage
     * @return TestCode
     */
    virtual TestCode PeerGetDeviceInfo(DeviceInfo &info, std::string &errorMessage);
};
class ITestModuleV2
{
public:
    ITestModuleV2() = default;
    virtual ~ITestModuleV2() = default;
    static std::shared_ptr<ITestModuleV2> &GetInstance(std::shared_ptr<ITestModuleV2> *impl = nullptr);
    /**
     * @brief Initialize the test module and start the TCP server.
     * @param devcieIP The IP address of the device, start the TCP server.
     * @return StatusCode
     */
    virtual StatusCode Init(const std::string &devcieIP);
    virtual StatusCode UnInit(void);
    /**
     * @brief The life cycle of the passed smart pointer needs to be guaranteed, and all event reporting will only take
     * effect when the smart pointer is valid.
     * @param monitor
     * @return StatusCode
     */
    virtual StatusCode SetTestMointor(std::shared_ptr<VTestMonitor> &monitor);
    virtual TestCode SetRebootDeviceToPeer(std::shared_ptr<VTestAsk> &ask);
    virtual TestCode SetRtspUrlToPeer(std::shared_ptr<VTestAsk> &ask, const std::string &url, const int &videoWidth,
                                      const int &videoHeight);
    virtual TestCode SetSNToPeer(std::shared_ptr<VTestAsk> &ask, const std::string &sn);
    virtual TestCode SetIrcutSwitchToPeer(std::shared_ptr<VTestAsk> &ask, const unsigned short &times,
                                          const std::string &errorMessage);
    /**
     * @brief Send PIR trigger event.
     * @param ask [in] - Request instance. Blocking/non-blocking requests can be implemented through this instance.
     * @param value [in] - PIR trigger event set. For example, "111" means there are three PIRs and all three PIRs are
     * triggered. "000" means none of the three PIRs are triggered.
     * @param errorMessage [in] - Ignored now.
     * @return TestCode
     */
    virtual TestCode SetPirEventToPeer(std::shared_ptr<VTestAsk> &ask, const std::string &value,
                                       const std::string &errorMessage);
    /**
     * @brief Send key events to the production test tool.
     * @param ask [in] - Request instance. Blocking/non-blocking requests can be implemented through this instance.
     * @param keyName [in] - The name of the key, such as "reset_key".
     * @param event [in] - See KeyTest for details.
     * @param errorMessage [in] - Ignored now.
     * @return TestCode
     */
    virtual TestCode SetKeyEventToPeer(std::shared_ptr<VTestAsk> &ask, const std::string &keyName, const KeyTest &event,
                                       const std::string &errorMessage);
    virtual TestCode SendDeviceInfoToPeer(std::shared_ptr<VTestAsk> &ask, const DeviceInfo &info,
                                          const std::string &errorMessage);

public:
    static const char *PrintfDayNightMode(const DayNightMode &mode);
    static const char *PrintfLEDTest(const LEDTest &test);
    static const char *PrintfTestCode(const TestCode &code);
};
bool CreateTestModuleV2Module(void);
bool DestroyTestModuleV2Module(void);
#endif