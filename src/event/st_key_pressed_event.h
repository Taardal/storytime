#pragma once

#include "event/st_event.h"

namespace Storytime {
    struct KeyPressedEvent : Event {
        static const EventType type;
        static const std::string type_name;

        i32 key_code = 0;
        i32 mods = 0;
        i32 scan_code = 0;

        KeyPressedEvent();

        std::string to_string() const override;
    };

    std::ostream& operator<<(std::ostream& os, const KeyPressedEvent& event);
}

template<>
inline void lua_to(lua_State* L, int index, Storytime::KeyPressedEvent* value) {
    if (lua_getfield(L, index, "type") != LUA_TNIL) {
        value->event_type = lua_tonumber(L, -1);
    }
    lua_pop(L, 1);

    if (lua_getfield(L, index, "type_name") != LUA_TNIL) {
        value->event_type_name = lua_tostring(L, -1);
    }
    lua_pop(L, 1);

    if (lua_getfield(L, index, "key_code") != LUA_TNIL) {
        value->key_code = lua_tonumber(L, -1);
    }
    lua_pop(L, 1);

    if (lua_getfield(L, index, "mods") != LUA_TNIL) {
        value->mods = lua_tonumber(L, -1);
    }
    lua_pop(L, 1);

    if (lua_getfield(L, index, "scan_code") != LUA_TNIL) {
        value->scan_code = lua_tonumber(L, -1);
    }
    lua_pop(L, 1);
}

template<>
inline void lua_push(lua_State* L, Storytime::KeyPressedEvent* value) {
    lua_newtable(L);

    lua_pushnumber(L, value->event_type);
    lua_setfield(L, -2, "type");

    lua_pushstring(L, value->event_type_name.c_str());
    lua_setfield(L, -2, "type_name");

    lua_pushnumber(L, value->key_code);
    lua_setfield(L, -2, "key_code");

    lua_pushnumber(L, value->mods);
    lua_setfield(L, -2, "mods");

    lua_pushnumber(L, value->scan_code);
    lua_setfield(L, -2, "scan_code");
}