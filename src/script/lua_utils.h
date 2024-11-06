#pragma once

void print_lua_stack(lua_State* L, const std::string& tag = "");

std::string lua_table_to_string(lua_State* L, i32 table_index, i32 max_depth = 10);

std::string get_metatable_name(lua_State* L, i32 index);

i32 lua_getbottom(lua_State* L);

bool lua_isempty(lua_State* L);