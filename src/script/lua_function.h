#pragma once

namespace Storytime {
    class LuaFunction {
    private:
        lua_State* L;
        std::string name;
        LuaRef ref;
        u8 argument_count = 0;
        u8 return_value_count = 0;

    public:
        LuaFunction(lua_State* L, const std::string& name);

        LuaFunction(lua_State* L, LuaRef ref);

        LuaRef get_ref() const;

        //
        // Overload the function call operator to allow LuaFunction objects to be invoked using ():
        //
        // LuaFunction lua_main(L, "main");
        // lua_main();
        //
        template<class... Args>
        void operator()(Args&&... args) {
            invoke(std::forward<Args>(args)...);
        }

        // Invoke the Lua function with dynamic arguments
        template<class... Args>
        void invoke(Args&&... args) {
            // Error handler function
            lua_pushcfunction(L, print_lua_stacktrace);

            // Lua function to invoke
            if (ref != 0) {
                int type = lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
                ST_ASSERT(type == LUA_TFUNCTION, "Lua ref type [" << type << "] must be function [" << LUA_TFUNCTION << "]");
            } else {
                ST_ASSERT(name.length() > 0, "Name is required invoke global Lua function");
                lua_getglobal(L, name.c_str());
            }

            // Assert expected Lua stack
            ST_ASSERT(lua_isfunction(L, -1), "Lua function to invoke must be at the top of the stack"); // Function to invoke
            ST_ASSERT(lua_isfunction(L, -2), "Lua error handler function must be second on the stack"); // Error handler function

            // Lua function parameters
            argument_count = sizeof...(args);
            push_args(L, args...);

            // Error handler function location on Lua stack = location before arguments - argument count
            int error_handler_index = -2 - argument_count;

            // Invoke the Lua function
            int result = lua_pcall(L, argument_count, return_value_count, error_handler_index);
            if (result != LUA_OK) {
                std::string error_code_name = get_result_code_name(result);
                std::string error_message = lua_tostring(L, -1);
                ST_LOG_ERROR("[{}] {}", error_code_name, error_message);
            }
        }

    private:
        // Use variadic templates to recursively push each argument onto the Lua stack (pushes one argument at a time).
        template<class T, class... Args>
        void push_args(lua_State* L, T value, Args... args) {
            lua_push(L, value); // Push the first argument
            push_args(L, args...); // Recursively call push_args with the rest of the arguments
        }

        // Base case for the recursion. When there are no arguments left to push, this function does nothing.
        void push_args(lua_State* L) {
        }

        static std::string get_result_code_name(int pcall_result);
    };
}

template<>
inline Storytime::LuaFunction lua_to(lua_State* L, int index) {
    ST_ASSERT(lua_isfunction(L, index), "Item at index [" << index << "] on Lua stack must be of type function");
    i32 ref = luaL_ref(L, LUA_REGISTRYINDEX);
    Storytime::LuaFunction lua_function(L, ref);
    return lua_function;
}