#pragma once

#include <string>

namespace Storytime {
    typedef u32 EventType;

    struct Event {
        EventType event_type;
        std::string event_type_name;

        Event() = default;

        Event(EventType event_type, const std::string& name);

        virtual ~Event() = default;

        EventType get_type() const;

        const std::string& get_type_name() const;

        virtual std::string to_string() const;
    };
}
