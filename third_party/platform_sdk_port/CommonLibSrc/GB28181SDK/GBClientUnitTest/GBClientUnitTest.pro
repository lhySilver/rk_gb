#-------------------------------------------------
#
# Project created by QtCreator 2019-01-30T16:15:45
#
#-------------------------------------------------

QT       += testlib
QT       -= gui
TARGET = tst_gbclientunittesttest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app



INCLUDEPATH += ../../../CommonFile/CommonLib
INCLUDEPATH += ../../../CommonFile/Common/Layer2_Platform/$$PLATFORM


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

LIBS +=  -lGB28181ClientSDK

SOURCES += tst_gbclientunittesttest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

