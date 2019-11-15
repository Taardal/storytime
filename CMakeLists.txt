cmake_minimum_required(VERSION 3.15)
project(storytime)

add_subdirectory("${PROJECT_SOURCE_DIR}/storytime")
add_subdirectory("${PROJECT_SOURCE_DIR}/sandbox")

if (WIN32)
    message(STATUS "Platform is Windows")
    add_compile_definitions(ST_PLATFORM_WINDOWS)
endif ()

if (APPLE)
    message(STATUS "Platform is Apple")
    add_compile_definitions(ST_PLATFORM_APPLE)
endif ()