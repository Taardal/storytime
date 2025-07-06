#include "st_lua_process_manager_binding.h"
#include "st_lua_process_binding.h"

namespace Storytime {
    const std::string LuaProcessManagerBinding::metatable_name = "LuaProcessManagerBinding";

    LuaProcessManagerBinding::LuaProcessManagerBinding(lua_State* L, ProcessManager* process_manager)
        : L(L), process_manager(process_manager)  {
    }

    i32 LuaProcessManagerBinding::create_metatable(lua_State* L) {
        luaL_newmetatable(L, metatable_name.c_str());

        lua_pushcfunction(L, index);
        lua_setfield(L, -2, "__index");

        return 1;
    }

    i32 LuaProcessManagerBinding::create(lua_State* L, ProcessManager* process_manager) {
        ST_ASSERT(process_manager != nullptr, "ProcessManager cannot be null");

        void* userdata = lua_newuserdata(L, sizeof(LuaProcessManagerBinding));
        new (userdata) LuaProcessManagerBinding(L, process_manager);

        luaL_getmetatable(L, metatable_name.c_str());
        ST_ASSERT(!lua_isnil(L, -1), "Metatable [" << metatable_name.c_str() << "] cannot be null");
        lua_setmetatable(L, -2);

        return 1;
    }

    i32 LuaProcessManagerBinding::destroy(lua_State* L) {
        ST_ASSERT(lua_type(L, -1) == LUA_TUSERDATA, "Binding must be at expected stack location");

        auto binding = static_cast<LuaProcessManagerBinding*>(lua_touserdata(L, -1));
        ST_ASSERT(binding != nullptr, "Binding cannot be null");

        binding->~LuaProcessManagerBinding();
        return 0;
    }

    i32 LuaProcessManagerBinding::index(lua_State* L) {
        ST_ASSERT(lua_type(L, -1) == LUA_TSTRING, "Key must be at expected stack location");

        const char* key = lua_tostring(L, -1);
        ST_ASSERT(strlen(key) > 0, "Key cannot be empty");

        if (strcmp(key, "add") == 0) {
            lua_pushcfunction(L, add);
            return 1;
        }
        if (strcmp(key, "abort") == 0) {
            lua_pushcfunction(L, abort);
            return 1;
        }
        if (strcmp(key, "clear") == 0) {
            lua_pushcfunction(L, clear);
            return 1;
        }
        if (strcmp(key, "empty") == 0) {
            lua_pushcfunction(L, empty);
            return 1;
        }

        return 0;
    }

    i32 LuaProcessManagerBinding::add(lua_State* L) {
        ST_ASSERT(lua_type(L, -1) == LUA_TTABLE, "Lua process instance table must be at expected stack location");

        lua_getfield(L, -1, LuaProcessBinding::instance_binding_field_name.c_str());
        ST_ASSERT(lua_type(L, -1) == LUA_TLIGHTUSERDATA, "Lua process instance table must have a C++ process binding");

        auto process_binding = (LuaProcessBinding*) lua_touserdata(L, -1);
        ST_ASSERT(process_binding != nullptr, "The process binding cannot be null");

        auto process_manager_binding = (LuaProcessManagerBinding*) lua_touserdata(L, -3);
        ST_ASSERT(process_manager_binding != nullptr, "The process manager binding cannot be null");

        process_manager_binding->process_manager->add(Shared<LuaProcessBinding>(process_binding));
        return 0;
    }

    i32 LuaProcessManagerBinding::abort(lua_State* L) {
        bool abort_immediately = false;
        i32 binding_index = -1;

        bool abort_immediately_argument_exists = lua_isboolean(L, -1);
        if (abort_immediately_argument_exists) {
            abort_immediately = lua_toboolean(L, -1);
            binding_index--; // If the optional `abort_immediately` argument exists, the binding must be one index lower.
        } else {
            ST_ASSERT(
                lua_isuserdata(L, -1),
                "The process manager binding must be at expected stack location when the `abort_immediately` argument is not supplied"
            );
        }

        ST_ASSERT(
            lua_isuserdata(L, binding_index),
            "The process manager binding must be at expected stack location"
        );

        auto process_manager_binding = (LuaProcessManagerBinding*) lua_touserdata(L, binding_index);
        ST_ASSERT(process_manager_binding != nullptr, "The process manager binding cannot be null");

        process_manager_binding->process_manager->abort(abort_immediately);
        return 0;
    }

    i32 LuaProcessManagerBinding::clear(lua_State* L) {
        auto process_manager_binding = (LuaProcessManagerBinding*) lua_touserdata(L, -1);
        ST_ASSERT(process_manager_binding != nullptr, "The process manager binding cannot be null");
        process_manager_binding->process_manager->clear();
        return 0;
    }

    i32 LuaProcessManagerBinding::empty(lua_State* L) {
        auto process_manager_binding = (LuaProcessManagerBinding*) lua_touserdata(L, -1);
        ST_ASSERT(process_manager_binding != nullptr, "The process manager binding cannot be null");
        lua_pushboolean(L, process_manager_binding->process_manager->empty());
        return 1;
    }
}
