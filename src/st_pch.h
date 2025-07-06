#pragma once

// --------------------------------------------------------------------------------------------------------------
// Standard library
// --------------------------------------------------------------------------------------------------------------

#include <algorithm>
#include <array>
#include <filesystem>
#include <iostream>
#include <list>
#include <memory>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

// --------------------------------------------------------------------------------------------------------------
// 3rd-party
// --------------------------------------------------------------------------------------------------------------

// GLAD
#include <glad/glad.h> // Include GLAD before GLFW to let GLAD include the OpenGL header (gl.h)

// GLFW
#define GLFW_INCLUDE_NONE // Explicitly prevent GLFW from including the OpenGL header (gl.h)
#include <GLFW/glfw3.h>

// GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Lua
#include <lua.hpp>

// --------------------------------------------------------------------------------------------------------------
// App
// --------------------------------------------------------------------------------------------------------------

// System
#include "system/st_assert.h"
#include "system/st_environment.h"
#include "system/st_error.h"
#include "system/st_error_signal.h"
#include "system/st_log.h"
#include "system/st_memory.h"
#include "system/st_numbers.h"
#include "system/st_pointers.h"
#include "system/st_size.h"
#include "system/st_utils.h"

// Lua
#include "lua/st_lua_error.h"
#include "lua/st_lua_ref.h"
#include "lua/st_lua_utils.h"

