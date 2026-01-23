#pragma once

#include "lua/st_lua_function.h"
#include "system/st_dispatcher.h"

namespace Storytime {
    class LuaDispatcherBinding {
    public:
        static const std::string metatable_name;

    private:
        typedef std::function<i32(lua_State*, LuaRef, LuaDispatcherBinding*)> SubscribeFn;
        static std::map<std::string, SubscribeFn> subscribe_fns;

        typedef std::function<void(lua_State*, i32, LuaDispatcherBinding*)> TriggerFn;
        static std::map<std::string, TriggerFn> trigger_fns;

        typedef std::function<void(lua_State*, i32, LuaDispatcherBinding*)> EnqueueFn;
        static std::map<std::string, EnqueueFn> enqueue_fns;

    private:
        lua_State* L;
        Dispatcher& dispatcher;
        std::vector<i32> subscription_ids;
        std::map<i32, LuaRef> subscription_fn_refs;

    public:
        LuaDispatcherBinding(lua_State* L, Dispatcher& dispatcher);

        ~LuaDispatcherBinding();

        template<typename T>
        static void bind_type(const std::string& type_name = T::type_name) {
            ST_ASSERT(
                !subscribe_fns.contains(type_name),
                "Cannot bind type [" << type_name << "] because it has already been bound"
            );

            subscribe_fns.emplace(type_name, [](lua_State* L, LuaRef lua_subscription_fn_ref, LuaDispatcherBinding* binding) -> i32 {
                i32 subscription_id = binding->dispatcher.subscribe<T>([L, lua_subscription_fn_ref](const T& value) {
                    ST_ASSERT(lua_subscription_fn_ref.is_valid(), "Subscription function ref must be valid");
                    LuaFunction lua_fn(L, lua_subscription_fn_ref);
                    lua_fn.invoke((T*) &value);
                });
                binding->subscription_ids.push_back(subscription_id);
                binding->subscription_fn_refs.emplace(subscription_id, lua_subscription_fn_ref);
                return subscription_id;
            });

            trigger_fns.emplace(type_name, [](lua_State* L, i32 value_lua_index, const LuaDispatcherBinding* binding) {
                T value;
                lua_to<T>(L, value_lua_index, &value);
                binding->dispatcher.trigger<T>(std::move(value));
            });

            enqueue_fns.emplace(type_name, [](lua_State* L, i32 value_lua_index, const LuaDispatcherBinding* binding) {
                T value;
                lua_to<T>(L, value_lua_index, &value);
                binding->dispatcher.enqueue<T>(value);
            });
        }

        static i32 create_metatable(lua_State* L);

        static i32 create(lua_State* L, Dispatcher& dispatcher);

        static i32 lua_destroy(lua_State* L);

        static i32 lua_index(lua_State* L);

        static i32 lua_subscribe(lua_State* L);

        static i32 lua_unsubscribe(lua_State* L);

        static i32 lua_trigger(lua_State* L);

        static i32 lua_enqueue(lua_State* L);
    };
}
