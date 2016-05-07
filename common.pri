CONFIG   += c++11

# order of libraries is wrong if using pkg-config
unix {
    LIBS += -losrm -lrt -ltbb -lboost_date_time -lboost_filesystem -lboost_iostreams -lboost_program_options -lboost_regex -lboost_system -lboost_thread

    # FIXME: do not make this path hardcoded
    INCLUDEPATH += /usr/local/include/osrm
}

