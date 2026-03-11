TEMPLATE = subdirs
TEMPLATE = lib
CONFIG += debug_and_release warn_on plugin
QT   -= gui
CONFIG -= qt
TARGET = GB28181ClientSDK

include(../../../PLATFORM_SUFFIX.pri)
include(../../../common.pri)

CONFIG(debug, debug|release) {
    DESTDIR = ../../../CommonFile/CommonLib/Debug$$SUFFIX
    OBJECTS_DIR = ./Debug
    UI_DIR = ./Debug
    RCC_DIR = ./Debug
    MOC_DIR = ./Debug
    LIBS += -L../../../CommonFile/CommonLib/Debug$$SUFFIX
} else {
    DESTDIR = ../../../CommonFile/CommonLib/Release$$SUFFIX
    OBJECTS_DIR = ./Release
    UI_DIR = ./Release
    RCC_DIR = ./Release
    MOC_DIR = ./Release
    LIBS += -L../../../CommonFile/CommonLib/Release$$SUFFIX
}

win32: {
   DEFINES += GB28181SDK_EXPORTS
}
unix:!mac: {
    QMAKE_CC += -g
    QMAKE_CXX += -g
    QMAKE_LINK += -g
}

INCLUDEPATH += ../include/XML/rapidxml
INCLUDEPATH += ../include/XML
INCLUDEPATH += ../include/SDP
INCLUDEPATH += ../include/
INCLUDEPATH += ../../../CommonFile/CommonLib
INCLUDEPATH += ../../../CommonFile/Common/Layer1_Common  ../../../CommonFile/Common/Layer3_Abstract
INCLUDEPATH += ../../../CommonFile/Common/Layer2_Platform/$$PLATFORM
INCLUDEPATH += ../../../CommonFile/CommonLib/third_lib/sip/osip/include
INCLUDEPATH += ../../../CommonFile/CommonLib/third_lib/sip/eXosip/include
INCLUDEPATH += ../../../CommonFile/CommonLib/third_lib/sip/osip/include/osipparser2/headers

LIBS += -lShareLib -lSipSDK

HEADERS += ../../../CommonFile/Common/Layer2_Platform/$$PLATFORM/Lock.h \
    ../../../CommonFile/Common/Layer2_Platform/$$PLATFORM/Interlocked.h \
    ../../../CommonFile/Common/Layer2_Platform/$$PLATFORM/PFHelper.h \
    ../../../CommonFile/Common/Layer3_Abstract/MarkupSTL.h \
    ../../../CommonFile/Common/Layer3_Abstract/tinyxml2.h \
    ../../../CommonFile/Common/Layer3_Abstract/SysHelper.h \
    ../include/SDP/SdpMessage.h \
    ../include/SDP/SdpUtil.h \
    ../include/GBTime.h \
    ../include/Rtsp/GBMANSRTSP.h \
    ../include/Rtsp/RtspParser.h \
     GBClientImpl.h \
    ../include/XML/GB28181XmlParser.h \
    ../include/StreamRequestManager.h \
    ../include/XML/base_cmd.h \
    ../include/XML/base_proco.h \
    ../include/XML/alarm_notify.h \
    ../include/XML/home_position_control.h \
    ../include/XML/record_index_response.h \
    ../include/XML/catalog_response.h \
    ../include/XML/device_status_response.h \
    ../include/XML/preset_response.h \
    ../include/XML/ptz_control.h \
    ../include/XML/mobile_position_notify.h \
    ../include/XML/alarm_reset.h \
    ../include/XML/record_query.h \
    ../include/XML/Iframe_control.h \
    ../include/XML/configdownload_response.h \
    ../include/XML/broadcast_notify.h \
    ../include/XML/media_status_notify.h \
    ../include/XML/keepalive_response.h \
	../include/XML/guard_control.h \
	../include/XML/teleboot_control.h \
    ../include/SubscribeManager.h \
   ../include/waiter/WaiterManager.h \
    ../include/waiter/ResponseWaiter.h


SOURCES += ../../../CommonFile/Common/Layer2_Platform/$$PLATFORM/Lock.cpp \
        ../../../CommonFile/Common/Layer2_Platform/$$PLATFORM/Interlocked.cpp \
       ../../../CommonFile/Common/Layer2_Platform/$$PLATFORM/PFHelper.cpp \
    GB28181ClientSDK.cpp \
    GBClientImpl.cpp \
    ../include/SDP/SdpUtil.cpp \
    ../include/SDP/SdpMessage.cpp \
     ../include/StreamRequestManager.cpp \
    ../include/Rtsp/GBMANSRTSP.cpp \
    ../../../CommonFile/Common/Layer3_Abstract/TVTXMLParse.cpp \
    ../../../CommonFile/Common/Layer3_Abstract/tinyxml2.cpp \
    ../../../CommonFile/Common/Layer3_Abstract/SysHelper.cpp \
    ../../../CommonFile/Common/Layer3_Abstract/MarkupSTL.cpp \
    ../include/GBTime.cpp \
    ../include/XML/GB28181XmlParser.cpp \
    ../include/XML/slothxml.cpp \
    ../include/XML/catalog_query.cpp \
    ../include/XML/device_info.cpp \
    ../include/XML/ptz_control.cpp \
    ../include/XML/record_query.cpp \
    ../include/XML/zoom_control.cpp \
    ../include/XML/record_control.cpp \
    ../include/XML/Iframe_control.cpp \
    ../include/XML/alarm_sub.cpp \
    ../include/XML/alarm_reset.cpp \
    ../include/XML/response.cpp \
    ../include/XML/catalog_sub.cpp \
    ../include/XML/mobile_position_sub.cpp \
    ../include/XML/mobile_position_notify.cpp \
    ../include/XML/config_query.cpp \
    ../include/XML/record_index_response.cpp \
    ../include/XML/device_status_response.cpp \
    ../include/XML/home_position_control.cpp \
    ../include/XML/catalog_response.cpp \
    ../include/XML/alarm_notify.cpp \
    ../include/XML/base_proco.cpp \
    ../include/XML/preset_response.cpp \
    ../include/XML/configdownload_response.cpp \
    ../include/XML/broadcast_notify.cpp \
    ../include/XML/media_status_notify.cpp \
    ../include/XML/keepalive_response.cpp \
	../include/XML/guard_control.cpp \
	../include/XML/teleboot_control.cpp \
    ../include/SubscribeManager.cpp \
    ../include/waiter/WaiterManager.cpp \
    ../include/waiter/ResponseWaiter.cpp



LIBS += -L../../../CommonFile/CommonLib/third_lib$$SUFFIX/sip/osip/lib/ -losipparser2
win32:: PRE_TARGETDEPS += -L../../../CommonFile/CommonLib/third_lib$$SUFFIX/sip/osip/lib/osipparser2.lib
else:unix:!macx: PRE_TARGETDEPS += $$PWD/../../../CommonFile/CommonLib/third_lib$$SUFFIX/sip/osip/lib/libosipparser2.a

LIBS += -L../../../CommonFile/CommonLib/third_lib$$SUFFIX/sip/osip/lib/ -losip2
win32:: PRE_TARGETDEPS += -L../../../CommonFile/CommonLib/third_lib$$SUFFIX/sip/osip/lib/osip2.lib
else:unix:!macx: PRE_TARGETDEPS += $$PWD/../../../CommonFile/CommonLib/third_lib$$SUFFIX/sip/osip/lib/libosip2.a








