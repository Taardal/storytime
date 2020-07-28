cmake_minimum_required(VERSION 3.15)
project(storytiled)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} src/World.cpp src/World.h src/Tileset.cpp src/Tileset.h src/Layer.cpp src/Layer.h src/Object.cpp src/Object.h src/Property.cpp src/Property.h)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)

target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR})

set(JSON_DIR ${LIB_DIR}/nlohmann/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${JSON_DIR})

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME}>
        ${BIN_DIR}/${PROJECT_NAME}
)