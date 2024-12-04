#pragma once

namespace Storytime {
    int lua_log_trace(lua_State* L);

    int lua_log_debug(lua_State* L);

    int lua_log_info(lua_State* L);

    int lua_log_warning(lua_State* L);

    int lua_log_error(lua_State* L);

    int lua_log_critical(lua_State* L);

    LuaStacktraceEntry get_stacktrace_entry(lua_State* L);

    class LuaLog {
    public:
        static const std::string table_name;

        static int create(lua_State* L);
    };
}