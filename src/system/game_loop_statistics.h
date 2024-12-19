#pragma once

#include "event.h"

namespace Storytime {
    struct GameLoopStatistics {
        f64 cycle_duration_ms = 0.0;
        f64 frames_per_second = 0.0;
        f64 game_events_duration_ms = 0.0;
        f64 imgui_render_duration_ms = 0.0;
        f64 render_duration_ms = 0.0;
        f64 swap_buffers_duration_ms = 0.0;
        f64 update_duration_ms = 0.0;
        f64 update_timestep_ms = 0.0;
        f64 updates_per_second = 0.0;
        f64 window_events_duration_ms = 0.0;
    };

    struct GameLoopStatisticsEvent : Event {
        static const EventType type;
        static const std::string name;

        GameLoopStatistics game_loop_statistics;

        GameLoopStatisticsEvent() : Event(type, name) {
        }

        GameLoopStatisticsEvent(const GameLoopStatistics& other) : Event(type, name), game_loop_statistics(other) {
        }
    };
}