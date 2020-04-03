cmake_minimum_required(VERSION 3.15)
project(storytime.test)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} src/main.cpp)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)

target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR})

set(STORYTIME_DIR ${CMAKE_SOURCE_DIR}/storytime)
target_include_directories(${PROJECT_NAME} PRIVATE ${STORYTIME_DIR}/include)
target_link_libraries(${PROJECT_NAME} storytime)

set(GOOGLE_TEST_DIR ${LIB_DIR}/googletest-1.10.0)
add_subdirectory(${GOOGLE_TEST_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${GOOGLE_TEST_DIR}/googletest/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${GOOGLE_TEST_DIR}/googlemock/include)
target_link_libraries(${PROJECT_NAME} gtest gtest_main)
target_link_libraries(${PROJECT_NAME} gmock gmock_main)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME}>
        ${BIN_DIR}/${PROJECT_NAME}
)