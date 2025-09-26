#pragma once

// --------------------------------------------------------------------------------------------------------------
// INCLUDES
// --------------------------------------------------------------------------------------------------------------

// Entrypoint
#include "st_pch.h"
#include "st_main.h"
#include "st_config.h"

// Audio
#include "audio/st_audio.h"
#include "audio/st_audio_engine.h"

// Event
#include "event/st_event.h"
#include "event/st_key_pressed_event.h"
#include "event/st_key_released_event.h"
#include "event/st_key_repeated_event.h"
#include "event/st_key_typed_event.h"
#include "event/st_mouse_button_pressed_event.h"
#include "event/st_mouse_button_released_event.h"
#include "event/st_mouse_moved_event.h"
#include "event/st_mouse_scroll_event.h"
#include "event/st_window_closed_event.h"
#include "event/st_window_minimized_event.h"
#include "event/st_window_resized_event.h"

// Graphics
#include "graphics/st_animation.h"
#include "graphics/st_bitmap_font.h"
#include "graphics/st_camera.h"
#include "graphics/st_imgui_renderer.h"
#include "graphics/st_spritesheet.h"
#include "graphics/st_view_projection.h"

// Lua
#include "lua/st_lua_dispatcher_binding.h"
#include "lua/st_lua_function.h"
#include "lua/st_lua_glm_binding.h"
#include "lua/st_lua_keyboard_binding.h"
#include "lua/st_lua_log_binding.h"
#include "lua/st_lua_mouse_binding.h"
#include "lua/st_lua_process_binding.h"
#include "lua/st_lua_process_manager_binding.h"
#include "lua/st_lua_state.h"
#include "lua/st_lua_table.h"
#include "lua/st_lua_to_from.h"
#include "lua/st_lua_usertype.h"

// Process
#include "process/st_process.h"
#include "process/st_process_manager.h"

// Resource
#include "resource/st_resource_loader.h"

// Scene
#include "scene/st_entity.h"
#include "scene/st_scene.h"

// Shapes
#include "shapes/st_ellipse.h"
#include "shapes/st_intersect.h"
#include "shapes/st_point.h"
#include "shapes/st_polygon.h"
#include "shapes/st_rectangle.h"
#include "shapes/st_shape.h"

// System
#include "system/st_clock.h"
#include "system/st_command_line_arguments.h"
#include "system/st_defer.h"
#include "system/st_dispatcher.h"
#include "system/st_file_reader.h"
#include "system/st_json_to_from.h"
#include "system/st_game_loop_metrics.h"
#include "system/st_random.h"
#include "system/st_subscriber.h"
#include "system/st_variant.h"

// Tiled
#include "tiled/st_tiled_class.h"
#include "tiled/st_tiled_enum.h"
#include "tiled/st_tiled_id.h"
#include "tiled/st_tiled_layer.h"
#include "tiled/st_tiled_map.h"
#include "tiled/st_tiled_object.h"
#include "tiled/st_tiled_project.h"
#include "tiled/st_tiled_property.h"
#include "tiled/st_tiled_template.h"
#include "tiled/st_tiled_tile.h"
#include "tiled/st_tiled_tileset.h"
#include "tiled/st_tiled_transformation_flags.h"

// Window
#include "window/st_key.h"
#include "window/st_keyboard.h"
#include "window/st_mouse.h"
#include "window/st_mouse_button.h"
#include "window/st_window.h"

// --------------------------------------------------------------------------------------------------------------
// NAMESPACES
// --------------------------------------------------------------------------------------------------------------

namespace st = Storytime;
