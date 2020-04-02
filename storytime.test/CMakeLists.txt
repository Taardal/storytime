cmake_minimum_required(VERSION 3.15)
project(storytime.test)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} src/main.cpp)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)

target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR})
install(TARGETS ${PROJECT_NAME} DESTINATION ${BIN_DIR})

set(STORYTIME_DIR ${CMAKE_SOURCE_DIR}/storytime)
target_include_directories(${PROJECT_NAME} PRIVATE ${STORYTIME_DIR}/include)
target_link_libraries(${PROJECT_NAME} storytime)

set(CATCH2_DIR ${LIB_DIR}/catch2-2.11.3)
target_include_directories(${PROJECT_NAME} PRIVATE ${CATCH2_DIR}/include)