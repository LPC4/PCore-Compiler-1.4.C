cmake_minimum_required(VERSION 3.20.0)
project(compiler)

# Set the C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Include LLVM and Clang directories
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
add_definitions(${CLANG_DEFINITIONS})

# Collect source and header files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

# Create the executable
add_executable(compiler ${SOURCES} ${HEADERS})

# Map the LLVM components to their library names
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link against LLVM and Clang libraries
target_link_libraries(compiler ${llvm_libs} ${CLANG_LIBRARIES})

# Optional: Print the LLVM config flags for debugging
message(STATUS "LLVM Libraries: ${llvm_libs}")
message(STATUS "LLVM Definitions: ${LLVM_DEFINITIONS_LIST}")