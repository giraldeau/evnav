#-------------------------------------------------
#
# Project created by QtCreator 2016-05-10T22:22:13
#
#-------------------------------------------------

QT       += core
QT -= gui

TARGET = evnav
TEMPLATE = lib
CONFIG += staticlib
CONFIG += TUFAO1

SOURCES += graph.cpp \
    edge.cpp \
    shortestpath.cpp \
    charger.cpp \
    chargerprovider.cpp \
    evnav.cpp \
    evnavserver.cpp

HEADERS += graph.h \
    edge.h \
    shortestpath.h \
    charger.h \
    chargerprovider.h \
    evnav.h \
    evnavserver.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}

include(../common.pri)
