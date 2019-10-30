cmake_minimum_required(VERSION 3.15)
project(darkle)

add_subdirectory("${PROJECT_SOURCE_DIR}/darkle")
add_subdirectory("${PROJECT_SOURCE_DIR}/sandbox")

if (APPLE)
    message(STATUS "Platform is Apple")
    add_compile_definitions(PLATFORM_APPLE)
endif ()

if (WIN32)
    message(STATUS "Platform is Windows")
    add_compile_definitions(PLATFORM_WINDOWS)
endif ()