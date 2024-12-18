#pragma once

#include "lua_serialize_deserialize.h"
#include "lua_utils.h"

namespace Storytime {
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
}

template<>
inline void to_lua(lua_State* L, Storytime::LuaRef value) {
    value.push(L);
}

template<>
inline void to_lua(lua_State* L, Storytime::LuaRef& value) {
    value.push(L);
}
