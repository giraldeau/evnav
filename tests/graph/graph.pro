#-------------------------------------------------
#
# Project created by QtCreator 2016-05-10T22:26:52
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_graphtest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += testcase

TEMPLATE = app

SOURCES += tst_graphtest.cpp

include(../../common.pri)
include(../../libevnav.pri)
