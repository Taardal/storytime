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
        SHARED
        ${STORYTIME_SRC_DIR}/storytime_pch.h
        ${STORYTIME_SRC_DIR}/storytime_main.cpp
        ${STORYTIME_SRC_DIR}/storytime_main.h
        ${STORYTIME_SRC_DIR}/audio/audio.cpp
        ${STORYTIME_SRC_DIR}/audio/audio.h
        ${STORYTIME_SRC_DIR}/audio/audio_engine.cpp
        ${STORYTIME_SRC_DIR}/audio/audio_engine.h
        ${STORYTIME_SRC_DIR}/graphics/bitmap_font.h
        ${STORYTIME_SRC_DIR}/graphics/bitmap_font.cpp
        ${STORYTIME_SRC_DIR}/graphics/camera.h
        ${STORYTIME_SRC_DIR}/graphics/camera.cpp
        ${STORYTIME_SRC_DIR}/graphics/framebuffer.h
        ${STORYTIME_SRC_DIR}/graphics/framebuffer.cpp
        ${STORYTIME_SRC_DIR}/graphics/imgui_renderer.h
        ${STORYTIME_SRC_DIR}/graphics/imgui_renderer.cpp
        ${STORYTIME_SRC_DIR}/graphics/index_buffer.h
        ${STORYTIME_SRC_DIR}/graphics/index_buffer.cpp
        ${STORYTIME_SRC_DIR}/graphics/open_gl.h
        ${STORYTIME_SRC_DIR}/graphics/open_gl.cpp
        ${STORYTIME_SRC_DIR}/graphics/renderer.h
        ${STORYTIME_SRC_DIR}/graphics/renderer.cpp
        ${STORYTIME_SRC_DIR}/graphics/shader.h
        ${STORYTIME_SRC_DIR}/graphics/shader.cpp
        ${STORYTIME_SRC_DIR}/graphics/sprite.h
        ${STORYTIME_SRC_DIR}/graphics/sprite.cpp
        ${STORYTIME_SRC_DIR}/graphics/spritesheet.h
        ${STORYTIME_SRC_DIR}/graphics/spritesheet.cpp
        ${STORYTIME_SRC_DIR}/graphics/texture.h
        ${STORYTIME_SRC_DIR}/graphics/texture.cpp
        ${STORYTIME_SRC_DIR}/graphics/texture_coordinate.h
        ${STORYTIME_SRC_DIR}/graphics/texture_coordinate.cpp
        ${STORYTIME_SRC_DIR}/graphics/vertex_array.h
        ${STORYTIME_SRC_DIR}/graphics/vertex_array.cpp
        ${STORYTIME_SRC_DIR}/graphics/vertex_attribute.h
        ${STORYTIME_SRC_DIR}/graphics/vertex_attribute.cpp
        ${STORYTIME_SRC_DIR}/graphics/vertex_buffer.h
        ${STORYTIME_SRC_DIR}/graphics/vertex_buffer.cpp
        ${STORYTIME_SRC_DIR}/graphics/view_projection.h
        ${STORYTIME_SRC_DIR}/graphics/view_projection.cpp
        ${STORYTIME_SRC_DIR}/resource/resource_loader.h
        ${STORYTIME_SRC_DIR}/resource/resource_loader.cpp
        ${STORYTIME_SRC_DIR}/scene/entity.h
        ${STORYTIME_SRC_DIR}/scene/entity.cpp
        ${STORYTIME_SRC_DIR}/scene/scene.h
        ${STORYTIME_SRC_DIR}/scene/scene.cpp
        ${STORYTIME_SRC_DIR}/script/lua_error.h
        ${STORYTIME_SRC_DIR}/script/lua_error.cpp
        ${STORYTIME_SRC_DIR}/script/lua_function.h
        ${STORYTIME_SRC_DIR}/script/lua_function.cpp
        ${STORYTIME_SRC_DIR}/script/lua_glm.cpp
        ${STORYTIME_SRC_DIR}/script/lua_glm.h
        ${STORYTIME_SRC_DIR}/script/lua_keyboard.cpp
        ${STORYTIME_SRC_DIR}/script/lua_keyboard.h
        ${STORYTIME_SRC_DIR}/script/lua_log.h
        ${STORYTIME_SRC_DIR}/script/lua_log.cpp
        ${STORYTIME_SRC_DIR}/script/lua_mouse.cpp
        ${STORYTIME_SRC_DIR}/script/lua_mouse.h
        ${STORYTIME_SRC_DIR}/script/lua_ref.h
        ${STORYTIME_SRC_DIR}/script/lua_ref.cpp
        ${STORYTIME_SRC_DIR}/script/lua_state.h
        ${STORYTIME_SRC_DIR}/script/lua_state.cpp
        ${STORYTIME_SRC_DIR}/script/lua_state.h
        ${STORYTIME_SRC_DIR}/script/lua_state.cpp
        ${STORYTIME_SRC_DIR}/script/lua_table.h
        ${STORYTIME_SRC_DIR}/script/lua_table.cpp
        ${STORYTIME_SRC_DIR}/script/lua_usertype.h
        ${STORYTIME_SRC_DIR}/script/lua_usertype.cpp
        ${STORYTIME_SRC_DIR}/script/lua_utils.h
        ${STORYTIME_SRC_DIR}/script/lua_utils.cpp
        ${STORYTIME_SRC_DIR}/system/assert.h
        ${STORYTIME_SRC_DIR}/system/clock.h
        ${STORYTIME_SRC_DIR}/system/clock.cpp
        ${STORYTIME_SRC_DIR}/system/command_line_arguments.h
        ${STORYTIME_SRC_DIR}/system/defer.h
        ${STORYTIME_SRC_DIR}/system/environment.h
        ${STORYTIME_SRC_DIR}/system/error.h
        ${STORYTIME_SRC_DIR}/system/error.cpp
        ${STORYTIME_SRC_DIR}/system/error_signal.h
        ${STORYTIME_SRC_DIR}/system/error_signal.cpp
        ${STORYTIME_SRC_DIR}/system/event.h
        ${STORYTIME_SRC_DIR}/system/event.cpp
        ${STORYTIME_SRC_DIR}/system/event_manager.h
        ${STORYTIME_SRC_DIR}/system/event_manager.cpp
        ${STORYTIME_SRC_DIR}/system/file_reader.h
        ${STORYTIME_SRC_DIR}/system/file_reader.cpp
        ${STORYTIME_SRC_DIR}/system/json_serialize_deserialize.h
        ${STORYTIME_SRC_DIR}/system/log.h
        ${STORYTIME_SRC_DIR}/system/log.cpp
        ${STORYTIME_SRC_DIR}/system/memory.h
        ${STORYTIME_SRC_DIR}/system/memory.cpp
        ${STORYTIME_SRC_DIR}/system/memory_allocator.h
        ${STORYTIME_SRC_DIR}/system/memory_allocator.cpp
        ${STORYTIME_SRC_DIR}/system/pointers.h
        ${STORYTIME_SRC_DIR}/system/random.h
        ${STORYTIME_SRC_DIR}/system/random.cpp
        ${STORYTIME_SRC_DIR}/system/uuid.h
        ${STORYTIME_SRC_DIR}/system/uuid.cpp
        ${STORYTIME_SRC_DIR}/system/utils.h
        ${STORYTIME_SRC_DIR}/system/utils.cpp
        ${STORYTIME_SRC_DIR}/tiled/tiled_class.cpp
        ${STORYTIME_SRC_DIR}/tiled/tiled_class.h
        ${STORYTIME_SRC_DIR}/tiled/tiled_enum.cpp
        ${STORYTIME_SRC_DIR}/tiled/tiled_enum.h
        ${STORYTIME_SRC_DIR}/tiled/tiled_layer.cpp
        ${STORYTIME_SRC_DIR}/tiled/tiled_layer.h
        ${STORYTIME_SRC_DIR}/tiled/tiled_map.cpp
        ${STORYTIME_SRC_DIR}/tiled/tiled_map.h
        ${STORYTIME_SRC_DIR}/tiled/tiled_object.cpp
        ${STORYTIME_SRC_DIR}/tiled/tiled_object.h
        ${STORYTIME_SRC_DIR}/tiled/tiled_project.cpp
        ${STORYTIME_SRC_DIR}/tiled/tiled_project.h
        ${STORYTIME_SRC_DIR}/tiled/tiled_property.cpp
        ${STORYTIME_SRC_DIR}/tiled/tiled_property.h
        ${STORYTIME_SRC_DIR}/tiled/tiled_template.cpp
        ${STORYTIME_SRC_DIR}/tiled/tiled_template.h
        ${STORYTIME_SRC_DIR}/tiled/tiled_tile.cpp
        ${STORYTIME_SRC_DIR}/tiled/tiled_tile.h
        ${STORYTIME_SRC_DIR}/tiled/tiled_tileset.cpp
        ${STORYTIME_SRC_DIR}/tiled/tiled_tileset.h
        ${STORYTIME_SRC_DIR}/window/key.cpp
        ${STORYTIME_SRC_DIR}/window/key.h
        ${STORYTIME_SRC_DIR}/window/key_event.h
        ${STORYTIME_SRC_DIR}/window/key_event.cpp
        ${STORYTIME_SRC_DIR}/window/mouse_button.h
        ${STORYTIME_SRC_DIR}/window/mouse_button.cpp
        ${STORYTIME_SRC_DIR}/window/mouse_event.h
        ${STORYTIME_SRC_DIR}/window/mouse_event.cpp
        ${STORYTIME_SRC_DIR}/window/user_input.h
        ${STORYTIME_SRC_DIR}/window/user_input.cpp
        ${STORYTIME_SRC_DIR}/window/window.h
        ${STORYTIME_SRC_DIR}/window/window.cpp
        ${STORYTIME_SRC_DIR}/window/window_event.h
        ${STORYTIME_SRC_DIR}/window/window_event.cpp
)

