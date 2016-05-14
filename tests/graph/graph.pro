#-------------------------------------------------
#
# Project created by QtCreator 2016-05-10T22:26:52
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_graphtest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += testcase

TEMPLATE = app

SOURCES += tst_graphtest.cpp

unix:!macx: LIBS += -L$$OUT_PWD/../../libevnav/ -levnav

INCLUDEPATH += $$PWD/../../libevnav
DEPENDPATH += $$PWD/../../libevnav

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../libevnav/libevnav.a

include(../../common.pri)
