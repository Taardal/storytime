#pragma once

#include "st_lua_function.h"

namespace Storytime {
    class LuaTable {
    private:
        lua_State* L;
        LuaRef ref = LUA_NOREF;
        std::string global_name;

    public:
        LuaTable(lua_State* L);

        LuaTable(lua_State* L, LuaRef ref);

        LuaTable(lua_State* L, const std::string& global_name);

        LuaRef get_ref() const;

        LuaFunction get_function(const std::string& key) const;

        template<class... Args>
        void invoke_function(const std::string& key, Args&&... args) {
            LuaFunction fn = get_function(key);
            fn(std::forward<Args>(args)...);
        }

        bool has_field(const std::string& key) const;

        template<typename T>
        T get_field(const std::string& key) const {
            T value;
            get_field(L, key, &value);
            return value;
        }

        template<typename T>
        T get_field(const std::string& key, T* value) const {
            push_self();
            lua_getfield(L, -1, key.c_str());
            lua_to<T>(L, -1, &value);
            lua_pop(L, 2);
            return value;
        }

        template<typename T>
        std::optional<T> try_get_field(const std::string& key) const {
            if (!has_field(key)) {
                return std::nullopt;
            }
            T value;
            get_field(L, key, &value);
            return value;
        }

        template<typename T>
        void try_get_field(const std::string& key, T* value) const {
            if (!has_field(key)) {
                return;
            }
            push_self();
            lua_getfield(L, -1, key.c_str());
            lua_to<T>(L, -1, &value);
            lua_pop(L, 2);
        }

        template<typename T>
        void set_field(const std::string& key, T* value) const {
            push_self();
            lua_push<T>(L, value);
            lua_setfield(L, -2, key.c_str());
            lua_pop(L, 1);
        }

        class GetterSetter {
        private:
            LuaTable* lua_table;
            std::string key;

        public:
            explicit GetterSetter(LuaTable* table, const std::string& key) : lua_table(table), key(key) {
            }

            template<typename T>
            operator T() const {
                return lua_table->get_field<T>(key);
            }

            template<typename T>
            GetterSetter& operator=(T value) {
                lua_table->set_field<T>(key, value);
                return *this;
            }
        };

        GetterSetter operator[](const std::string& key);

    private:
        void push_self() const;
    };
}
