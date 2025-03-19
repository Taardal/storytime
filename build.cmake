cmake_minimum_required(VERSION 3.10)

set(GENERATE FALSE)
set(RELEASE FALSE)
set(EXE_NAME "")

foreach(i RANGE 1 ${CMAKE_ARGC})
    set(arg "${CMAKE_ARGV${i}}")
    if(arg STREQUAL "help")
        message(STATUS "USAGE: cmake -P build.cmake [options]")
        message(STATUS "OPTIONS:")
        message(STATUS "  generate  Generate new build files")
        message(STATUS "  release   Use Release as the build type")
        return()
    endif()
    if(arg STREQUAL "generate")
        set(GENERATE TRUE)
    endif()
    if(arg STREQUAL "release")
        set(RELEASE TRUE)
    endif()
endforeach()

if(RELEASE)
    set(BUILD_TYPE "Release")
else()
    set(BUILD_TYPE "Debug")
endif()
string(TOLOWER "${BUILD_TYPE}" BUILD_TYPE_DIR_NAME)
message(STATUS "Using build type: ${BUILD_TYPE}")

set(SOURCE_DIRECTORY "${CMAKE_SOURCE_DIR}")
message(STATUS "Using CMake source directory: ${SOURCE_DIRECTORY}")

set(BUILD_DIRECTORY "${CMAKE_BINARY_DIR}/build/${BUILD_TYPE_DIR_NAME}")
message(STATUS "Using CMake build directory: ${BUILD_DIRECTORY}")

set(BIN_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${BUILD_TYPE_DIR_NAME}")
message(STATUS "Using CMake output directory: ${BIN_DIRECTORY}")

set(GENERATOR "")
if(NOT IS_DIRECTORY "${BUILD_DIRECTORY}")
    set(GENERATE TRUE)
    find_program(NINJA_EXECUTABLE ninja)
    if(NINJA_EXECUTABLE)
        set(GENERATOR "-G Ninja")
    endif()
endif()

if(GENERATE)
    message(STATUS "Generating build files...")
    execute_process(COMMAND cmake -D CMAKE_EXPORT_COMPILE_COMMANDS=1 -D CMAKE_BUILD_TYPE=${BUILD_TYPE} -S ${SOURCE_DIRECTORY} -B ${BUILD_DIRECTORY} ${GENERATOR} RESULT_VARIABLE GEN_RESULT)
    if(NOT GEN_RESULT EQUAL 0)
        message(FATAL_ERROR "CMake generation failed")
    endif ()
endif ()

message(STATUS "Building binary...")
execute_process(COMMAND cmake --build ${BUILD_DIRECTORY} --config ${BUILD_TYPE} RESULT_VARIABLE BUILD_RESULT)
if (NOT BUILD_RESULT EQUAL 0)
    message(FATAL_ERROR "Build failed")
endif ()

