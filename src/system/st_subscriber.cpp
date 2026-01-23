#include "st_subscriber.h"

namespace Storytime {
    Subscriber::Subscriber(Dispatcher& dispatcher) : dispatcher(dispatcher) {}

    Subscriber::~Subscriber() {
        dispatcher.unsubscribe_all_and_clear(subscription_ids);
    }
}
