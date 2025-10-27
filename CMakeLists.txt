cmake_minimum_required(VERSION 4.0.0)

project(storytime)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ST_TARGET ${PROJECT_NAME})
set(ST_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(ST_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(ST_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(ST_CMAKE_DIR ${PROJECT_SOURCE_DIR}/cmake)
set(ST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif ()

if ("${ST_OUTPUT_DIR}" STREQUAL "")
    if ("${CMAKE_BUILD_TYPE}" STREQUAL Debug)
        set(ST_OUTPUT_DIR ${ST_BIN_DIR}/debug)
    else ()
        set(ST_OUTPUT_DIR ${ST_BIN_DIR}/release)
    endif ()
endif ()

add_library(
    ${ST_TARGET}
    ${ST_SRC_DIR}/st_app.h
    ${ST_SRC_DIR}/st_config.h
    ${ST_SRC_DIR}/st_engine.cpp
    ${ST_SRC_DIR}/st_engine.h
    ${ST_SRC_DIR}/st_main.cpp
    ${ST_SRC_DIR}/st_main.h
    ${ST_SRC_DIR}/audio/st_audio.cpp
    ${ST_SRC_DIR}/audio/st_audio.h
    ${ST_SRC_DIR}/audio/st_audio_engine.cpp
    ${ST_SRC_DIR}/audio/st_audio_engine.h
    ${ST_SRC_DIR}/event/st_event.cpp
    ${ST_SRC_DIR}/event/st_event.h
    ${ST_SRC_DIR}/event/st_key_pressed_event.cpp
    ${ST_SRC_DIR}/event/st_key_pressed_event.h
    ${ST_SRC_DIR}/event/st_key_released_event.cpp
    ${ST_SRC_DIR}/event/st_key_released_event.h
    ${ST_SRC_DIR}/event/st_key_repeated_event.cpp
    ${ST_SRC_DIR}/event/st_key_repeated_event.h
    ${ST_SRC_DIR}/event/st_key_typed_event.cpp
    ${ST_SRC_DIR}/event/st_key_typed_event.h
    ${ST_SRC_DIR}/event/st_mouse_button_pressed_event.cpp
    ${ST_SRC_DIR}/event/st_mouse_button_pressed_event.h
    ${ST_SRC_DIR}/event/st_mouse_button_released_event.cpp
    ${ST_SRC_DIR}/event/st_mouse_button_released_event.h
    ${ST_SRC_DIR}/event/st_mouse_moved_event.cpp
    ${ST_SRC_DIR}/event/st_mouse_moved_event.h
    ${ST_SRC_DIR}/event/st_mouse_scroll_event.cpp
    ${ST_SRC_DIR}/event/st_mouse_scroll_event.h
    ${ST_SRC_DIR}/event/st_window_closed_event.cpp
    ${ST_SRC_DIR}/event/st_window_closed_event.h
    ${ST_SRC_DIR}/event/st_window_focused_event.cpp
    ${ST_SRC_DIR}/event/st_window_focused_event.h
    ${ST_SRC_DIR}/event/st_window_minimized_event.cpp
    ${ST_SRC_DIR}/event/st_window_minimized_event.h
    ${ST_SRC_DIR}/event/st_window_resized_event.cpp
    ${ST_SRC_DIR}/event/st_window_resized_event.h
    ${ST_SRC_DIR}/graphics/st_animation.cpp
    ${ST_SRC_DIR}/graphics/st_animation.h
    ${ST_SRC_DIR}/graphics/st_bitmap_font.cpp
    ${ST_SRC_DIR}/graphics/st_bitmap_font.h
    ${ST_SRC_DIR}/graphics/st_camera.cpp
    ${ST_SRC_DIR}/graphics/st_camera.h
    ${ST_SRC_DIR}/graphics/st_frame.cpp
    ${ST_SRC_DIR}/graphics/st_frame.h
    ${ST_SRC_DIR}/graphics/st_imgui_renderer.cpp
    ${ST_SRC_DIR}/graphics/st_imgui_renderer.h
    ${ST_SRC_DIR}/graphics/st_quad.h
    ${ST_SRC_DIR}/graphics/st_quad_vertex.h
    ${ST_SRC_DIR}/graphics/st_renderer.cpp
    ${ST_SRC_DIR}/graphics/st_renderer.h
    ${ST_SRC_DIR}/graphics/st_sprite.cpp
    ${ST_SRC_DIR}/graphics/st_sprite.h
    ${ST_SRC_DIR}/graphics/st_spritesheet.cpp
    ${ST_SRC_DIR}/graphics/st_spritesheet.h
    ${ST_SRC_DIR}/graphics/st_texture.cpp
    ${ST_SRC_DIR}/graphics/st_texture.h
    ${ST_SRC_DIR}/graphics/st_view_projection.cpp
    ${ST_SRC_DIR}/graphics/st_view_projection.h
    ${ST_SRC_DIR}/graphics/st_vulkan_buffer.cpp
    ${ST_SRC_DIR}/graphics/st_vulkan_buffer.h
    ${ST_SRC_DIR}/graphics/st_vulkan_command_buffer.cpp
    ${ST_SRC_DIR}/graphics/st_vulkan_command_buffer.h
    ${ST_SRC_DIR}/graphics/st_vulkan_command_callbacks.h
    ${ST_SRC_DIR}/graphics/st_vulkan_command_pool.cpp
    ${ST_SRC_DIR}/graphics/st_vulkan_command_pool.h
    ${ST_SRC_DIR}/graphics/st_vulkan_context.cpp
    ${ST_SRC_DIR}/graphics/st_vulkan_context.h
    ${ST_SRC_DIR}/graphics/st_vulkan_descriptor_pool.cpp
    ${ST_SRC_DIR}/graphics/st_vulkan_descriptor_pool.h
    ${ST_SRC_DIR}/graphics/st_vulkan_descriptor_set.cpp
    ${ST_SRC_DIR}/graphics/st_vulkan_descriptor_set.h
    ${ST_SRC_DIR}/graphics/st_vulkan_device.cpp
    ${ST_SRC_DIR}/graphics/st_vulkan_device.h
    ${ST_SRC_DIR}/graphics/st_vulkan_graphics_pipeline.cpp
    ${ST_SRC_DIR}/graphics/st_vulkan_graphics_pipeline.h
    ${ST_SRC_DIR}/graphics/st_vulkan_image.cpp
    ${ST_SRC_DIR}/graphics/st_vulkan_image.h
    ${ST_SRC_DIR}/graphics/st_vulkan_index_buffer.cpp
    ${ST_SRC_DIR}/graphics/st_vulkan_index_buffer.h
    ${ST_SRC_DIR}/graphics/st_vulkan_physical_device.cpp
    ${ST_SRC_DIR}/graphics/st_vulkan_physical_device.h
    ${ST_SRC_DIR}/graphics/st_vulkan_queue.cpp
    ${ST_SRC_DIR}/graphics/st_vulkan_queue.h
    ${ST_SRC_DIR}/graphics/st_vulkan_swapchain.cpp
    ${ST_SRC_DIR}/graphics/st_vulkan_swapchain.h
    ${ST_SRC_DIR}/graphics/st_vulkan_uniform_buffer.cpp
    ${ST_SRC_DIR}/graphics/st_vulkan_uniform_buffer.h
    ${ST_SRC_DIR}/graphics/st_vulkan_utils.cpp
    ${ST_SRC_DIR}/graphics/st_vulkan_utils.h
    ${ST_SRC_DIR}/graphics/st_vulkan_vertex_buffer.cpp
    ${ST_SRC_DIR}/graphics/st_vulkan_vertex_buffer.h
    ${ST_SRC_DIR}/lua/st_lua_dispatcher_binding.cpp
    ${ST_SRC_DIR}/lua/st_lua_dispatcher_binding.h
    ${ST_SRC_DIR}/lua/st_lua_error.cpp
    ${ST_SRC_DIR}/lua/st_lua_error.h
    ${ST_SRC_DIR}/lua/st_lua_function.cpp
    ${ST_SRC_DIR}/lua/st_lua_function.h
    ${ST_SRC_DIR}/lua/st_lua_glm_binding.cpp
    ${ST_SRC_DIR}/lua/st_lua_glm_binding.h
    ${ST_SRC_DIR}/lua/st_lua_keyboard_binding.cpp
    ${ST_SRC_DIR}/lua/st_lua_keyboard_binding.h
    ${ST_SRC_DIR}/lua/st_lua_log_binding.cpp
    ${ST_SRC_DIR}/lua/st_lua_log_binding.h
    ${ST_SRC_DIR}/lua/st_lua_mouse_binding.cpp
    ${ST_SRC_DIR}/lua/st_lua_mouse_binding.h
    ${ST_SRC_DIR}/lua/st_lua_process_binding.cpp
    ${ST_SRC_DIR}/lua/st_lua_process_binding.h
    ${ST_SRC_DIR}/lua/st_lua_process_manager_binding.cpp
    ${ST_SRC_DIR}/lua/st_lua_process_manager_binding.h
    ${ST_SRC_DIR}/lua/st_lua_ref.cpp
    ${ST_SRC_DIR}/lua/st_lua_ref.h
    ${ST_SRC_DIR}/lua/st_lua_to_from.h
    ${ST_SRC_DIR}/lua/st_lua_state.cpp
    ${ST_SRC_DIR}/lua/st_lua_state.h
    ${ST_SRC_DIR}/lua/st_lua_table.cpp
    ${ST_SRC_DIR}/lua/st_lua_table.h
    ${ST_SRC_DIR}/lua/st_lua_usertype.cpp
    ${ST_SRC_DIR}/lua/st_lua_usertype.h
    ${ST_SRC_DIR}/lua/st_lua_utils.cpp
    ${ST_SRC_DIR}/lua/st_lua_utils.h
    ${ST_SRC_DIR}/process/st_process.cpp
    ${ST_SRC_DIR}/process/st_process.h
    ${ST_SRC_DIR}/process/st_process_manager.cpp
    ${ST_SRC_DIR}/process/st_process_manager.h
    ${ST_SRC_DIR}/resource/st_resource_loader.cpp
    ${ST_SRC_DIR}/resource/st_resource_loader.h
    ${ST_SRC_DIR}/scene/st_entity.cpp
    ${ST_SRC_DIR}/scene/st_entity.h
    ${ST_SRC_DIR}/scene/st_scene.cpp
    ${ST_SRC_DIR}/scene/st_scene.h
    ${ST_SRC_DIR}/shapes/st_ellipse.cpp
    ${ST_SRC_DIR}/shapes/st_ellipse.h
    ${ST_SRC_DIR}/shapes/st_intersect.cpp
    ${ST_SRC_DIR}/shapes/st_intersect.h
    ${ST_SRC_DIR}/shapes/st_point.cpp
    ${ST_SRC_DIR}/shapes/st_point.h
    ${ST_SRC_DIR}/shapes/st_polygon.cpp
    ${ST_SRC_DIR}/shapes/st_polygon.h
    ${ST_SRC_DIR}/shapes/st_rectangle.cpp
    ${ST_SRC_DIR}/shapes/st_rectangle.h
    ${ST_SRC_DIR}/shapes/st_shape.cpp
    ${ST_SRC_DIR}/shapes/st_shape.h
    ${ST_SRC_DIR}/system/st_assert.h
    ${ST_SRC_DIR}/system/st_clock.cpp
    ${ST_SRC_DIR}/system/st_clock.h
    ${ST_SRC_DIR}/system/st_command_line_arguments.cpp
    ${ST_SRC_DIR}/system/st_command_line_arguments.h
    ${ST_SRC_DIR}/system/st_defer.h
    ${ST_SRC_DIR}/system/st_dispatcher.cpp
    ${ST_SRC_DIR}/system/st_dispatcher.h
    ${ST_SRC_DIR}/system/st_environment.h
    ${ST_SRC_DIR}/system/st_error.cpp
    ${ST_SRC_DIR}/system/st_error.h
    ${ST_SRC_DIR}/system/st_error_signal.cpp
    ${ST_SRC_DIR}/system/st_error_signal.h
    ${ST_SRC_DIR}/system/st_file_reader.cpp
    ${ST_SRC_DIR}/system/st_file_reader.h
    ${ST_SRC_DIR}/system/st_metrics.h
    ${ST_SRC_DIR}/system/st_json_to_from.h
    ${ST_SRC_DIR}/system/st_log.cpp
    ${ST_SRC_DIR}/system/st_log.h
    ${ST_SRC_DIR}/system/st_memory.cpp
    ${ST_SRC_DIR}/system/st_memory.h
    ${ST_SRC_DIR}/system/st_memory_allocator.cpp
    ${ST_SRC_DIR}/system/st_memory_allocator.h
    ${ST_SRC_DIR}/system/st_pointers.h
    ${ST_SRC_DIR}/system/st_random.cpp
    ${ST_SRC_DIR}/system/st_random.h
    ${ST_SRC_DIR}/system/st_size.cpp
    ${ST_SRC_DIR}/system/st_size.h
    ${ST_SRC_DIR}/system/st_subscriber.cpp
    ${ST_SRC_DIR}/system/st_subscriber.h
    ${ST_SRC_DIR}/system/st_uuid.cpp
    ${ST_SRC_DIR}/system/st_uuid.h
    ${ST_SRC_DIR}/system/st_variant.h
    ${ST_SRC_DIR}/tiled/st_tiled_class.cpp
    ${ST_SRC_DIR}/tiled/st_tiled_class.h
    ${ST_SRC_DIR}/tiled/st_tiled_enum.cpp
    ${ST_SRC_DIR}/tiled/st_tiled_enum.h
    ${ST_SRC_DIR}/tiled/st_tiled_id.h
    ${ST_SRC_DIR}/tiled/st_tiled_layer.cpp
    ${ST_SRC_DIR}/tiled/st_tiled_layer.h
    ${ST_SRC_DIR}/tiled/st_tiled_map.cpp
    ${ST_SRC_DIR}/tiled/st_tiled_map.h
    ${ST_SRC_DIR}/tiled/st_tiled_object.cpp
    ${ST_SRC_DIR}/tiled/st_tiled_object.h
    ${ST_SRC_DIR}/tiled/st_tiled_project.cpp
    ${ST_SRC_DIR}/tiled/st_tiled_project.h
    ${ST_SRC_DIR}/tiled/st_tiled_property.cpp
    ${ST_SRC_DIR}/tiled/st_tiled_property.h
    ${ST_SRC_DIR}/tiled/st_tiled_template.cpp
    ${ST_SRC_DIR}/tiled/st_tiled_template.h
    ${ST_SRC_DIR}/tiled/st_tiled_tile.cpp
    ${ST_SRC_DIR}/tiled/st_tiled_tile.h
    ${ST_SRC_DIR}/tiled/st_tiled_tileset.cpp
    ${ST_SRC_DIR}/tiled/st_tiled_tileset.h
    ${ST_SRC_DIR}/tiled/st_tiled_transformation_flags.h
    ${ST_SRC_DIR}/utils/st_glm.cpp
    ${ST_SRC_DIR}/utils/st_glm.h
    ${ST_SRC_DIR}/utils/st_print.h
    ${ST_SRC_DIR}/utils/st_null_safety.h
    ${ST_SRC_DIR}/utils/st_running_average.cpp
    ${ST_SRC_DIR}/utils/st_running_average.h
    ${ST_SRC_DIR}/utils/st_string.cpp
    ${ST_SRC_DIR}/utils/st_string.h
    ${ST_SRC_DIR}/utils/st_type_name.cpp
    ${ST_SRC_DIR}/utils/st_type_name.h
    ${ST_SRC_DIR}/window/st_key.cpp
    ${ST_SRC_DIR}/window/st_key.h
    ${ST_SRC_DIR}/window/st_keyboard.cpp
    ${ST_SRC_DIR}/window/st_keyboard.h
    ${ST_SRC_DIR}/window/st_mouse.cpp
    ${ST_SRC_DIR}/window/st_mouse.h
    ${ST_SRC_DIR}/window/st_mouse_button.cpp
    ${ST_SRC_DIR}/window/st_mouse_button.h
    ${ST_SRC_DIR}/window/st_window.cpp
    ${ST_SRC_DIR}/window/st_window.h
)

set_target_properties(
    ${ST_TARGET}
    PROPERTIES
    PREFIX ""
    ARCHIVE_OUTPUT_NAME ${ST_TARGET}
    ARCHIVE_OUTPUT_DIRECTORY ${ST_BIN_DIR}
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${ST_BIN_DIR}/debug
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${ST_BIN_DIR}/release
    LIBRARY_OUTPUT_NAME ${ST_TARGET}
    LIBRARY_OUTPUT_DIRECTORY ${ST_BIN_DIR}
    LIBRARY_OUTPUT_DIRECTORY_DEBUG ${ST_BIN_DIR}/debug
    LIBRARY_OUTPUT_DIRECTORY_RELEASE ${ST_BIN_DIR}/release
    RUNTIME_OUTPUT_NAME ${ST_TARGET}
    RUNTIME_OUTPUT_DIRECTORY ${ST_BIN_DIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${ST_BIN_DIR}/debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${ST_BIN_DIR}/release
)

target_include_directories(${ST_TARGET} PUBLIC ${ST_INCLUDE_DIR})
target_include_directories(${ST_TARGET} PUBLIC ${ST_SRC_DIR})

target_precompile_headers(${ST_TARGET} PUBLIC ${ST_SRC_DIR}/st_pch.h)

option(ST_USE_VULKAN "Use Vulkan renderer" ON)
target_compile_definitions(${ST_TARGET} PUBLIC ST_USE_VULKAN=${ST_USE_VULKAN})

# --------------------------------------------------------------------------------------------------------------
# Clang compiler flags
# --------------------------------------------------------------------------------------------------------------

target_compile_options(${ST_TARGET} PRIVATE -Wno-shadow)

# --------------------------------------------------------------------------------------------------------------
# Custom targets
# --------------------------------------------------------------------------------------------------------------

#
# Copy compile_commands.json file to project root
#

set(ST_COPY_COMPILE_COMMANDS_TARGET st_copy_compile_commands)
if (CMAKE_EXPORT_COMPILE_COMMANDS)
    add_custom_target(
        ${ST_COPY_COMPILE_COMMANDS_TARGET}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/compile_commands.json
        COMMENT "Copying compile_commands.json"
        VERBATIM
    )
    add_dependencies(${ST_TARGET} ${ST_COPY_COMPILE_COMMANDS_TARGET})
endif()

#
# Copy resource files to runtime directory
#

set(ST_RESOURCES_SOURCE_DIR ${PROJECT_SOURCE_DIR}/res)
set(ST_RESOURCES_OUTPUT_DIR ${ST_OUTPUT_DIR}/res)

set(ST_COPY_RESOURCES_TARGET st_copy_resources)
add_custom_target(
    ${ST_COPY_RESOURCES_TARGET}
    COMMENT "Copying resources"
    COMMAND ${CMAKE_COMMAND} -D SOURCE_DIR=${ST_RESOURCES_SOURCE_DIR} -D OUTPUT_DIR=${ST_RESOURCES_OUTPUT_DIR} -P ${ST_CMAKE_DIR}/copy.cmake
    VERBATIM
)
add_dependencies(${ST_TARGET} ${ST_COPY_RESOURCES_TARGET})

target_compile_definitions(${ST_TARGET} PUBLIC ST_RES_DIR="${ST_RESOURCES_OUTPUT_DIR}")

#
# Compile Lua files to runtime directory
#

set(ST_LUA_SOURCE_DIR ${PROJECT_SOURCE_DIR}/lua)
set(ST_LUA_OUTPUT_DIR ${ST_OUTPUT_DIR}/lua)

set(ST_COMPILE_LUA_TARGET st_compile_lua)
add_custom_target(
    ${ST_COMPILE_LUA_TARGET}
    COMMAND ${CMAKE_COMMAND} -D LUA_SOURCE_DIR=${ST_LUA_SOURCE_DIR} -D LUA_OUTPUT_DIR=${ST_LUA_OUTPUT_DIR} -P ${ST_CMAKE_DIR}/compile_lua.cmake
    COMMENT "Compiling Lua"
    VERBATIM
)
add_dependencies(${ST_TARGET} ${ST_COMPILE_LUA_TARGET})

target_compile_definitions(${ST_TARGET} PUBLIC ST_LUA_DIR="${ST_LUA_OUTPUT_DIR}")

#
# Compile Vulkan shaders to SPIR-V
#

set(SHADERS_SOURCE_DIR ${PROJECT_SOURCE_DIR}/res/shaders)
set(SHADERS_OUTPUT_DIR ${ST_OUTPUT_DIR}/res/shaders)

set(ST_COMPILE_SHADERS_TARGET st_compile_shaders)
add_custom_target(
    ${ST_COMPILE_SHADERS_TARGET}
    COMMAND ${CMAKE_COMMAND} -D SHADERS_SOURCE_DIR=${SHADERS_SOURCE_DIR} -D SHADERS_OUTPUT_DIR=${SHADERS_OUTPUT_DIR} -P ${ST_CMAKE_DIR}/compile_shaders.cmake
    COMMENT "Compiling shaders"
    VERBATIM
)
add_dependencies(${ST_TARGET} ${ST_COMPILE_SHADERS_TARGET})

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
target_link_libraries(${ST_TARGET} EnTT::EnTT)

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)
target_link_libraries(${ST_TARGET} glm::glm)

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
target_link_libraries(${ST_TARGET} glfw)

if (${ST_USE_VULKAN})
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
target_include_directories(${ST_TARGET} PUBLIC ${IMGUI_DIR})
target_link_libraries(${ST_TARGET} ${IMGUI_NAME})
endif ()

# JSON
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)
target_link_libraries(${ST_TARGET} nlohmann_json::nlohmann_json)

# Lua
find_package(Lua REQUIRED)
target_include_directories(${ST_TARGET} PUBLIC ${LUA_INCLUDE_DIR})
target_link_libraries(${ST_TARGET} ${LUA_LIBRARIES})

# Miniaudio
FetchContent_Declare(
    miniaudio
    GIT_REPOSITORY https://github.com/mackron/miniaudio.git
    GIT_TAG 0.11.21
)
FetchContent_MakeAvailable(miniaudio)
target_include_directories(${ST_TARGET} PUBLIC ${miniaudio_SOURCE_DIR})

# STB image
set(STB_IMAGE_NAME stb_image)
set(STB_IMAGE_DIR ${ST_LIB_DIR}/stb_image-2.22)
add_library(
    ${STB_IMAGE_NAME}
    ${STB_IMAGE_DIR}/src/stb_image.cpp
)
target_include_directories(${STB_IMAGE_NAME} PRIVATE ${STB_IMAGE_DIR}/include)
target_include_directories(${ST_TARGET} PUBLIC ${STB_IMAGE_DIR}/include)
target_link_libraries(${ST_TARGET} ${STB_IMAGE_NAME} ${CMAKE_DL_LIBS}) # Link with platform-specific dynamic loading libraries

# SPDLOG
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG v1.15.3
)
FetchContent_MakeAvailable(spdlog)
target_link_libraries(${ST_TARGET} spdlog::spdlog)

# Vulkan
find_package(Vulkan REQUIRED)
target_include_directories(${ST_TARGET} PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${ST_TARGET} ${Vulkan_LIBRARIES})
