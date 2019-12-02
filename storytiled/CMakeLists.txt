cmake_minimum_required(VERSION 3.15)
project(storytiled)

set(CMAKE_CXX_STANDARD 17)

add_library(storytiled src/World.cpp src/World.h)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})

set(JSON_DIR ${LIB_DIR}/nlohmann/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${JSON_DIR})