add_executable(evnav-cli main.cpp)
target_link_libraries(evnav-cli evnavcore)
install(TARGETS evnav-cli DESTINATION bin)
