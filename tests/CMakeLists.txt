# Check needs threads library
set(CHECK check pthread subunit m)

#
#   Network Tests
#

add_executable(test_network network/test_network.c)
target_link_libraries(test_network ${CHECK_LIBRARIES} ${CHECK} lib_simulation)

add_test(NAME test_network COMMAND $<TARGET_FILE:test_network> WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(test_network PROPERTIES TIMEOUT 30) 



add_executable(test_queue network/test_queue.c)
target_link_libraries(test_queue ${CHECK_LIBRARIES} ${CHECK} lib_simulation)

add_test(NAME test_queue COMMAND $<TARGET_FILE:test_queue> WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(test_queue PROPERTIES TIMEOUT 30) 

