#include "event_manager.h"

namespace Storytime {
    u32 EventManager::subscription_counter = 0;

    SubscriptionID EventManager::subscribe(const EventType event_type, const Subscription& subscription) {
        ST_LOG_T("Adding listener for event type [{}]", event_type);
        SubscriptionID subscription_id = ++subscription_counter;
        subscriptions[event_type].emplace_back(subscription_id, subscription);
        ST_LOG_D("Added subscription with ID [{}] for event type [{}]", subscription_id, event_type);
        return subscription_id;
    }

    bool EventManager::trigger_event(const EventType event_type, const Event& event) {
        ST_LOG_T("Triggering event of type [{}]", event_type);
        if (auto it = subscriptions.find(event_type); it != subscriptions.end()) {
            for (auto& [subscription_id, subscription] : it->second) {
                subscription(event);
            }
            ST_LOG_D("Triggered event of type [{}]", event_type);
            return true;
        }
        ST_LOG_W("Could not find any listeners for event type [{}]", event_type);
        return false;
    }
}
