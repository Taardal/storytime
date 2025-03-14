#include "lua_function.h"

namespace Storytime {
    LuaFunction::LuaFunction(lua_State* L) : L(L), name(""), ref(LUA_NOREF) {
    }

    LuaFunction::LuaFunction(lua_State* L, const std::string& name) : L(L), name(name), ref(LUA_NOREF) {
    // Check if the name is a single name or a dot-separated path into one-or-more tables
    char path_delimiter = '.';

    // If the name is a single name, assert that it is a global function and use the name to invoke it later
    if (name.find(path_delimiter) == std::string::npos) {
        lua_getglobal(L, name.c_str());
        ST_ASSERT(lua_isfunction(L, -1), "Lua function name does not point to a global Lua function");
        lua_pop(L, 1); // Cleanup
        return;
    }

    // If the name is a dot-separated path into one-or-more tables (i.e. "SomeTable.someOtherTable.theFunction"),
    // split the name by the delimiter and get each segment onto the Lua stack.
    std::string segment;
    std::vector<std::string> path_segments;
    std::stringstream ss(name);
    while (std::getline(ss, segment, path_delimiter)) {
        path_segments.push_back(segment);
    }
    for (int i = 0; i < path_segments.size(); ++i) {
        auto& path_segment = path_segments[i];
        if (i == 0) {
            lua_getglobal(L, path_segment.c_str());
            ST_ASSERT(lua_istable(L, -1), "Global variable [" << path_segment << "] is not a table");
        } else {
            lua_getfield(L, -1, path_segment.c_str());
        }
    }

    // At this point, the item at the top of the Lua stack should be the function.
    ST_ASSERT(lua_isfunction(L, -1), "Lua function must be at the top of the stack");

    // Obtain a reference to the function to invoke it with later
    ref = luaL_ref(L, LUA_REGISTRYINDEX);
    ST_ASSERT(ref != 0, "Reference to Lua function is invalid");

    // Cleanup
    lua_pop(L, lua_gettop(L));
}

LuaFunction::LuaFunction(lua_State* L, const LuaRef ref) : L(L), name(""), ref(ref) {
}

LuaRef LuaFunction::get_ref() const {
    return ref;
}

bool LuaFunction::is_valid() const {
    i32 type = LUA_TNONE;
    if (ref != LUA_NOREF) {
        type = lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
        lua_pop(L, 1);
    } else if (name.length() > 0) {
        type = lua_getglobal(L, name.c_str());
        lua_pop(L, 1);
    } else {
        type = lua_type(L, -1);
    }
    return type == LUA_TFUNCTION;
}

std::string LuaFunction::get_result_code_name(int pcall_result) {
    switch (pcall_result) {
        // Success
        case LUA_OK:
            return "LUA_OK";
        // Runtime error
        case LUA_ERRRUN:
            return "LUA_ERRRUN";
        // Memory allocation error (Lua will not call the message handler)
        case LUA_ERRMEM:
            return "LUA_ERRMEM";
        // Error while running the error message handler
        case LUA_ERRERR:
            return "LUA_ERRERR";
        default:
            return "";
    }
}

}