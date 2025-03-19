#include "st_lua_log.h"

#define ST_LUA_LOG(log_command, lua_message)\
    LuaStacktraceEntry stacktrace_entry = get_stacktrace_entry(L);\
    std::string& entry_name = stacktrace_entry.name;\
    std::string entry_tag = stacktrace_entry.get_tag();\
    if (entry_name.size() > 0) {\
        log_command("[{}] [{}] {}", entry_tag, entry_name, lua_message);\
    } else {\
        log_command("[{}] {}", entry_tag, lua_message);\
    }

#define ST_LUA_LOG_TRACE(lua_message) ST_LUA_LOG(ST_LOG_TRACE, lua_message)
#define ST_LUA_LOG_DEBUG(lua_message) ST_LUA_LOG(ST_LOG_DEBUG, lua_message)
#define ST_LUA_LOG_INFO(lua_message) ST_LUA_LOG(ST_LOG_INFO, lua_message)
#define ST_LUA_LOG_WARNING(lua_message) ST_LUA_LOG(ST_LOG_WARNING, lua_message)
#define ST_LUA_LOG_ERROR(lua_message) ST_LUA_LOG(ST_LOG_ERROR, lua_message)
#define ST_LUA_LOG_CRITICAL(lua_message) ST_LUA_LOG(ST_LOG_CRITICAL, lua_message)

namespace Storytime {
    void lua_log_message(lua_State* L, LogLevel log_level) {
        const char* message = lua_tostring(L, -1);
        if (log_level == LogLevel::trace) {
            ST_LUA_LOG_TRACE(message);
        } else if (log_level == LogLevel::debug) {
            ST_LUA_LOG_DEBUG(message);
        } else if (log_level == LogLevel::info) {
            ST_LUA_LOG_INFO(message);
        } else if (log_level == LogLevel::warn) {
            ST_LUA_LOG_WARNING(message);
        } else if (log_level == LogLevel::err) {
            ST_LUA_LOG_ERROR(message);
        } else if (log_level == LogLevel::critical) {
            ST_LUA_LOG_CRITICAL(message);
        } else {
            ST_ASSERT(log_level >= LogLevel::trace, "Lua log level cannot be lower than trace");
            ST_ASSERT(log_level <= LogLevel::critical, "Lua log level cannot be higher than critical");
        }
    }

    int lua_log_trace(lua_State* L) {
        lua_log_message(L, LogLevel::trace);
        return 0;
    }

    int lua_log_debug(lua_State* L) {
        lua_log_message(L, LogLevel::debug);
        return 0;
    }

    int lua_log_info(lua_State* L) {
        lua_log_message(L, LogLevel::info);
        return 0;
    }

    int lua_log_warning(lua_State* L) {
        lua_log_message(L, LogLevel::warn);
        return 0;
    }

    int lua_log_error(lua_State* L) {
        lua_log_message(L, LogLevel::err);
        return 0;
    }

    int lua_log_critical(lua_State* L) {
        lua_log_message(L, LogLevel::critical);
        return 0;
    }

    LuaStacktraceEntry get_stacktrace_entry(lua_State* L) {
        // We want to get the stacktrace entry of the calling Lua function to print extra
        // debug information. The first entry of the stack is this function. We only want
        // the second entry, which is the Lua code that called this function.
        constexpr u8 stacktrace_depth = 2;
        std::vector<LuaStacktraceEntry> stacktrace = get_lua_stacktrace(L, stacktrace_depth);
        ST_ASSERT(
            stacktrace.size() == stacktrace_depth,
            "Lua stacktrace size [" << stacktrace.size() << "] must match provided stacktrace depth [" << stacktrace_depth << "]"
        );
        return stacktrace[1];
    }

    const std::string LuaLog::metatable_name = "Log";

    i32 LuaLog::create_metatable(lua_State* L) {
        luaL_newmetatable(L, metatable_name.c_str());

        lua_newtable(L);
        lua_pushcfunction(L, lua_log_trace);
        lua_setfield(L, -2, "t");
        lua_pushcfunction(L, lua_log_debug);
        lua_setfield(L, -2, "d");
        lua_pushcfunction(L, lua_log_info);
        lua_setfield(L, -2, "i");
        lua_pushcfunction(L, lua_log_warning);
        lua_setfield(L, -2, "w");
        lua_pushcfunction(L, lua_log_error);
        lua_setfield(L, -2, "e");
        lua_pushcfunction(L, lua_log_critical);
        lua_setfield(L, -2, "c");

        lua_setfield(L, -2, "__index");

        return 1;
    }

    i32 LuaLog::create(lua_State* L) {
        lua_newtable(L);

        luaL_getmetatable(L, metatable_name.c_str());
        ST_ASSERT(!lua_isnil(L, -1), "Metatable [" << metatable_name.c_str() << "] cannot be null");
        lua_setmetatable(L, -2);

        return 1;
    }
}
