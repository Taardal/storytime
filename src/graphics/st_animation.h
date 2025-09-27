#pragma once

#include "st_sprite.h"

namespace Storytime {
    typedef SpriteRenderConfig AnimationRenderConfig;

    struct AnimationFrame {
        Sprite sprite;
        u32 duration_ms = 0;
    };

    struct AnimationConfig {
        std::vector<AnimationFrame> frames;
        bool looping = false;
        bool paused = false;
    };

    class Animation {
    private:
        AnimationConfig config;
        u32 frame_index = 0;
        f64 frame_elapsed_ms = 0.0;
        i32 start_frame_index = 0;
        i32 end_frame_index = 0;

    public:
        Animation() = default;

        Animation(const AnimationConfig& config);

        Animation(AnimationConfig&& config);

        AnimationFrame& get_current_frame();

        std::vector<AnimationFrame>& get_frames();

        u32 get_frame_count() const;

        void set_start_frame(i32 frame_index);

        void set_end_frame(i32 frame_index);

        void set_frame_duration(u32 frame_index, f32 duration_ms);

        void set_looping(bool looping);

        void set_paused(bool paused);

        bool has_ended() const;

        void reset();

        void update(f64 timestep);

        void render(Renderer* renderer, const AnimationRenderConfig& render_config);
    };
}
