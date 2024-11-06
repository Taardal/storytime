#pragma once

#include "lua_function.h"
#include "lua_table.h"

namespace Storytime {
    class LuaState {
    private:
        lua_State* L = nullptr;

    public:
        LuaState();

        LuaState(lua_State* L);

        operator lua_State*() const;

        void close() const;

        void open_standard_libraries() const;

        void file(const std::string& path) const;

        void script(const std::string& script) const;

        void add_package_path(const std::string& path) const;

        void add_c_package_path(const std::string& path) const;

        i32 require(const std::string& name) const;

        i32 create_ref() const;

        void destroy_ref(i32 ref) const;

        class GetterSetter {
        private:
            lua_State* L;
            std::string key;
            i32 ref;

        public:
            GetterSetter(lua_State* L, const std::string& key) : L(L), key(key), ref(LUA_NOREF) {
            }

            GetterSetter(lua_State* L, i32 ref) : L(L), key(""), ref(ref) {
            }

            template<typename T>
            operator T() const {
                i32 type = LUA_NOREF;
                bool use_ref = ref != LUA_NOREF;
                if (use_ref) {
                    type = lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
                } else {
                    lua_getglobal(L, key.c_str());
                    type = lua_type(L, -1);
                }
                ST_ASSERT(type > LUA_TNONE, "Lua " << (use_ref ? "ref" : "global") << " type [" << type << "] must be greater than LUA_TNONE [" << LUA_TNONE << "]");
                ST_ASSERT(type < LUA_NUMTYPES, "Lua " << (use_ref ? "ref" : "global") << " type [" << type << "] must be lower than LUA_NUMTYPES [" << LUA_NUMTYPES << "]");
                T value = lua_to<T>(L, -1);
                lua_pop(L, 1);
                return value;
            }

            operator LuaFunction() const {
                lua_getglobal(L, key.c_str());
                ST_ASSERT(lua_isfunction(L, -1), "Lua global must be function");
                LuaFunction function(L, key);
                lua_pop(L, 1);
                return function;
            }

            operator LuaTable() const {
                lua_getglobal(L, key.c_str());
                ST_ASSERT(lua_istable(L, -1), "Lua global must be table");
                LuaTable object(L, key);
                lua_pop(L, 1);
                return object;
            }

            template<typename T>
            GetterSetter& operator=(T value) {
                lua_push(L, value);
                lua_setglobal(L, key.c_str());
                return *this;
            }
        };

        GetterSetter operator[](const std::string& key) const {
            return GetterSetter(L, key);
        }

        GetterSetter operator[](i32 ref) const {
            return GetterSetter(L, ref);
        }
    };
}

i32 lua_require(const Storytime::LuaState& L, const std::string& name);