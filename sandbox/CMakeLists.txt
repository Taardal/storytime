cmake_minimum_required(VERSION 3.15)
project(sandbox)

set(CMAKE_CXX_STANDARD 17)

add_executable(sandbox src/main.cpp src/SandboxLayer.cpp src/SandboxLayer.h)

set(SRC_DIR ${${CMAKE_CURRENT_SOURCE_DIR}}/src)
set(LIB_DIR ${${CMAKE_CURRENT_SOURCE_DIR}}/lib)
get_filename_component(SOLUTION_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})

target_link_libraries(${PROJECT_NAME} darkle)
target_include_directories(${PROJECT_NAME} PRIVATE ${SOLUTION_DIR}/darkle/include)