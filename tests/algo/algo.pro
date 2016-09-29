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

QMAKE_CXXFLAGS += -I$${PWD}/../../3rdparty/RoutingKit/include
QMAKE_LFLAGS += -L$${PWD}/../../3rdparty/RoutingKit/lib

LIBS += -L$${PWD}/../../3rdparty/RoutingKit/lib -lroutingkit
