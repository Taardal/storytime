#include "lua_mouse.h"
#include "window/user_input.h"

namespace Storytime {
    const std::string LuaMouse::metatable_name = "Mouse";

    i32 LuaMouse::create_metatable(lua_State* L) {
        luaL_newmetatable(L, metatable_name.c_str());

        lua_pushstring(L, "__index");
        lua_pushcfunction(L, index);
        lua_settable(L, -3);

        return 1;
    }

    i32 LuaMouse::create(lua_State* L) {
        lua_newtable(L);

        luaL_getmetatable(L, metatable_name.c_str());
        ST_ASSERT(!lua_isnil(L, -1), "Metatable [" << metatable_name.c_str() << "] cannot be null");
        lua_setmetatable(L, -2);

        return 1;
    }

    int LuaMouse::index(lua_State* L) {
        ST_ASSERT(lua_isstring(L, -1), "Index name must be at top of stack");
        std::string index_name = lua_tostring(L, -1);
        if (index_name == "is_pressed") {
            lua_pushcfunction(L, LuaMouse::is_pressed);
            return 1;
        }
        MouseButtonCode button_code = MouseButton::from_name(index_name);
        if (button_code != MouseButton::NONE) {
            lua_pushnumber(L, button_code);
            return 1;
        }
        ST_LOG_WARNING("Could not resolve index [{}]", index_name);
        return 0;
    }

    int LuaMouse::is_pressed(lua_State* L) {
        i32 parameter_type = lua_type(L, -1);
        ST_ASSERT(parameter_type == LUA_TSTRING || parameter_type == LUA_TNUMBER, "Mouse button parameter must be either string or number");
        bool pressed;
        if (lua_type(L, -1) == LUA_TSTRING) {
            std::string button_name = lua_tostring(L, -1);
            ST_ASSERT(button_name.length() > 0, "Mouse name cannot be empty");
            MouseButtonCode button_code = MouseButton::from_name(button_name);
            ST_ASSERT(button_code != MouseButton::NONE, "Invalid button name given");
            pressed = Mouse::is_pressed(button_code);
        } else {
            MouseButtonCode button_code = lua_tonumber(L, -1);
            ST_ASSERT(button_code != MouseButton::NONE, "Invalid button name given");
            pressed = Mouse::is_pressed(button_code);
        }
        lua_pushboolean(L, pressed);
        return 1;
    }
}
