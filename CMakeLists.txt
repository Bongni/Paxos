# Dont build in source directories
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

# Version
cmake_minimum_required(VERSION 3.1...3.28)

# Project statement
project(
  Paxos
  VERSION 1.0
  LANGUAGES C
)

set(CMAKE_BUILD_TYPE Debug)

# Code
add_subdirectory(src)

# Tests
enable_testing()

add_subdirectory(tests)