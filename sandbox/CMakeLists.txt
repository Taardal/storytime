cmake_minimum_required(VERSION 3.15)
project(sandbox)

set(CMAKE_CXX_STANDARD 17)

add_executable(sandbox src/main.cpp src/TriangleLayer.cpp src/TriangleLayer.h src/Sandbox.cpp src/Sandbox.h)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})

set(STORYTIME_DIR ${CMAKE_SOURCE_DIR}/storytime)
target_include_directories(${PROJECT_NAME} PRIVATE ${STORYTIME_DIR}/include)
target_link_libraries(${PROJECT_NAME} storytime)
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${STORYTIME_DIR}/assets
        ${PROJECT_BINARY_DIR}/assets
)

set(STORYTILED_DIR ${CMAKE_SOURCE_DIR}/storytiled)
target_link_libraries(${PROJECT_NAME} storytiled)
target_include_directories(${PROJECT_NAME} PRIVATE ${STORYTILED_DIR}/include)