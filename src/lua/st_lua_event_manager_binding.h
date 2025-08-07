#pragma once

#include "event/st_event.h"
#include "event/st_event_manager.h"

#include "lua/st_lua_function.h"

namespace Storytime {
    class LuaEventManagerBinding {
    public:
        static const std::string metatable_name;

    private:
        static std::map<EventType, std::string> event_types_to_names;
        static std::map<std::string, EventType> event_names_to_types;

        typedef std::function<void(lua_State*, LuaRef, const Event&)> SendEventToLuaFn;
        static std::map<EventType, SendEventToLuaFn> send_event_to_lua_fns;

        typedef std::function<void(lua_State*, i32, void* event)> ReceiveEventFromLuaFn;
        static std::map<EventType, ReceiveEventFromLuaFn> receive_event_from_lua_fns;

    private:
        lua_State* L = nullptr;
        EventManager* event_manager = nullptr;
        std::vector<SubscriptionID> subscriptions;
        std::vector<LuaRef> subscription_fn_refs;

    public:
        LuaEventManagerBinding(lua_State* L, EventManager* event_manager);

        ~LuaEventManagerBinding();

        static i32 create_metatable(lua_State* L);

        static i32 create(lua_State* L, EventManager* event_manager);

        static i32 lua_destroy(lua_State* L);

        static i32 lua_index(lua_State* L);

        static i32 lua_subscribe(lua_State* L);

        static i32 lua_unsubscribe(lua_State* L);

        static i32 lua_trigger(lua_State* L);

        static i32 lua_queue(lua_State* L);

        template<typename T>
        static void bind_event() {
            ST_ASSERT(
                !event_names_to_types.contains(T::type_name),
                "Cannot bind event [" << T::type_name << "] that has already been bound"
            );

            event_types_to_names.emplace(T::type, T::type_name);
            event_names_to_types.emplace(T::type_name, T::type);

            SendEventToLuaFn send_event_to_lua_fn = [](lua_State* L, LuaRef lua_fn_ref, const Event& event) {
                LuaFunction lua_fn(L, lua_fn_ref);
                lua_fn.invoke((T*) &event);
            };
            send_event_to_lua_fns.emplace(T::type, send_event_to_lua_fn);

            ReceiveEventFromLuaFn receive_event_from_lua_fn = [](lua_State* L, i32 lua_index, void* event) {
                lua_to<T>(L, lua_index, (T*) event);
            };
            receive_event_from_lua_fns.emplace(T::type, receive_event_from_lua_fn);
        }
    };
}

