#pragma once

namespace Storytime {
    struct Metrics {
        f64 cycle_duration_ms = 0.0;
        f64 frames_per_second = 0.0;
        f64 imgui_render_duration_ms = 0.0;
        f64 render_duration_ms = 0.0;
        f64 swap_buffers_duration_ms = 0.0;
        f64 update_duration_ms = 0.0;
        f64 update_timestep_ms = 0.0;
        f64 updates_per_second = 0.0;
        f64 window_events_duration_ms = 0.0;

        f64 fps = 0.0;
        f64 fpss = 0.0;
        u64 quads = 0;
        f64 frame_duration_ms = 0.0;

        u32 draw_calls = 0;
        u32 quad_count = 0;
        u32 vertex_count = 0;
        u32 index_count = 0;
    };
}
