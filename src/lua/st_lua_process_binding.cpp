#include "st_lua_process_binding.h"
#include "st_lua_function.h"
#include "st_lua_usertype.h"

namespace Storytime {
    const std::string LuaProcessBinding::metatable_name = "LuaProcessBinding";
    const std::string LuaProcessBinding::class_table_name = "Process";
    const std::string LuaProcessBinding::instance_binding_field_name = "binding";

    LuaProcessBinding::LuaProcessBinding(lua_State* L) : L(L) {}

    LuaProcessBinding::~LuaProcessBinding() {
        lua_instance_table_ref.destroy(L);
        lua_on_initialize_fn_ref.destroy(L);
        lua_on_update_fn_ref.destroy(L);
        lua_on_success_fn_ref.destroy(L);
        lua_on_fail_fn_ref.destroy(L);
        lua_on_abort_fn_ref.destroy(L);
    }

    i32 LuaProcessBinding::create_metatable(lua_State* L) {
        luaL_newmetatable(L, metatable_name.c_str());

        lua_pushcfunction(L, index);
        lua_setfield(L, -2, "__index");

        //
        // This metatable doubles as a Lua class table.
        //
        // A class table is a Lua table that uses the `class` Lua function to mimic OOP inheritance,
        // which is useful to let Lua tables inherit from and extend C++ classes.
        //
        // In this case, this allows us to define a process class table in Lua code, and let that table inherit
        // from the C++ Process class so that it can call code on its C++ Process parent class.
        //
        // So, because this metatable also acts as a class table, we need to assign it as its own metatable
        // so that it calls its own metafields and metamethods like `__index` and similar.
        //

        // Set the metatable as its own metatable
        lua_pushvalue(L, -1);
        lua_setmetatable(L, -2);

        // Add the metatable to Lua as a class table under its class name
        lua_pushvalue(L, -1);
        lua_setglobal(L, class_table_name.c_str());

        return 1;
    }

    void LuaProcessBinding::on_initialize() {
        if (!lua_on_initialize_fn_ref.is_valid()) {
            return;
        }
        LuaFunction lua_on_initialize_fn(L, lua_on_initialize_fn_ref);
        lua_on_initialize_fn(lua_instance_table_ref);
    }

    void LuaProcessBinding::on_update(f64 timestep) {
        ST_ASSERT(lua_on_update_fn_ref.is_valid(), "Every Lua process must have implemented on_update");
        // LuaFunction lua_on_update_fn(L, lua_on_update_fn_ref);
        // lua_on_update_fn(lua_instance_table_ref, timestep);

        i32 start = lua_gettop(L);
        lua_instance_table_ref.push(L);
        lua_getfield(L, -1, "on_update");
        // print_lua_stack(L, "foo");
        LuaFunction lua_on_update_fn(L);
        lua_on_update_fn(lua_instance_table_ref, timestep);
        lua_pop(L, std::max(0, lua_gettop(L) - start));
        i32 end = lua_gettop(L);
        ST_ASSERT(start == end, "[" << start << "] == [" << end << "]");
    }

    void LuaProcessBinding::on_success() {
        if (!lua_on_success_fn_ref.is_valid()) {
            return;
        }
        LuaFunction lua_on_success_fn(L, lua_on_success_fn_ref);
        lua_on_success_fn(lua_instance_table_ref);
    }

    void LuaProcessBinding::on_fail() {
        if (!lua_on_fail_fn_ref.is_valid()) {
            return;
        }
        LuaFunction lua_on_fail_fn(L, lua_on_fail_fn_ref);
        lua_on_fail_fn(lua_instance_table_ref);
    }

    void LuaProcessBinding::on_abort() {
        if (!lua_on_abort_fn_ref.is_valid()) {
            return;
        }
        LuaFunction lua_on_abort_fn(L, lua_on_abort_fn_ref);
        lua_on_abort_fn(lua_instance_table_ref);
    }

    i32 LuaProcessBinding::index(lua_State* L) {
        const char* key = lua_tostring(L, -1);
        ST_ASSERT(strlen(key) > 0, "Key cannot be empty");

        if (strcmp(key, "new") == 0) {
            lua_pushcfunction(L, new_instance);
            return 1;
        }

        // Try to get the value from the uservalue table if it exists
        lua_getuservalue(L, -2);
        i32 uservalue_type = lua_type(L, -1);
        if (uservalue_type == LUA_TTABLE) {
            lua_getfield(L, -1, key);
            return 1;
        }
        if (uservalue_type != LUA_TNIL) {
            return 1;
        }

        return 0;
    }

