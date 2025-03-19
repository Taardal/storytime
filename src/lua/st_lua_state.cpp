#include "st_lua_state.h"

namespace Storytime {
    LuaState::LuaState() : L(luaL_newstate()) {
    }

    LuaState::LuaState(lua_State* L) : L(L) {
    }

    LuaState::operator lua_State*() const {
        return L;
    }

    void LuaState::close() const {
        lua_close(L);
    }

    void LuaState::open_standard_libraries() const {
        luaL_openlibs(L);
    }

    void LuaState::file(const std::string& path) const {
        ST_ASSERT(!path.empty(), "Lua file path cannot be empty");
        ST_LOG_TRACE("Running file [{}]", path);

        // Error handler function
        lua_pushcfunction(L, [](lua_State* L) {
            print_lua_stacktrace(L);
            return 1; // Return the error message
        });

        luaL_loadfile(L, path.c_str());
        int error_handler_index = -2;
        constexpr int argument_count = 0;
        int result = lua_pcall(L, argument_count, LUA_MULTRET, error_handler_index);
        if (result != LUA_OK) {
            const char* error = lua_tostring(L, -1);
            ST_LOG_ERROR("[{}] {}", result, error);
            ST_THROW("Could not load Lua file");
        }

        lua_pop(L, lua_gettop(L));
        ST_ASSERT(lua_gettop(L) == 0, "Lua stack must be empty after executing file");
    }

    void LuaState::script(const std::string& script) const {
        ST_ASSERT(!script.empty(), "Lua script cannot be empty");
        ST_LOG_TRACE("Running Lua script:\n{}", script);

        // Error handler function
        lua_pushcfunction(L, [](lua_State* L) {
            print_lua_stacktrace(L);
            return 1; // Return the error message
        });

        luaL_loadstring(L, script.c_str());
        int error_handler_index = -2;
        constexpr int argument_count = 0;
        int result = lua_pcall(L, argument_count, LUA_MULTRET, error_handler_index);
        if (result != LUA_OK) {
            const char* error = lua_tostring(L, -1);
            ST_LOG_ERROR("[{}] {}", result, error);
            ST_THROW("Could not load Lua script");
        }
    }

    void LuaState::add_package_path(const std::string& path) const {
        ST_ASSERT(!path.empty(), "Lua package path cannot be empty");
        std::stringstream ss;
        ss << "package.path = '" << path << ";' .. package.path";
        std::string package_path = ss.str();
        script(package_path);
    }

    void LuaState::add_c_package_path(const std::string& path) const {
        ST_ASSERT(!path.empty(), "Lua C package path cannot be empty");
        std::stringstream ss;
        ss << "package.cpath = '" << path << ";' .. package.cpath";
        std::string package_path = ss.str();
        script(package_path);
    }

    i32 LuaState::require_package(const std::string& package_name) const {
        ST_ASSERT(!package_name.empty(), "Lua package name cannot be empty");

        std::stringstream ss;
        ss << "require('" << package_name << "')";
        std::string require_package_script = ss.str();
        script(require_package_script);

        lua_getglobal(L, "package");
        lua_getfield(L, -1, "loaded");
        lua_getfield(L, -1, package_name.c_str());
        ST_ASSERT(lua_type(L, -1) > LUA_TNIL, "Required Lua package type must be greater than LUA_TNIL");
        ST_ASSERT(lua_type(L, -1) < LUA_NUMTYPES, "Required Lua package type must be lower than LUA_NUMTYPES");

        i32 ref = luaL_ref(L, LUA_REGISTRYINDEX);
        ST_ASSERT(ref != LUA_NOREF, "Required Lua package ref is invalid");
        return ref;
    }

    // Load package by requiring it which adds it to the `package.loaded` global table
    void LuaState::load_package(const std::string& package_name) const {
        i32 package_ref = require_package(package_name);
        ST_ASSERT(package_ref != LUA_NOREF, "Package [" << package_name << "] was not loaded");
    }

    // Unload package by setting it to nil in the `package.loaded` global table
    void LuaState::unload_package(const std::string& package_name) const {
        lua_getglobal(L, "package");
        lua_getfield(L, -1, "loaded");
        lua_pushnil(L);
        lua_setfield(L, -2, package_name.c_str());
    }

    i32 LuaState::create_ref() const {
        i32 ref = luaL_ref(L, LUA_REGISTRYINDEX);
        ST_ASSERT(ref != LUA_NOREF, "Invalid Lua ref");
        ST_ASSERT(ref != LUA_REFNIL, "Lua ref cannot be nil");
        return ref;
    }

    void LuaState::destroy_ref(i32 ref) const {
        ST_ASSERT(ref != LUA_NOREF, "Invalid Lua ref");
        luaL_unref(L, LUA_REGISTRYINDEX, ref);
    }

    std::string LuaState::get_version() const {
        script("return _VERSION:match('Lua (.+)')");
        return lua_tostring(L, -1);
    }
}

i32 lua_require(const Storytime::LuaState& L, const std::string& name) {
    return L.require_package(name);
}
