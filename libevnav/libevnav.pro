#-------------------------------------------------
#
# Project created by QtCreator 2016-05-10T22:22:13
#
#-------------------------------------------------

QT       += core
QT -= gui

# Unkown module in QT: positioning
# QT += positioning

TARGET = evnav
TEMPLATE = lib
CONFIG += staticlib
CONFIG += TUFAO1

SOURCES += graph.cpp \
    edge.cpp \
    shortestpath.cpp \
    charger.cpp \
    chargerprovider.cpp \
    evnavserver.cpp \
    evnavrequest.cpp \
    evnav.cpp \
    elevation.cpp \
    elevationtile.cpp

HEADERS += graph.h \
    edge.h \
    shortestpath.h \
    charger.h \
    chargerprovider.h \
    evnavserver.h \
    evnavrequest.h \
    evnav.h \
    elevation.h \
    elevationtile.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}

include(../common.pri)
