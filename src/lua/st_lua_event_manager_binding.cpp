#include "st_lua_event_manager_binding.h"

namespace Storytime {
    const std::string LuaEventManagerBinding::metatable_name = "LuaEventManagerBinding";

    std::map<std::string, EventType> LuaEventManagerBinding::event_names_to_types = {};
    std::map<EventType, std::string> LuaEventManagerBinding::event_types_to_names = {};

    std::map<EventType, LuaEventManagerBinding::SendEventToLuaFn> LuaEventManagerBinding::send_event_to_lua_fns = {};
    std::map<EventType, LuaEventManagerBinding::ReceiveEventFromLuaFn> LuaEventManagerBinding::receive_event_from_lua_fns = {};

    LuaEventManagerBinding::LuaEventManagerBinding(lua_State* L, EventManager* event_manager)
        : L(L), event_manager(event_manager) {
    }

    LuaEventManagerBinding::~LuaEventManagerBinding() {
        for (auto subscription : subscriptions) {
            event_manager->unsubscribe(subscription);
        }
        subscriptions.clear();
        for (auto ref : subscription_fn_refs) {
            ref.destroy(L);
        }
        subscription_fn_refs.clear();
    }

    i32 LuaEventManagerBinding::create_metatable(lua_State* L) {
        luaL_newmetatable(L, metatable_name.c_str());

        lua_pushcfunction(L, lua_destroy);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_index);
        lua_setfield(L, -2, "__index");

