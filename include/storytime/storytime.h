#pragma once

// Entrypoint
#include "storytime_main.h"

// System
#include "system/defer.h"
#include "system/event_manager.h"
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

// Tiled
#include "tiled/map.h"

// Scene
#include "scene/scene.h"
#include "scene/tiled_scene.h"

// Script
#include "script/glm_lua_binding.h"
#include "script/keyboard_lua_binding.h"
#include "script/lua_function.h"
#include "script/lua_log.h"
#include "script/lua_state.h"
#include "script/lua_table.h"
#include "script/mouse_lua_binding.h"

// Namespaces
namespace st = Storytime;