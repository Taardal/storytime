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
#include <unordered_map>
#include <vector>

// --------------------------------------------------------------------------------------------------------------
// 3rd-party
// --------------------------------------------------------------------------------------------------------------

// Include GLAD before GLFW to let GLAD include the OpenGL header (gl.h)
#include <glad/glad.h>

// Explicitly prevent GLFW from including the OpenGL header (gl.h)
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// --------------------------------------------------------------------------------------------------------------
// App
// --------------------------------------------------------------------------------------------------------------

#include "system/assert.h"
#include "system/environment.h"
#include "system/error.h"
#include "system/log.h"
#include "system/numbers.h"
#include "system/pointers.h"
#include "system/utils.h"

