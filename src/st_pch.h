#pragma once

// --------------------------------------------------------------------------------------------------------------
// Standard library
// --------------------------------------------------------------------------------------------------------------

#include <algorithm>
#include <array>
#include <filesystem>
#include <iostream>
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

// ImGui
#include <imgui.h>

// Lua
#include <lua.hpp>

// --------------------------------------------------------------------------------------------------------------
// App
// --------------------------------------------------------------------------------------------------------------

// System
#include "system/assert.h"
#include "system/environment.h"
#include "system/error.h"
#include "system/error_signal.h"
#include "system/log.h"
#include "system/memory.h"
#include "system/numbers.h"
#include "system/pointers.h"
#include "system/utils.h"

// Script
#include "script/lua_error.h"
#include "script/lua_ref.h"
#include "script/lua_utils.h"

