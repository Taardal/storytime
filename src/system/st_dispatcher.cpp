#include "st_dispatcher.h"

#include <ranges>

namespace Storytime {
    u32 Dispatcher::next_subscription_id = 0;

    Dispatcher::Dispatcher(const DispatcherConfig& config) : config(config) {
    }

    Dispatcher::~Dispatcher() {
        for (entt::connection& connection : subscription_connections | std::views::values) {
            connection.release();
        }
        subscription_connections.clear();
        subscription_functions.clear();
    }

    void Dispatcher::update() const {
        ST_ASSERT_NOT_NULL(config.dispatcher);
        config.dispatcher->update();
    }

    bool Dispatcher::unsubscribe(i32 subscription_id) {
        bool subscription_connection_erased = false;
        auto subscription_connection_it = subscription_connections.find(subscription_id);
        if (subscription_connection_it != subscription_connections.end()) {
            entt::connection& connection = subscription_connection_it->second;
            connection.release();
            subscription_connections.erase(subscription_connection_it);
            subscription_connection_erased = true;
        }

        bool subscription_function_erased = false;
        auto subscription_function_it = subscription_functions.find(subscription_id);
        if (subscription_function_it != subscription_functions.end()) {
            subscription_functions.erase(subscription_function_it);
            subscription_function_erased = true;
        }

        bool unsubscribed = subscription_connection_erased || subscription_function_erased;
        if (unsubscribed) {
            ST_LOG_TRACE("Removed subscription with ID [{}]", subscription_id);
        } else {
            ST_LOG_WARNING("Could not remove subscription with ID [{}] because it did not exist", subscription_id);
        }
        return unsubscribed;
    }
}
