cmake_minimum_required(VERSION 3.15)

project(storytime)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ST_TARGET_NAME ${PROJECT_NAME})
set(ST_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(ST_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(ST_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(ST_CMAKE_DIR ${PROJECT_SOURCE_DIR}/cmake)

add_library(
        ${ST_TARGET_NAME}
        ${ST_SRC_DIR}/st_run.cpp
        ${ST_SRC_DIR}/st_run.h
        ${ST_SRC_DIR}/audio/audio.cpp
        ${ST_SRC_DIR}/audio/audio.h
        ${ST_SRC_DIR}/audio/audio_engine.cpp
        ${ST_SRC_DIR}/audio/audio_engine.h
        ${ST_SRC_DIR}/entity/entity.h
        ${ST_SRC_DIR}/entity/entity.cpp
        ${ST_SRC_DIR}/graphics/animation.h
        ${ST_SRC_DIR}/graphics/animation.cpp
        ${ST_SRC_DIR}/graphics/bitmap_font.h
        ${ST_SRC_DIR}/graphics/bitmap_font.cpp
        ${ST_SRC_DIR}/graphics/camera.h
        ${ST_SRC_DIR}/graphics/camera.cpp
        ${ST_SRC_DIR}/graphics/framebuffer.h
        ${ST_SRC_DIR}/graphics/framebuffer.cpp
        ${ST_SRC_DIR}/graphics/imgui_renderer.h
        ${ST_SRC_DIR}/graphics/imgui_renderer.cpp
        ${ST_SRC_DIR}/graphics/imgui_window_event.h
        ${ST_SRC_DIR}/graphics/imgui_window_event.cpp
        ${ST_SRC_DIR}/graphics/index_buffer.h
        ${ST_SRC_DIR}/graphics/index_buffer.cpp
        ${ST_SRC_DIR}/graphics/open_gl.h
        ${ST_SRC_DIR}/graphics/open_gl.cpp
        ${ST_SRC_DIR}/graphics/renderer.h
        ${ST_SRC_DIR}/graphics/renderer.cpp
        ${ST_SRC_DIR}/graphics/shader.h
        ${ST_SRC_DIR}/graphics/shader.cpp
        ${ST_SRC_DIR}/graphics/sprite.h
        ${ST_SRC_DIR}/graphics/sprite.cpp
        ${ST_SRC_DIR}/graphics/spritesheet.h
        ${ST_SRC_DIR}/graphics/spritesheet.cpp
        ${ST_SRC_DIR}/graphics/texture.h
        ${ST_SRC_DIR}/graphics/texture.cpp
        ${ST_SRC_DIR}/graphics/texture_coordinate.h
        ${ST_SRC_DIR}/graphics/texture_coordinate.cpp
        ${ST_SRC_DIR}/graphics/vertex_array.h
        ${ST_SRC_DIR}/graphics/vertex_array.cpp
        ${ST_SRC_DIR}/graphics/vertex_attribute.h
        ${ST_SRC_DIR}/graphics/vertex_attribute.cpp
        ${ST_SRC_DIR}/graphics/vertex_buffer.h
        ${ST_SRC_DIR}/graphics/vertex_buffer.cpp
        ${ST_SRC_DIR}/graphics/view_projection.h
        ${ST_SRC_DIR}/graphics/view_projection.cpp
        ${ST_SRC_DIR}/resource/resource_loader.h
        ${ST_SRC_DIR}/resource/resource_loader.cpp
        ${ST_SRC_DIR}/lua/lua_error.h
        ${ST_SRC_DIR}/lua/lua_error.cpp
        ${ST_SRC_DIR}/lua/lua_function.h
        ${ST_SRC_DIR}/lua/lua_function.cpp
        ${ST_SRC_DIR}/lua/lua_glm.cpp
        ${ST_SRC_DIR}/lua/lua_glm.h
        ${ST_SRC_DIR}/lua/lua_keyboard.cpp
        ${ST_SRC_DIR}/lua/lua_keyboard.h
        ${ST_SRC_DIR}/lua/lua_log.h
        ${ST_SRC_DIR}/lua/lua_log.cpp
        ${ST_SRC_DIR}/lua/lua_mouse.cpp
        ${ST_SRC_DIR}/lua/lua_mouse.h
        ${ST_SRC_DIR}/lua/lua_ref.h
        ${ST_SRC_DIR}/lua/lua_ref.cpp
        ${ST_SRC_DIR}/lua/lua_to_from.h
        ${ST_SRC_DIR}/lua/lua_state.h
        ${ST_SRC_DIR}/lua/lua_state.cpp
        ${ST_SRC_DIR}/lua/lua_state.h
        ${ST_SRC_DIR}/lua/lua_state.cpp
        ${ST_SRC_DIR}/lua/lua_table.h
        ${ST_SRC_DIR}/lua/lua_table.cpp
        ${ST_SRC_DIR}/lua/lua_usertype.h
        ${ST_SRC_DIR}/lua/lua_usertype.cpp
        ${ST_SRC_DIR}/lua/lua_utils.h
        ${ST_SRC_DIR}/lua/lua_utils.cpp
        ${ST_SRC_DIR}/system/assert.h
        ${ST_SRC_DIR}/system/binary_to_from.h
        ${ST_SRC_DIR}/system/clock.h
        ${ST_SRC_DIR}/system/clock.cpp
        ${ST_SRC_DIR}/system/command_line_arguments.cpp
        ${ST_SRC_DIR}/system/command_line_arguments.h
        ${ST_SRC_DIR}/system/defer.h
        ${ST_SRC_DIR}/system/environment.h
        ${ST_SRC_DIR}/system/error.h
        ${ST_SRC_DIR}/system/error.cpp
        ${ST_SRC_DIR}/system/error_signal.h
        ${ST_SRC_DIR}/system/error_signal.cpp
        ${ST_SRC_DIR}/system/event.h
        ${ST_SRC_DIR}/system/event.cpp
        ${ST_SRC_DIR}/system/event_manager.h
        ${ST_SRC_DIR}/system/event_manager.cpp
        ${ST_SRC_DIR}/system/file_reader.h
        ${ST_SRC_DIR}/system/file_reader.cpp
        ${ST_SRC_DIR}/system/game_loop_metrics.h
        ${ST_SRC_DIR}/system/json_to_from.h
        ${ST_SRC_DIR}/system/log.h
        ${ST_SRC_DIR}/system/log.cpp
        ${ST_SRC_DIR}/system/memory.h
        ${ST_SRC_DIR}/system/memory.cpp
        ${ST_SRC_DIR}/system/memory_allocator.h
        ${ST_SRC_DIR}/system/memory_allocator.cpp
        ${ST_SRC_DIR}/system/msgpack_filesystem_adaptor.h
        ${ST_SRC_DIR}/system/msgpack_glm_adaptor.h
        ${ST_SRC_DIR}/system/msgpack_utils.cpp
        ${ST_SRC_DIR}/system/msgpack_utils.h
        ${ST_SRC_DIR}/system/pointers.h
        ${ST_SRC_DIR}/system/random.h
        ${ST_SRC_DIR}/system/random.cpp
        ${ST_SRC_DIR}/system/size.h
        ${ST_SRC_DIR}/system/size.cpp
        ${ST_SRC_DIR}/system/utils.h
        ${ST_SRC_DIR}/system/utils.cpp
        ${ST_SRC_DIR}/system/uuid.h
        ${ST_SRC_DIR}/system/uuid.cpp
        ${ST_SRC_DIR}/system/variant.h
        ${ST_SRC_DIR}/tiled/tiled_class.cpp
        ${ST_SRC_DIR}/tiled/tiled_class.h
        ${ST_SRC_DIR}/tiled/tiled_enum.cpp
        ${ST_SRC_DIR}/tiled/tiled_enum.h
        ${ST_SRC_DIR}/tiled/tiled_layer.cpp
        ${ST_SRC_DIR}/tiled/tiled_layer.h
        ${ST_SRC_DIR}/tiled/tiled_map.cpp
        ${ST_SRC_DIR}/tiled/tiled_map.h
        ${ST_SRC_DIR}/tiled/tiled_object.cpp
        ${ST_SRC_DIR}/tiled/tiled_object.h
        ${ST_SRC_DIR}/tiled/tiled_project.cpp
        ${ST_SRC_DIR}/tiled/tiled_project.h
        ${ST_SRC_DIR}/tiled/tiled_property.cpp
        ${ST_SRC_DIR}/tiled/tiled_property.h
        ${ST_SRC_DIR}/tiled/tiled_template.cpp
        ${ST_SRC_DIR}/tiled/tiled_template.h
        ${ST_SRC_DIR}/tiled/tiled_tile.cpp
        ${ST_SRC_DIR}/tiled/tiled_tile.h
        ${ST_SRC_DIR}/tiled/tiled_tileset.cpp
        ${ST_SRC_DIR}/tiled/tiled_tileset.h
        ${ST_SRC_DIR}/window/key.cpp
        ${ST_SRC_DIR}/window/key.h
        ${ST_SRC_DIR}/window/key_event.h
        ${ST_SRC_DIR}/window/key_event.cpp
        ${ST_SRC_DIR}/window/mouse_button.h
        ${ST_SRC_DIR}/window/mouse_button.cpp
        ${ST_SRC_DIR}/window/mouse_event.h
        ${ST_SRC_DIR}/window/mouse_event.cpp
        ${ST_SRC_DIR}/window/user_input.h
        ${ST_SRC_DIR}/window/user_input.cpp
        ${ST_SRC_DIR}/window/window.h
        ${ST_SRC_DIR}/window/window.cpp
        ${ST_SRC_DIR}/window/window_event.h
        ${ST_SRC_DIR}/window/window_event.cpp
)

set_target_properties(
        ${ST_TARGET_NAME}
        PROPERTIES
        PREFIX ""
        LIBRARY_OUTPUT_NAME ${ST_TARGET_NAME}
        LIBRARY_OUTPUT_DIRECTORY ${ST_BIN_DIR}
        LIBRARY_OUTPUT_DIRECTORY_DEBUG ${ST_BIN_DIR}/debug
        LIBRARY_OUTPUT_DIRECTORY_RELEASE ${ST_BIN_DIR}/release
)

# Public include header
target_include_directories(${ST_TARGET_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Precompiled header
target_include_directories(${ST_TARGET_NAME} PUBLIC ${ST_SRC_DIR})
target_precompile_headers(${ST_TARGET_NAME} PUBLIC ${ST_SRC_DIR}/st_pch.h)

# --------------------------------------------------------------------------------------------------------------
# Custom targets
# --------------------------------------------------------------------------------------------------------------

if ("${ST_OUTPUT_DIR}" STREQUAL "")
    if ("${CMAKE_BUILD_TYPE}" STREQUAL Debug)
        set(ST_OUTPUT_DIR ${ST_BIN_DIR}/debug)
    else()
        set(ST_OUTPUT_DIR ${ST_BIN_DIR}/release)
    endif()
endif ()
set(ST_RESOURCES_OUTPUT_DIR ${ST_OUTPUT_DIR}/res)
set(ST_RESOURCES_SOURCE_DIR ${PROJECT_SOURCE_DIR}/res)
add_custom_target(
        ST_CopyResources
        COMMENT "Copying resources"
        COMMAND ${CMAKE_COMMAND} -D RESOURCES_SOURCE_DIR=${ST_RESOURCES_SOURCE_DIR} -D RESOURCES_OUTPUT_DIR=${ST_RESOURCES_OUTPUT_DIR} -P ${ST_CMAKE_DIR}/copy_resources.cmake
)
add_dependencies(${ST_TARGET_NAME} ST_CopyResources)

# --------------------------------------------------------------------------------------------------------------
# Dependencies
# --------------------------------------------------------------------------------------------------------------

set(FETCHCONTENT_QUIET ON)
include(FetchContent)

# EnTT
FetchContent_Declare(
        entt
        GIT_REPOSITORY https://github.com/skypjack/entt
        GIT_TAG v3.13.1
)
FetchContent_MakeAvailable(entt)
target_link_libraries(${ST_TARGET_NAME} EnTT::EnTT)

# GLM
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)
target_link_libraries(${ST_TARGET_NAME} glm::glm)

# GLAD
set(GLAD_NAME glad)
set(GLAD_DIR ${ST_LIB_DIR}/glad-0.1.33)
add_library(${GLAD_NAME} ${GLAD_DIR}/src/glad.c)
target_include_directories(${GLAD_NAME} PRIVATE ${GLAD_DIR}/include)
target_include_directories(${ST_TARGET_NAME} PUBLIC ${GLAD_DIR}/include)
target_link_libraries(${ST_TARGET_NAME} glad ${CMAKE_DL_LIBS}) # Link with platform-specific dynamic loading libraries

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
target_link_libraries(${ST_TARGET_NAME} glfw)

# ImGui
set(IMGUI_NAME imgui)
set(IMGUI_DIR ${ST_LIB_DIR}/imgui-docking-1.77)
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
target_include_directories(${ST_TARGET_NAME} PUBLIC ${IMGUI_DIR})
target_link_libraries(${ST_TARGET_NAME} ${IMGUI_NAME})

# JSON
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)
target_link_libraries(${ST_TARGET_NAME} nlohmann_json::nlohmann_json)

# Lua
find_package(Lua REQUIRED)
target_include_directories(${ST_TARGET_NAME} PUBLIC ${LUA_INCLUDE_DIR})
target_link_libraries(${ST_TARGET_NAME} ${LUA_LIBRARIES})

# Message Pack
set(MSGPACK_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(MSGPACK_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(MSGPACK_CXX_STANDARD 17 CACHE STRING "" FORCE)
set(MSGPACK_USE_BOOST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
        msgpack
        GIT_REPOSITORY https://github.com/msgpack/msgpack-c.git
        GIT_TAG cpp-4.1.2
)
FetchContent_MakeAvailable(msgpack)
target_link_libraries(${ST_TARGET_NAME} msgpackc-cxx)

# Miniaudio
FetchContent_Declare(
        miniaudio
        GIT_REPOSITORY https://github.com/mackron/miniaudio.git
        GIT_TAG 0.11.21
)
FetchContent_MakeAvailable(miniaudio)
target_include_directories(${ST_TARGET_NAME} PUBLIC ${miniaudio_SOURCE_DIR})

# STB image
set(STB_IMAGE_NAME stb_image)
set(STB_IMAGE_DIR ${ST_LIB_DIR}/stb_image-2.22)
add_library(
        ${STB_IMAGE_NAME}
        ${STB_IMAGE_DIR}/src/stb_image.cpp
)
target_include_directories(${STB_IMAGE_NAME} PRIVATE ${STB_IMAGE_DIR}/include)
target_include_directories(${ST_TARGET_NAME} PUBLIC ${STB_IMAGE_DIR}/include)
target_link_libraries(${ST_TARGET_NAME} ${STB_IMAGE_NAME} ${CMAKE_DL_LIBS}) # Link with platform-specific dynamic loading libraries

# SPDLOG
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.9.2
)
FetchContent_MakeAvailable(spdlog)
target_link_libraries(${ST_TARGET_NAME} spdlog::spdlog)