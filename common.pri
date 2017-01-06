CONFIG   += c++14

# order of libraries is wrong if using pkg-config
unix {
    LIBS += -losrm -lrt -ltbb -lboost_date_time -lboost_filesystem -lboost_iostreams -lboost_program_options -lboost_regex -lboost_system -lboost_thread

    # FIXME: do not make this path hardcoded
    INCLUDEPATH += /usr/local/include/osrm

    # RPATH is set here for convenience to run the programs in the shadow build
    # without installing libevnav.so or setting LD_LIBRARY_PATH.
    CONFIG(debug) {
        QMAKE_RPATHDIR += $${top_builddir}/libevnav/
    }

    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }

}

DEFINES += TOPSRCDIR=\\\"\"$${top_srcdir}\"\\\"
