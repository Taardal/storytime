#pragma once

struct LuaRef {
    i32 ref = LUA_NOREF;

    LuaRef(lua_State* L);

    LuaRef(i32 ref);

    operator i32() const;

    bool is_valid() const;

    i32 push_to_stack(lua_State* L) const;
};
