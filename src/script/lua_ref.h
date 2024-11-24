#pragma once

#include "lua_utils.h"

struct LuaRef {
    i32 ref = LUA_NOREF;

    LuaRef(lua_State* L);

    LuaRef(i32 ref);

    operator i32() const;

    bool is_valid() const;

    LuaType push(lua_State* L) const;

    void destroy(lua_State* L);

    static i32 create(lua_State* L, i32 index = -1);

    static void destroy(lua_State* L, i32 ref);
};
