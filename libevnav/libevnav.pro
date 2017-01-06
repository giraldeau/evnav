#-------------------------------------------------
#
# Project created by QtCreator 2016-05-10T22:22:13
#
#-------------------------------------------------

QT       += core
QT -= gui

TARGET = evnav
TEMPLATE = lib
CONFIG += sharedlib
CONFIG += TUFAO1

SOURCES += graph.cpp \
    edge.cpp \
    shortestpath.cpp \
    charger.cpp \
    chargerprovider.cpp \
    evnavserver.cpp \
    evnavrequest.cpp \
    evnav.cpp

HEADERS += graph.h \
    edge.h \
    shortestpath.h \
    charger.h \
    chargerprovider.h \
    evnavserver.h \
    evnavrequest.h \
    evnav.h

LIBS += -L$${top_builddir}/3rdparty/qhttp/xbin -l:libqhttp.a

include(../common.pri)

unix {
    target.path = $$PREFIX/lib
    INSTALLS += target
}
