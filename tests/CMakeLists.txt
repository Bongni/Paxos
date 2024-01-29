#find_package(Check REQUIRED)
#
#include(CheckCSourceCompiles)
#include(CheckCSourceRuns)
#include(CheckFunctionExists)
#include(CheckIncludeFile)
#include(CheckIncludeFiles)
#include(CheckLibraryExists)
#include(CheckSymbolExists)
#include(CheckTypeSize)

# Check uses threads
add_executable(test_network test_network.c)
set(CHECK check pthread subunit m)
target_link_libraries(test_network ${CHECK_LIBRARIES} ${CHECK} network)

# Create testing target and redirect its output to `Testing` folder
add_test(NAME test_network COMMAND $<TARGET_FILE:test_network> WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(test_network PROPERTIES TIMEOUT 30) 

