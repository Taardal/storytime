#pragma once

namespace Storytime {
    struct GameLoopStatistics {
        f64 frames_per_second = 0.0;
        f64 updates_per_second = 0.0;
        f64 cycle_duration_ms = 0.0;
        f64 update_duration_ms = 0.0;
        f64 update_timestep_ms = 0.0;
        f64 render_duration_ms = 0.0;
        f64 imgui_render_duration_ms = 0.0;
    };
}