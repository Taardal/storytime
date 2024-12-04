#include "lua_error.h"

namespace Storytime {
    std::string LuaStacktraceEntry::get_tag() const {
        std::string file_name;
        size_t source_last_slash_index = source.find_last_of("/");
        if (source_last_slash_index != std::string::npos && source_last_slash_index + 1 < source.length()) {
            file_name = source.substr(source_last_slash_index + 1);
        }
        std::stringstream ss;
        if (file_name.length() > 0) {
            ss << file_name;
        }
        if (current_line > -1) {
            if (file_name.length() > 0) {
                ss << ":";
            }
            ss << current_line;
        }
        return ss.str();
    }

    std::vector<LuaStacktraceEntry> get_lua_stacktrace(lua_State* L, const i32 max_depth) {
        std::vector<LuaStacktraceEntry> entries;
        u32 stack_level = 0;
        lua_Debug debug_info;

        // Populate lua_Debug with information about the interpreter runtime stack
        // Level 0 is the current running function, whereas level n+1 is the function that has called level n (except for tail calls, which do not count in the stack).
        // When called with a level greater than the stack depth, lua_getstack returns 0; otherwise it returns 1.
        while (lua_getstack(L, stack_level, &debug_info) > 0 && stack_level <= max_depth - 1) {

            // Get information about a specific function or function invocation.
            // - [n] fills in the field name and namewhat;
            // - [S] fills in the fields source, short_src, linedefined, lastlinedefined, and what;
            // - [l] fills in the field currentline;
            // - [t] fills in the field istailcall;
            // - [u] fills in the fields nups, nparams, and isvararg;
            // - [f] pushes onto the stack the function that is running at the given level;
            // - [L] pushes onto the stack a table whose indices are the numbers of the lines that are valid on the function.
            //      - A valid line is a line with some associated code, that is, a line where you can put a break point.
            //      - Non-valid lines include empty lines and comments.
            lua_getinfo(L, "nSltuf", &debug_info);

            LuaStacktraceEntry entry;
            entry.name = debug_info.name ? debug_info.name : "";
            entry.namewhat = debug_info.namewhat != nullptr ? debug_info.namewhat : "";
            entry.what = debug_info.what != nullptr ? debug_info.what : "";
            entry.source = debug_info.source != nullptr ? debug_info.source : "";
            entry.short_source = strlen(debug_info.short_src) > 0 ? debug_info.short_src : "";
            entry.current_line = debug_info.currentline;
            entry.line_defined = debug_info.linedefined;
            entry.last_line_defined = debug_info.lastlinedefined;
            entry.number_of_upvalues = debug_info.nups;
            entry.number_of_parameters = debug_info.nparams;
            entry.variadic_function = debug_info.isvararg;
            entry.tail_call = debug_info.istailcall;

            entries.push_back(entry);
            lua_pop(L, 1); // Remove the function pushed by lua_getstack
            ++stack_level;
        }
        return entries;
    }

    void print_lua_stacktrace(lua_State* L, LuaStacktracePrintConfig config) {
        std::cerr << "--------------------------------------------------------------------------------------------------------------" << std::endl;
        std::cerr << "[Lua error] " << lua_tostring(L, -1) << std::endl;
        std::cerr << "--------------------------------------------------------------------------------------------------------------" << std::endl;

        std::vector<LuaStacktraceEntry> stacktrace = get_lua_stacktrace(L);

        for (i32 i = 0; i < stacktrace.size(); ++i) {
            LuaStacktraceEntry& entry = stacktrace[i];

            std::string file_name = "";
            size_t source_last_slash_index = entry.source.find_last_of("/");
            if (source_last_slash_index != std::string::npos && source_last_slash_index + 1 < entry.source.length()) {
                file_name = entry.source.substr(source_last_slash_index + 1);
            }
            std::stringstream ss;
            if (file_name.length() > 0) {
                ss << file_name;
            } else {
                ss << entry.source;
            }
            if (entry.current_line > -1) {
                if (file_name.length() > 0) {
                    ss << ":";
                }
                ss << entry.current_line;
            }
            if (entry.name.length() > 0) {
                ss << " ";
                ss << "[" << entry.name << "]";
            }
            std::string line = ss.str();
            std::cerr << "[" << i << "]" << " " << line << std::endl;

            if (!config.verbose) {
                continue;
            }

            std::cerr << "  Source: " << (entry.source.length() > 0 ? entry.source : "--") << std::endl;
            std::cerr << "  Line: " << entry.current_line << std::endl;
            std::cerr << "  Name: " << (entry.name.length() > 0 ? entry.name : "--") << std::endl;
            std::cerr << "  Description: " << (entry.namewhat.length() > 0 ? entry.namewhat : "--") << std::endl;
            std::cerr << "  Parameters: " << entry.number_of_parameters << std::endl;
            std::cerr << "  Upvalues: " << entry.number_of_upvalues << std::endl;
            std::cerr << "  Start line: " << entry.line_defined << std::endl;
            std::cerr << "  End line: " << entry.last_line_defined << std::endl;
            std::cerr << "  Variadic function: " << (entry.variadic_function ? "Yes" : "No") << std::endl;
            std::cerr << "  Tail call: " << (entry.tail_call ? "Yes" : "No") << std::endl;
        }

        std::cerr << "--------------------------------------------------------------------------------------------------------------" << std::endl;
        std::cerr << std::endl;
    }
}
