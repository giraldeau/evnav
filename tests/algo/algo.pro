#-------------------------------------------------
#
# Project created by QtCreator 2016-09-29T19:27:16
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_algotest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += tst_algotest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
DEFINES += TOPSRCDIR=\\\"\"$${top_srcdir}\"\\\"

QMAKE_CXXFLAGS += -I$${PWD}/../../3rdparty/RoutingKit/include
QMAKE_LFLAGS += -L$${PWD}/../../3rdparty/RoutingKit/lib

LIBS += -L$${PWD}/../../3rdparty/RoutingKit/lib -lroutingkit

unix:!macx: LIBS += -L$$OUT_PWD/../../libevnav/ -levnav

INCLUDEPATH += $$PWD/../../libevnav
DEPENDPATH += $$PWD/../../libevnav

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../libevnav/libevnav.a

include(../../common.pri)
