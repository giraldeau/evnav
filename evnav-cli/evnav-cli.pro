QT += core
QT -= gui

TARGET = evnav
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

HEADERS +=

unix:!macx: LIBS += -L$$OUT_PWD/../libevnav/ -levnav

INCLUDEPATH += $$PWD/../libevnav
DEPENDPATH += $$PWD/../libevnav

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../libevnav/libevnav.a

unix {
    target.path = /usr/bin
    INSTALLS += target
}

include(../common.pri)


