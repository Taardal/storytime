#pragma once

namespace Storytime {
    template<typename T>
    struct Size {
        T width;
        T height;

        MSGPACK_DEFINE_MAP(
            width,
            height
        );
    };
}

template<typename T>
void lua_pushsize(lua_State* L, const Storytime::Size<T>& size) {
    lua_newtable(L);
    lua_pushnumber(L, size.width);
    lua_setfield(L, -2, "width");
    lua_pushnumber(L, size.height);
    lua_setfield(L, -2, "height");
}

template<typename T>
Storytime::Size<T> lua_tosize(lua_State* L, i32 index) {
    Storytime::Size<T> size;

    lua_getfield(L, index, "width");
    size.width = (T) lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "height");
    size.height = (T) lua_tonumber(L, -1);
    lua_pop(L, 1);

    return size;
}
