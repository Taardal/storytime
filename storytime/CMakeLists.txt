cmake_minimum_required(VERSION 3.15)
project(storytime)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} src/system/Log.cpp src/system/Log.h src/Engine.cpp src/Engine.h src/system/Application.cpp src/system/Application.h src/window/Window.cpp src/window/Window.h src/graphics/GraphicsContext.cpp src/graphics/GraphicsContext.h src/window/events/Event.cpp src/window/events/Event.h src/window/events/WindowEvent.cpp src/window/events/WindowEvent.h src/window/events/KeyEvent.cpp src/window/events/KeyEvent.h src/window/events/MouseEvent.cpp src/window/events/MouseEvent.h src/graphics/Renderer.cpp src/graphics/Renderer.h src/graphics/VertexArray.cpp src/graphics/VertexArray.h src/graphics/VertexBuffer.cpp src/graphics/VertexBuffer.h src/graphics/VertexAttribute.cpp src/graphics/VertexAttribute.h src/graphics/Shader.cpp src/graphics/Shader.h src/system/Core.h src/window/LayerStack.cpp src/window/LayerStack.h src/window/Layer.cpp src/window/Layer.h src/system/Timestep.cpp src/system/Timestep.h src/graphics/OrthographicCameraController.cpp src/graphics/OrthographicCameraController.h src/graphics/OrthographicCamera.cpp src/graphics/OrthographicCamera.h src/window/Input.cpp src/window/Input.h src/system/FileReader.cpp src/system/FileReader.h src/graphics/IndexBuffer.cpp src/graphics/IndexBuffer.h)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)

target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR})

set(SPDLOG_DIR ${LIB_DIR}/spdlog-1.4.2)
target_include_directories(${PROJECT_NAME} PUBLIC ${SPDLOG_DIR}/include)

set(GLAD_DIR ${LIB_DIR}/glad-0.1.33)
add_library(glad ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PRIVATE ${GLAD_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${GLAD_DIR}/include)
target_link_libraries(${PROJECT_NAME} glad ${CMAKE_DL_LIBS})

set(GLFW_DIR ${LIB_DIR}/glfw-3.3)
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL Build the GLFW example programs)
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL Build the GLFW test programs)
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL Build the GLFW documentation)
set(GLFW_INSTALL OFF CACHE INTERNAL Generate installation target)
add_subdirectory(${GLFW_DIR})
target_compile_definitions(${PROJECT_NAME} PUBLIC GLFW_INCLUDE_NONE)
target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW_DIR}/include)
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})

set(GLM_DIR ${LIB_DIR}/glm-0.9.9.6)
target_include_directories(${PROJECT_NAME} PUBLIC ${GLM_DIR})

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME}>
        ${BIN_DIR}/${PROJECT_NAME}
)