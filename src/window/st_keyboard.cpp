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

    void Keyboard::on_pressed(const OnPressedFn& on_pressed) {
        on_pressed_fns.emplace_back(on_pressed);
    }

    void Keyboard::on_released(const OnReleasedFn& on_released) {
        on_released_fns.emplace_back(on_released);
    }

    void Keyboard::on_repeated(const OnRepeatedFn& on_repeated) {
        on_repeated_fns.emplace_back(on_repeated);
    }

    void Keyboard::initialize() {
        event_subscriptions.push_back(
            config.event_manager->subscribe(KeyPressedEvent::type, [this](const Event& e) {
                if (!enabled) {
                    return;
                }
                auto& event = (KeyPressedEvent&) e;
                for (auto on_pressed : on_pressed_fns) {
                    on_pressed(event);
                }
            })
        );
        event_subscriptions.push_back(
            config.event_manager->subscribe(KeyReleasedEvent::type, [this](const Event& e) {
                if (!enabled) {
                    return;
                }
                auto& event = (KeyReleasedEvent&) e;
                for (auto on_released : on_released_fns) {
                    on_released(event);
                }
            })
        );
        event_subscriptions.push_back(
            config.event_manager->subscribe(KeyRepeatedEvent::type, [this](const Event& e) {
                if (!enabled) {
                    return;
                }
                auto& event = (KeyRepeatedEvent&) e;
                for (auto on_repeated : on_repeated_fns) {
                    on_repeated(event);
                }
            })
        );
    }

    void Keyboard::terminate() {
        on_repeated_fns.clear();
        on_released_fns.clear();
        on_pressed_fns.clear();
        config.event_manager->unsubscribe_and_clear(event_subscriptions);
    }
}
