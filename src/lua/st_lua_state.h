#pragma once

#include "st_lua_function.h"
#include "st_lua_table.h"

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

        void run_file(const std::string& path) const;

        void run_script(const std::string& script) const;

        void add_package_path(const std::string& path) const;

        void add_c_package_path(const std::string& path) const;

        i32 require_package(const std::string& package_name) const;

        void load_package(const std::string& package_name) const;

        void unload_package(const std::string& package_name) const;

        i32 create_ref() const;

        void destroy_ref(i32 ref) const;

        std::string get_version() const;

        class GetterSetter {
        private:
            lua_State* L;
            std::string key;
            i32 index_or_ref;

        public:
            GetterSetter(lua_State* L, const std::string& key) : L(L), key(key), index_or_ref(LUA_NOREF) {
            }

            GetterSetter(lua_State* L, i32 index_or_ref) : L(L), key(""), index_or_ref(index_or_ref) {
            }

            template<typename T>
            operator T() const {
                i32 stack_size_start = lua_gettop(L);

                bool use_global = key.length() > 0;
                bool use_ref = index_or_ref >= 0;

                i32 index;
                i32 type = LUA_NOREF;
                if (use_global) {
                    lua_getglobal(L, key.c_str());
                    index = -1;
                    type = lua_type(L, index);
                } else if (use_ref) {
                    index = -1;
                    type = lua_rawgeti(L, LUA_REGISTRYINDEX, index_or_ref);
                } else {
                    index = index_or_ref;
                    ST_ASSERT(index < 0, "Lua stack index must be negative");
                    type = lua_type(L, index);
                }
                ST_ASSERT(type > LUA_TNONE, "Lua type [" << type << "] on index [" << index << "] must be greater than LUA_TNONE [" << LUA_TNONE << "]");
                ST_ASSERT(type < LUA_NUMTYPES, "Lua type [" << type << "] on index [" << index << "] must be less than LUA_NUMTYPES [" << LUA_NUMTYPES << "]");

                T value = lua_to<T>(L, index);

                i32 stack_size_end = lua_gettop(L);
                if (stack_size_end > stack_size_start) {
                    lua_pop(L, stack_size_end - stack_size_start);
                }

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

