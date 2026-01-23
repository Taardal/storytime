#include "st_lua_dispatcher_binding.h"

#include <ranges>

namespace Storytime {
    const std::string LuaDispatcherBinding::metatable_name = "LuaDispatcherBinding";

    std::map<std::string, LuaDispatcherBinding::SubscribeFn> LuaDispatcherBinding::subscribe_fns = {};
    std::map<std::string, LuaDispatcherBinding::TriggerFn> LuaDispatcherBinding::trigger_fns = {};
    std::map<std::string, LuaDispatcherBinding::EnqueueFn> LuaDispatcherBinding::enqueue_fns = {};

    LuaDispatcherBinding::LuaDispatcherBinding(lua_State* L, Dispatcher& dispatcher)
        : L(L), dispatcher(dispatcher) {}

    LuaDispatcherBinding::~LuaDispatcherBinding() {
        for (i32 subscription_id : subscription_ids) {
            dispatcher.unsubscribe(subscription_id);
        }
        subscription_ids.clear();

        for (LuaRef subscription_fn_ref : subscription_fn_refs | std::views::values) {
            subscription_fn_ref.destroy(L);
        }
        subscription_fn_refs.clear();
    }

    i32 LuaDispatcherBinding::create_metatable(lua_State* L) {
        ST_ASSERT_NOT_NULL(L);

        luaL_newmetatable(L, metatable_name.c_str());

        lua_pushcfunction(L, lua_destroy);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_index);
        lua_setfield(L, -2, "__index");