    // This function creates a new Lua instance table from a Lua process class.
    //
    // A Lua process class table must inherit from the C++ Process class using the `class` Lua function.
    // The `class` function sets ut an inheritance chain from the Lua process class table to the C++ process class.
    //
    // When a Lua process class calls `SomeProcess:new()`, it will go up the inheritance chain and eventually call this function.
    // This function creates a new Lua instance table that can call C++ functions in the C++ Process class, and lets C++ code call
    // Lua functions defined in the Lua process class table.
    //
    // This function expects the following arguments:
    // [1] [lua index: -3] The LuaProcessBinding class table.
    // [2] [lua index: -2] The `construction_data` table that contains fields that the final instance table should contain.
    // [3] [lua index: -1] The Lua process class table that needs to inherit from the C++ Process class.
    //
    i32 LuaProcessBinding::new_instance(lua_State* L) {
        ST_ASSERT(lua_istable(L, -1), "Expected the Lua class table as the third argument");
        ST_ASSERT(lua_istable(L, -2) || lua_isnil(L, -2), "Expected the construction data table, or nil, as the second argument");
        ST_ASSERT(lua_istable(L, -3), "Expected the LuaProcessBinding self table as the first argument");

        //
        // Create the instance table
        //

        bool construction_data_exists = !lua_isnil(L, -2);
        if (construction_data_exists) {
            lua_pushvalue(L, -2); // If the construction data table exists, use it as the instance table.
        } else {
            lua_newtable(L); // Create a new instance table.
        }

        // Create the C++ binding object that is used to let the Lua instance table call into C++ code in the Process class.
        // The C++ object is alloacted on the heap, but explicit cleanup is not needed here because the process will be
        // added to a ProcessManager which will take control its lifetime and delete it when it's complete.
        auto binding = new LuaProcessBinding(L);
        lua_pushlightuserdata(L, binding);
        lua_setfield(L, -2, instance_binding_field_name.c_str());

        // Obtain a reference to the Lua instance table.
        // This is used to let C++ code call the lifecycle functions on the Lua instance object like on_update, on_success, etc.
        lua_pushvalue(L, -1);
        binding->lua_instance_table_ref = LuaRef::create(L);

        //
        // Obtain references to the lifecycle functions of the process that are defined in the Lua process class table
        //

        lua_getfield(L, -2, "on_update");
        ST_ASSERT(lua_isfunction(L, -1), "The Lua process must implement the `on_update` lifecycle function");
        binding->lua_on_update_fn_ref = LuaRef::create(L);

        lua_getfield(L, -2, "on_initialize");
        if (lua_isfunction(L, -1)) {
            binding->lua_on_initialize_fn_ref = LuaRef::create(L);
        } else {
            lua_pop(L, 1);
        }

        lua_getfield(L, -2, "on_success");
        if (lua_isfunction(L, -1)) {
            binding->lua_on_success_fn_ref = LuaRef::create(L);
        } else {
            lua_pop(L, 1);
        }

        lua_getfield(L, -2, "on_fail");
        if (lua_isfunction(L, -1)) {
            binding->lua_on_fail_fn_ref = LuaRef::create(L);
        } else {
            lua_pop(L, 1);
        }

        lua_getfield(L, -2, "on_abort");
        if (lua_isfunction(L, -1)) {
            binding->lua_on_abort_fn_ref = LuaRef::create(L);
        } else {
            lua_pop(L, 1);
        }

        //
        // Add C++ Process functions to the Lua instance table.
        //

        lua_pushcfunction(L, [](lua_State* L) -> i32 {
            ST_ASSERT_LUA_TYPE(L, -1, LUA_TTABLE); // The LuaProcessBinding table for the child process.
            ST_ASSERT_LUA_TYPE(L, -2, LUA_TTABLE); // The LuaProcessBinding table that calls the function (self).

            lua_getfield(L, -1, instance_binding_field_name.c_str());
            ST_ASSERT_LUA_TYPE(L, -1, LUA_TLIGHTUSERDATA);
            auto child_binding = (LuaProcessBinding*) lua_touserdata(L, -1);
            ST_ASSERT_NOT_NULL(child_binding);

            lua_getfield(L, -3, instance_binding_field_name.c_str());
            ST_ASSERT_LUA_TYPE(L, -1, LUA_TLIGHTUSERDATA);
            auto binding = (LuaProcessBinding*) lua_touserdata(L, -1);
            ST_ASSERT_NOT_NULL(binding);

            binding->set_child(Shared<LuaProcessBinding>(child_binding));
            return 0;
        });
        lua_setfield(L, -2, "set_child");

        lua_pushcfunction(L, [](lua_State* L) -> i32 {
            ST_ASSERT_LUA_TYPE(L, -1, LUA_TTABLE); // The LuaProcessBinding table (self).

            lua_getfield(L, -1, instance_binding_field_name.c_str());
            ST_ASSERT_LUA_TYPE(L, -1, LUA_TLIGHTUSERDATA);

            auto binding = (LuaProcessBinding*) lua_touserdata(L, -1);
            ST_ASSERT_NOT_NULL(binding);

            binding->succeed();
            return 0;
        });
        lua_setfield(L, -2, "succeed");

        lua_pushcfunction(L, [](lua_State* L) -> i32 {
            ST_ASSERT_LUA_TYPE(L, -1, LUA_TTABLE); // The LuaProcessBinding table (self).

            lua_getfield(L, -1, instance_binding_field_name.c_str());
            ST_ASSERT_LUA_TYPE(L, -1, LUA_TLIGHTUSERDATA);

            auto binding = (LuaProcessBinding*) lua_touserdata(L, -1);
            ST_ASSERT_NOT_NULL(binding);

            binding->fail();
            return 0;
        });
        lua_setfield(L, -2, "fail");

        lua_pushcfunction(L, [](lua_State* L) -> i32 {
            ST_ASSERT_LUA_TYPE(L, -1, LUA_TTABLE); // The LuaProcessBinding table (self).

            lua_getfield(L, -1, instance_binding_field_name.c_str());
            ST_ASSERT_LUA_TYPE(L, -1, LUA_TLIGHTUSERDATA);

            auto binding = (LuaProcessBinding*) lua_touserdata(L, -1);
            ST_ASSERT_NOT_NULL(binding);

            return 0;
        });
        lua_setfield(L, -2, "abort");

        lua_pushcfunction(L, [](lua_State* L) -> i32 {
            ST_ASSERT_LUA_TYPE(L, -1, LUA_TTABLE); // The LuaProcessBinding table (self).

            lua_getfield(L, -1, instance_binding_field_name.c_str());
            ST_ASSERT_LUA_TYPE(L, -1, LUA_TLIGHTUSERDATA);

            auto binding = (LuaProcessBinding*) lua_touserdata(L, -1);
            ST_ASSERT_NOT_NULL(binding);

            binding->pause();
            return 0;
        });
        lua_setfield(L, -2, "pause");

        lua_pushcfunction(L, [](lua_State* L) -> i32 {
            ST_ASSERT_LUA_TYPE(L, -1, LUA_TTABLE); // The LuaProcessBinding table (self).

            lua_getfield(L, -1, instance_binding_field_name.c_str());
            ST_ASSERT_LUA_TYPE(L, -1, LUA_TLIGHTUSERDATA);

            auto binding = (LuaProcessBinding*) lua_touserdata(L, -1);
            ST_ASSERT_NOT_NULL(binding);

            binding->resume();
            return 0;
        });
        lua_setfield(L, -2, "resume");

        lua_pushcfunction(L, [](lua_State* L) -> i32 {
            ST_ASSERT_LUA_TYPE(L, -1, LUA_TTABLE); // The LuaProcessBinding table (self).

            lua_getfield(L, -1, instance_binding_field_name.c_str());
            ST_ASSERT_LUA_TYPE(L, -1, LUA_TLIGHTUSERDATA);

            auto binding = (LuaProcessBinding*) lua_touserdata(L, -1);
            ST_ASSERT_NOT_NULL(binding);

            lua_pushboolean(L, binding->is_alive());
            return 1;
        });
        lua_setfield(L, -2, "is_alive");

        lua_pushcfunction(L, [](lua_State* L) -> i32 {
            ST_ASSERT_LUA_TYPE(L, -1, LUA_TTABLE); // The LuaProcessBinding table (self).

            lua_getfield(L, -1, instance_binding_field_name.c_str());
            ST_ASSERT_LUA_TYPE(L, -1, LUA_TLIGHTUSERDATA);

            auto binding = (LuaProcessBinding*) lua_touserdata(L, -1);
            ST_ASSERT_NOT_NULL(binding);

            lua_pushboolean(L, binding->is_dead());
            return 1;
        });
        lua_setfield(L, -2, "is_dead");

        lua_pushcfunction(L, [](lua_State* L) -> i32 {
            ST_ASSERT_LUA_TYPE(L, -1, LUA_TTABLE); // The LuaProcessBinding table (self).

            lua_getfield(L, -1, instance_binding_field_name.c_str());
            ST_ASSERT_LUA_TYPE(L, -1, LUA_TLIGHTUSERDATA);

            auto binding = (LuaProcessBinding*) lua_touserdata(L, -1);
            ST_ASSERT_NOT_NULL(binding);

            lua_pushboolean(L, binding->is_paused());
            return 1;
        });
        lua_setfield(L, -2, "is_paused");

        return 1; // Return the Lua instance table
    }
}
