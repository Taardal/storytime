#include "st_lua_mouse_binding.h"

namespace Storytime {
    const std::string LuaMouseBinding::metatable_name = "LuaMouseBinding";

    LuaMouseBinding::LuaMouseBinding(lua_State* L, Mouse* mouse) : L(L), mouse(mouse)  {
    }

    i32 LuaMouseBinding::create_metatable(lua_State* L) {
        luaL_newmetatable(L, metatable_name.c_str());

        lua_pushstring(L, "__gc");
        lua_pushcfunction(L, lua_destroy);
        lua_settable(L, -3);

        lua_pushstring(L, "__index");
        lua_pushcfunction(L, lua_index);
        lua_settable(L, -3);

        return 1;
    }

    i32 LuaMouseBinding::create(lua_State* L, Mouse* mouse) {
        ST_ASSERT(mouse != nullptr, "Mouse cannot be null");

        void* userdata = lua_newuserdata(L, sizeof(LuaMouseBinding));
        new (userdata) LuaMouseBinding(L, mouse);

        luaL_getmetatable(L, metatable_name.c_str());
        ST_ASSERT(!lua_isnil(L, -1), "Metatable [" << metatable_name.c_str() << "] cannot be null");
        lua_setmetatable(L, -2);

        return 1;
    }

    i32 LuaMouseBinding::lua_destroy(lua_State* L) {
        ST_ASSERT(lua_type(L, -1) == LUA_TUSERDATA, "Binding must be at expected stack location");

        auto binding = static_cast<LuaMouseBinding*>(lua_touserdata(L, -1));
        ST_ASSERT(binding != nullptr, "Binding cannot be null");

        binding->~LuaMouseBinding();
        return 0;
    }

    i32 LuaMouseBinding::lua_index(lua_State* L) {
        ST_ASSERT(lua_isstring(L, -1), "Index name must be at top of stack");

        const char* key = lua_tostring(L, -1);
        if (strcmp(key, "is_pressed") == 0) {
            lua_pushcfunction(L, LuaMouseBinding::lua_is_pressed);
            return 1;
        }

        MouseButtonCode button_code = MouseButton::from_name(key);
        if (button_code != MouseButton::NONE) {
            lua_pushnumber(L, button_code);
            return 1;
        }

        return 0;
    }

    i32 LuaMouseBinding::lua_is_pressed(lua_State* L) {
        i32 parameter_type = lua_type(L, -1);
        ST_ASSERT(parameter_type == LUA_TSTRING || parameter_type == LUA_TNUMBER, "Mouse button parameter must be either string or number");

        auto userdata = static_cast<LuaMouseBinding*>(lua_touserdata(L, -2));
        ST_ASSERT(userdata != nullptr, "Userdata cannot be null");

        bool pressed;
        if (lua_type(L, -1) == LUA_TSTRING) {
            std::string button_name = lua_tostring(L, -1);
            ST_ASSERT(button_name.length() > 0, "Mouse name cannot be empty");
            MouseButtonCode button_code = MouseButton::from_name(button_name);
            ST_ASSERT(button_code != MouseButton::NONE, "Invalid button name given");
            pressed = userdata->mouse->is_pressed(button_code);
        } else {
            MouseButtonCode button_code = lua_tonumber(L, -1);
            ST_ASSERT(button_code != MouseButton::NONE, "Invalid button name given");
            pressed = userdata->mouse->is_pressed(button_code);
        }
        lua_pushboolean(L, pressed);
        return 1;
    }
}