        return 1;
    }

    i32 LuaDispatcherBinding::create(lua_State* L, Dispatcher& dispatcher) {
        ST_ASSERT_NOT_NULL(L);

        auto userdata = lua_newuserdata(L, sizeof(LuaDispatcherBinding));
        new (userdata) LuaDispatcherBinding(L, dispatcher);

        luaL_getmetatable(L, metatable_name.c_str());
        ST_ASSERT(!lua_isnil(L, -1), "Metatable [" << metatable_name.c_str() << "] cannot be null");
        lua_setmetatable(L, -2);

        return 1;
    }

    i32 LuaDispatcherBinding::lua_destroy(lua_State* L) {
        ST_ASSERT(lua_isuserdata(L, -1), "Binding must be on expected Lua stack index");

        auto binding = (LuaDispatcherBinding*) lua_touserdata(L, -1);
        ST_ASSERT(binding != nullptr, "Userdata cannot be null");

        binding->~LuaDispatcherBinding();
        return 0;
    }

    i32 LuaDispatcherBinding::lua_index(lua_State* L) {
        ST_ASSERT(lua_isstring(L, -1), "Index key must be on expected Lua stack index");

        const char* key = lua_tostring(L, -1);
        ST_ASSERT_NOT_CEMPTY(key);

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
        if (strcmp(key, "enqueue") == 0) {
            lua_pushcfunction(L, lua_enqueue);
            return 1;
        }

        return 0;
    }

    i32 LuaDispatcherBinding::lua_subscribe(lua_State* L) {
        ST_ASSERT(lua_isfunction(L, -1), "Subscription function argument must be on expected Lua stack index");
        ST_ASSERT(lua_isstring(L, -2), "Type name argument must be on expected Lua stack index");
        ST_ASSERT(lua_isuserdata(L, -3), "Binding must be on expected Lua stack index");

        LuaRef lua_subscription_fn_ref = LuaRef::create(L, { .keep_on_stack = true });
        ST_ASSERT(lua_subscription_fn_ref.is_valid(), "Subscription function ref must be valid");

        std::string type_name = lua_tostring(L, -2);
        ST_ASSERT_NOT_EMPTY(type_name);

        auto binding = (LuaDispatcherBinding*) lua_touserdata(L, -3);
        ST_ASSERT_NOT_NULL(binding);

        auto subscribe_fn_it = subscribe_fns.find(type_name);
        ST_ASSERT(subscribe_fn_it != subscribe_fns.end(), "Subscribe function for type [" << type_name << "] must exist");
        SubscribeFn& subscribe = subscribe_fn_it->second;

        i32 subscription_id = subscribe(L, lua_subscription_fn_ref, binding);
        ST_ASSERT(subscription_id > 0, "Subscription ID must be greater than zero to be valid");

        lua_pushnumber(L, subscription_id);
        return 1;
    }

    i32 LuaDispatcherBinding::lua_unsubscribe(lua_State* L) {
        ST_ASSERT(lua_isnumber(L, -1), "Subscription ID argument must be on expected Lua stack index");
        ST_ASSERT(lua_isuserdata(L, -2), "Binding must be on expected Lua stack index");

        i32 subscription_id = lua_tonumber(L, -1);
        ST_ASSERT(subscription_id > 0, "Subscription ID must be greater than zero to be valid");

        auto binding = (LuaDispatcherBinding*) lua_touserdata(L, -2);
        ST_ASSERT_NOT_NULL(binding);

        bool unsubscribed = binding->dispatcher.unsubscribe(subscription_id);
        if (unsubscribed) {
            i32 erased_subscription_ids = std::erase_if(binding->subscription_ids, [subscription_id](i32 id) {
                return id == subscription_id;
            });
            ST_ASSERT(
                erased_subscription_ids > 0,
                "Lua subscription ID [" << subscription_id << "] must exist when unsubscribing"
            );
            auto subscription_fn_lua_ref_it = binding->subscription_fn_refs.find(subscription_id);
            ST_ASSERT(
                subscription_fn_lua_ref_it != binding->subscription_fn_refs.end(),
                "Lua subscription function ref must exist for subscription with ID [" << subscription_id << "] when unsubscribing"
            );
            LuaRef subscription_fn_lua_ref = subscription_fn_lua_ref_it->second;
            ST_ASSERT(
                subscription_fn_lua_ref.is_valid(),
                "Lua subscription function ref must be valid for subscription with ID [" << subscription_id << "] when unsubscribing"
            );
            subscription_fn_lua_ref.destroy(L);
        }

        lua_pushboolean(L, unsubscribed);
        return 1;
    }

    i32 LuaDispatcherBinding::lua_trigger(lua_State* L) {
        ST_ASSERT(lua_istable(L, -1), "Value argument must be on expected Lua stack index");
        ST_ASSERT(lua_isstring(L, -2), "Type name argument must be on expected Lua stack index");
        ST_ASSERT(lua_isuserdata(L, -3), "Binding must be on expected Lua stack index");

        std::string type_name = lua_tostring(L, -2);
        ST_ASSERT_NOT_EMPTY(type_name);

        auto binding = (LuaDispatcherBinding*) lua_touserdata(L, -3);
        ST_ASSERT_NOT_NULL(binding);

        auto trigger_fn_it = trigger_fns.find(type_name);
        ST_ASSERT(trigger_fn_it != trigger_fns.end(), "Trigger function for type [" << type_name << "] must exist");
        TriggerFn& trigger = trigger_fn_it->second;

        trigger(L, -1, binding);
        return 0;
    }

    i32 LuaDispatcherBinding::lua_enqueue(lua_State* L) {
        ST_ASSERT(lua_istable(L, -1), "Value argument must be on expected Lua stack index");
        ST_ASSERT(lua_isstring(L, -2), "Type name argument must be on expected Lua stack index");
        ST_ASSERT(lua_isuserdata(L, -3), "Binding must be on expected Lua stack index");

        std::string type_name = lua_tostring(L, -2);
        ST_ASSERT_NOT_EMPTY(type_name);

        auto binding = (LuaDispatcherBinding*) lua_touserdata(L, -3);
        ST_ASSERT_NOT_NULL(binding);

        auto enqueue_fn_it = enqueue_fns.find(type_name);
        ST_ASSERT(enqueue_fn_it != enqueue_fns.end(), "Enqueue function for type [" << type_name << "] must exist");
        EnqueueFn& enqueue = enqueue_fn_it->second;

        enqueue(L, -1, binding);
        return 0;
    }
}
