#include "st_keyboard.h"

#include "event/st_key_pressed_event.h"
#include "event/st_key_released_event.h"
#include "event/st_key_repeated_event.h"

namespace Storytime {
    Keyboard::Keyboard(const KeyboardConfig& config) : config(config) {
        initialize();
    }

    Keyboard::Keyboard(KeyboardConfig&& config) : config(std::move(config)) {
        initialize();
    }

    Keyboard::~Keyboard() {
        terminate();
    }

    bool Keyboard::is_enabled() const {
        return enabled;
    }

    void Keyboard::set_enabled(bool enabled) {
        this->enabled = enabled;
    }

    bool Keyboard::is_pressed(KeyCode key_code) const {
        if (!enabled) {
            return false;
        }
        ST_ASSERT_NOT_NULL(config.window);
        return glfwGetKey(*config.window, key_code) == GLFW_PRESS;
    }

    void Keyboard::on_pressed(const OnPressedSubscription& subscription) {
        on_pressed_subscriptions.emplace_back(subscription);
    }

    void Keyboard::on_released(const OnReleasedSubscription& subscription) {
        on_released_subscriptions.emplace_back(subscription);
    }

    void Keyboard::on_repeated(const OnRepeatedSubscription& subscription) {
        on_repeated_subscriptions.emplace_back(subscription);
    }

    void Keyboard::initialize() {
        event_subscriptions.push_back(
            config.event_manager->subscribe(KeyPressedEvent::type, [this](const Event& e) {
                if (!enabled) {
                    return;
                }
                auto& event = (KeyPressedEvent&) e;
                for (auto subscription : on_pressed_subscriptions) {
                    subscription(event);
                }
            })
        );
        event_subscriptions.push_back(
            config.event_manager->subscribe(KeyReleasedEvent::type, [this](const Event& e) {
                if (!enabled) {
                    return;
                }
                auto& event = (KeyReleasedEvent&) e;
                for (auto subscription : on_released_subscriptions) {
                    subscription(event);
                }
            })
        );
        event_subscriptions.push_back(
            config.event_manager->subscribe(KeyRepeatedEvent::type, [this](const Event& e) {
                if (!enabled) {
                    return;
                }
                auto& event = (KeyRepeatedEvent&) e;
                for (auto subscription : on_repeated_subscriptions) {
                    subscription(event);
                }
            })
        );
    }

    void Keyboard::terminate() {
        on_repeated_subscriptions.clear();
        on_released_subscriptions.clear();
        on_pressed_subscriptions.clear();
        config.event_manager->unsubscribe_and_clear(event_subscriptions);
    }
}
