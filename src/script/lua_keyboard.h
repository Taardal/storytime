#pragma once

namespace Storytime {
    class LuaKeyboard {
    public:
        static const std::string table_name;
        static const std::string metatable_name;

    public:
        static i32 create_metatable(lua_State* L);

        static i32 create(lua_State* L);

    private:
        static int index(lua_State* L);

        static int is_pressed(lua_State* L);
    };
}
