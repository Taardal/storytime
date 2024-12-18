#pragma once

// Entrypoint
#include "st_main.h"

// System
#include "system/defer.h"
#include "system/event_manager.h"
#include "system/file_reader.h"
#include "system/random.h"

// Window
#include "window/key.h"
#include "window/key_event.h"
#include "window/mouse_button.h"
#include "window/mouse_event.h"
#include "window/user_input.h"
#include "window/window.h"
#include "window/window_event.h"

// Graphics
#include "graphics/bitmap_font.h"
#include "graphics/camera.h"
#include "graphics/spritesheet.h"
#include "graphics/view_projection.h"
#ifdef ST_IMGUI_ENABLED
    #include "graphics/imgui_renderer.h"
    #include "graphics/imgui_window_event.h"
#endif

// Resource
#include "resource/resource_loader.h"

// Scene
#include "scene/scene.h"

// Script
#include "script/lua_function.h"
#include "script/lua_glm.h"
#include "script/lua_keyboard.h"
#include "script/lua_log.h"
#include "script/lua_mouse.h"
#include "script/lua_serialize_deserialize.h"
#include "script/lua_state.h"
#include "script/lua_table.h"
#include "script/lua_usertype.h"

// System
#include "system/json_serialize_deserialize.h"

// Tiled
#include "tiled/tiled_project.h"
#include "tiled/tiled_map.h"

// Namespaces
namespace st = Storytime;