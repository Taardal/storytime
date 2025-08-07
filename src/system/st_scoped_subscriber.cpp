#include "st_scoped_subscriber.h"

namespace Storytime {
    ScopedSubscriber::ScopedSubscriber(const ScopedSubscriberConfig& config)
        : config(config) {}

    ScopedSubscriber::~ScopedSubscriber() {
        config.dispatcher->unsubscribe_and_clear(subscription_ids);
    }
}
