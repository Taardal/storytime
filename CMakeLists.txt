cmake_minimum_required(VERSION 3.15)
project(storytime)

add_subdirectory(${PROJECT_SOURCE_DIR}/storytime)
add_subdirectory(${PROJECT_SOURCE_DIR}/storytime.test)
if (CMAKE_BUILD_TYPE EQUAL DEBUG)
    add_subdirectory(${PROJECT_SOURCE_DIR}/sandbox)
    add_subdirectory(${PROJECT_SOURCE_DIR}/storytiled)
endif ()
