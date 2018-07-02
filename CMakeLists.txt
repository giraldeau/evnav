cmake_minimum_required(VERSION 3.10)

project(EvNav)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_AUTOMOC ON)

include(GNUInstallDirs)
find_package(Threads REQUIRED)
find_package(Qt5 COMPONENTS Core Network Test REQUIRED)
find_package(LibOSRM REQUIRED)
link_directories(${LibOSRM_LIBRARY_DIRS})

enable_testing()

add_subdirectory(3rdparty)
add_subdirectory(libevnav)
add_subdirectory(evnav-srv)
add_subdirectory(evnav-cli)
add_subdirectory(example)
add_subdirectory(tests)
