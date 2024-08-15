cmake_minimum_required(VERSION 3.15)

project(storytime)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(STORYTIME_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(STORYTIME_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(STORYTIME_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(STORYTIME_CMAKE_DIR ${PROJECT_SOURCE_DIR}/cmake)

add_library(
        ${PROJECT_NAME}
        ${STORYTIME_SRC_DIR}/storytime_app.cpp
        ${STORYTIME_SRC_DIR}/storytime_app.h
        ${STORYTIME_SRC_DIR}/storytime_pch.h
        ${STORYTIME_SRC_DIR}/storytime_main.cpp
        ${STORYTIME_SRC_DIR}/storytime_main.h
        ${STORYTIME_SRC_DIR}/graphics/camera.cpp
        ${STORYTIME_SRC_DIR}/graphics/camera.h
        ${STORYTIME_SRC_DIR}/graphics/Framebuffer.h
        ${STORYTIME_SRC_DIR}/graphics/Framebuffer.cpp
        ${STORYTIME_SRC_DIR}/graphics/GraphicsContext.h
        ${STORYTIME_SRC_DIR}/graphics/GraphicsContext.cpp
        ${STORYTIME_SRC_DIR}/graphics/GraphicsLog.h
        ${STORYTIME_SRC_DIR}/graphics/GraphicsLog.cpp
        ${STORYTIME_SRC_DIR}/graphics/graphics_module.h
        ${STORYTIME_SRC_DIR}/graphics/graphics_module.cpp
        ${STORYTIME_SRC_DIR}/graphics/OrthographicCamera.h
        ${STORYTIME_SRC_DIR}/graphics/OrthographicCamera.cpp
        ${STORYTIME_SRC_DIR}/graphics/IndexBuffer.h
        ${STORYTIME_SRC_DIR}/graphics/IndexBuffer.cpp
        ${STORYTIME_SRC_DIR}/graphics/ImGuiRenderer.h
        ${STORYTIME_SRC_DIR}/graphics/ImGuiRenderer.cpp
        ${STORYTIME_SRC_DIR}/graphics/OrthographicCameraController.h
        ${STORYTIME_SRC_DIR}/graphics/OrthographicCameraController.cpp
        ${STORYTIME_SRC_DIR}/graphics/Renderer.h
        ${STORYTIME_SRC_DIR}/graphics/Renderer.cpp
        ${STORYTIME_SRC_DIR}/graphics/Shader.h
        ${STORYTIME_SRC_DIR}/graphics/Shader.cpp
        ${STORYTIME_SRC_DIR}/graphics/Texture.h
        ${STORYTIME_SRC_DIR}/graphics/Texture.cpp
        ${STORYTIME_SRC_DIR}/graphics/VertexArray.h
        ${STORYTIME_SRC_DIR}/graphics/VertexArray.cpp
        ${STORYTIME_SRC_DIR}/graphics/VertexAttribute.h
        ${STORYTIME_SRC_DIR}/graphics/VertexAttribute.cpp
        ${STORYTIME_SRC_DIR}/graphics/VertexBuffer.h
        ${STORYTIME_SRC_DIR}/graphics/VertexBuffer.cpp
        ${STORYTIME_SRC_DIR}/system/assert.h
        ${STORYTIME_SRC_DIR}/system/clock.h
        ${STORYTIME_SRC_DIR}/system/clock.cpp
        ${STORYTIME_SRC_DIR}/system/environment.h
        ${STORYTIME_SRC_DIR}/system/error.h
        ${STORYTIME_SRC_DIR}/system/error.cpp
        ${STORYTIME_SRC_DIR}/system/error_signal.h
        ${STORYTIME_SRC_DIR}/system/error_signal.cpp
        ${STORYTIME_SRC_DIR}/system/file_system.h
        ${STORYTIME_SRC_DIR}/system/file_system.cpp
        ${STORYTIME_SRC_DIR}/system/log.h
        ${STORYTIME_SRC_DIR}/system/log.cpp
        ${STORYTIME_SRC_DIR}/system/pointers.h
        ${STORYTIME_SRC_DIR}/system/random.h
        ${STORYTIME_SRC_DIR}/system/random.cpp
        ${STORYTIME_SRC_DIR}/system/resource_loader.h
        ${STORYTIME_SRC_DIR}/system/resource_loader.cpp
        ${STORYTIME_SRC_DIR}/system/system_module.h
        ${STORYTIME_SRC_DIR}/system/system_module.cpp
        ${STORYTIME_SRC_DIR}/system/utils.h

        ${STORYTIME_SRC_DIR}/window/event.h
        ${STORYTIME_SRC_DIR}/window/event.cpp
        ${STORYTIME_SRC_DIR}/window/event_manager.h
        ${STORYTIME_SRC_DIR}/window/event_manager.cpp
        ${STORYTIME_SRC_DIR}/window/Input.h
        ${STORYTIME_SRC_DIR}/window/Input.cpp
        ${STORYTIME_SRC_DIR}/window/key_event.h
        ${STORYTIME_SRC_DIR}/window/key_event.cpp
        ${STORYTIME_SRC_DIR}/window/mouse_event.h
        ${STORYTIME_SRC_DIR}/window/mouse_event.cpp
        ${STORYTIME_SRC_DIR}/window/window.h
        ${STORYTIME_SRC_DIR}/window/window.cpp
        ${STORYTIME_SRC_DIR}/window/window_event.h
        ${STORYTIME_SRC_DIR}/window/window_event.cpp
        ${STORYTIME_SRC_DIR}/window/window_module.h
        ${STORYTIME_SRC_DIR}/window/window_module.cpp
)

# Precompiled header
target_include_directories(${PROJECT_NAME} PUBLIC ${STORYTIME_SRC_DIR})
target_precompile_headers(${PROJECT_NAME} PUBLIC ${STORYTIME_SRC_DIR}/storytime_pch.h)

# --------------------------------------------------------------------------------------------------------------
# Custom targets
# --------------------------------------------------------------------------------------------------------------

if (NOT "${STORYTIME_OUTPUT_DIR}" STREQUAL "")
    set(STORYTIME_RESOURCES_OUTPUT_DIR ${STORYTIME_OUTPUT_DIR}/res)
    set(STORYTIME_RESOURCES_SOURCE_DIR ${PROJECT_SOURCE_DIR}/res)
    add_custom_target(
            CopyResources
            COMMAND ${CMAKE_COMMAND} -D RESOURCES_SOURCE_DIR=${STORYTIME_RESOURCES_SOURCE_DIR} -D RESOURCES_OUTPUT_DIR=${STORYTIME_RESOURCES_OUTPUT_DIR} -P ${STORYTIME_CMAKE_DIR}/copy_resources.cmake
            COMMENT "Copying resources"
    )
    add_dependencies(
            ${PROJECT_NAME}
            CopyResources
    )
endif()

# --------------------------------------------------------------------------------------------------------------
# Dependencies
# --------------------------------------------------------------------------------------------------------------

include(FetchContent)

# GLM
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)
target_link_libraries(${PROJECT_NAME} glm::glm)

# GLAD
set(GLAD_NAME glad)
set(GLAD_DIR ${STORYTIME_LIB_DIR}/glad-0.1.33)
add_library(${GLAD_NAME} ${GLAD_DIR}/src/glad.c)
target_include_directories(${GLAD_NAME} PRIVATE ${GLAD_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${GLAD_DIR}/include)
target_link_libraries(${PROJECT_NAME} glad ${CMAKE_DL_LIBS}) # Link with platform-specific dynamic loading libraries

# GLFW
set(GLFW_INSTALL OFF CACHE INTERNAL Generate installation target)
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL Build the GLFW documentation)
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL Build the GLFW test programs)
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL Build the GLFW example programs)
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)
target_link_libraries(${PROJECT_NAME} glfw)

