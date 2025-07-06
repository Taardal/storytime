#pragma once

namespace Storytime {
    std::string lua_function_result_to_string(int result);

    class LuaFunction {
    private:
        lua_State* L;
        std::string name = "";
        LuaRef ref = LUA_NOREF;
        u8 argument_count = 0;
        u8 return_value_count = 0;

    public:
        LuaFunction(lua_State* L);

        LuaFunction(lua_State* L, const std::string& name);

        LuaFunction(lua_State* L, LuaRef ref);

        LuaRef get_ref() const;

        bool is_valid() const;

        // Overload the function call operator to allow LuaFunction objects to be invoked using the `()` operator:
        // ```
        // LuaFunction lua_main(L, "main");
        // lua_main();
        // ```
        template<class... Args>
        void operator()(Args&&... args) {
            invoke(std::forward<Args>(args)...);
        }

        // Invoke the Lua function with dynamic arguments
        template<class... Args>
        void invoke(Args&&... args) {
            // Error handler function
            lua_pushcfunction(L, [](lua_State* L) {
                print_lua_stacktrace(L);
                return 1; // Return the error message
            });

            int type = LUA_TNONE;
            if (ref != LUA_NOREF) {
                type = lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
            } else if (name.length() > 0) {
                type = lua_getglobal(L, name.c_str());
            } else {
                lua_pushvalue(L, -2);
                type = lua_type(L, -1); // Invoke the function at the top of the stack
            }
            ST_ASSERT(type == LUA_TFUNCTION, "Invalid Lua function");

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
                std::string error_code_name = lua_function_result_to_string(result);
                std::string error_message = lua_tostring(L, -1);
                ST_LOG_ERROR("[{}] {}", error_code_name, error_message);
            }
        }

    private:
        // Use variadic templates to recursively push each argument onto the Lua stack (pushes one argument at a time).
        template<class T, class... Args>
        void push_args(lua_State* L, T& value, Args&&... args) {

            // At compile time, check whether T is a pointer type (like int*, const Foo*, etc.).
            // - [std::is_pointer_v<T>] Returns true if T is a pointer type (int*, MyType*, etc.)
            // - [std::decay_t<T>] Normalize the type by...
            //      - Removing reference types (T&, T&&),
            //      - Removing const and volatile,
            //      - Converting array types to pointers
            //      - Converting function types to function pointers
            //
            if constexpr (std::is_pointer_v<std::decay_t<T>>) {
                lua_push(L, value);
            } else {
                lua_push(L, &value);
            }

            // Recursively call push_args with the rest of the arguments
            push_args(L, args...);
        }

        // Base case for the recursion. When there are no arguments left to push, this function does nothing.
        void push_args(lua_State* L) {
        }
    };
}

template<>
inline void lua_to(lua_State* L, int index, Storytime::LuaFunction* value) {
    ST_ASSERT(lua_isfunction(L, index), "Item at index [" << index << "] on Lua stack must be of type function");
    Storytime::LuaRef ref(L);
    *value = Storytime::LuaFunction(L, ref);
}