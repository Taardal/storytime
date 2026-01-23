#pragma once

#include "process/st_process_manager.h"

namespace Storytime {
    class LuaProcessManagerBinding {
    public:
        static const std::string metatable_name;

    private:
        lua_State* L = nullptr;
        ProcessManager& process_manager;

    public:
        LuaProcessManagerBinding(lua_State* L, ProcessManager& process_manager);

        static i32 create_metatable(lua_State* L);

        static i32 create(lua_State* L, ProcessManager& process_manager);

        static i32 lua_destroy(lua_State* L);

        static i32 lua_index(lua_State* L);

        static i32 lua_add(lua_State* L);

        static i32 lua_abort(lua_State* L);

        static i32 lua_clear(lua_State* L);

        static i32 lua_empty(lua_State* L);
    };
}
