cmake_minimum_required(VERSION 3.22)

project(ringbuffer C)

add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/ringbuffer.c
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

add_executable(${PROJECT_NAME}-demo)
target_sources(${PROJECT_NAME}-demo PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/ringbuffer_demo.c
)
target_link_libraries(${PROJECT_NAME}-demo ${PROJECT_NAME})

if (UNITTEST)
    include(CTest)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/test)
endif()
