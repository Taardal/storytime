#pragma once

// --------------------------------------------------------------------------------------------------------------
// Standard library
// --------------------------------------------------------------------------------------------------------------

#include <algorithm>
#include <array>
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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// ImGui
#include <imgui.h>

// --------------------------------------------------------------------------------------------------------------
// App
// --------------------------------------------------------------------------------------------------------------

#include "system/assert.h"
#include "system/environment.h"
#include "system/error.h"
#include "system/error_signal.h"
#include "system/log.h"
#include "system/numbers.h"
#include "system/pointers.h"
#include "system/utils.h"

