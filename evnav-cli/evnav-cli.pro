QT += core
QT -= gui

TARGET = evnav
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

HEADERS +=

unix {
    target.path = /usr/bin
    INSTALLS += target
}

include(../common.pri)
include(../libevnav.pri)

