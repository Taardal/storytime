cmake_minimum_required(VERSION 3.15)
project(darkle)

set(CMAKE_CXX_STANDARD 17)

add_library(darkle src/Log.cpp src/Log.h src/Engine.cpp src/Engine.h src/Application.cpp src/Application.h)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
get_filename_component(SOLUTION_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${LIB_DIR}/spdlog-1.4.2/include)