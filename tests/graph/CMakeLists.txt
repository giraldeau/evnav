add_executable(tst_graphtest tst_graphtest.cpp)
target_link_libraries(tst_graphtest evnavcore Qt5::Test)
#target_include_directories(tst_graphtest PRIVATE "${LibOSRM_INCLUDE_DIRS}")
target_compile_definitions(tst_graphtest PRIVATE TOPSRCDIR="${CMAKE_SOURCE_DIR}")
