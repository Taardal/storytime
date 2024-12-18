#include "event_manager.h"

namespace Storytime {
    u32 EventManager::next_subscription_id = 0;

    EventManager::EventManager(EventManagerConfig config) : config(std::move(config)) {
        ST_ASSERT(config.queue_count > 0, "Event manager must have at least one queue");
        queues.resize(config.queue_count);
    }

    SubscriptionID EventManager::subscribe(const EventType event_type, const Subscription& subscription) {
        SubscriptionID subscription_id = ++next_subscription_id;
        subscriptions[event_type].emplace_back(subscription_id, subscription);
        ST_LOG_TRACE("Added subscription with ID [{}] for event type [{}]", subscription_id, event_type);
        return subscription_id;
    }

    bool EventManager::unsubscribe(SubscriptionID subscription_id) {
        for (auto& subscription : subscriptions) {
            EventType event_type = subscription.first;
            SubscriptionList& event_subscriptions = subscription.second;
            for (int i = 0; i < event_subscriptions.size(); ++i) {
                if (event_subscriptions[i].first == subscription_id) {
                    event_subscriptions.erase(event_subscriptions.begin() + i);
                    ST_LOG_TRACE("Removed subscription with ID [{}] for event type [{}]", subscription_id, event_type);
                    return true;
                }
            }
        }
        return false;
    }

    bool EventManager::unsubscribe(SubscriptionID subscription_id, EventType event_type) {
        if (auto it = subscriptions.find(event_type); it != subscriptions.end()) {
            SubscriptionList& event_subscriptions = it->second;
            for (int i = 0; i < event_subscriptions.size(); ++i) {
                if (event_subscriptions[i].first == subscription_id) {
                    event_subscriptions.erase(event_subscriptions.begin() + i);
                    ST_LOG_TRACE("Removed subscription with ID [{}] for event type [{}]", subscription_id, event_type);
                    return true;
                }
            }
        }
        return false;
    }

    bool EventManager::unsubscribe(const std::vector<SubscriptionID>& subscriptions) {
        for (u32 subscription_id : subscriptions) {
            bool unsubscribed = unsubscribe(subscription_id);
            if (!unsubscribed) {
                return false;
            }
        }
        return true;
    }

    bool EventManager::unsubscribe_and_clear(std::vector<SubscriptionID>& subscriptions) {
        bool unsubscribed = unsubscribe(subscriptions);
        if (!unsubscribed) {
            return false;
        }
        subscriptions.clear();
        return true;
    }

    bool EventManager::trigger_event(EventType event_type, const Event& event, bool silent) {
        if (auto it = subscriptions.find(event_type); it != subscriptions.end()) {
            for (auto& [subscription_id, subscription] : it->second) {
                subscription(event);
            }
            if (!silent) {
                ST_LOG_TRACE("Triggered event {}", event.to_string());
            }
            return true;
        }
        return false;
    }

    bool EventManager::trigger_event_silent(EventType event_type, const Event& event) {
        return trigger_event(event_type, event, true);
    }

    void EventManager::queue_event(EventType event_type, const Shared<Event>& event) {
        ST_ASSERT(event != nullptr, "Event to be queued cannot be null");
        ST_ASSERT(queue_index < queues.size(), "Queue index [" << queue_index << "] must be less than queue count [" << queues.size() << "]");
        EventQueue& queue = queues[queue_index];
        queue[event_type].push_back(event);
        ST_LOG_TRACE("Queued event {}", event->to_string());
    }

    void EventManager::process_events() {
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
                ST_LOG_TRACE("Processed [{}] events of type [{}]", processed_event_count_for_type, event_type);
                if (processed_event_count_for_type < events.size()) {
                    ST_LOG_WARNING("Processed only [{}] of [{}] events of type [{}]", processed_event_count_for_type, events.size(), event_type);
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
