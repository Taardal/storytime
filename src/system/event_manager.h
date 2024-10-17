#pragma once

#include "system/event.h"

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
        static u32 subscription_counter;

    private:
        EventManagerConfig config;
        SubscriptionMap subscriptions;
        EventQueueList queues;
        u8 queue_index = 0;

    public:
        explicit EventManager(EventManagerConfig config = {});

        SubscriptionID subscribe(EventType event_type, const Subscription& subscription);

        bool remove_subscription(EventType event_type, SubscriptionID subscription_id);

        bool trigger_event(EventType event_type, const Event& event);

        void queue_event(EventType event_type, const Shared<Event>& event);

        void queue_event(EventType event_type, const Event& event);

        void process_event_queue();
    };
}
