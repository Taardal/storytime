#pragma once

#include "st_lua_to_from.h"
#include "st_lua_utils.h"

namespace Storytime {
    struct LuaRefConfig {
        bool keep_on_stack = false;
    };

    struct LuaRef {
        i32 ref = LUA_NOREF;

        LuaRef(lua_State* L, const LuaRefConfig& config = {});

        LuaRef(i32 ref);

        operator i32() const;

        bool is_valid() const;

        LuaType push(lua_State* L) const;

        void destroy(lua_State* L);

        static i32 create(lua_State* L, const LuaRefConfig& config = {});

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
