#-------------------------------------------------
#
# Project created by QtCreator 2016-05-10T22:22:13
#
#-------------------------------------------------

include(../common.pri)

QT       += core
QT -= gui

TARGET = graph
TEMPLATE = lib
CONFIG += staticlib

SOURCES += graph.cpp \
    edge.cpp \
    shortestpath.cpp

HEADERS += graph.h \
    edge.h \
    shortestpath.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
