#include "lua_utils.h"

std::string create_metatable_name(const std::string& prefix) {
    return prefix + ".meta";
}

void print_lua_stack(lua_State* L, const std::string& tag) {
    std::cout << "Lua stack";
    if (tag.length() > 0) {
        std::cout << " [" << tag << "]";
    }
    std::cout << std::endl;
    int stack_size = lua_gettop(L);
    if (stack_size == 0) {
        std::cout << "- Empty" << std::endl;
        return;
    }
    for (int i = -1; i >= -stack_size; --i) {
        int type = lua_type(L, i);
        ST_ASSERT(type > LUA_TNONE, "Invalid type on Lua stack: Type must be greater than [" << LUA_TNONE << "]");
        ST_ASSERT(type < LUA_NUMTYPES, "Invalid type on Lua stack: Type must be less than [" << LUA_NUMTYPES << "]");

        std::string type_name = lua_typename(L, type);
        ST_ASSERT(type_name.length() > 0, "Invalid type on Lua stack: Type name is empty");

        std::cout << "- [" << i << "] " << type_name;
        if (type == LUA_TBOOLEAN) {
            std::cout << " -> " << lua_toboolean(L, i);
        }
        if (type == LUA_TNUMBER) {
            std::cout << " -> " << lua_tonumber(L, i);
        }
        if (type == LUA_TSTRING) {
            std::cout << " -> " << lua_tostring(L, i);
        }
        if (type == LUA_TTABLE) {
            std::cout << " -> " << lua_table_to_string(L, i);
        }
        if (type == LUA_TUSERDATA) {
            std::string metatable_name = get_metatable_name(L, i);
            if (metatable_name.length() > 0) {
                std::cout << " -> " << metatable_name;
            }
        }
        std::cout << std::endl;
    }
    // Make sure the stack is unchanged after printing (detect code bugs)
    i32 current_stack_size = lua_gettop(L);
    ST_ASSERT(current_stack_size == stack_size,
              "Lua stack size after printing [" << current_stack_size <<
              "] must equal stack size from before printing [" << stack_size << "]");
}

// Format:
// - table{key: value_type -> value}
//
// Examples:
// - table{some_number: number -> 123}
// - table{some_string: string -> "foobar"}
// - table{some_function: function -> ...}
std::string lua_table_to_string(lua_State* L, const int table_index, const int max_depth) {
    i32 type = lua_type(L, table_index);
    const char* type_name = lua_typename(L, type);
    ST_ASSERT(type == LUA_TTABLE,
              "Cannot stringify Lua table: Item to stringify on stack index [" << table_index << "] is of type [" <<
              type_name << "]");

    std::stringstream ss;
    ss << "{";

    u32 count = 0;
    lua_pushnil(L); // First key

    while (lua_next(L, table_index - 1) != 0) {
        if (count > 0) {
            ss << ", ";
        }

        int key_index = -2;
        const char* key = lua_tostring(L, key_index);

        int value_index = -1;
        int value_type = lua_type(L, value_index);

        ss << key << ": " << lua_typename(L, value_type) << " -> ";
        if (value_type == LUA_TSTRING) {
            ss << lua_tostring(L, value_index);
        } else if (value_type == LUA_TNUMBER) {
            ss << lua_tonumber(L, value_index);
        } else if (value_type == LUA_TBOOLEAN) {
            ss << lua_toboolean(L, value_index);
        } else {
            ss << "...";
        }

        lua_pop(L, 1); // Remove value, keep key for next iteration
        count++;
        if (count >= max_depth) {
            lua_pop(L, 1); // Remove key when breaking early
            ss << ", ...";
            break;
        }
    }

    ss << "}";
    return ss.str();
}

std::string get_metatable_name(lua_State* L, const int index) {
    if (!lua_getmetatable(L, index)) {
        return "";
    }
    if (!lua_getfield(L, -1, "__name")) {
        return "";
    }
    if (!lua_isstring(L, -1)) {
        return "";
    }
    std::string name = lua_tostring(L, -1);
    lua_pop(L, 2);
    return name;
}

int lua_getbottom(lua_State* L) {
    return lua_gettop(L) * -1;
}

bool lua_isempty(lua_State* L) {
    return lua_gettop(L) == 0;
}
