cmake_minimum_required(VERSION 3.10)

project(server)

add_executable(server main.cpp icon.rc)

include_directories(include)

# Windows cần socket lib
if (WIN32)
    target_link_libraries(server ws2_32)
endif()