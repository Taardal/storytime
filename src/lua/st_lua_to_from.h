#pragma once

//
// Lua push and pull
//
// Problem:
// We want to be able to push and pull items of any type to and from the Lua stack,
// but different types require different implementation details.
//
// Solution:
// Use template specializations. We define templated functions to serialize/deserialize
// items and implement specializations of those templated functions for each type that
// require specific implementation details.
//
// Source:
// - https://en.cppreference.com/w/cpp/language/template_specialization
// - https://www.geeksforgeeks.org/template-specialization-c/
//

template<class T>
void lua_to(lua_State* L, int index, T* value) {
    static_assert(sizeof(T) == 0, "Cannot pull value from Lua stack: Unimplemented template specialization");
}

template<class T>
void lua_push(lua_State* L, T* value) {
    static_assert(sizeof(T) == 0, "Cannot push value to Lua stack: Unimplemented template specialization");
}

// --------------------------------------------------------------------------------------------------------------
// int
// --------------------------------------------------------------------------------------------------------------

template<>
inline void lua_to(lua_State* L, int index, int* value) {
    ST_ASSERT(lua_isinteger(L, index), "Item at index [" << index << "] on Lua stack must be of type integer");
    *value = lua_tointeger(L, index);
}

template<>
inline void lua_push(lua_State* L, int* value) {
    lua_pushinteger(L, *value);
}

// --------------------------------------------------------------------------------------------------------------
// float
// --------------------------------------------------------------------------------------------------------------

template<>
inline void lua_to(lua_State* L, int index, float* value) {
    ST_ASSERT(lua_isnumber(L, index), "Item at index [" << index << "] on Lua stack must be of type number");
    *value = lua_tonumber(L, index);
}

template<>
inline void lua_push(lua_State* L, float* value) {
    lua_pushnumber(L, *value);
}

// --------------------------------------------------------------------------------------------------------------
// double
// --------------------------------------------------------------------------------------------------------------

template<>
inline void lua_to(lua_State* L, int index, double* value) {
    ST_ASSERT(lua_isnumber(L, index), "Item at index [" << index << "] on Lua stack must be of type number");
    *value = lua_tonumber(L, index);
}

template<>
inline void lua_push(lua_State* L, double* value) {
    lua_pushnumber(L, *value);
}

// --------------------------------------------------------------------------------------------------------------
// boolean
// --------------------------------------------------------------------------------------------------------------

template<>
inline void lua_to(lua_State* L, int index, bool* value) {
    ST_ASSERT(lua_isboolean(L, index), "Item at index [" << index << "] on Lua stack must be of type boolean");
    *value = lua_toboolean(L, index);
}

template<>
inline void lua_push(lua_State* L, bool* value) {
    lua_pushboolean(L, *value);
}

// --------------------------------------------------------------------------------------------------------------
// char*
// --------------------------------------------------------------------------------------------------------------

template<>
inline void lua_to(lua_State* L, int index, const char* value) {
    ST_ASSERT(lua_isstring(L, index), "Item at index [" << index << "] on Lua stack must be of type string");
    value = lua_tostring(L, index);
}

template<>
inline void lua_push(lua_State* L, const char* value) {
    lua_pushstring(L, value);
}

// --------------------------------------------------------------------------------------------------------------
// std::string_view
// --------------------------------------------------------------------------------------------------------------

template<>
inline void lua_to(lua_State* L, int index, std::string_view* value) {
    ST_ASSERT(lua_isstring(L, index), "Item at index [" << index << "] on Lua stack must be of type string");
    size_t len;
    *value = std::string_view(lua_tolstring(L, index, &len), len);
}

template<>
inline void lua_push(lua_State* L, std::string_view* value) {
    lua_pushlstring(L, value->data(), value->size());
}

// --------------------------------------------------------------------------------------------------------------
// std::string
// --------------------------------------------------------------------------------------------------------------

template<>
inline void lua_to(lua_State* L, int index, std::string* value) {
    ST_ASSERT(lua_isstring(L, index), "Item at index [" << index << "] on Lua stack must be of type string");
    size_t len;
    *value = std::string(lua_tolstring(L, index, &len), len);
}

template<>
inline void lua_push(lua_State* L, std::string* value) {
    lua_pushlstring(L, value->c_str(), value->size());
}