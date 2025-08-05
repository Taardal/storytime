#pragma once

#include <entt/entt.hpp>

namespace Storytime {
    template<typename T>
    using SubscriptionFn = std::function<void(const T&)>;

    template<typename T>
    using Sink = entt::sink<entt::sigh<void(T&)>>;

    struct DispatcherConfig {
        entt::dispatcher* dispatcher = nullptr;
    };

    class Dispatcher {
    private:
        static u32 next_subscription_id;

    private:
        DispatcherConfig config;
        std::map<i32, entt::connection> subscription_connections;
        std::map<i32, std::shared_ptr<void>> subscription_functions;

    public:
        Dispatcher(const DispatcherConfig& config);

        /// Get the sink for a type. A sink manages the subscribers for a type. Each type `T` has its own sink.
        /// @tparam T The sink type.
        /// @return The sink.
        template<typename T>
        Sink<T> sink() const {
            return config.dispatcher->sink<T>();
        }

        /// Subscribe a function (static or member function) to a type.
        /// @tparam T The type to subscribe to (e.g., FooEvent, BarCommand).
        /// @tparam SubscriptionFnPtr A function pointer: either static or member (e.g., &SomeClass::on_event).
        /// @tparam Args Additional argument types needed to bind to the function (e.g., an instance pointer)
        /// @param args Additional arguments needed to bind to the function (e.g., an instance pointer)
        template<typename T, auto SubscriptionFnPtr, typename... Args>
        void subscribe(Args&&...args) {
            ST_ASSERT_NOT_NULL(config.dispatcher);

            //
            // Bind the subscription function pointer to the type.
            //
            // The subscription function pointer is a _non-type template parameter_, meaning it's passed as a value instead a type
            // to the template. Using `auto` lets the compiler automatically deduce the correct type of the function pointer.
            //
            // The `template` keyword is required because the sink object is templated, and we are calling a templated function on it.
            // When you call a templated function on a templated type in C++, you need to explicitly make it clear to the compiler that
            // you are invoking a templated function, and not invoking the `<>` comparison operator.
            //
            // `std::forward<T>(x)` preserves the value category (lvalue/rvalue) of arguments when forwarding them to the `connect`
            // function to avoid unnecessary copies or moves. rvalues are forwarded as rvalues, and lvalues are forwarded as lvalues.
            //
            config.dispatcher->sink<T>().template connect<SubscriptionFnPtr>(std::forward<Args>(args)...);
        }

        /// Unsubscribe a function (static or member function) from a type.
        /// @tparam T The type to unsubscribe from (e.g., FooEvent, BarCommand).
        /// @tparam SubscriptionFnPtr A function pointer: either static or member (e.g., &SomeClass::on_event).
        /// @tparam Args Additional argument types needed to unbind from the function (e.g., an instance pointer)
        /// @param args Additional arguments needed to unbind from the function (e.g., an instance pointer)
        template<typename T, auto SubscriptionFnPtr, typename... Args>
        void unsubscribe(Args&&...args) {
            ST_ASSERT_NOT_NULL(config.dispatcher);

            //
            // Unbind the subscription function pointer from the type.
            //
            // The subscription function pointer is a _non-type template parameter_, meaning it's passed as a value instead a type
            // to the template. Using `auto` lets the compiler automatically deduce the correct type of the function pointer.
            //
            // The `template` keyword is required because the sink object is templated, and we are calling a templated function on it.
            // When you call a templated function on a templated type in C++, you need to explicitly make it clear to the compiler that
            // you are invoking a templated function, and not invoking the `<>` comparison operator.
            //
            // `std::forward<T>(x)` preserves the value category (lvalue/rvalue) of arguments when forwarding them to the `connect`
            // function to avoid unnecessary copies or moves. rvalues are forwarded as rvalues, and lvalues are forwarded as lvalues.
            //
            config.dispatcher->sink<T>().template disconnect<SubscriptionFnPtr>(std::forward<Args>(args)...);
        }

        /// Subscribe a lambda function to a type.
        /// @tparam T The type to subscribe to (e.g., FooEvent, BarCommand).
        /// @param subscription_fn A lambda function (e.g., []() { ... }). May be a capturing lambda.
        /// @return The ID of the subscription. May be used to unsubscribe.
        template<typename T>
        i32 subscribe(const SubscriptionFn<T>& subscription_fn) {
            ST_ASSERT_NOT_NULL(config.dispatcher);
            i32 subscription_id = ++next_subscription_id;

            // Keep the subscription function alive.
            auto subscription_fn_ptr = std::make_shared<SubscriptionFn<T>>(subscription_fn);
            subscription_functions.emplace(subscription_id, subscription_fn_ptr);

            //
            // Bind the subscription function to the type.
            //
            // The `template` keyword is required because the sink object is templated, and we are calling a templated function on it.
            // When you call a templated function on a templated type in C++, you need to explicitly make it clear to the compiler that
            // you are invoking a templated function, and not invoking the `<>` comparison operator.
            //
            // Using `&SubscriptionFn<T>::operator()` lets us bind the pointer to the subscription function's call operator to the type.
            // Since the subscription function is expected to be a lambda, we don't have a free/static/member function pointer to bind, so
            // we store the function as a shared pointer to keep it alive, and then bind the function's call operator instead to the type.
            //
            entt::connection connection = config.dispatcher->sink<T>().template connect<&SubscriptionFn<T>::operator()>(*subscription_fn_ptr);
            subscription_connections.emplace(subscription_id, connection);

            ST_LOG_TRACE("Added subscription with ID [{}] for event type [{}]", subscription_id, type_name<T>());
            return subscription_id;
        }

        /// Unsubscribe a lambda function from a type.
        /// @tparam T The type to unsubscribe from (e.g., FooEvent, BarCommand).
        /// @return True if successful.
        bool unsubscribe(i32 subscription_id);

        // T&& makes this a universal reference (aka forwarding reference).
        // std::decay_t<T> strips off references and const qualifiers — giving you the raw value type.
        // std::forward<T>(event) moves if rvalue, copies if lvalue — exactly what EnTT expects.
        template<typename T>
        void trigger(T&& event) {
            ST_ASSERT_NOT_NULL(config.dispatcher);

            //
            // `T&&` is a rvalue reference, which is a temporary object, literal, or expression that do not have a persistent memory
            // address or name.
            //
            // `std::decay_t<T>` strips off references and const qualifiers to give us the raw value type.
            //
            // `std::forward<T>(x)` preserves the value category (lvalue/rvalue) of arguments when forwarding them to the `enqueue`
            // function to avoid unnecessary copies or moves. rvalues are forwarded as rvalues, and lvalues are forwarded as lvalues.
            //
            config.dispatcher->trigger<std::decay_t<T>>(std::forward<T>(event));

            ST_LOG_TRACE("Triggered event [{}]", type_name<T>());
        }

        /// Add an object of a given type to a queue to be dispatched later.
        /// @tparam T The type of the object.
        /// @tparam Args Additional argument types needed to construct the event.
        /// @param args Additional arguments needed to construct the event.
        template<typename T, typename... Args>
        void enqueue(Args&&... args) {
            ST_ASSERT_NOT_NULL(config.dispatcher);

            // `std::forward<T>(x)` preserves the value category (lvalue/rvalue) of arguments when forwarding them to the `enqueue`
            // function to avoid unnecessary copies or moves. rvalues are forwarded as rvalues, and lvalues are forwarded as lvalues.
            config.dispatcher->enqueue<T>(std::forward<Args>(args)...);

            ST_LOG_TRACE("Enqueued event [{}]", type_name<T>());
        }

        void update() const;
    };
}