        return 1;
    }

    i32 LuaEventManagerBinding::create(lua_State* L, EventManager* event_manager) {
        ST_ASSERT(event_manager != nullptr, "Event manager cannot be null");

        void* binding = lua_newuserdata(L, sizeof(LuaEventManagerBinding));
        new (binding) LuaEventManagerBinding(L, event_manager);

        luaL_getmetatable(L, metatable_name.c_str());
        ST_ASSERT(!lua_isnil(L, -1), "Metatable [" << metatable_name.c_str() << "] cannot be null");
        lua_setmetatable(L, -2);

        return 1;
    }

    i32 LuaEventManagerBinding::lua_destroy(lua_State* L) {
        ST_ASSERT(lua_type(L, -1) == LUA_TUSERDATA, "Userdata must be at expected stack location");

        auto binding = static_cast<LuaEventManagerBinding*>(lua_touserdata(L, -1));
        ST_ASSERT(binding != nullptr, "Userdata cannot be null");

        binding->~LuaEventManagerBinding();
        return 0;
    }

    i32 LuaEventManagerBinding::lua_index(lua_State* L) {
        ST_ASSERT(lua_type(L, -1) == LUA_TSTRING, "Key must be at expected stack location");

        const char* key = lua_tostring(L, -1);
        ST_ASSERT(strlen(key) > 0, "Key cannot be empty");

        if (strcmp(key, "subscribe") == 0) {
            lua_pushcfunction(L, lua_subscribe);
            return 1;
        }
        if (strcmp(key, "unsubscribe") == 0) {
            lua_pushcfunction(L, lua_unsubscribe);
            return 1;
        }
        if (strcmp(key, "trigger") == 0) {
            lua_pushcfunction(L, lua_trigger);
            return 1;
        }
        if (strcmp(key, "queue") == 0) {
            lua_pushcfunction(L, lua_queue);
            return 1;
        }

        return 0;
    }

    i32 LuaEventManagerBinding::lua_subscribe(lua_State* L) {
        ST_ASSERT(
            lua_type(L, -1) == LUA_TFUNCTION,
            "Subscription function must be at expected stack location"
        );
        ST_ASSERT(
            lua_type(L, -2) == LUA_TNUMBER || lua_type(L, -2) == LUA_TSTRING,
            "Event type or name must be at expected stack location"
        );
        ST_ASSERT(
            lua_type(L, -3) == LUA_TUSERDATA,
            "Userdata must be at expected stack location"
        );

        LuaRef lua_subscription_fn_ref = LuaRef::create(L, { .keep_on_stack = true });
        ST_ASSERT(lua_subscription_fn_ref.is_valid(), "Subscription function ref must be valid");

        EventType event_type = 0;
        if (lua_type(L, -2) == LUA_TNUMBER) {
            event_type = lua_tonumber(L, -2);
        }
        if (lua_type(L, -2) == LUA_TSTRING) {
            std::string event_type_name = lua_tostring(L, -2);
            auto it = event_names_to_types.find(event_type_name);
            ST_ASSERT(
                it != event_names_to_types.end(),
                "Event type name [" << event_type_name << "] must have an equivalent event type"
            );
            event_type = it->second;
        }
        ST_ASSERT(event_type > 0, "Event type must be valid");

        auto binding = static_cast<LuaEventManagerBinding*>(lua_touserdata(L, -3));
        ST_ASSERT(binding != nullptr, "Userdata cannot be null");

        auto event_subscription = [L, event_type, lua_subscription_fn_ref](const Event& event) {
            ST_ASSERT(lua_subscription_fn_ref.is_valid(), "Subscription function ref must be valid");

            auto it = send_event_to_lua_fns.find(event_type);
            ST_ASSERT(it != send_event_to_lua_fns.end(), "Event serializer function is required");
            const SendEventToLuaFn& send_event_to_lua = it->second;

            send_event_to_lua(L, lua_subscription_fn_ref, event);
        };

        SubscriptionID subscription_id = binding->event_manager->subscribe(event_type, event_subscription);
        binding->subscriptions.push_back(subscription_id);
        binding->subscription_fn_refs.push_back(lua_subscription_fn_ref);

        lua_pushnumber(L, subscription_id);
        return 1;
    }

    i32 LuaEventManagerBinding::lua_unsubscribe(lua_State* L) {
        ST_ASSERT(lua_type(L, -1) == LUA_TNUMBER, "Subscription ID must be at expected stack location");
        ST_ASSERT(lua_type(L, -2) == LUA_TUSERDATA, "Userdata must be at expected stack location");

        SubscriptionID subscription_id = lua_tonumber(L, -1);
        ST_ASSERT(subscription_id > 0, "Subscription ID must be valid");

        auto binding = static_cast<LuaEventManagerBinding*>(lua_touserdata(L, -2));
        ST_ASSERT(binding != nullptr, "Userdata cannot be null");

        bool unsubscribed = binding->event_manager->unsubscribe(subscription_id);

        lua_pushboolean(L, unsubscribed);
        return 1;
    }

    i32 LuaEventManagerBinding::lua_trigger(lua_State* L) {
        ST_ASSERT(lua_type(L, -1) == LUA_TTABLE, "Event object must be at expected stack location");
        ST_ASSERT(lua_type(L, -2) == LUA_TNUMBER, "Event type must be at expected stack location");
        ST_ASSERT(lua_type(L, -3) == LUA_TUSERDATA, "Userdata must be at expected stack location");

        EventType event_type = lua_tonumber(L, -2);
        ST_ASSERT(event_type > 0, "Event type must be valid");

        auto binding = static_cast<LuaEventManagerBinding*>(lua_touserdata(L, -3));
        ST_ASSERT(binding != nullptr, "Userdata cannot be null");

        auto it = receive_event_from_lua_fns.find(event_type);
        ST_ASSERT(it != receive_event_from_lua_fns.end(), "Event deserializer must exist");
        const ReceiveEventFromLuaFn& receive_event_from_lua = it->second;

        Event event;
        receive_event_from_lua(L, -1, &event);

        binding->event_manager->trigger_event(event_type, event);
        return 0;
    }

    i32 LuaEventManagerBinding::lua_queue(lua_State* L) {
        ST_ASSERT(lua_type(L, -1) == LUA_TTABLE, "Event object data must be at expected stack location");
        ST_ASSERT(lua_type(L, -2) == LUA_TNUMBER, "Event type must be at expected stack location");
        ST_ASSERT(lua_type(L, -3) == LUA_TUSERDATA, "Userdata must be at expected stack location");

        EventType event_type = lua_tonumber(L, -2);
        ST_ASSERT(event_type > 0, "Event type must be valid");

        auto binding = static_cast<LuaEventManagerBinding*>(lua_touserdata(L, -3));
        ST_ASSERT(binding != nullptr, "Userdata cannot be null");

        auto it = receive_event_from_lua_fns.find(event_type);
        ST_ASSERT(it != receive_event_from_lua_fns.end(), "Event deserializer must exist");
        const ReceiveEventFromLuaFn& receive_event_from_lua = it->second;

        auto event = std::make_shared<Event>();
        receive_event_from_lua(L, -1, &event);

        binding->event_manager->queue_event(event_type, event);
        return 0;
    }
}
