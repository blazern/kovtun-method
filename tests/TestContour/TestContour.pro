#-------------------------------------------------
#
# Project created by QtCreator 2014-01-14T22:23:33
#
#-------------------------------------------------

include(../../common.pri)

QT       += testlib

QT       -= gui

TARGET = tst_TestContourTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    tst_TestContour.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

DESTDIR = $$OUT_PWD/../../main
OBJECTS_DIR = $$OUT_PWD/../../main
MOC_DIR = $$OUT_PWD/../../main
RCC_DIR = $$OUT_PWD/../../main
UI_DIR = $$OUT_PWD/../../main
