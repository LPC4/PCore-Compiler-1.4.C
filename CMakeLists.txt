cmake_minimum_required(VERSION 3.30)
project(projects)

# Set the C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_SECURE_SCL=0 -D_CRT_SECURE_NO_WARNINGS")

# Collect source and header files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

# Create the executable
add_executable(projects ${SOURCES} ${HEADERS})

# Find LLVM package
#find_package(LLVM REQUIRED CONFIG)

# Output found LLVM version and directory
#message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
#message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Include LLVM directories and add definitions
#include_directories(${LLVM_INCLUDE_DIRS})
#add_definitions(${LLVM_DEFINITIONS})

# Find required LLVM components and map them to libraries
#llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link the target against the LLVM libraries
#target_link_libraries(projects ${llvm_libs})