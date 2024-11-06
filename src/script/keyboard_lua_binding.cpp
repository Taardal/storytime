#include "keyboard_lua_binding.h"
#include "window/user_input.h"

namespace Storytime {
    const std::string KeyboardLuaBinding::table_name = "key";
    const std::string KeyboardLuaBinding::metatable_name = table_name + "__meta";

    i32 KeyboardLuaBinding::create(lua_State* L) {
        lua_newtable(L);
        luaL_newmetatable(L, metatable_name.c_str());
        lua_pushstring(L, "__index");
        lua_pushcfunction(L, KeyboardLuaBinding::index);
        lua_settable(L, -3);
        lua_setmetatable(L, -2);
        i32 ref = luaL_ref(L, LUA_REGISTRYINDEX);
        ST_ASSERT(ref != 0, "Invalid Lua ref");
        return ref;
    }

    int KeyboardLuaBinding::index(lua_State* L) {
        ST_ASSERT(lua_isstring(L, -1), "Index name must be at top of stack");
        std::string index_name = lua_tostring(L, -1);
        if (index_name == "is_pressed") {
            lua_pushcfunction(L, KeyboardLuaBinding::is_pressed);
            return 1;
        }
        KeyCode key_code = Key::from_name(index_name);
        if (key_code != Key::NONE) {
            lua_pushnumber(L, key_code);
            return 1;
        }
        ST_LOG_WARNING("Could not resolve index [{}]", index_name);
        return 0;
    }

    int KeyboardLuaBinding::is_pressed(lua_State* L) {
        i32 parameter_type = lua_type(L, -1);
        ST_ASSERT(parameter_type == LUA_TSTRING || parameter_type == LUA_TNUMBER, "Key parameter must be either string or number");
        bool pressed;
        if (lua_type(L, -1) == LUA_TSTRING) {
            std::string key_name = lua_tostring(L, -1);
            ST_ASSERT(key_name.length() > 0, "Key name cannot be empty");
            KeyCode key_code = Key::from_name(key_name);
            ST_ASSERT(key_code != Key::NONE, "Invalid key name given");
            pressed = Keyboard::is_pressed(key_code);
        } else {
            KeyCode key_code = lua_tonumber(L, -1);
            ST_ASSERT(key_code != Key::NONE, "Invalid key name given");
            pressed = Keyboard::is_pressed(key_code);
        }
        lua_pushboolean(L, pressed);
        return 1;
    }
}
