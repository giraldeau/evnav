#-------------------------------------------------
#
# Project created by QtCreator 2016-05-10T22:26:52
#
#-------------------------------------------------

include(../../common.pri)

QT       += testlib

QT       -= gui

TARGET = tst_graphtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_graphtest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libevnav/release/ -levnav
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libevnav/debug/ -levnav
else:unix: LIBS += -L$$OUT_PWD/../../libevnav/ -levnav

INCLUDEPATH += $$PWD/../../libevnav
DEPENDPATH += $$PWD/../../libevnav

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../libevnav/release/libevnav.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../libevnav/debug/libevnav.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../libevnav/release/evnav.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../libevnav/debug/evnav.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../libevnav/libevnav.a
