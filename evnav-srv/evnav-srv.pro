QT += core
QT += network
QT -= gui

TARGET = evnav-srv
CONFIG += console
CONFIG -= app_bundle
CONFIG += TUFAO1

TEMPLATE = app

SOURCES += main.cpp

include(../common.pri)
include(../libevnav.pri)

LIBS += -L$${top_builddir}/3rdparty/qhttp/xbin -l:libqhttp.a
