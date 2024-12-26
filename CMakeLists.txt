cmake_minimum_required(VERSION 3.30)
project(projects)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h")

add_executable(projects ${SOURCES} ${HEADERS}
        src/main.cpp
        include/Errors.h
        src/Parser.cpp
        include/Parser.h)