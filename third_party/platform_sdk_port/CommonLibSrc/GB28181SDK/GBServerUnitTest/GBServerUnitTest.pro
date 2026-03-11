#-------------------------------------------------
#
# Project created by QtCreator 2019-01-30T16:17:19
#
#-------------------------------------------------

QT       += testlib
QT       -= gui
TARGET = tst_gbserverunittesttest
CONFIG   += console
CONFIG += console debug_and_release warn_on


TEMPLATE = app

CONFIG(debug, debug|release) {
    DESTDIR = ./Debug$$SUFFIX
    OBJECTS_DIR = ./Debug
    INCLUDEPATH += ./Debug
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

LIBS += -lGB28181ServerSDK

INCLUDEPATH += ../../../CommonFile/CommonLib
SOURCES += tst_gbserverunittesttest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"


