#pragma once

#define ST_METATABLE_NAME(name) name + std::string(".meta")

#ifdef ST_ENABLE_ASSERT
    #define ST_ASSERT_LUA_TYPE(L, index, expected_type)\
        {\
            i32 type = lua_type(L, index);\
            ST_ASSERT(\
                type == expected_type,\
                "Expected item on Lua stack index [" << index << "] to be of type [" << lua_typename(L, expected_type) << "], was [" << lua_typename(L, type) << "]";\
            );\
        }
#else
    #define ST_ASSERT_LUA_TYPE(L, index, expected_type)
#endif

typedef i32 LuaType;

std::string create_metatable_name(const std::string& prefix);

void print_lua_stack(lua_State* L, const std::string& tag = "");

#define lua_print_stack print_lua_stack
#define lua_dump print_lua_stack

std::string lua_table_to_string(lua_State* L, i32 table_index, i32 max_depth = 10);

std::string get_metatable_name(lua_State* L, i32 index);

i32 lua_getbottom(lua_State* L);

bool lua_isempty(lua_State* L);

struct LuaFnConfig {
    i32 arg_count = 0;
    i32 return_count = 0;
    i32 error_handler_index = 0;
};

i32 lua_invoke_function(lua_State* L, const LuaFnConfig& config);