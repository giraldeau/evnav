add_executable(evnav-srv main.cpp)
target_link_libraries(evnav-srv evnavcore)
install(TARGETS evnav-srv DESTINATION bin)
