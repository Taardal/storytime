#pragma once

#include "system/st_dispatcher.h"

/// This is a utility class for subscribing to events and unsubscribing from them
/// automatically when it is destroyed.
///
/// @Dispatcher currently returns a @SubscriptionID when you subscribe to a type,
/// and it's up to the subscriber to decide how, when and where to unsubscribe from it.
///
/// The problem is that many classes that subscribes doesn't need any special subscription handling,
/// and just wants to unsubscribe from everything in the destructor.
///
/// This class can be instantiated and used to subscribe what is needed, and then unsubscribe from
/// everything in its destructor, thus "automating" the unsubscription process.

namespace Storytime {
    class Subscriber {
    private:
        Dispatcher& dispatcher;
        std::vector<SubscriptionID> subscription_ids;

    public:
        Subscriber(Dispatcher& dispatcher);

        ~Subscriber();

        template<typename T>
        void subscribe(const SubscriptionFn<T>& subscription) {
            subscription_ids.push_back(
                dispatcher.subscribe<T>(subscription)
            );
        }
    };
}
