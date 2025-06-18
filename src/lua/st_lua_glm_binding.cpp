#include "st_lua_glm_binding.h"

namespace Storytime {
    const std::string LuaGLMBinding::metatable_name = "LuaGLMBinding";
    const std::string LuaGLMBinding::vec2_metatable_name = metatable_name + ".Vec2";
    const std::string LuaGLMBinding::vec3_metatable_name = metatable_name + ".Vec3";
    const std::string LuaGLMBinding::vec4_metatable_name = metatable_name + ".Vec4";
    const std::string LuaGLMBinding::mat2_metatable_name = metatable_name + ".Mat2";
    const std::string LuaGLMBinding::mat3_metatable_name = metatable_name + ".Mat3";
    const std::string LuaGLMBinding::mat4_metatable_name = metatable_name + ".Mat4";
    const std::string LuaGLMBinding::quat_metatable_name = metatable_name + ".Quat";

    int LuaGLMBinding::create_metatable(lua_State* L) {
        create_vec2_metatable(L);
        lua_pop(L, 1);
        create_vec3_metatable(L);
        lua_pop(L, 1);
        create_vec4_metatable(L);
        lua_pop(L, 1);
        create_mat2_metatable(L);
        lua_pop(L, 1);
        create_mat3_metatable(L);
        lua_pop(L, 1);
        create_mat4_metatable(L);
        lua_pop(L, 1);
        create_quat_metatable(L);
        lua_pop(L, 1);

        luaL_newmetatable(L, metatable_name.c_str());
        lua_pushcfunction(L, index);
        lua_setfield(L, -2, "__index");

        return 1;
    }

    int LuaGLMBinding::create(lua_State* L) {
        lua_newtable(L);

        luaL_getmetatable(L, metatable_name.c_str());
        ST_ASSERT(!lua_isnil(L, -1), "Metatable [" << metatable_name.c_str() << "] cannot be null");
        lua_setmetatable(L, -2);

        return 1;
    }

    int LuaGLMBinding::create_vec2_metatable(lua_State* L) {
        luaL_newmetatable(L, vec2_metatable_name.c_str());

        lua_pushcfunction(L, add_vec2);
        lua_setfield(L, -2, "__add");

        lua_pushcfunction(L, divide_vec2);
        lua_setfield(L, -2, "__div");

        lua_pushcfunction(L, index_vec2);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, length_vec2);
        lua_setfield(L, -2, "__len");

        lua_pushcfunction(L, multiply_vec2);
        lua_setfield(L, -2, "__mul");

        lua_pushcfunction(L, subtract_vec2);
        lua_setfield(L, -2, "__sub");

        lua_pushcfunction(L, unary_minus_vec2);
        lua_setfield(L, -2, "__unm");

        lua_pushcfunction(L, to_string_vec2);
        lua_setfield(L, -2, "__tostring");

        lua_pushcfunction(L, concat_vec2);
        lua_setfield(L, -2, "__concat");

