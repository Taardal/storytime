#include "lua_table.h"

namespace Storytime {
    LuaTable::LuaTable(lua_State* L, const std::string& name) : L(L), name(name), ref(LUA_NOREF) {
    }

    LuaTable::LuaTable(lua_State* L, const LuaRef ref) : L(L), name(""), ref(ref) {
    }

    LuaRef LuaTable::get_ref() const {
        return ref;
    }

    LuaTable::GetterSetter LuaTable::operator[](const std::string& key) {
        return GetterSetter(this, key);
    }

    void LuaTable::push_self() const {
        if (ref != 0) {
            int type = lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
            ST_ASSERT(type == LUA_TTABLE, "Invalid ref: Lua table ref must point to item of type table");
        } else {
            ST_ASSERT(name.length() > 0, "Invalid name: Global Lua table name must not be empty");
            lua_getglobal(L, name.c_str());
        }
    }

}