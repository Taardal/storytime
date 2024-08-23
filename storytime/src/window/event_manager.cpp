#include "event_manager.h"

namespace Storytime {
    u32 EventManager::subscription_counter = 0;

    SubscriptionID EventManager::subscribe(const EventType event_type, const Subscription& subscription) {
        ST_LOG_TRACE("Adding subscription for event type [{}]", event_type);
        SubscriptionID subscription_id = ++subscription_counter;
        subscriptions[event_type].emplace_back(subscription_id, subscription);
        ST_LOG_DEBUG("Added subscription with ID [{}] for event type [{}]", subscription_id, event_type);
        return subscription_id;
    }

    bool EventManager::trigger_event(const EventType event_type, const Event& event) {
        ST_LOG_TRACE("Triggering event of type [{}]", event_type);
        if (auto it = subscriptions.find(event_type); it != subscriptions.end()) {
            for (auto& [subscription_id, subscription] : it->second) {
                subscription(event);
            }
            ST_LOG_DEBUG("Triggered event of type [{}]", event_type);
            return true;
        }
        return false;
    }
}
