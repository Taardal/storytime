#include "event_manager.h"

namespace Storytime {
    u32 EventManager::subscription_counter = 0;

    EventManager::EventManager(EventManagerConfig config) : config(std::move(config)) {
        ST_ASSERT(config.queue_count > 0, "Event manager must have at least one queue");
        queues.resize(config.queue_count);
    }

    SubscriptionID EventManager::subscribe(const EventType event_type, const Subscription& subscription) {
        SubscriptionID subscription_id = ++subscription_counter;
        subscriptions[event_type].emplace_back(subscription_id, subscription);
        ST_LOG_TRACE("Added subscription with ID [{}] for event type [{}]", subscription_id, Event::get_name(event_type));
        return subscription_id;
    }

    bool EventManager::remove_subscription(EventType event_type, SubscriptionID subscription_id) {
        if (auto it = subscriptions.find(event_type); it != subscriptions.end()) {
            SubscriptionList& event_subscriptions = it->second;
            for (int i = 0; i < event_subscriptions.size(); ++i) {
                if (event_subscriptions[i].first == subscription_id) {
                    event_subscriptions.erase(event_subscriptions.begin() + i);
                    ST_LOG_TRACE("Removed subscription with ID [{}] for event type [{}]", subscription_id, Event::get_name(event_type));
                    return true;
                }
            }
        }
        return false;
    }

    bool EventManager::trigger_event(const EventType event_type, const Event& event) {
        if (auto it = subscriptions.find(event_type); it != subscriptions.end()) {
            for (auto& [subscription_id, subscription] : it->second) {
                subscription(event);
            }
            ST_LOG_TRACE("Triggered event {}", event.to_string());
            return true;
        }
        return false;
    }

    void EventManager::queue_event(EventType event_type, const Shared<Event>& event) {
        ST_ASSERT(event != nullptr, "Event to be queued cannot be null");
        ST_ASSERT(queue_index >= 0, "Queue index [" << queue_index << "] cannot be negative");
        ST_ASSERT(queue_index < queues.size(), "Queue index [" << queue_index << "] must be less than queue count [" << queues.size() << "]");
        EventQueue& queue = queues[queue_index];
        queue[event_type].emplace_back(event);
        ST_LOG_TRACE("Queued event {}", event->to_string());
    }

    void EventManager::queue_event(EventType event_type, const Event& event) {
        queue_event(event_type, make_shared<Event>(event));
    }

    void EventManager::process_event_queue() {
        u32 processed_event_count = 0;
        auto& queue = queues[queue_index];
        for (auto& [event_type, events] : queue) {
            u32 processed_event_count_for_type = 0;
            for (auto& event : events) {
                if (auto it = subscriptions.find(event_type); it != subscriptions.end()) {
                    for (auto& [listener_id, listener] : it->second) {
                        ST_ASSERT(event != nullptr, "Cannot process event that is null: Event queue should never contain any null events");
                        listener(*event);
                        processed_event_count++;
                        processed_event_count_for_type++;
                    }
                    ST_LOG_TRACE("Processed event {}", event->to_string());
                }
            }
            if (processed_event_count_for_type > 0) {
                ST_LOG_TRACE("Processed [{}] events of type [{}]", processed_event_count_for_type, Event::get_name(event_type));
                if (processed_event_count_for_type < events.size()) {
                    ST_LOG_WARNING("Processed only [{}] of [{}] events of type [{}]", processed_event_count_for_type, events.size(), Event::get_name(event_type));
                }
            }
        }
        if (processed_event_count > 0) {
            ST_LOG_TRACE("Processed [{}] events", processed_event_count);
        }
        queue.clear();
        queue_index = (queue_index + 1) % config.queue_count;
    }
}
