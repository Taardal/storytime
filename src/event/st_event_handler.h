/// This is a utility class for subscribing to events and unsubscribing from them
/// automatically when it is destroyed.
///
/// @EventManager currently returns @SubscriptionID when you subscribe to an event,
/// and it's up to the client to decide how, when and where to unsubscribe from it.
///
/// The problem is that many subscribers just wants to unsubscribe when they are
/// destroyed, and that may create unnecessary boilerplate code for clients.
///
/// The idea is for clients to create an instance of this class (preferably on the stack),
/// and have it unsubscribe everything automatically upon destruction of the client.

#pragma once

#include "st_event_manager.h"

namespace Storytime {
    struct EventHandlerConfig {
        EventManager* event_manager = nullptr;
    };

    class EventHandler {
    private:
        EventHandlerConfig config;
        std::vector<SubscriptionID> event_subscriptions;

    public:
        EventHandler(const EventHandlerConfig& config);

        ~EventHandler();

        void subscribe(EventType event_type, const Subscription& subscription);
    };
}