# IMGUI
set(IMGUI_NAME imgui)
set(IMGUI_DIR ${STORYTIME_LIB_DIR}/imgui-docking-1.77)
add_library(
        ${IMGUI_NAME}
        ${IMGUI_DIR}/imconfig.h
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui.h
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_internal.h
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imstb_rectpack.h
        ${IMGUI_DIR}/imstb_textedit.h
        ${IMGUI_DIR}/imstb_truetype.h
        ${IMGUI_DIR}/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/imgui_impl_glfw.h
        ${IMGUI_DIR}/imgui_impl_opengl3.cpp
        ${IMGUI_DIR}/imgui_impl_opengl3.h
)
target_include_directories(${IMGUI_NAME} PRIVATE ${GLAD_DIR}/include)
target_link_libraries(${IMGUI_NAME} glfw)
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_DIR})
target_link_libraries(${PROJECT_NAME} ${IMGUI_NAME})

# STB IMAGE
set(STB_IMAGE_NAME stb_image)
set(STB_IMAGE_DIR ${STORYTIME_LIB_DIR}/stb_image-2.22)
add_library(
        ${STB_IMAGE_NAME}
        ${STB_IMAGE_DIR}/src/stb_image.cpp
)
target_include_directories(${STB_IMAGE_NAME} PRIVATE ${STB_IMAGE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${STB_IMAGE_DIR}/include)
target_link_libraries(${PROJECT_NAME} ${STB_IMAGE_NAME} ${CMAKE_DL_LIBS}) # Link with platform-specific dynamic loading libraries

# SPDLOG
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.9.2
)
FetchContent_MakeAvailable(spdlog)
target_link_libraries(${PROJECT_NAME} spdlog::spdlog)