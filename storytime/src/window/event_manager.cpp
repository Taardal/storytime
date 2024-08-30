#include "event_manager.h"

namespace Storytime {
    u32 EventManager::subscription_counter = 0;

    SubscriptionID EventManager::subscribe(const EventType event_type, const Subscription& subscription) {
        SubscriptionID subscription_id = ++subscription_counter;
        subscriptions[event_type].emplace_back(subscription_id, subscription);
        ST_LOG_TRACE("Added subscription with ID [{}] for event type [{}]", subscription_id, Event::get_name(event_type));
        return subscription_id;
    }

    bool EventManager::trigger_event(const EventType event_type, const Event& event) {
        if (auto it = subscriptions.find(event_type); it != subscriptions.end()) {
            for (auto& [subscription_id, subscription] : it->second) {
                subscription(event);
            }
            ST_LOG_TRACE("Triggered event of type [{}]", Event::get_name(event_type));
            return true;
        }
        return false;
    }
}
