#pragma once

namespace Storytime {
    typedef u32 EventType;

    struct Event {
        EventType event_type;
        std::string event_type_name;

        Event() = default;

        Event(EventType type, const std::string& name);

        virtual ~Event() = default;

        virtual std::string to_string() const;
    };
}