        return 1;
    }

    int LuaGLMBinding::create_vec3_metatable(lua_State* L) {
        luaL_newmetatable(L, vec3_metatable_name.c_str());

        lua_pushcfunction(L, add_vec3);
        lua_setfield(L, -2, "__add");

        lua_pushcfunction(L, divide_vec3);
        lua_setfield(L, -2, "__div");

        lua_pushcfunction(L, index_vec3);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, length_vec3);
        lua_setfield(L, -2, "__len");

        lua_pushcfunction(L, multiply_vec3);
        lua_setfield(L, -2, "__mul");

        lua_pushcfunction(L, subtract_vec3);
        lua_setfield(L, -2, "__sub");

        lua_pushcfunction(L, unary_minus_vec3);
        lua_setfield(L, -2, "__unm");

        lua_pushcfunction(L, to_string_vec3);
        lua_setfield(L, -2, "__tostring");

        lua_pushcfunction(L, concat_vec3);
        lua_setfield(L, -2, "__concat");

        return 1;
    }

    int LuaGLMBinding::create_vec4_metatable(lua_State* L) {
        luaL_newmetatable(L, vec4_metatable_name.c_str());

        lua_pushcfunction(L, add_vec4);
        lua_setfield(L, -2, "__add");

        lua_pushcfunction(L, divide_vec4);
        lua_setfield(L, -2, "__div");

        lua_pushcfunction(L, index_vec4);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, length_vec4);
        lua_setfield(L, -2, "__len");

        lua_pushcfunction(L, multiply_vec4);
        lua_setfield(L, -2, "__mul");

        lua_pushcfunction(L, subtract_vec4);
        lua_setfield(L, -2, "__sub");

        lua_pushcfunction(L, unary_minus_vec4);
        lua_setfield(L, -2, "__unm");

        lua_pushcfunction(L, to_string_vec4);
        lua_setfield(L, -2, "__tostring");

        lua_pushcfunction(L, concat_vec4);
        lua_setfield(L, -2, "__concat");

        return 1;
    }

    int LuaGLMBinding::create_mat2_metatable(lua_State* L) {
        luaL_newmetatable(L, mat2_metatable_name.c_str());

        lua_pushcfunction(L, add_mat2);
        lua_setfield(L, -2, "__add");

        lua_pushcfunction(L, divide_mat2);
        lua_setfield(L, -2, "__div");

        lua_pushcfunction(L, multiply_mat2);
        lua_setfield(L, -2, "__mul");

        lua_pushcfunction(L, subtract_mat2);
        lua_setfield(L, -2, "__sub");

        return 1;
    }

    int LuaGLMBinding::create_mat3_metatable(lua_State* L) {
        luaL_newmetatable(L, mat3_metatable_name.c_str());

        lua_pushcfunction(L, add_mat3);
        lua_setfield(L, -2, "__add");

        lua_pushcfunction(L, divide_mat3);
        lua_setfield(L, -2, "__div");

        lua_pushcfunction(L, index_mat3);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, multiply_mat3);
        lua_setfield(L, -2, "__mul");

        lua_pushcfunction(L, subtract_mat3);
        lua_setfield(L, -2, "__sub");

        return 1;
    }

    int LuaGLMBinding::create_mat4_metatable(lua_State* L) {
        luaL_newmetatable(L, mat4_metatable_name.c_str());

        lua_pushcfunction(L, add_mat4);
        lua_setfield(L, -2, "__add");

        lua_pushcfunction(L, divide_mat4);
        lua_setfield(L, -2, "__div");

        lua_pushcfunction(L, index_mat4);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, multiply_mat4);
        lua_setfield(L, -2, "__mul");

        lua_pushcfunction(L, subtract_mat4);
        lua_setfield(L, -2, "__sub");

        return 1;
    }

    int LuaGLMBinding::create_quat_metatable(lua_State* L) {
        luaL_newmetatable(L, quat_metatable_name.c_str());

        lua_pushcfunction(L, add_quat);
        lua_setfield(L, -2, "__add");

        lua_pushcfunction(L, divide_quat);
        lua_setfield(L, -2, "__div");

        lua_pushcfunction(L, index_quat);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, multiply_quat);
        lua_setfield(L, -2, "__mul");

        lua_pushcfunction(L, subtract_quat);
        lua_setfield(L, -2, "__sub");

        return 1;
    }

    int LuaGLMBinding::index(lua_State* L) {
        constexpr i32 key_index = -1;
        ST_ASSERT(lua_type(L, key_index) == LUA_TSTRING, "Key must be at expected stack location");

        const char* key = lua_tostring(L, key_index);
        ST_ASSERT(strlen(key) > 0, "Key cannot be empty");

        if (strcmp(key, "add_mat2") == 0) {
            lua_pushcfunction(L, add_mat2);
            return 1;
        }
        if (strcmp(key, "add_mat3") == 0) {
            lua_pushcfunction(L, add_mat3);
            return 1;
        }
        if (strcmp(key, "add_mat4") == 0) {
            lua_pushcfunction(L, add_mat4);
            return 1;
        }
        if (strcmp(key, "add_quat") == 0) {
            lua_pushcfunction(L, add_quat);
            return 1;
        }
        if (strcmp(key, "add_vec2") == 0) {
            lua_pushcfunction(L, add_vec2);
            return 1;
        }
        if (strcmp(key, "add_vec3") == 0) {
            lua_pushcfunction(L, add_vec3);
            return 1;
        }
        if (strcmp(key, "add_vec4") == 0) {
            lua_pushcfunction(L, add_vec4);
            return 1;
        }
        if (strcmp(key, "angle_axis") == 0) {
            lua_pushcfunction(L, angle_axis);
            return 1;
        }
        if (strcmp(key, "cross") == 0) {
            lua_pushcfunction(L, cross);
            return 1;
        }
        if (strcmp(key, "degrees") == 0) {
            lua_pushcfunction(L, degrees);
            return 1;
        }
        if (strcmp(key, "divide_mat2") == 0) {
            lua_pushcfunction(L, divide_mat2);
            return 1;
        }
        if (strcmp(key, "divide_mat3") == 0) {
            lua_pushcfunction(L, divide_mat3);
            return 1;
        }
        if (strcmp(key, "divide_mat4") == 0) {
            lua_pushcfunction(L, divide_mat4);
            return 1;
        }
        if (strcmp(key, "divide_quat") == 0) {
            lua_pushcfunction(L, divide_quat);
            return 1;
        }
        if (strcmp(key, "divide_vec2") == 0) {
            lua_pushcfunction(L, divide_vec2);
            return 1;
        }
        if (strcmp(key, "divide_vec3") == 0) {
            lua_pushcfunction(L, divide_vec3);
            return 1;
        }
        if (strcmp(key, "divide_vec4") == 0) {
            lua_pushcfunction(L, divide_vec4);
            return 1;
        }
        if (strcmp(key, "dot_vec2") == 0) {
            lua_pushcfunction(L, dot_vec2);
            return 1;
        }
        if (strcmp(key, "dot_vec3") == 0) {
            lua_pushcfunction(L, dot_vec3);
            return 1;
        }
        if (strcmp(key, "dot_vec4") == 0) {
            lua_pushcfunction(L, dot_vec4);
            return 1;
        }
        if (strcmp(key, "euler_angles") == 0) {
            lua_pushcfunction(L, euler_angles);
            return 1;
        }
        if (strcmp(key, "inverse_quat") == 0) {
            lua_pushcfunction(L, inverse_quat);
            return 1;
        }
        if (strcmp(key, "inverse_mat2") == 0) {
            lua_pushcfunction(L, inverse_mat2);
            return 1;
        }
        if (strcmp(key, "inverse_mat3") == 0) {
            lua_pushcfunction(L, inverse_mat3);
            return 1;
        }
        if (strcmp(key, "inverse_mat4") == 0) {
            lua_pushcfunction(L, inverse_mat4);
            return 1;
        }
        if (strcmp(key, "length_vec2") == 0) {
            lua_pushcfunction(L, length_vec2);
            return 1;
        }
        if (strcmp(key, "length_vec3") == 0) {
            lua_pushcfunction(L, length_vec3);
            return 1;
        }
        if (strcmp(key, "length_vec4") == 0) {
            lua_pushcfunction(L, length_vec4);
            return 1;
        }
        if (strcmp(key, "lerp") == 0) {
            lua_pushcfunction(L, lerp);
            return 1;
        }
        if (strcmp(key, "look_at") == 0) {
            lua_pushcfunction(L, look_at);
            return 1;
        }
        if (strcmp(key, "mat2") == 0) {
            lua_pushcfunction(L, mat2);
            return 1;
        }
        if (strcmp(key, "mat3") == 0) {
            lua_pushcfunction(L, mat3);
            return 1;
        }
        if (strcmp(key, "mat4") == 0) {
            lua_pushcfunction(L, mat4);
            return 1;
        }
        if (strcmp(key, "mat3_to_quat") == 0) {
            lua_pushcfunction(L, mat3_to_quat);
            return 1;
        }
        if (strcmp(key, "mat4_to_quat") == 0) {
            lua_pushcfunction(L, mat4_to_quat);
            return 1;
        }
        if (strcmp(key, "multiply_mat2") == 0) {
            lua_pushcfunction(L, multiply_mat2);
            return 1;
        }
        if (strcmp(key, "multiply_mat3") == 0) {
            lua_pushcfunction(L, multiply_mat3);
            return 1;
        }
        if (strcmp(key, "multiply_mat4") == 0) {
            lua_pushcfunction(L, multiply_mat4);
            return 1;
        }
        if (strcmp(key, "multiply_quat") == 0) {
            lua_pushcfunction(L, multiply_quat);
            return 1;
        }
        if (strcmp(key, "multiply_vec2") == 0) {
            lua_pushcfunction(L, multiply_vec2);
            return 1;
        }
        if (strcmp(key, "multiply_vec3") == 0) {
            lua_pushcfunction(L, multiply_vec3);
            return 1;
        }
        if (strcmp(key, "multiply_vec4") == 0) {
            lua_pushcfunction(L, multiply_vec4);
            return 1;
        }
        if (strcmp(key, "normalize_vec2") == 0) {
            lua_pushcfunction(L, normalize_vec2);
            return 1;
        }
        if (strcmp(key, "normalize_vec3") == 0) {
            lua_pushcfunction(L, normalize_vec3);
            return 1;
        }
        if (strcmp(key, "normalize_vec4") == 0) {
            lua_pushcfunction(L, normalize_vec4);
            return 1;
        }
        if (strcmp(key, "normalize_quat") == 0) {
            lua_pushcfunction(L, normalize_quat);
            return 1;
        }
        if (strcmp(key, "quat") == 0) {
            lua_pushcfunction(L, quat);
            return 1;
        }
        if (strcmp(key, "quat_look_at") == 0) {
            lua_pushcfunction(L, quat_look_at);
            return 1;
        }
        if (strcmp(key, "quat_look_at_rh") == 0) {
            lua_pushcfunction(L, quat_look_at_rh);
            return 1;
        }
        if (strcmp(key, "quat_look_at_lh") == 0) {
            lua_pushcfunction(L, quat_look_at_lh);
            return 1;
        }
        if (strcmp(key, "quat_to_mat4") == 0) {
            lua_pushcfunction(L, quat_to_mat4);
            return 1;
        }
        if (strcmp(key, "radians") == 0) {
            lua_pushcfunction(L, radians);
            return 1;
        }
        if (strcmp(key, "rotate") == 0) {
            lua_pushcfunction(L, rotate);
            return 1;
        }
        if (strcmp(key, "rotate_x") == 0) {
            lua_pushcfunction(L, rotate_x);
            return 1;
        }
        if (strcmp(key, "rotate_y") == 0) {
            lua_pushcfunction(L, rotate_y);
            return 1;
        }
        if (strcmp(key, "rotate_z") == 0) {
            lua_pushcfunction(L, rotate_z);
            return 1;
        }
        if (strcmp(key, "slerp") == 0) {
            lua_pushcfunction(L, slerp);
            return 1;
        }
        if (strcmp(key, "subtract_mat2") == 0) {
            lua_pushcfunction(L, subtract_mat2);
            return 1;
        }
        if (strcmp(key, "subtract_mat3") == 0) {
            lua_pushcfunction(L, subtract_mat3);
            return 1;
        }
        if (strcmp(key, "subtract_mat4") == 0) {
            lua_pushcfunction(L, subtract_mat4);
            return 1;
        }
        if (strcmp(key, "subtract_quat") == 0) {
            lua_pushcfunction(L, subtract_quat);
            return 1;
        }
        if (strcmp(key, "subtract_vec2") == 0) {
            lua_pushcfunction(L, subtract_vec2);
            return 1;
        }
        if (strcmp(key, "subtract_vec3") == 0) {
            lua_pushcfunction(L, subtract_vec3);
            return 1;
        }
        if (strcmp(key, "subtract_vec4") == 0) {
            lua_pushcfunction(L, subtract_vec4);
            return 1;
        }
        if (strcmp(key, "translate") == 0) {
            lua_pushcfunction(L, translate);
            return 1;
        }
        if (strcmp(key, "vec2") == 0) {
            lua_pushcfunction(L, vec2);
            return 1;
        }
        if (strcmp(key, "vec3") == 0) {
            lua_pushcfunction(L, vec3);
            return 1;
        }
        if (strcmp(key, "vec4") == 0) {
            lua_pushcfunction(L, vec4);
            return 1;
        }
        ST_LOG_WARNING("Could not resolve index [{}]", key);
        return 0;
    }

    int LuaGLMBinding::index_mat3(lua_State* L) {
        constexpr i32 key_index = -1;
        ST_ASSERT(lua_type(L, key_index) == LUA_TSTRING, "Key must be at expected stack location");

        const char* key = lua_tostring(L, key_index);
        ST_ASSERT(strlen(key) > 0, "Key cannot be empty");

        if (strcmp(key, "toQuat") == 0) {
            lua_pushcfunction(L, mat3_to_quat);
            return 1;
        }
        return 0;
    }

    int LuaGLMBinding::index_mat4(lua_State* L) {
        constexpr i32 key_index = -1;
        ST_ASSERT(lua_type(L, key_index) == LUA_TSTRING, "Key must be at expected stack location");

        const char* key = lua_tostring(L, key_index);
        ST_ASSERT(strlen(key) > 0, "Key cannot be empty");

        if (strcmp(key, "toQuat") == 0) {
            lua_pushcfunction(L, mat4_to_quat);
            return 1;
        }
        return 0;
    }

    int LuaGLMBinding::index_quat(lua_State* L) {
        constexpr i32 key_index = -1;
        ST_ASSERT(lua_type(L, key_index) == LUA_TSTRING, "Key must be at expected stack location");

        const char* key = lua_tostring(L, key_index);
        ST_ASSERT(strlen(key) > 0, "Key cannot be empty");

        if (strcmp(key, "inverse") == 0) {
            lua_pushcfunction(L, inverse_quat);
            return 1;
        }
        if (strcmp(key, "normalize") == 0) {
            lua_pushcfunction(L, normalize_quat);
            return 1;
        }
        if (strcmp(key, "toMat4") == 0) {
            lua_pushcfunction(L, quat_to_mat4);
            return 1;
        }
        return 0;
    }

    int LuaGLMBinding::index_vec2(lua_State* L) {
        constexpr i32 key_index = -1;
        ST_ASSERT(lua_type(L, key_index) == LUA_TSTRING, "Key must be at expected stack location");

        const char* key = lua_tostring(L, key_index);
        ST_ASSERT(strlen(key) > 0, "Key cannot be empty");

        if (strcmp(key, "normalize") == 0) {
            lua_pushcfunction(L, normalize_vec2);
            return 1;
        }
        return 0;
    }

    int LuaGLMBinding::index_vec3(lua_State* L) {
        constexpr i32 key_index = -1;
        ST_ASSERT(lua_type(L, key_index) == LUA_TSTRING, "Key must be at expected stack location");

        const char* key = lua_tostring(L, key_index);
        ST_ASSERT(strlen(key) > 0, "Key cannot be empty");

        if (strcmp(key, "normalize") == 0) {
            lua_pushcfunction(L, normalize_vec3);
            return 1;
        }
        return 0;
    }

    int LuaGLMBinding::index_vec4(lua_State* L) {
        constexpr i32 key_index = -1;
        ST_ASSERT(lua_type(L, key_index) == LUA_TSTRING, "Key must be at expected stack location");

        const char* key = lua_tostring(L, key_index);
        ST_ASSERT(strlen(key) > 0, "Key cannot be empty");

        if (strcmp(key, "normalize") == 0) {
            lua_pushcfunction(L, normalize_vec4);
            return 1;
        }
        return 0;
    }

    // Lua stack
    // - [-1] table or number    Vector2 B or scalar B
    // - [-2] table or number    Vector2 A or scalar A
    int LuaGLMBinding::add_vec2(lua_State* L) {
        bool b_is_vector = lua_istable(L, -1);
        bool b_is_scalar = lua_isnumber(L, -1);

        bool a_is_vector = lua_istable(L, -2);
        bool a_is_scalar = lua_isnumber(L, -2);

        ST_ASSERT(b_is_vector || b_is_scalar, "b_is_vector || b_is_scalar");
        ST_ASSERT(a_is_vector || a_is_scalar, "a_is_vector || a_is_scalar");
        ST_ASSERT(b_is_vector || a_is_vector, "b_is_vector || a_is_vector");

        glm::vec2 vector_b{};
        if (b_is_vector) {
            vector_b = lua_tovec2(L, -1);
        }
        float scalar_b = 0.0f;
        if (b_is_scalar) {
            scalar_b = (float) lua_tonumber(L, -1);
        }

        glm::vec2 vector_a{};
        if (a_is_vector) {
            vector_a = lua_tovec2(L, -2);
        }
        float scalar_a = 0.0f;
        if (a_is_scalar) {
            scalar_a = (float) lua_tonumber(L, -2);
        }

        glm::vec2 result{};
        if (a_is_vector && b_is_vector) {
            result = vector_a + vector_b;
        }
        if (a_is_vector && b_is_scalar) {
            result = vector_a + scalar_b;
        }
        if (a_is_scalar && b_is_vector) {
            result = scalar_a + vector_b;
        }
        lua_pushvec2(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector3 B or scalar B
    // - [-2] table or number    Vector3 A or scalar A
    int LuaGLMBinding::add_vec3(lua_State* L) {
        bool b_is_vector = lua_istable(L, -1);
        bool b_is_scalar = lua_isnumber(L, -1);

        bool a_is_vector = lua_istable(L, -2);
        bool a_is_scalar = lua_isnumber(L, -2);

        ST_ASSERT(b_is_vector || b_is_scalar, "b_is_vector || b_is_scalar");
        ST_ASSERT(a_is_vector || a_is_scalar, "a_is_vector || a_is_scalar");
        ST_ASSERT(b_is_vector || a_is_vector, "b_is_vector || a_is_vector");

        glm::vec3 vector_b{};
        if (b_is_vector) {
            vector_b = lua_tovec3(L, -1);
        }
        float scalar_b = 0.0f;
        if (b_is_scalar) {
            scalar_b = (float) lua_tonumber(L, -1);
        }

        glm::vec3 vector_a{};
        if (a_is_vector) {
            vector_a = lua_tovec3(L, -2);
        }
        float scalar_a = 0.0f;
        if (a_is_scalar) {
            scalar_a = (float) lua_tonumber(L, -2);
        }

        glm::vec3 result{};
        if (a_is_vector && b_is_vector) {
            result = vector_a + vector_b;
        }
        if (a_is_vector && b_is_scalar) {
            result = vector_a + scalar_b;
        }
        if (a_is_scalar && b_is_vector) {
            result = scalar_a + vector_b;
        }
        lua_pushvec3(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector4 B or scalar B
    // - [-2] table or number    Vector4 A or scalar A
    int LuaGLMBinding::add_vec4(lua_State* L) {
        bool b_is_vector = lua_istable(L, -1);
        bool b_is_scalar = lua_isnumber(L, -1);

        bool a_is_vector = lua_istable(L, -2);
        bool a_is_scalar = lua_isnumber(L, -2);

        ST_ASSERT(b_is_vector || b_is_scalar, "b_is_vector || b_is_scalar");
        ST_ASSERT(a_is_vector || a_is_scalar, "a_is_vector || a_is_scalar");
        ST_ASSERT(b_is_vector || a_is_vector, "b_is_vector || a_is_vector");

        glm::vec4 vector_b{};
        if (b_is_vector) {
            vector_b = lua_tovec4(L, -1);
        }
        float scalar_b = 0.0f;
        if (b_is_scalar) {
            scalar_b = (float) lua_tonumber(L, -1);
        }

        glm::vec4 vector_a{};
        if (a_is_vector) {
            vector_a = lua_tovec4(L, -1);
        }
        float scalar_a = 0.0f;
        if (a_is_scalar) {
            scalar_a = (float) lua_tonumber(L, -2);
        }

        glm::vec4 result;
        if (a_is_vector && b_is_vector) {
            result = vector_a + vector_b;
        }
        if (a_is_vector && b_is_scalar) {
            result = vector_a + scalar_b;
        }
        if (a_is_scalar && b_is_vector) {
            result = scalar_a + vector_b;
        }
        lua_pushvec4(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table     Mat2 B
    // - [-2] table     Mat2 A
    int LuaGLMBinding::add_mat2(lua_State* L) {
        glm::mat2 matrix_b = lua_tomat2(L, -1);
        glm::mat2 matrix_a = lua_tomat2(L, -2);
        glm::mat2 result = matrix_a + matrix_b;
        lua_pushmat2(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat3 B
    // - [-2] table     Mat3 A
    int LuaGLMBinding::add_mat3(lua_State* L) {
        glm::mat3 matrix_b = lua_tomat3(L, -1);
        glm::mat3 matrix_a = lua_tomat3(L, -2);
        glm::mat3 result = matrix_a + matrix_b;
        lua_pushmat3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat4 B
    // - [-2] table     Mat4 A
    int LuaGLMBinding::add_mat4(lua_State* L) {
        glm::mat4 matrix_b = lua_tomat4(L, -1);
        glm::mat4 matrix_a = lua_tomat4(L, -2);
        glm::mat4 result = matrix_a + matrix_b;
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Quat B
    // - [-2] table     Quat A
    int LuaGLMBinding::add_quat(lua_State* L) {
        glm::quat quat_b = lua_toquat(L, -1);
        glm::quat quat_a = lua_toquat(L, -2);
        glm::quat result = quat_a + quat_b;
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Axis vector3
    // - [-2] number    Angle (radians)
    int LuaGLMBinding::angle_axis(lua_State* L) {
        glm::vec3 axis = lua_tovec3(L, -1);
        float angle = (float) lua_tonumber(L, -2);
        glm::quat result = glm::angleAxis(angle, axis);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table or string     Vec2 (self) or suffix string
    // - [-2] table or string     Vec2 (self) or prefix string
    int LuaGLMBinding::concat_vec2(lua_State* L) {
        bool concat_prefix = lua_type(L, -2) == LUA_TSTRING && lua_type(L, -1) == LUA_TTABLE;
        bool concat_suffix = lua_type(L, -1) == LUA_TSTRING && lua_type(L, -2) == LUA_TTABLE;
        ST_ASSERT(concat_prefix || concat_suffix, "Cannot concat [" << vec2_metatable_name << "] without a prefix or a suffix string");
        if (concat_prefix) {
            glm::vec2 self = lua_tovec2(L, -1);
            const char* str = lua_tostring(L, -2);
            lua_pushfstring(L, "%sx: %d, y: %d", str, self.x, self.y);
        } else {
            glm::vec2 self = lua_tovec2(L, -2);
            const char* str = lua_tostring(L, -1);
            lua_pushfstring(L, "x: %d, y: %d%s", self.x, self.y, str);
        }
        return 1;
    }

    // Lua stack
    // - [-1] table or string     Vec2 (self) or suffix string
    // - [-2] table or string     Vec2 (self) or prefix string
    int LuaGLMBinding::concat_vec3(lua_State* L) {
        bool concat_prefix = lua_type(L, -2) == LUA_TSTRING && lua_type(L, -1) == LUA_TTABLE;
        bool concat_suffix = lua_type(L, -1) == LUA_TSTRING && lua_type(L, -2) == LUA_TTABLE;
        ST_ASSERT(concat_prefix || concat_suffix, "Cannot concat [" << vec3_metatable_name << "] without a prefix or a suffix string");
        if (concat_prefix) {
            glm::vec3 self = lua_tovec3(L, -1);
            const char* str = lua_tostring(L, -2);
            lua_pushfstring(L, "%sx: %d, y: %d, z: %d", str, self.x, self.y, self.z);
        } else {
            glm::vec3 self = lua_tovec3(L, -2);
            const char* str = lua_tostring(L, -1);
            lua_pushfstring(L, "x: %d, y: %d, z: %d%s", self.x, self.y, self.z, str);
        }
        return 1;
    }

    // Lua stack
    // - [-1] table or string     Vec2 (self) or suffix string
    // - [-2] table or string     Vec2 (self) or prefix string
    int LuaGLMBinding::concat_vec4(lua_State* L) {
        bool concat_prefix = lua_type(L, -2) == LUA_TSTRING && lua_type(L, -1) == LUA_TTABLE;
        bool concat_suffix = lua_type(L, -1) == LUA_TSTRING && lua_type(L, -2) == LUA_TTABLE;
        ST_ASSERT(concat_prefix || concat_suffix, "Cannot concat [" << vec4_metatable_name << "] without a prefix or a suffix string");
        if (concat_prefix) {
            glm::vec4 self = lua_tovec4(L, -1);
            const char* str = lua_tostring(L, -2);
            lua_pushfstring(L, "%sx: %d, y: %d, z: %d, w: %d", str, self.x, self.y, self.z, self.w);
        } else {
            glm::vec4 self = lua_tovec4(L, -2);
            const char* str = lua_tostring(L, -1);
            lua_pushfstring(L, "x: %d, y: %d, z: %d, w: %d%s", self.x, self.y, self.z, self.w, str);
        }
        return 1;
    }

    // Lua stack
    // - [-1] table     Vector B
    // - [-2] table     Vector A
    int LuaGLMBinding::cross(lua_State* L) {
        glm::vec3 vector_a = lua_tovec3(L, -1);
        glm::vec3 vector_b = lua_tovec3(L, -2);
        glm::vec3 result = glm::cross(vector_a, vector_b);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec3
    int LuaGLMBinding::degrees(lua_State* L) {
        glm::vec3 vector = lua_tovec3(L, -1);
        glm::vec3 result = glm::degrees(vector);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector2 B or scalar B
    // - [-2] table or number    Vector2 A or scalar A
    int LuaGLMBinding::divide_vec2(lua_State* L) {
        bool b_is_vector = lua_istable(L, -1);
        bool b_is_scalar = lua_isnumber(L, -1);

        bool a_is_vector = lua_istable(L, -2);
        bool a_is_scalar = lua_isnumber(L, -2);

        ST_ASSERT(b_is_vector || b_is_scalar, "b_is_vector || b_is_scalar");
        ST_ASSERT(a_is_vector || a_is_scalar, "a_is_vector || a_is_scalar");
        ST_ASSERT(b_is_vector || a_is_vector, "b_is_vector || aIsVector");

        glm::vec2 vector_b{};
        if (b_is_vector) {
            vector_b = lua_tovec2(L, -1);
        }
        float scalar_b = 0.0f;
        if (b_is_scalar) {
            scalar_b = (float) lua_tonumber(L, -1);
        }

        glm::vec2 vector_a{};
        if (a_is_vector) {
            vector_a = lua_tovec2(L, -2);
        }
        float scalar_a = 0.0f;
        if (a_is_scalar) {
            scalar_a = (float) lua_tonumber(L, -2);
        }

        glm::vec2 result{};
        if (a_is_vector && b_is_vector) {
            result = vector_a / vector_b;
        }
        if (a_is_vector && b_is_scalar) {
            result = vector_a / scalar_b;
        }
        if (a_is_scalar && b_is_vector) {
            result = scalar_a / vector_b;
        }
        lua_pushvec2(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector3 B or scalar B
    // - [-2] table or number    Vector3 A or scalar A
    int LuaGLMBinding::divide_vec3(lua_State* L) {
        bool b_is_vector = lua_istable(L, -1);
        bool b_is_scalar = lua_isnumber(L, -1);

        bool a_is_vector = lua_istable(L, -2);
        bool a_is_scalar = lua_isnumber(L, -2);

        ST_ASSERT(b_is_vector || b_is_scalar, "b_is_vector || b_is_scalar");
        ST_ASSERT(a_is_vector || a_is_scalar, "a_is_vector || a_is_scalar");
        ST_ASSERT(b_is_vector || a_is_vector, "b_is_vector || aIsVector");

        glm::vec3 vector_b{};
        if (b_is_vector) {
            vector_b = lua_tovec3(L, -1);
        }
        float scalar_b = 0.0f;
        if (b_is_scalar) {
            scalar_b = (float) lua_tonumber(L, -1);
        }

        glm::vec3 vector_a{};
        if (a_is_vector) {
            vector_a = lua_tovec3(L, -2);
        }
        float scalar_a = 0.0f;
        if (a_is_scalar) {
            scalar_a = (float) lua_tonumber(L, -2);
        }

        glm::vec3 result{};
        if (a_is_vector && b_is_vector) {
            result = vector_a / vector_b;
        }
        if (a_is_vector && b_is_scalar) {
            result = vector_a / scalar_b;
        }
        if (a_is_scalar && b_is_vector) {
            result = scalar_a / vector_b;
        }
        lua_pushvec3(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector4 B or scalar B
    // - [-2] table or number    Vector4 A or scalar A
    int LuaGLMBinding::divide_vec4(lua_State* L) {
        bool b_is_vector = lua_istable(L, -1);
        bool b_is_scalar = lua_isnumber(L, -1);

        bool a_is_vector = lua_istable(L, -2);
        bool a_is_scalar = lua_isnumber(L, -2);

        ST_ASSERT(b_is_vector || b_is_scalar, "b_is_vector || b_is_scalar");
        ST_ASSERT(a_is_vector || a_is_scalar, "a_is_vector || a_is_scalar");
        ST_ASSERT(b_is_vector || a_is_vector, "b_is_vector || aIsVector");

        glm::vec4 vector_b{};
        if (b_is_vector) {
            vector_b = lua_tovec4(L, -1);
        }
        float scalar_b = 0.0f;
        if (b_is_scalar) {
            scalar_b = (float) lua_tonumber(L, -1);
        }

        glm::vec4 vector_a{};
        if (a_is_vector) {
            vector_a = lua_tovec4(L, -1);
        }
        float scalar_a = 0.0f;
        if (a_is_scalar) {
            scalar_a = (float) lua_tonumber(L, -2);
        }

        glm::vec4 result;
        if (a_is_vector && b_is_vector) {
            result = vector_a / vector_b;
        }
        if (a_is_vector && b_is_scalar) {
            result = vector_a / scalar_b;
        }
        if (a_is_scalar && b_is_vector) {
            result = scalar_a / vector_b;
        }
        lua_pushvec4(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table     Mat2 B
    // - [-2] table     Mat2 A
    int LuaGLMBinding::divide_mat2(lua_State* L) {
        glm::mat2 matrix_b = lua_tomat2(L, -1);
        glm::mat2 matrix_a = lua_tomat2(L, -2);
        glm::mat2 result = matrix_a / matrix_b;
        lua_pushmat2(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat3 B
    // - [-2] table     Mat3 A
    int LuaGLMBinding::divide_mat3(lua_State* L) {
        glm::mat3 matrix_b = lua_tomat3(L, -1);
        glm::mat3 matrix_a = lua_tomat3(L, -2);
        glm::mat3 result = matrix_a / matrix_b;
        lua_pushmat3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat4 B
    // - [-2] table     Mat4 A
    int LuaGLMBinding::divide_mat4(lua_State* L) {
        glm::mat4 matrix_b = lua_tomat4(L, -1);
        glm::mat4 matrix_a = lua_tomat4(L, -2);
        glm::mat4 result = matrix_a / matrix_b;
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Quat B
    // - [-2] table     Quat A
    int LuaGLMBinding::divide_quat(lua_State* L) {
        glm::quat quat_b = lua_toquat(L, -1);
        glm::quat quat_a = lua_toquat(L, -2);

        // The division operation quat_a / quat_b is equivalent to multiplying quat_a by the inverse of quat_b
        glm::quat result = quat_a * glm::inverse(quat_b);

        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec2 B
    // - [-2] table     Vec2 A
    int LuaGLMBinding::dot_vec2(lua_State* L) {
        glm::vec2 vector_b = lua_tovec2(L, -1);
        glm::vec2 vector_a = lua_tovec2(L, -2);
        float result = glm::dot(vector_a, vector_b);
        lua_pushnumber(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec3 B
    // - [-2] table     Vec3 A
    int LuaGLMBinding::dot_vec3(lua_State* L) {
        glm::vec3 vector_b = lua_tovec3(L, -1);
        glm::vec3 vector_a = lua_tovec3(L, -2);
        float result = glm::dot(vector_a, vector_b);
        lua_pushnumber(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec4 B
    // - [-2] table     Vec4 A
    int LuaGLMBinding::dot_vec4(lua_State* L) {
        glm::vec4 vector_b = lua_tovec4(L, -1);
        glm::vec4 vector_a = lua_tovec4(L, -2);
        float result = glm::dot(vector_a, vector_b);
        lua_pushnumber(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Quaternion
    int LuaGLMBinding::euler_angles(lua_State* L) {
        glm::quat quaternion = lua_toquat(L, -1);
        glm::vec3 result = glm::eulerAngles(quaternion);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Quaternion
    int LuaGLMBinding::inverse_quat(lua_State* L) {
        glm::quat quaternion = lua_toquat(L, -1);
        glm::quat result = glm::inverse(quaternion);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Mat2
    int LuaGLMBinding::inverse_mat2(lua_State* L) {
        glm::mat2 matrix = lua_tomat2(L, -1);
        glm::mat2 result = glm::inverse(matrix);
        lua_pushmat2(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Mat3
    int LuaGLMBinding::inverse_mat3(lua_State* L) {
        glm::mat3 matrix = lua_tomat3(L, -1);
        glm::mat3 result = glm::inverse(matrix);
        lua_pushmat3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Mat4
    int LuaGLMBinding::inverse_mat4(lua_State* L) {
        glm::mat4 matrix = lua_tomat4(L, -1);
        glm::mat4 result = glm::inverse(matrix);
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec2
    int LuaGLMBinding::length_vec2(lua_State* L) {
        glm::vec2 vector = lua_tovec2(L, -1);
        float result = glm::length(vector);
        lua_pushnumber(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec3
    int LuaGLMBinding::length_vec3(lua_State* L) {
        glm::vec3 vector = lua_tovec3(L, -1);
        float result = glm::length(vector);
        lua_pushnumber(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec4
    int LuaGLMBinding::length_vec4(lua_State* L) {
        glm::vec4 vector = lua_tovec4(L, -1);
        float result = glm::length(vector);
        lua_pushnumber(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number    timestep
    // - [-2] table     End position vec3
    // - [-3] table     Start position vec3
    int LuaGLMBinding::lerp(lua_State* L) {
        float timestep = (float) lua_tonumber(L, -1);
        glm::vec3 end = lua_tovec3(L, -2);
        glm::vec3 start = lua_tovec3(L, -3);
        glm::vec3 result = glm::mix(start, end, timestep);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Up direction vec3
    // - [-2] table     Center vec3
    // - [-3] table     Eye vec3
    int LuaGLMBinding::look_at(lua_State* L) {
        glm::vec3 up_direction = lua_tovec3(L, -1);
        glm::vec3 center = lua_tovec3(L, -2);
        glm::vec3 eye = lua_tovec3(L, -3);
        glm::mat4 result = glm::lookAt(eye, center, up_direction);
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number   Scalar
    int LuaGLMBinding::mat2(lua_State* L) {
        float scalar = 0.0f;
        bool missing = lua_isnil(L, -1);
        if (!missing) {
            scalar = (float) lua_tonumber(L, -1);
        }
        glm::mat4 result(scalar);
        lua_pushmat2(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number   Scalar
    int LuaGLMBinding::mat3(lua_State* L) {
        float scalar = 0.0f;
        bool missing = lua_isnil(L, -1);
        if (!missing) {
            scalar = (float) lua_tonumber(L, -1);
        }
        glm::mat4 result(scalar);
        lua_pushmat3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number   Scalar
    int LuaGLMBinding::mat4(lua_State* L) {
        float scalar = 0.0f;
        bool missing = lua_isnil(L, -1);
        if (!missing) {
            scalar = (float) lua_tonumber(L, -1);
        }
        glm::mat4 result(scalar);
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Matrix 3x3
    int LuaGLMBinding::mat3_to_quat(lua_State* L) {
        glm::mat3 matrix = lua_tomat3(L, -1);
        glm::quat result = glm::toQuat(matrix);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Matrix 4x4
    int LuaGLMBinding::mat4_to_quat(lua_State* L) {
        glm::mat4 matrix = lua_tomat4(L, -1);
        glm::quat result = glm::toQuat(matrix);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector2 B or scalar B
    // - [-2] table or number    Vector2 A or scalar A
    int LuaGLMBinding::multiply_vec2(lua_State* L) {
        bool b_is_vector = lua_istable(L, -1);
        bool b_is_scalar = lua_isnumber(L, -1);

        bool a_is_vector = lua_istable(L, -2);
        bool a_is_scalar = lua_isnumber(L, -2);

        ST_ASSERT(b_is_vector || b_is_scalar, "b_is_vector || b_is_scalar");
        ST_ASSERT(a_is_vector || a_is_scalar, "a_is_vector || a_is_scalar");
        ST_ASSERT(b_is_vector || a_is_vector, "b_is_vector || aIsVector");

        glm::vec2 vector_b{};
        if (b_is_vector) {
            vector_b = lua_tovec2(L, -1);
        }
        float scalar_b = 0.0f;
        if (b_is_scalar) {
            scalar_b = (float) lua_tonumber(L, -1);
        }

        glm::vec2 vector_a{};
        if (a_is_vector) {
            vector_a = lua_tovec2(L, -2);
        }
        float scalar_a = 0.0f;
        if (a_is_scalar) {
            scalar_a = (float) lua_tonumber(L, -2);
        }

        glm::vec2 result{};
        if (a_is_vector && b_is_vector) {
            result = vector_a * vector_b;
        }
        if (a_is_vector && b_is_scalar) {
            result = vector_a * scalar_b;
        }
        if (a_is_scalar && b_is_vector) {
            result = scalar_a * vector_b;
        }
        lua_pushvec2(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector3 B or scalar B
    // - [-2] table or number    Vector3 A or scalar A
    int LuaGLMBinding::multiply_vec3(lua_State* L) {
        bool b_is_vector = lua_istable(L, -1);
        bool b_is_scalar = lua_isnumber(L, -1);

        bool a_is_vector = lua_istable(L, -2);
        bool a_is_scalar = lua_isnumber(L, -2);

        ST_ASSERT(b_is_vector || b_is_scalar, "b_is_vector || b_is_scalar");
        ST_ASSERT(a_is_vector || a_is_scalar, "a_is_vector || a_is_scalar");
        ST_ASSERT(b_is_vector || a_is_vector, "b_is_vector || aIsVector");

        glm::vec3 vector_b{};
        if (b_is_vector) {
            vector_b = lua_tovec3(L, -1);
        }
        float scalar_b = 0.0f;
        if (b_is_scalar) {
            scalar_b = (float) lua_tonumber(L, -1);
        }

        glm::vec3 vector_a{};
        if (a_is_vector) {
            vector_a = lua_tovec3(L, -2);
        }
        float scalar_a = 0.0f;
        if (a_is_scalar) {
            scalar_a = (float) lua_tonumber(L, -2);
        }

        glm::vec3 result{};
        if (a_is_vector && b_is_vector) {
            result = vector_a * vector_b;
        }
        if (a_is_vector && b_is_scalar) {
            result = vector_a * scalar_b;
        }
        if (a_is_scalar && b_is_vector) {
            result = scalar_a * vector_b;
        }
        lua_pushvec3(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector4 B or scalar B
    // - [-2] table or number    Vector4 A or scalar A
    int LuaGLMBinding::multiply_vec4(lua_State* L) {
        bool b_is_vector = lua_istable(L, -1);
        bool b_is_scalar = lua_isnumber(L, -1);

        bool a_is_vector = lua_istable(L, -2);
        bool a_is_scalar = lua_isnumber(L, -2);

        ST_ASSERT(b_is_vector || b_is_scalar, "b_is_vector || b_is_scalar");
        ST_ASSERT(a_is_vector || a_is_scalar, "a_is_vector || a_is_scalar");
        ST_ASSERT(b_is_vector || a_is_vector, "b_is_vector || aIsVector");

        glm::vec4 vector_b{};
        if (b_is_vector) {
            vector_b = lua_tovec4(L, -1);
        }
        float scalar_b = 0.0f;
        if (b_is_scalar) {
            scalar_b = (float) lua_tonumber(L, -1);
        }

        glm::vec4 vector_a{};
        if (a_is_vector) {
            vector_a = lua_tovec4(L, -1);
        }
        float scalar_a = 0.0f;
        if (a_is_scalar) {
            scalar_a = (float) lua_tonumber(L, -2);
        }

        glm::vec4 result;
        if (a_is_vector && b_is_vector) {
            result = vector_a * vector_b;
        }
        if (a_is_vector && b_is_scalar) {
            result = vector_a * scalar_b;
        }
        if (a_is_scalar && b_is_vector) {
            result = scalar_a * vector_b;
        }
        lua_pushvec4(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table     Mat2 B
    // - [-2] table     Mat2 A
    int LuaGLMBinding::multiply_mat2(lua_State* L) {
        glm::mat2 matrix_b = lua_tomat2(L, -1);
        glm::mat2 matrix_a = lua_tomat2(L, -2);
        glm::mat2 result = matrix_a * matrix_b;
        lua_pushmat2(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat3 B
    // - [-2] table     Mat3 A
    int LuaGLMBinding::multiply_mat3(lua_State* L) {
        glm::mat3 matrix_b = lua_tomat3(L, -1);
        glm::mat3 matrix_a = lua_tomat3(L, -2);
        glm::mat3 result = matrix_a * matrix_b;
        lua_pushmat3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat4 B
    // - [-2] table     Mat4 A
    int LuaGLMBinding::multiply_mat4(lua_State* L) {
        glm::mat4 matrix_b = lua_tomat4(L, -1);
        glm::mat4 matrix_a = lua_tomat4(L, -2);
        glm::mat4 result = matrix_a * matrix_b;
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Quat B
    // - [-2] table     Quat A
    int LuaGLMBinding::multiply_quat(lua_State* L) {
        glm::quat quat_b = lua_toquat(L, -1);
        glm::quat quat_a = lua_toquat(L, -2);
        glm::quat result = quat_a * quat_b;
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Quaternion
    int LuaGLMBinding::normalize_quat(lua_State* L) {
        glm::quat quaternion = lua_toquat(L, -1);
        glm::quat result = glm::normalize(quaternion);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec2
    int LuaGLMBinding::normalize_vec2(lua_State* L) {
        glm::vec2 vector = lua_tovec2(L, -1);
        glm::vec2 result = glm::normalize(vector);
        lua_pushvec2(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec3
    int LuaGLMBinding::normalize_vec3(lua_State* L) {
        glm::vec3 vector = lua_tovec3(L, -1);
        glm::vec3 result = glm::normalize(vector);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec4
    int LuaGLMBinding::normalize_vec4(lua_State* L) {
        glm::vec4 vector = lua_tovec4(L, -1);
        glm::vec4 result = glm::normalize(vector);
        lua_pushvec4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number     Z
    // - [-2] number     Y
    // - [-3] number     X
    // - [-4] number     W
    int LuaGLMBinding::quat(lua_State* L) {
        glm::quat result{};
        result.z = (float) lua_tonumber(L, -1);
        result.y = (float) lua_tonumber(L, -2);
        result.x = (float) lua_tonumber(L, -3);
        result.w = (float) lua_tonumber(L, -4);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Up direction vector3
    // - [-2] table     Forward direction vector3
    int LuaGLMBinding::quat_look_at(lua_State* L) {
        glm::vec3 up_direction = lua_tovec3(L, -1);
        glm::vec3 forward_direction = lua_tovec3(L, -2);
        glm::quat result = glm::quatLookAt(forward_direction, up_direction);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Up direction vector3
    // - [-2] table     Forward direction vector3
    int LuaGLMBinding::quat_look_at_rh(lua_State* L) {
        glm::vec3 up_direction = lua_tovec3(L, -1);
        glm::vec3 forward_direction = lua_tovec3(L, -2);
        glm::quat result = glm::quatLookAtRH(forward_direction, up_direction);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Up direction vector3
    // - [-2] table     Forward direction vector3
    int LuaGLMBinding::quat_look_at_lh(lua_State* L) {
        glm::vec3 up_direction = lua_tovec3(L, -1);
        glm::vec3 forward_direction = lua_tovec3(L, -2);
        glm::quat result = glm::quatLookAtLH(forward_direction, up_direction);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Quaternion
    int LuaGLMBinding::quat_to_mat4(lua_State* L) {
        glm::quat quaternion = lua_toquat(L, -1);
        glm::mat4 result = glm::toMat4(quaternion);
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number    Angle (degrees)
    int LuaGLMBinding::radians(lua_State* L) {
        float angle = (float) lua_tonumber(L, -1);
        float result = glm::radians(angle);
        lua_pushnumber(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     vector
    // - [-2] table     quaternion
    int LuaGLMBinding::rotate(lua_State* L) {
        glm::vec3 vector = lua_tovec3(L, -1);
        glm::quat quaternion = lua_toquat(L, -2);
        glm::vec3 result = glm::rotate(quaternion, vector);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number    Angle
    // - [-2] table     Vector3
    int LuaGLMBinding::rotate_x(lua_State* L) {
        auto angle = (float) lua_tonumber(L, -1);
        glm::vec3 vector = lua_tovec3(L, -2);
        glm::vec3 result = glm::rotateX(vector, angle);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number    Angle
    // - [-2] table     Vector3
    int LuaGLMBinding::rotate_y(lua_State* L) {
        auto angle = (float) lua_tonumber(L, -1);
        glm::vec3 vector = lua_tovec3(L, -2);
        glm::vec3 result = glm::rotateY(vector, angle);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number    Angle
    // - [-2] table     Vector3
    int LuaGLMBinding::rotate_z(lua_State* L) {
        auto angle = (float) lua_tonumber(L, -1);
        glm::vec3 vector = lua_tovec3(L, -2);
        glm::vec3 result = glm::rotateZ(vector, angle);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number    Timestep
    // - [-2] table     End quaternion
    // - [-3] table     Start quaternion
    int LuaGLMBinding::slerp(lua_State* L) {
        float timestep = (float) lua_tonumber(L, -1);
        glm::quat end = lua_toquat(L, -2);
        glm::quat start = lua_toquat(L, -3);
        glm::quat result = glm::slerp(start, end, timestep);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector2 B or scalar B
    // - [-2] table or number    Vector2 A or scalar A
    int LuaGLMBinding::subtract_vec2(lua_State* L) {
        bool b_is_vector = lua_istable(L, -1);
        bool b_is_scalar = lua_isnumber(L, -1);

        bool a_is_vector = lua_istable(L, -2);
        bool a_is_scalar = lua_isnumber(L, -2);

        ST_ASSERT(b_is_vector || b_is_scalar, "b_is_vector || b_is_scalar");
        ST_ASSERT(a_is_vector || a_is_scalar, "a_is_vector || a_is_scalar");
        ST_ASSERT(b_is_vector || a_is_vector, "b_is_vector || aIsVector");

        glm::vec2 vector_b{};
        if (b_is_vector) {
            vector_b = lua_tovec2(L, -1);
        }
        float scalar_b = 0.0f;
        if (b_is_scalar) {
            scalar_b = (float) lua_tonumber(L, -1);
        }

        glm::vec2 vector_a{};
        if (a_is_vector) {
            vector_a = lua_tovec2(L, -2);
        }
        float scalar_a = 0.0f;
        if (a_is_scalar) {
            scalar_a = (float) lua_tonumber(L, -2);
        }

        glm::vec2 result{};
        if (a_is_vector && b_is_vector) {
            result = vector_a - vector_b;
        }
        if (a_is_vector && b_is_scalar) {
            result = vector_a - scalar_b;
        }
        if (a_is_scalar && b_is_vector) {
            result = scalar_a - vector_b;
        }
        lua_pushvec2(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector3 B or scalar B
    // - [-2] table or number    Vector3 A or scalar A
    int LuaGLMBinding::subtract_vec3(lua_State* L) {
        bool b_is_vector = lua_istable(L, -1);
        bool b_is_scalar = lua_isnumber(L, -1);

        bool a_is_vector = lua_istable(L, -2);
        bool a_is_scalar = lua_isnumber(L, -2);

        ST_ASSERT(b_is_vector || b_is_scalar, "b_is_vector || b_is_scalar");
        ST_ASSERT(a_is_vector || a_is_scalar, "a_is_vector || a_is_scalar");
        ST_ASSERT(b_is_vector || a_is_vector, "b_is_vector || aIsVector");

        glm::vec3 vector_b{};
        if (b_is_vector) {
            vector_b = lua_tovec3(L, -1);
        }
        float scalar_b = 0.0f;
        if (b_is_scalar) {
            scalar_b = (float) lua_tonumber(L, -1);
        }

        glm::vec3 vector_a{};
        if (a_is_vector) {
            vector_a = lua_tovec3(L, -2);
        }
        float scalar_a = 0.0f;
        if (a_is_scalar) {
            scalar_a = (float) lua_tonumber(L, -2);
        }

        glm::vec3 result{};
        if (a_is_vector && b_is_vector) {
            result = vector_a - vector_b;
        }
        if (a_is_vector && b_is_scalar) {
            result = vector_a - scalar_b;
        }
        if (a_is_scalar && b_is_vector) {
            result = scalar_a - vector_b;
        }
        lua_pushvec3(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector4 B or scalar B
    // - [-2] table or number    Vector4 A or scalar A
    int LuaGLMBinding::subtract_vec4(lua_State* L) {
        bool b_is_vector = lua_istable(L, -1);
        bool b_is_scalar = lua_isnumber(L, -1);

        bool a_is_vector = lua_istable(L, -2);
        bool a_is_scalar = lua_isnumber(L, -2);

        ST_ASSERT(b_is_vector || b_is_scalar, "b_is_vector || b_is_scalar");
        ST_ASSERT(a_is_vector || a_is_scalar, "a_is_vector || a_is_scalar");
        ST_ASSERT(b_is_vector || a_is_vector, "b_is_vector || aIsVector");

        glm::vec4 vector_b{};
        if (b_is_vector) {
            vector_b = lua_tovec4(L, -1);
        }
        float scalar_b = 0.0f;
        if (b_is_scalar) {
            scalar_b = (float) lua_tonumber(L, -1);
        }

        glm::vec4 vector_a{};
        if (a_is_vector) {
            vector_a = lua_tovec4(L, -1);
        }
        float scalar_a = 0.0f;
        if (a_is_scalar) {
            scalar_a = (float) lua_tonumber(L, -2);
        }

        glm::vec4 result;
        if (a_is_vector && b_is_vector) {
            result = vector_a - vector_b;
        }
        if (a_is_vector && b_is_scalar) {
            result = vector_a - scalar_b;
        }
        if (a_is_scalar && b_is_vector) {
            result = scalar_a - vector_b;
        }
        lua_pushvec4(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table     Vec2 (self)
    int LuaGLMBinding::to_string_vec2(lua_State* L) {
        glm::vec2 self = lua_tovec2(L, -1);
        lua_pushfstring(L, "x: %d, y: %d", self.x, self.y);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec3 (self)
    int LuaGLMBinding::to_string_vec3(lua_State* L) {
        glm::vec3 self = lua_tovec3(L, -1);
        lua_pushfstring(L, "x: %d, y: %d, z: %d", self.x, self.y, self.z);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec4 (self)
    int LuaGLMBinding::to_string_vec4(lua_State* L) {
        glm::vec4 self = lua_tovec4(L, -1);
        lua_pushfstring(L, "x: %d, y: %d, z: %d, w: %d", self.x, self.y, self.z, self.w);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat2 B
    // - [-2] table     Mat2 A
    int LuaGLMBinding::subtract_mat2(lua_State* L) {
        glm::mat2 matrix_b = lua_tomat2(L, -1);
        glm::mat2 matrix_a = lua_tomat2(L, -2);
        glm::mat2 result = matrix_a - matrix_b;
        lua_pushmat2(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat3 B
    // - [-2] table     Mat3 A
    int LuaGLMBinding::subtract_mat3(lua_State* L) {
        glm::mat3 matrix_b = lua_tomat3(L, -1);
        glm::mat3 matrix_a = lua_tomat3(L, -2);
        glm::mat3 result = matrix_a - matrix_b;
        lua_pushmat3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat4 B
    // - [-2] table     Mat4 A
    int LuaGLMBinding::subtract_mat4(lua_State* L) {
        glm::mat4 matrix_b = lua_tomat4(L, -1);
        glm::mat4 matrix_a = lua_tomat4(L, -2);
        glm::mat4 result = matrix_a - matrix_b;
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Quat B
    // - [-2] table     Quat A
    int LuaGLMBinding::subtract_quat(lua_State* L) {
        glm::quat quat_b = lua_toquat(L, -1);
        glm::quat quat_a = lua_toquat(L, -2);
        glm::quat result = quat_a - quat_b;
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Vec3
    // - [-2] table    Mat4
    int LuaGLMBinding::translate(lua_State* L) {
        glm::vec3 vector = lua_tovec3(L, -1);
        glm::mat4 matrix = lua_tomat4(L, -2);
        glm::mat4 result = glm::translate(matrix, vector);
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec2 (self)
    int LuaGLMBinding::unary_minus_vec2(lua_State* L) {
        glm::vec2 self = lua_tovec2(L, -1);
        lua_pushvec2(L, -self);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec3 (self)
    int LuaGLMBinding::unary_minus_vec3(lua_State* L) {
        glm::vec3 self = lua_tovec3(L, -1);
        lua_pushvec3(L, -self);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec4 (self)
    int LuaGLMBinding::unary_minus_vec4(lua_State* L) {
        glm::vec4 self = lua_tovec4(L, -1);
        lua_pushvec4(L, -self);
        return 1;
    }

    // Lua stack
    //
    // glm.vec()
    // - Empty
    //
    // glm.vec({ x, y })
    // - [-1] table
    //
    // glm.vec(x, y)
    // - [-1] number    Y
    // - [-2] number    X
    int LuaGLMBinding::vec2(lua_State* L) {
        glm::vec2 vector{};
        uint32_t argument_count = lua_gettop(L);
        if (argument_count == 1) {
            vector = lua_tovec2(L, -1);
        }
        if (argument_count == 2) {
            vector.y = (float) lua_tonumber(L, -1);
            lua_pop(L, 1);

            vector.x = (float) lua_tonumber(L, -1);
            lua_pop(L, 1);
        }
        lua_pushvec2(L, vector);
        return 1;
    }

    // Lua stack
    //
    // glm.vec()
    // - Empty
    //
    // glm.vec({ x, y, z })
    // - [-1] table
    //
    // glm.vec(x, y, z)
    // - [-1] number    Z
    // - [-2] number    Y
    // - [-3] number    X
    int LuaGLMBinding::vec3(lua_State* L) {
        glm::vec3 vector{0, 0, 0};
        uint32_t argument_count = lua_gettop(L);
        if (argument_count == 1) {
            vector = lua_tovec3(L, -1);
        }
        if (argument_count == 3) {
            vector.z = (float) lua_tonumber(L, -1);
            lua_pop(L, 1);

            vector.y = (float) lua_tonumber(L, -1);
            lua_pop(L, 1);

            vector.x = (float) lua_tonumber(L, -1);
            lua_pop(L, 1);
        }
        lua_pushvec3(L, vector);
        return 1;
    }

    // Lua stack
    //
    // glm.vec()
    // - Empty
    //
    // glm.vec({ x, y, z, w })
    // - [-1] table
    //
    // glm.vec(x, y, z, w)
    // - [-1] number    W
    // - [-2] number    Z
    // - [-3] number    Y
    // - [-4] number    X
    int LuaGLMBinding::vec4(lua_State* L) {
        glm::vec4 vector{};
        uint32_t argument_count = lua_gettop(L);
        if (argument_count == 1) {
            vector = lua_tovec4(L, -1);
        }
        if (argument_count == 4) {
            vector.w = (float) lua_tonumber(L, -1);
            lua_pop(L, 1);

            vector.z = (float) lua_tonumber(L, -1);
            lua_pop(L, 1);

            vector.y = (float) lua_tonumber(L, -1);
            lua_pop(L, 1);

            vector.x = (float) lua_tonumber(L, -1);
            lua_pop(L, 1);
        }
        lua_pushvec4(L, vector);
        return 1;
    }
}

glm::vec2 lua_tovec2(lua_State* L, int index) {
    glm::vec2 vector{};

    lua_getfield(L, index, "x");
    vector.x = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "y");
    vector.y = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    return vector;
}

void lua_pushvec2(lua_State* L, const glm::vec2& vector) {
    lua_newtable(L);

    lua_pushnumber(L, vector.x);
    lua_setfield(L, -2, "x");

    lua_pushnumber(L, vector.y);
    lua_setfield(L, -2, "y");

    luaL_getmetatable(L, Storytime::LuaGLMBinding::vec2_metatable_name.c_str());
    lua_setmetatable(L, -2);
}

glm::vec3 lua_tovec3(lua_State* L, int index) {
    glm::vec3 vector{};

    lua_getfield(L, index, "x");
    vector.x = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "y");
    vector.y = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "z");
    vector.z = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    return vector;
}

void lua_pushvec3(lua_State* L, const glm::vec3& vector) {
    lua_newtable(L);

    lua_pushnumber(L, vector.x);
    lua_setfield(L, -2, "x");

    lua_pushnumber(L, vector.y);
    lua_setfield(L, -2, "y");

    lua_pushnumber(L, vector.z);
    lua_setfield(L, -2, "z");

    luaL_getmetatable(L, Storytime::LuaGLMBinding::vec3_metatable_name.c_str());
    lua_setmetatable(L, -2);
}

glm::vec4 lua_tovec4(lua_State* L, int index) {
    glm::vec4 vector{};

    lua_getfield(L, index, "x");
    vector.x = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "y");
    vector.y = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "z");
    vector.z = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "w");
    vector.w = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    return vector;
}

void lua_pushvec4(lua_State* L, const glm::vec4& vector) {
    lua_newtable(L);

    lua_pushnumber(L, vector.x);
    lua_setfield(L, -2, "x");

    lua_pushnumber(L, vector.y);
    lua_setfield(L, -2, "y");

    lua_pushnumber(L, vector.z);
    lua_setfield(L, -2, "z");

    lua_pushnumber(L, vector.w);
    lua_setfield(L, -2, "w");

    luaL_getmetatable(L, Storytime::LuaGLMBinding::vec4_metatable_name.c_str());
    lua_setmetatable(L, -2);
}

glm::mat2 lua_tomat2(lua_State* L, int index) {
    glm::mat2 matrix{};
    for (int i = 0; i < 2; ++i) {
        lua_geti(L, index, i + 1); // Lua uses 1-based indexing

        lua_getfield(L, -1, "x");
        matrix[i].x = (float) lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "y");
        matrix[i].y = (float) lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pop(L, 1);
    }
    return matrix;
}

void lua_pushmat2(lua_State* L, const glm::mat2& matrix) {
    lua_newtable(L);
    for (uint8_t i = 0; i < 2; ++i) {
        lua_newtable(L);
        lua_pushnumber(L, matrix[i].x);
        lua_setfield(L, -2, "x");
        lua_pushnumber(L, matrix[i].y);
        lua_setfield(L, -2, "y");
        luaL_getmetatable(L, Storytime::LuaGLMBinding::vec2_metatable_name.c_str());
        lua_setmetatable(L, -2);
        lua_seti(L, -2, i + 1); // Lua uses 1-based indexing
    }
    luaL_getmetatable(L, Storytime::LuaGLMBinding::mat2_metatable_name.c_str());
    lua_setmetatable(L, -2);
}

glm::mat3 lua_tomat3(lua_State* L, int index) {
    glm::mat3 matrix{};
    for (int i = 0; i < 3; ++i) {
        lua_geti(L, index, i + 1); // Lua uses 1-based indexing

        lua_getfield(L, -1, "x");
        matrix[i].x = (float) lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "y");
        matrix[i].y = (float) lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "z");
        matrix[i].z = (float) lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pop(L, 1);
    }
    return matrix;
}

void lua_pushmat3(lua_State* L, const glm::mat3& matrix) {
    lua_newtable(L);
    for (uint8_t i = 0; i < 3; ++i) {
        lua_newtable(L);
        lua_pushnumber(L, matrix[i].x);
        lua_setfield(L, -2, "x");
        lua_pushnumber(L, matrix[i].y);
        lua_setfield(L, -2, "y");
        lua_pushnumber(L, matrix[i].z);
        lua_setfield(L, -2, "z");
        luaL_getmetatable(L, Storytime::LuaGLMBinding::vec3_metatable_name.c_str());
        lua_setmetatable(L, -2);
        lua_seti(L, -2, i + 1); // Lua uses 1-based indexing
    }
    luaL_getmetatable(L, Storytime::LuaGLMBinding::mat3_metatable_name.c_str());
    lua_setmetatable(L, -2);
}

glm::mat4 lua_tomat4(lua_State* L, int index) {
    glm::mat4 matrix{};
    for (int i = 0; i < 4; ++i) {
        lua_geti(L, index, i + 1); // Lua uses 1-based indexing

        lua_getfield(L, -1, "x");
        matrix[i].x = (float) lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "y");
        matrix[i].y = (float) lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "z");
        matrix[i].z = (float) lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "w");
        matrix[i].w = (float) lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pop(L, 1);
    }
    return matrix;
}

void lua_pushmat4(lua_State* L, const glm::mat4& matrix) {
    lua_newtable(L);
    for (uint8_t i = 0; i < 4; ++i) {
        lua_newtable(L);
        lua_pushnumber(L, matrix[i].x);
        lua_setfield(L, -2, "x");
        lua_pushnumber(L, matrix[i].y);
        lua_setfield(L, -2, "y");
        lua_pushnumber(L, matrix[i].z);
        lua_setfield(L, -2, "z");
        lua_pushnumber(L, matrix[i].w);
        lua_setfield(L, -2, "w");
        luaL_getmetatable(L, Storytime::LuaGLMBinding::vec4_metatable_name.c_str());
        lua_setmetatable(L, -2);
        lua_seti(L, -2, i + 1); // Lua uses 1-based indexing
    }
    luaL_getmetatable(L, Storytime::LuaGLMBinding::mat4_metatable_name.c_str());
    lua_setmetatable(L, -2);
}

glm::quat lua_toquat(lua_State* L, int index) {
    glm::quat quaternion{};

    lua_getfield(L, index, "x");
    quaternion.x = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "y");
    quaternion.y = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "z");
    quaternion.z = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "w");
    quaternion.w = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    return quaternion;
}

void lua_pushquat(lua_State* L, const glm::quat& quaternion) {
    lua_newtable(L);
    lua_pushnumber(L, quaternion.x);
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, quaternion.y);
    lua_setfield(L, -2, "y");
    lua_pushnumber(L, quaternion.z);
    lua_setfield(L, -2, "z");
    lua_pushnumber(L, quaternion.w);
    lua_setfield(L, -2, "w");
    luaL_getmetatable(L, Storytime::LuaGLMBinding::quat_metatable_name.c_str());
    lua_setmetatable(L, -2);
}
