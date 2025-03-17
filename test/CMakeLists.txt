add_executable(test_ringbuffer ${CMAKE_CURRENT_LIST_DIR}/test_ringbuffer.c)

target_link_libraries(test_ringbuffer cmocka ringbuffer)
add_test(NAME test_ringbuffer COMMAND test_ringbuffer)
