#pragma once

#include <string>
#include <list>
#include <stdint.h>
#include <functional>
namespace visitor {

    enum AppointmentState {
        AppointmentRequested = 1 << 0,  // 预约申请已提交
        AppointmentUnused = 1 << 1,     // 预留，暂时没用
        AppointmentChecking = 1 << 2,   // 预约申请正在审核
        AppointmentAccepted = 1 << 3,   // 预约申请审核通过
        AppointmentReject = 1 << 4,     // 预约申请拒绝
        AppointmentVisiting = 1 << 5,     // 访问中
        AppointmentCanceled = 1 << 6,     // 已取消
        AppointmentDone = 1 << 7,     // 预约已完成
        AppointmentExpired = 1 << 8,     // 预约已过期
    };

    enum AppointmentSource {
        FromWeb = 0,
        FromApp = 1,
        FromMicroProgram = 2
    };

    class VisitError
    {
    public:
        int64_t     m_errCode;  //json:"errcode"
        std::string m_errMsg;   //json:"errmsg"
    };

    //////////////////////////////////////////////////////////////////////////
    // 在nvms上将
    // 命令：audit
    //
    class Operation : public VisitError
    {
    public:
        Operation() : m_visitID(""), m_operator(""), m_operatorName(""), m_log(""), m_time(0), m_version(0) {}
    public:
        std::string     m_visitID;      //json:"visitId"
        int64_t         m_version;      //json:"version"
        std::string     m_operator;     //json:"operator"
        std::string     m_operatorName;     //json:"operatorName"
        int64_t         m_time;         //json:"time"
        std::string     m_log;          //json:"log"
        AppointmentState m_state;       //json:"state"
    };

    class VersionedItem
    {
    public:
        VersionedItem() : m_visitID(""),
            m_version(0)
        {}
    public:
        std::string     m_visitID;      //json:"visitId"
        int64_t         m_version;      //json:"version"
    };

    class AppointmentInfo
    {
    public:
        AppointmentInfo() : m_name(""),
            m_gender(""),
            m_phoneNum(""),
            m_idType(""),
            m_idNum(""),
            m_startTime(0),
            m_endTime(0),
            m_reason(""),
            m_carNum(""),
            m_department(""),
            m_pictureName(""),
            m_pictureURL(""),
            m_receiver(""),
            m_receiverPhone("")
        {
        }
        virtual ~AppointmentInfo() {
            for (auto iter = m_companion.begin(); iter != m_companion.end(); ++iter) {
                delete *iter;
            }
            m_companion.clear();
        }

    public:
        std::string     m_name;         //json:"name"
        std::string     m_gender;       //json:"gender"
        std::string     m_phoneNum;     //json:"mobile"
        std::string     m_idType;       //json:"idTypeValue"
        std::string     m_idNum;        //json:"idNum"
        int64_t         m_startTime;    //json:"start_time_utc"  yyyy-MM-ddThh:mm:ss.msZ
        int64_t         m_endTime;      //json:"end_time_utc"    yyyy-MM-dd hh:mm:ss.msZ
        // 访问缘由
        std::string     m_reason;       //json:"reason"
        std::string     m_carNum;       //json:"plateNum"
        // 来访人员单位
        std::string     m_department;   //json:"unit"
        // 图片名称
        std::string     m_pictureName;  //json:"pictureName"
        // 给一个URL
        std::string     m_pictureURL;   //json:"pictureUrl"
        // 被访问者信息
        std::string     m_receiver;     //json:"receiver"
        std::string     m_receiverPhone;//json:"receiverMobile"

        // 同行者
        std::list<AppointmentInfo*> m_companion;    //json:"together"
    };

    class Appointment
    {
    public:
        Appointment() : m_visitID(""),
            m_version(0),
            m_userID("")
        {
        }
    public:
        std::string     m_visitID;          //json:"visitId"
        int64_t         m_version;          //json:"version"
        std::string     m_userID;           //json:"visitor"
        AppointmentInfo m_appointment;      //json:"appointment"
        Operation       m_operation;        //json:"operation"
    };

    class RequestAuditParam
    {
    public:
        RequestAuditParam() :
            m_version(0),
            m_visitorID(""),
            m_picbase64(""),
            m_picURL(""),
            m_picName(""),
            m_isRawImage(false)
        {}
    public:
        int64_t     m_version;      //json:"version"
        std::string m_visitorID;    //json:"visitId"
        std::string m_picbase64;    //json:"picture"
        std::string m_picURL;       //json:"pictureUrl"
        std::string m_picName;      //json:"pictureName"
        bool        m_isRawImage;   //json:"-"
        Operation   m_operation;    //json:"operation"
        std::list<std::string> m_deviceGuidLst; //json:"permissionSet"
    };

    class RequestAuditResult
    {
    public:
        RequestAuditResult() : m_visitorID(""),
            m_version(0),
            m_state(0),
            m_errCode(0),
            m_errMsg("")
        {}
    public:
        std::string m_visitorID;//json:"visitId"
        int64_t     m_version;  //json:"version"
        int64_t     m_state;    //json:"state"
        int64_t     m_errCode;  //json:"errcode"
        std::string m_errMsg;   //json:"errmsg"
    };

    //////////////////////////////////////////////////////////////////////////
    // 设备列表
    enum  DeviceState
    {
        DeviceOffline = 0,
        DeviceOnline = 1,
    };

    enum  _AreaNodeType
    {
        AreaType = 0,
        AreaDeviceType = 1,
    };
    // Device 设备信息
    class Device
    {
    public:
        Device() : m_name(""),
            m_guid("")
            , m_state(DeviceOffline)
        {}

    public:
        std::string     m_name; //json:"name"
        std::string     m_guid; //json:"guid"
        DeviceState     m_state;//json:"state"
    };

    // Area 区域，指定当前nvms包含的区域， 各区域下属的设备列表
    class Area
    {
    public:
        Area() : m_type(AreaType), m_state(DeviceOnline) {}
        virtual ~Area() {
            for (auto iter = m_children.begin(); iter != m_children.end(); ++iter) {
                delete *iter;
            }
            m_children.clear();
        }

        void addDevice(const Device* dev) {
            Area* tmp = new Area;
            tmp->m_name = dev->m_name;
            tmp->m_guid = dev->m_guid;
            tmp->m_state = dev->m_state;
            tmp->m_type = AreaDeviceType;
            m_children.push_back(tmp);
        }

        void addArea(const Area* area) {
            std::function<void(Area*, const Area*)> copyArea;
            copyArea = [&](Area * dst, const Area * src) {
                dst->m_name = src->m_name;
                dst->m_guid = src->m_guid;
                dst->m_type = src->m_type;
                dst->m_state = src->m_state;

                // 添加区域
                for (auto iter = src->m_children.begin(); iter != src->m_children.end(); ++iter) {
                    Area* subArea = new Area;
                    copyArea(subArea, *iter);
                    dst->m_children.push_back(subArea);
                }
            };

            Area* ptr = new Area;
            copyArea(ptr, area);
            m_children.push_back(ptr);
        }

        const std::list<Area*> subAreaList() const {
            return m_children;
        }


    public:
        std::string     m_name;                //json:"name"
        std::string     m_guid;                //json:"guid"
        uint64_t        m_state;               //json:"state"
        uint64_t        m_type;                //json:"type"
        std::list<Area*> m_children;           //json:"children"
    };
}