#-------------------------------------------------
#
# Project created by QtCreator 2016-05-28T22:53:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = altview
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

unix:!macx: LIBS += -L$$OUT_PWD/../libevnav/ -levnav

INCLUDEPATH += $$PWD/../libevnav
DEPENDPATH += $$PWD/../libevnav

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../libevnav/libevnav.a
