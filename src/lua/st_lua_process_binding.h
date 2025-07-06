#pragma once

#include "process/st_process.h"

namespace Storytime {
    class LuaProcessBinding : public Process {
        friend class LuaProcessManagerBinding;

    public:
        static const std::string metatable_name;

    private:
        static const std::string class_table_name;
        static const std::string instance_binding_field_name;

    private:
        lua_State* L;
        LuaRef lua_instance_table_ref = LUA_NOREF;
        LuaRef lua_on_initialize_fn_ref = LUA_NOREF;
        LuaRef lua_on_update_fn_ref = LUA_NOREF;
        LuaRef lua_on_success_fn_ref = LUA_NOREF;
        LuaRef lua_on_fail_fn_ref = LUA_NOREF;
        LuaRef lua_on_abort_fn_ref = LUA_NOREF;

    public:
        explicit LuaProcessBinding(lua_State* L);

        ~LuaProcessBinding() override;

        static i32 create_metatable(lua_State* L);

    protected:
        void on_initialize() override;

        void on_update(f64 timestep) override;

        void on_success() override;

        void on_fail() override;

        void on_abort() override;

    private:
        static i32 index(lua_State* L);

        static i32 new_instance(lua_State* L);
    };
}
