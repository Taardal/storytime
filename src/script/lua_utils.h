#pragma once

#define ST_METATABLE_NAME(name) name + std::string(".meta")

#define ST_LUA_ASSERT_CREATED(create, expected_created_type)\
    {\
        i32 created_count = create;\
        constexpr i32 expected_created_count = 1;\
        ST_ASSERT(\
            created_count == expected_created_count,\
            "Expected number of created items on Lua stack [" << created_count << "] to be [" << expected_created_count << "]"\
        );\
        i32 created_type = lua_type(L, -1);\
        ST_ASSERT(\
            created_type == expected_created_type,\
            "Expected type of created item [" << created_type << ", " << lua_typename(L, created_type) << "] to be [" << expected_created_type << ", " << lua_typename(L, expected_created_type) << "]"\
        );\
    }

typedef i32 LuaType;

std::string create_metatable_name(const std::string& prefix);

void print_lua_stack(lua_State* L, const std::string& tag = "");

#define lua_print_stack print_lua_stack
#define lua_dump print_lua_stack

std::string lua_table_to_string(lua_State* L, i32 table_index, i32 max_depth = 10);

std::string get_metatable_name(lua_State* L, i32 index);

i32 lua_getbottom(lua_State* L);

bool lua_isempty(lua_State* L);