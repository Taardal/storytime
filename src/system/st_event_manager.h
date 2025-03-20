#pragma once

#include "system/st_event.h"

namespace Storytime {
    typedef u32 SubscriptionID;
    typedef std::function<void(const Event&)> Subscription;

    struct EventManagerConfig {
        u8 queue_count = 1;
    };

    class EventManager {
    private:
        typedef std::pair<SubscriptionID, Subscription> SubscriptionItem;
        typedef std::vector<SubscriptionItem> SubscriptionList;
        typedef std::map<EventType, SubscriptionList> SubscriptionMap;
        typedef std::vector<Shared<Event>> EventList;
        typedef std::map<EventType, EventList> EventQueue;
        typedef std::vector<EventQueue> EventQueueList;

    private:
        static u32 next_subscription_id;

    private:
        EventManagerConfig config;
        SubscriptionMap subscriptions;
        EventQueueList queues;
        u8 queue_index = 0;
        std::map<EventType, bool> muted_event_types;

    public:
        explicit EventManager(EventManagerConfig config = {});

        SubscriptionID subscribe(EventType event_type, const Subscription& subscription);

        bool unsubscribe(SubscriptionID subscription_id);

        bool unsubscribe(SubscriptionID subscription_id, EventType event_type);

        bool unsubscribe(const std::vector<SubscriptionID>& subscriptions);

        bool unsubscribe_and_clear(std::vector<SubscriptionID>& subscriptions);

        bool trigger_event(EventType event_type, const Event& event);

        void queue_event(EventType event_type, const Shared<Event>& event);

        void process_events();

        void mute(EventType event_type);

        void unmute(EventType event_type);

        bool is_muted(EventType event_type);
    };
}
