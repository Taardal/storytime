#pragma once

namespace Storytime {
    struct LuaStacktraceEntry {
        // A reasonable name for the given function.
        // Because functions in Lua are first-class values, they do not have a fixed name.
        // The lua_getinfo function checks how the function was called to find a suitable name.
        // If it cannot find a name, then name is set to NULL.
        std::string name;

        // Explains the name field according to how the function was called.
        // Possible values:
        // - [global]
        // - [local]
        // - [method]
        // - [field]
        // - [upvalue]
        // - [""] (Empty string when no other option seems to apply)
        std::string namewhat;

        // What type of function it is
        // - [Lua] if the function is a Lua function
        // - [C] if it is a C function
        // - [main] if it is the main part of a chunk
        std::string what;

        // The source of the chunk that created the function.
        // If the source starts with...
        // - [@] The function was defined in a file where the file name follows the '@'
        // - [=] The remainder of its contents describes the source in a user-dependent manner.
        // - [ ] The function was defined in a string where source is that string.
        std::string source;

        // A "printable" version of source, to be used in error messages.
        std::string short_source;

        // The current line where the given function is executing. When no line information is available, currentline is set to -1.
        i32 current_line;

        // The line number where the definition of the function starts.
        i32 line_defined;

        // The line number where the definition of the function ends.
        i32 last_line_defined;

        // The number of upvalues of the function.
        i32 number_of_upvalues;

        // The number of parameters of the function (always 0 for C functions).
        i32 number_of_parameters;

        // True if the function is a variadic function (always true for C functions).
        bool variadic_function;

        // True if this function invocation was called by a tail call.
        // In this case, the caller of this level is not in the stack.
        bool tail_call;

        // A "printable" combination of the file name (everything after the last '/' in `source`) and `current_line`
        std::string get_tag() const;
    };

    std::vector<LuaStacktraceEntry> get_lua_stacktrace(lua_State* L, i32 max_depth = 10);

    struct LuaStacktracePrintConfig {
        bool verbose;
    };

    void print_lua_stacktrace(lua_State* L, LuaStacktracePrintConfig config = { .verbose = false });
}
