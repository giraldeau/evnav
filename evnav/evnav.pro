include(../common.pri)
QT += core
QT -= gui

TARGET = evnav
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    evnav.cpp \
    chargerprovider.cpp \
    charger.cpp

HEADERS += \
    evnav.h \
    chargerprovider.h \
    charger.h

