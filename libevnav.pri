# common definitions for libevnav users

unix:!macx: LIBS += -L$${top_builddir}/libevnav/ -levnav

INCLUDEPATH += $${top_srcdir}/libevnav
DEPENDPATH += $${top_srcdir}/libevnav

INCLUDEPATH += $${top_srcdir}/3rdparty/qhttp/src/
DEPENDPATH += $${top_srcdir}/3rdparty/qhttp/src/
