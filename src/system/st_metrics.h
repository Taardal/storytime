#pragma once

namespace Storytime {
    struct Metrics {

        // Game loop
        f64 cycle_duration_ms = 0.0;
        f64 frames_per_second = 0.0;
        f64 imgui_render_duration_ms = 0.0;
        f64 scene_render_duration_ms = 0.0;
        f64 render_duration_ms = 0.0;
        f64 update_duration_ms = 0.0;
        f64 update_timestep_ms = 0.0;
        f64 updates_per_second = 0.0;
        f64 window_events_duration_ms = 0.0;

        // Rendering
        u64 draw_calls = 0;
        u64 quad_count = 0;
        u64 vertex_count = 0;
        u64 index_count = 0;
        u64 texture_count = 0;
    };
}
