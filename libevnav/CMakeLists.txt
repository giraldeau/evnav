file(GLOB EVNAVCORE_SOURCES "*.cpp" "*.h")

add_library(evnavcore ${EVNAVCORE_SOURCES})
target_link_libraries(evnavcore PUBLIC Qt5::Core qhttp Threads::Threads ${LibOSRM_LIBRARIES} ${LibOSRM_DEPENDENT_LIBRARIES})
target_include_directories(evnavcore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${LibOSRM_INCLUDE_DIR} "${LibOSRM_INCLUDE_DIR}/osrm")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LibOSRM_CXXFLAGS}")
