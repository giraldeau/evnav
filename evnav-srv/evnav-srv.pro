include(../common.pri)

QT += core
QT -= gui

TARGET = evnav-srv
CONFIG += console
CONFIG -= app_bundle
CONFIG += TUFAO1

TEMPLATE = app

SOURCES += main.cpp

HEADERS +=


unix:!macx: LIBS += -L$$OUT_PWD/../libevnav/ -levnav

INCLUDEPATH += $$PWD/../libevnav
DEPENDPATH += $$PWD/../libevnav

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../libevnav/libevnav.a
