#-------------------------------------------------
#
# Project created by QtCreator 2016-05-10T22:22:13
#
#-------------------------------------------------

QT       -= core gui

TARGET = graph
TEMPLATE = lib
CONFIG += staticlib

SOURCES += graph.cpp

HEADERS += graph.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
