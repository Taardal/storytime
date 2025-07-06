#include "st_event_handler.h"

namespace Storytime {
    EventHandler::EventHandler(const EventHandlerConfig& config) : config(config) {}

    EventHandler::~EventHandler() {
        config.event_manager->unsubscribe_and_clear(event_subscriptions);
    }

    void EventHandler::subscribe(EventType event_type, const Subscription& subscription) {
        ST_ASSERT(config.event_manager != nullptr, "Event manager cannot be null");
        event_subscriptions.push_back(config.event_manager->subscribe(event_type, subscription));
    }
}
