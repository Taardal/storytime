#pragma once

#include "lua_ref.h"

//
// C++ template specialization
//
// Problem:
// We want to be able to push function arguments of any type to the Lua stack, but different types of arguments
// must be pushed using different types of "lua_push(...)" functions.
//
// Solution:
// Use template specializations. We use a templated function to push arguments and implement specializations for
// different types to pushed using the appropriate lua_push(...) function.
//
// ChatGPT:
// Template specialization in C++ is a feature that allows the customization of template code for specific data types
// or conditions. It enables the programmer to provide specific implementations of a template when the default
// implementation is not suitable for certain types or values.
//
// Source(s):
// - https://en.cppreference.com/w/cpp/language/template_specialization
// - https://www.geeksforgeeks.org/template-specialization-c/
//

// --------------------------------------------------------------------------------------------------------------
// PARSE values from Lua stack
// --------------------------------------------------------------------------------------------------------------

template<class T>
T lua_to(lua_State* L, int index) {
    static_assert(sizeof(T) == 0, "Cannot parse Lua value: Unknown type");
    return 0;
}

template<>
inline int lua_to(lua_State* L, int index) {
    ST_ASSERT(lua_isinteger(L, index), "Item at index [" << index << "] on Lua stack must be of type integer");
    return lua_tointeger(L, index);
}

template<>
inline float lua_to(lua_State* L, int index) {
    ST_ASSERT(lua_isnumber(L, index), "Item at index [" << index << "] on Lua stack must be of type number");
    return lua_tonumber(L, index);
}

template<>
inline double lua_to(lua_State* L, int index) {
    ST_ASSERT(lua_isnumber(L, index), "Item at index [" << index << "] on Lua stack must be of type number");
    return lua_tonumber(L, index);
}

template<>
inline const char* lua_to(lua_State* L, int index) {
    ST_ASSERT(lua_isstring(L, index), "Item at index [" << index << "] on Lua stack must be of type string");
    return lua_tostring(L, index);
}

template<>
inline bool lua_to(lua_State* L, int index) {
    ST_ASSERT(lua_isboolean(L, index), "Item at index [" << index << "] on Lua stack must be of type boolean");
    return lua_toboolean(L, index);
}

// --------------------------------------------------------------------------------------------------------------
// PUSH values to Lua stack
// --------------------------------------------------------------------------------------------------------------

template<class T>
void lua_push(lua_State* L, T value) {
    static_assert(sizeof(T) == 0, "Cannot push Lua value: Unknown type");
}

// An integer value may be either a normal integer number or a Lua reference number to data stored in the registry
template<>
inline void lua_push(lua_State* L, int value) {
    lua_pushinteger(L, value);
}

template<>
inline void lua_push(lua_State* L, float value) {
    lua_pushnumber(L, value);
}

template<>
inline void lua_push(lua_State* L, double value) {
    lua_pushnumber(L, value);
}

template<>
inline void lua_push(lua_State* L, bool value) {
    lua_pushboolean(L, value);
}

template<>
inline void lua_push(lua_State* L, const char* value) {
    lua_pushstring(L, value);
}

template<>
inline void lua_push(lua_State* L, LuaRef value) {
    lua_rawgeti(L, LUA_REGISTRYINDEX, value);
}
