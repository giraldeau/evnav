file(GLOB EVNAVCORE_SOURCES "*.cpp" "*.h")

add_library(evnavcore ${EVNAVCORE_SOURCES})
target_link_libraries(evnavcore PUBLIC Qt5::Core qhttp Threads::Threads osrm)
target_include_directories(evnavcore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/3rdparty/osrm-backend/include
    ${CMAKE_SOURCE_DIR}/3rdparty/osrm-backend/third_party/variant/include
)
