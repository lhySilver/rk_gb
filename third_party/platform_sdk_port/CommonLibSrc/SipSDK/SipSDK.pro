TEMPLATE = lib
CONFIG += debug_and_release warn_on plugin
QT   -= gui
CONFIG -= qt
TARGET = SipSDK

include(../../PLATFORM_SUFFIX.pri)
include(../../common.pri)

CONFIG(debug, debug|release) {
    DESTDIR = ../../CommonFile/CommonLib/Debug$$SUFFIX
    OBJECTS_DIR = ./Debug
    UI_DIR = ./Debug
    RCC_DIR = ./Debug
    MOC_DIR = ./Debug
    LIBS += -L../../CommonFile/CommonLib/Debug$$SUFFIX
} else {
    DESTDIR = ../../CommonFile/CommonLib/Release$$SUFFIX
    OBJECTS_DIR = ./Release
    UI_DIR = ./Release
    RCC_DIR = ./Release
    MOC_DIR = ./Release
    LIBS += -L../../CommonFile/CommonLib/Release$$SUFFIX
}

win32: {
   DEFINES += SIP_EXPORTS
   LIBS += -lWS2_32  -lDnsapi   -liphlpapi
   LIBS += -L../../CommonFile/CommonLib/third_lib$$SUFFIX/sip/eXosip/lib/  -leXosip
}

unix: {
     LIBS += -lresolv
     LIBS += -L../../CommonFile/CommonLib/third_lib$$SUFFIX/sip/eXosip/lib/  -leXosip2
}
unix:!mac: {
    QMAKE_CC += -g
    QMAKE_CXX += -g
    QMAKE_LINK += -g
}


INCLUDEPATH += ../../CommonFile/CommonLib
INCLUDEPATH += ../../CommonFile/Common/Layer1_Common  ../../CommonFile/Common/Layer3_Abstract
INCLUDEPATH += ../../CommonFile/Common/Layer2_Platform/$$PLATFORM
INCLUDEPATH += ../../CommonFile/CommonLib/third_lib/sip/osip/include
INCLUDEPATH += ../../CommonFile/CommonLib/third_lib/sip/eXosip/include
INCLUDEPATH += ../../CommonFile/CommonLib/third_lib/sip/osip/include/osipparser2/headers

HEADERS += \
    common/SipStackImpl.h \
    common/SipEventManager.h \
    common/SipUtil.h \
    common/WaiterManager.h \
    client/SipClientImpl.h \
    server/SipServerImpl.h \
    ../../CommonFile/Common/Layer2_Platform/$$PLATFORM/Lock.h \
    ../../CommonFile/Common/Layer2_Platform/$$PLATFORM/Interlocked.h \
   ../../CommonFile/Common/Layer2_Platform/$$PLATFORM/PFHelper.h \
    common/ResponseWaiter.h \
    common/ClientInfoManager.h \
    ../../CommonFile/Common/Layer3_Abstract/ThreadPool.h

SOURCES += ../../CommonFile/Common/Layer2_Platform/$$PLATFORM/Lock.cpp \
        ../../CommonFile/Common/Layer2_Platform/$$PLATFORM/Interlocked.cpp \
       ../../CommonFile/Common/Layer2_Platform/$$PLATFORM/PFHelper.cpp \
    SipStackSDK.cpp \
    common/SipStackImpl.cpp \
    common/SipEventManager.cpp \
    common/SipUtil.cpp \
    common/WaiterManager.cpp \
    client/SipClientImpl.cpp \
    server/SipServerImpl.cpp \
    common/ResponseWaiter.cpp \
    common/ClientInfoManager.cpp \
    ../../CommonFile/Common/Layer3_Abstract/ThreadPool.cpp

LIBS += -lShareLib -luuid

LIBS += -L../../CommonFile/CommonLib/third_lib$$SUFFIX/sip/eXosip/lib/  -leXosip2
win32:PRE_TARGETDEPS += -L../../CommonFile/CommonLib/third_lib$$SUFFIX/sip/eXosip/lib/eXosip.lib
else:unix:!macx: PRE_TARGETDEPS += $$PWD/../../CommonFile/CommonLib/third_lib$$SUFFIX/sip/eXosip/lib/libeXosip2.a

LIBS += -L../../CommonFile/CommonLib/third_lib$$SUFFIX/sip/osip/lib/ -losipparser2
win32:: PRE_TARGETDEPS += -L../../CommonFile/CommonLib/third_lib$$SUFFIX/sip/osip/lib/osipparser2.lib
else:unix:!macx: PRE_TARGETDEPS += $$PWD/../../CommonFile/CommonLib/third_lib$$SUFFIX/sip/osip/lib/libosipparser2.a

LIBS += -L../../CommonFile/CommonLib/third_lib$$SUFFIX/sip/osip/lib/ -losip2
win32:: PRE_TARGETDEPS += -L../../CommonFile/CommonLib/third_lib$$SUFFIX/sip/osip/lib/osip2.lib
else:unix:!macx: PRE_TARGETDEPS += $$PWD/../../CommonFile/CommonLib/third_lib$$SUFFIX/sip/osip/lib/libosip2.a



