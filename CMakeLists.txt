cmake_minimum_required(VERSION 3.30)
project(projects)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_executable(projects ${SOURCES} ${HEADERS})