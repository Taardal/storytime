#include "lua_ref.h"

LuaRef::LuaRef(lua_State* L) : ref(luaL_ref(L, LUA_REGISTRYINDEX)) {
}

LuaRef::LuaRef(i32 ref) : ref(ref) {
}

LuaRef::operator i32() const {
    return ref;
}

bool LuaRef::is_valid() const {
    return ref != LUA_NOREF;
}

i32 LuaRef::push_to_stack(lua_State* L) const {
    ST_ASSERT(ref != LUA_NOREF, "Invalid Lua ref");
    i32 type = lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
    ST_ASSERT(type > LUA_TNONE, "Lua ref type [" << type << "] must be greater than [" << LUA_TNONE << "]");
    ST_ASSERT(type < LUA_NUMTYPES, "Lua ref type [" << type << "] must be lower than [" << LUA_NUMTYPES << "]");
    return type;
}
