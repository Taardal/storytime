#pragma once

#include "window/st_keyboard.h"

namespace Storytime {
    class LuaKeyboardBinding {
    public:
        static const std::string metatable_name;
    private:
        lua_State* L = nullptr;
        const Keyboard& keyboard;

    public:
        LuaKeyboardBinding(lua_State* L, const Keyboard& keyboard);

        static i32 create_metatable(lua_State* L);

        static i32 create(lua_State* L, const Keyboard& keyboard);

        static i32 lua_destroy(lua_State* L);

        static i32 lua_index(lua_State* L);

        static i32 lua_is_pressed(lua_State* L);
    };
}
