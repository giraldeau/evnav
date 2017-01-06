QT += core
QT -= gui

TARGET = evnav
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

HEADERS +=

include(../common.pri)
include(../libevnav.pri)

unix {
    target.path = $$PREFIX/bin
    INSTALLS += target
}
