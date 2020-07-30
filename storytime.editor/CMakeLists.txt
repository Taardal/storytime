cmake_minimum_required(VERSION 3.15)
project(editor)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} src/main.cpp src/EditorLayer.cpp src/EditorLayer.h)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(RES_DIR ${PROJECT_SOURCE_DIR}/res)

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})

set(STORYTIME_DIR ${CMAKE_SOURCE_DIR}/storytime)
target_include_directories(${PROJECT_NAME} PRIVATE ${STORYTIME_DIR}/include)
target_link_libraries(${PROJECT_NAME} storytime)

file(REMOVE_RECURSE ${PROJECT_BINARY_DIR}/res)
file(COPY ${RES_DIR} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${STORYTIME_DIR}/res DESTINATION ${PROJECT_BINARY_DIR})