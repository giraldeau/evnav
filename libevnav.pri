unix:!macx: PRE_TARGETDEPS += $${top_builddir}/libevnav/libevnav.so

unix:!macx: LIBS += -L$${top_builddir}/libevnav/ -levnav

INCLUDEPATH += $${top_srcdir}/libevnav
DEPENDPATH += $${top_srcdir}/libevnav
