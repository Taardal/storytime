#pragma once

// --------------------------------------------------------------------------------------------------------------
// INCLUDES
// --------------------------------------------------------------------------------------------------------------

// Entrypoint
#include "st_pch.h"
#include "st_run.h"

// Graphics
#include "graphics/st_animation.h"
#include "graphics/st_bitmap_font.h"
#include "graphics/st_camera.h"
#include "graphics/st_spritesheet.h"
#include "graphics/st_view_projection.h"
#ifdef ST_IMGUI_ENABLED
    #include <imgui.h>
    #include <imgui_internal.h>
    #include "graphics/st_imgui_renderer.h"
    #include "graphics/st_imgui_window_event.h"
#endif

// Lua
#include "lua/st_lua_function.h"
#include "lua/st_lua_glm.h"
#include "lua/st_lua_keyboard.h"
#include "lua/st_lua_log.h"
#include "lua/st_lua_mouse.h"
#include "lua/st_lua_to_from.h"
#include "lua/st_lua_state.h"
#include "lua/st_lua_table.h"
#include "lua/st_lua_usertype.h"

// Resource
#include "resource/st_resource_loader.h"

// Scene
#include "scene/st_entity.h"
#include "scene/st_scene.h"

// System
#include "system/st_binary_to_from.h"
#include "system/st_defer.h"
#include "system/st_event_manager.h"
#include "system/st_file_reader.h"
#include "system/st_json_to_from.h"
#include "system/st_game_loop_metrics.h"
#include "system/st_msgpack_filesystem_adaptor.h"
#include "system/st_msgpack_glm_adaptor.h"
#include "system/st_msgpack_utils.h"
#include "system/st_random.h"
#include "system/st_variant.h"

// Tiled
#include "tiled/st_tiled_class.h"
#include "tiled/st_tiled_enum.h"
#include "tiled/st_tiled_layer.h"
#include "tiled/st_tiled_map.h"
#include "tiled/st_tiled_object.h"
#include "tiled/st_tiled_project.h"
#include "tiled/st_tiled_property.h"
#include "tiled/st_tiled_template.h"
#include "tiled/st_tiled_tile.h"
#include "tiled/st_tiled_tileset.h"

// Window
#include "window/st_key.h"
#include "window/st_key_event.h"
#include "window/st_keyboard.h"
#include "window/st_mouse.h"
#include "window/st_mouse_button.h"
#include "window/st_mouse_event.h"
#include "window/st_window.h"
#include "window/st_window_event.h"

// --------------------------------------------------------------------------------------------------------------
// NAMESPACES
// --------------------------------------------------------------------------------------------------------------

namespace st = Storytime;