set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        PREFIX ""
        LIBRARY_OUTPUT_NAME ${PROJECT_NAME}
        LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR}
        LIBRARY_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR}/debug
        LIBRARY_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}/release
)

# Public include header
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Precompiled header
target_include_directories(${PROJECT_NAME} PUBLIC ${STORYTIME_SRC_DIR})
target_precompile_headers(${PROJECT_NAME} PUBLIC ${STORYTIME_SRC_DIR}/storytime_pch.h)

# On macOS, exported symbols from other binaries aren’t available to shared libraries during linking,
# so instruct the linker to defer the resolution of symbols until runtime.
if (APPLE)
    target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-undefined,dynamic_lookup")
endif ()

# --------------------------------------------------------------------------------------------------------------
# Custom targets
# --------------------------------------------------------------------------------------------------------------

if ("${STORYTIME_OUTPUT_DIR}" STREQUAL "")
    message(FATAL_ERROR "Required variable STORYTIME_OUTPUT_DIR is not set")
endif ()
set(STORYTIME_RESOURCES_OUTPUT_DIR ${STORYTIME_OUTPUT_DIR}/res)
set(STORYTIME_RESOURCES_SOURCE_DIR ${PROJECT_SOURCE_DIR}/res)
add_custom_target(
        Storytime_CopyResources
        COMMENT "Copying resources"
        COMMAND ${CMAKE_COMMAND} -D RESOURCES_SOURCE_DIR=${STORYTIME_RESOURCES_SOURCE_DIR} -D RESOURCES_OUTPUT_DIR=${STORYTIME_RESOURCES_OUTPUT_DIR} -P ${STORYTIME_CMAKE_DIR}/copy_resources.cmake
)
add_dependencies(${PROJECT_NAME} Storytime_CopyResources)

# --------------------------------------------------------------------------------------------------------------
# Dependencies
# --------------------------------------------------------------------------------------------------------------

include(FetchContent)

# EnTT
FetchContent_Declare(
        entt
        GIT_REPOSITORY https://github.com/skypjack/entt
        GIT_TAG v3.13.1
)
FetchContent_MakeAvailable(entt)
target_link_libraries(${PROJECT_NAME} EnTT::EnTT)

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

# ImGui
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

# JSON
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)

# Lua
find_package(Lua REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${LUA_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${LUA_LIBRARIES})

# Miniaudio
FetchContent_Declare(
        miniaudio
        GIT_REPOSITORY https://github.com/mackron/miniaudio.git
        GIT_TAG 0.11.21
)
FetchContent_MakeAvailable(miniaudio)
target_include_directories(${PROJECT_NAME} PUBLIC ${miniaudio_SOURCE_DIR})

# STB image
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