cmake_minimum_required(VERSION 3.17)
project(storytime)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${PROJECT_SOURCE_DIR}/storytime)
#add_subdirectory(${PROJECT_SOURCE_DIR}/storytiled)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/storytime/include)
