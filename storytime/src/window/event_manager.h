#pragma once

#include "window/events/Event.h"

namespace Storytime {
    //typedef uint64_t EventType;
    typedef u32 SubscriptionID;
    typedef std::function<void(const Event&)> Subscription;

    class EventManager {
    private:
        static u32 subscription_counter;
        std::map<EventType, std::vector<std::pair<SubscriptionID, Subscription>>> subscriptions;
    public:
        SubscriptionID subscribe(EventType event_type, const Subscription& subscription);

        bool trigger_event(EventType event_type, const Event& event);
    };
}