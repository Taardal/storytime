#pragma once

// --------------------------------------------------------------------------------------------------------------
// INCLUDES
// --------------------------------------------------------------------------------------------------------------

// Entrypoint
#include "st_run.h"

// Entity
#include "entity/entity.h"

// Graphics
#include "graphics/animation.h"
#include "graphics/bitmap_font.h"
#include "graphics/camera.h"
#include "graphics/spritesheet.h"
#include "graphics/view_projection.h"
#ifdef ST_IMGUI_ENABLED
    #include <imgui.h>
    #include <imgui_internal.h>
    #include "graphics/imgui_renderer.h"
    #include "graphics/imgui_window_event.h"
#endif

// Lua
#include "lua/lua_function.h"
#include "lua/lua_glm.h"
#include "lua/lua_keyboard.h"
#include "lua/lua_log.h"
#include "lua/lua_mouse.h"
#include "lua/lua_to_from.h"
#include "lua/lua_state.h"
#include "lua/lua_table.h"
#include "lua/lua_usertype.h"

// Resource
#include "resource/resource_loader.h"

// System
#include "system/defer.h"
#include "system/event_manager.h"
#include "system/file_reader.h"
#include "system/random.h"

// System
#include "system/binary_to_from.h"
#include "system/json_to_from.h"
#include "system/game_loop_metrics.h"
#include "system/msgpack_filesystem_adaptor.h"
#include "system/msgpack_glm_adaptor.h"
#include "system/msgpack_utils.h"
#include "system/random.h"
#include "system/variant.h"

// Tiled
#include "tiled/tiled_class.h"
#include "tiled/tiled_enum.h"
#include "tiled/tiled_layer.h"
#include "tiled/tiled_map.h"
#include "tiled/tiled_object.h"
#include "tiled/tiled_project.h"
#include "tiled/tiled_property.h"
#include "tiled/tiled_template.h"
#include "tiled/tiled_tile.h"
#include "tiled/tiled_tileset.h"

// Window
#include "window/key.h"
#include "window/key_event.h"
#include "window/mouse_button.h"
#include "window/mouse_event.h"
#include "window/user_input.h"
#include "window/window.h"
#include "window/window_event.h"

// --------------------------------------------------------------------------------------------------------------
// NAMESPACES
// --------------------------------------------------------------------------------------------------------------

namespace st = Storytime;