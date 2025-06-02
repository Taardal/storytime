#pragma once

#include "window/st_mouse.h"

namespace Storytime {
    class LuaMouse {
    public:
        static const std::string metatable_name;

    private:
        lua_State* L = nullptr;
        Mouse* mouse = nullptr;

    public:
        LuaMouse(lua_State* L, Mouse* mouse);

        static i32 create_metatable(lua_State* L);

        static i32 create(lua_State* L, Mouse* mouse);

    private:
        static int index(lua_State* L);

        static int is_pressed(lua_State* L);
    };
}
