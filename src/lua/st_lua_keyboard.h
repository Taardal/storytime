#pragma once

namespace Storytime {
    class LuaKeyboard {
    public:
        static const std::string metatable_name;

    public:
        static i32 create_metatable(lua_State* L);

        static i32 create(lua_State* L);

    private:
        static i32 index(lua_State* L);

        static i32 is_pressed(lua_State* L);
    };
}
