#include "lua_ref.h"

namespace Storytime {
    LuaRef::LuaRef(lua_State* L) : ref(create(L)) {
    }

    LuaRef::LuaRef(i32 ref) : ref(ref) {
    }

    LuaRef::operator i32() const {
        return ref;
    }

    bool LuaRef::is_valid() const {
        return ref != LUA_NOREF;
    }

    LuaType LuaRef::push(lua_State* L) const {
        ST_ASSERT(ref != LUA_NOREF, "Invalid Lua ref");
        i32 type = lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
        ST_ASSERT(type > LUA_TNONE, "Lua ref type [" << type << "] must be greater than [" << LUA_TNONE << "]");
        ST_ASSERT(type < LUA_NUMTYPES, "Lua ref type [" << type << "] must be lower than [" << LUA_NUMTYPES << "]");
        return type;
    }

    void LuaRef::destroy(lua_State* L) {
        destroy(L, ref);
        ref = LUA_NOREF;
    }

    i32 LuaRef::create(lua_State* L, i32 index) {
        if (index < -1) {
            lua_pushvalue(L, index);
        }
        return luaL_ref(L, LUA_REGISTRYINDEX);
    }

    void LuaRef::destroy(lua_State* L, i32 ref) {
        luaL_unref(L, LUA_REGISTRYINDEX, ref);
    }
}