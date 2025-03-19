#pragma once

#include "st_lua_function.h"

namespace Storytime {
    class LuaTable {
    private:
        lua_State* L;
        std::string name;
        LuaRef ref;

    public:
        LuaTable(lua_State* L, const std::string& name);

        LuaTable(lua_State* L, LuaRef ref);

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
            push_self();
            lua_getfield(L, -1, key.c_str());
            T value = lua_to<T>(L, -1);
            lua_pop(L, 2);
            return value;
        }

        template<typename T>
        void set_field(const std::string& key, T value) const {
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
