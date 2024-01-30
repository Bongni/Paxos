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