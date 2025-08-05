#include "st_lua_table.h"

namespace Storytime {
    LuaTable::LuaTable(lua_State* L)
        : L(L), ref(luaL_ref(L, LUA_REGISTRYINDEX)), global_name("") {
    }

    LuaTable::LuaTable(lua_State* L, const LuaRef ref)
        : L(L), ref(ref), global_name("") {
    }

    LuaTable::LuaTable(lua_State* L, const std::string& global_name)
        : L(L), ref(LUA_NOREF), global_name(global_name) {
    }

    LuaRef LuaTable::get_ref() const {
        return ref;
    }

    LuaFunction LuaTable::get_function(const std::string& key) const {
        push_self();
        lua_getfield(L, -1, key.c_str());
        LuaFunction lua_function(L);
        return lua_function;
    }

    bool LuaTable::has_field(const std::string& key) const {
        push_self();
        lua_getfield(L, -1, key.c_str());
        bool has_field = lua_type(L, -1) != LUA_TNIL;
        lua_pop(L, 2);
        return has_field;
    }

    LuaTable::GetterSetter LuaTable::operator[](const std::string& key) {
        return GetterSetter(this, key);
    }

    void LuaTable::push_self() const {
        if (ref != LUA_NOREF) {
            ref.push(L);
        } else if (global_name.length() > 0) {
            lua_getglobal(L, global_name.c_str());
        } else {
            ST_THROW("Could not push table to Lua stack, neither ref or global name is valid");
        }
    }

}