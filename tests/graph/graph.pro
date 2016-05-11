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

TEMPLATE = app


SOURCES += tst_graphtest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../graph/release/ -lgraph
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../graph/debug/ -lgraph
else:unix: LIBS += -L$$OUT_PWD/../../graph/ -lgraph

INCLUDEPATH += $$PWD/../../graph
DEPENDPATH += $$PWD/../../graph

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../graph/release/libgraph.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../graph/debug/libgraph.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../graph/release/graph.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../graph/debug/graph.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../graph/libgraph.a
