QT += core
QT -= gui

TARGET = evnav-srv
CONFIG += console
CONFIG -= app_bundle
CONFIG += TUFAO1

TEMPLATE = app

SOURCES += main.cpp

include(../common.pri)
include(../libevnav.pri)
