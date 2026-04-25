cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_COMPILER "D:/0.BANG/winlib/lib/mingw64/bin/g++.exe")
set(CMAKE_C_COMPILER   "D:/0.BANG/winlib/lib/mingw64/bin/gcc.exe")

project(server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES main.cpp)

if (WIN32)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/icon.rc")
        list(APPEND SOURCES icon.rc)
    endif()
endif()

add_executable(server ${SOURCES})
target_include_directories(server PRIVATE include .)

if (WIN32)
    target_link_libraries(server ws2_32 stdc++fs)
    target_link_options(server PRIVATE "-static")
else()
    target_link_libraries(server stdc++fs pthread)
endif()