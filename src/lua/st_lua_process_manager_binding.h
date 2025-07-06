#pragma once

#include "process/st_process_manager.h"

namespace Storytime {
    class LuaProcessManagerBinding {
    public:
        static const std::string metatable_name;

    private:
        lua_State* L = nullptr;
        ProcessManager* process_manager = nullptr;

    public:
        LuaProcessManagerBinding(lua_State* L, ProcessManager* process_manager);

        static i32 create_metatable(lua_State* L);

        static i32 create(lua_State* L, ProcessManager* process_manager);

    private:
        static i32 destroy(lua_State* L);

        static i32 index(lua_State* L);

        static i32 add(lua_State* L);

        static i32 abort(lua_State* L);

        static i32 clear(lua_State* L);

        static i32 empty(lua_State* L);
    };
}