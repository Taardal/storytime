cmake_minimum_required(VERSION 3.17)
project(storytime)

add_subdirectory(${PROJECT_SOURCE_DIR}/storytime)
#add_subdirectory(${PROJECT_SOURCE_DIR}/storytime.editor)
#add_subdirectory(${PROJECT_SOURCE_DIR}/storytiled)
#add_subdirectory(${PROJECT_SOURCE_DIR}/sandbox)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/storytime/include)