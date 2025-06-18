#pragma once

#include "window/st_keyboard.h"

namespace Storytime {
    class LuaKeyboardBinding {
    public:
        static const std::string metatable_name;
    private:
        lua_State* L = nullptr;
        Keyboard* keyboard = nullptr;

    public:
        LuaKeyboardBinding(lua_State* L, Keyboard* keyboard);

        static i32 create_metatable(lua_State* L);

        static i32 create(lua_State* L, Keyboard* keyboard);

    private:
        static i32 index(lua_State* L);

        static i32 is_pressed(lua_State* L);
    };
}
