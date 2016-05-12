include(../common.pri)
QT += core
QT -= gui

TARGET = evnav
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    evnav.cpp \
    chargerprovider.cpp \
    charger.cpp

HEADERS += \
    evnav.h \
    chargerprovider.h \
    charger.h


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../graph/release/ -lgraph
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../graph/debug/ -lgraph
else:unix: LIBS += -L$$OUT_PWD/../graph/ -lgraph

INCLUDEPATH += $$PWD/../graph
DEPENDPATH += $$PWD/../graph

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../graph/release/libgraph.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../graph/debug/libgraph.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../graph/release/graph.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../graph/debug/graph.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../graph/libgraph.a
