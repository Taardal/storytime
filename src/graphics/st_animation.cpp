#include "st_animation.h"

namespace Storytime {
    Animation::Animation(const AnimationConfig& config) : config(config) {
        set_start_frame(0);
        set_end_frame(config.frames.size() - 1);
    }

    Animation::Animation(AnimationConfig&& config) : config(std::move(config)) {
        set_start_frame(0);
        set_end_frame(config.frames.size() - 1);
    }

    AnimationFrame& Animation::get_current_frame() {
        ST_ASSERT(
            frame_index < config.frames.size(),
            "Frame index [" << frame_index << "] must be lower than frame count [" << config.frames.size() << "]"
        );
        return config.frames.at(frame_index);
    }

    std::vector<AnimationFrame>& Animation::get_frames() {
        return config.frames;
    }

    u32 Animation::get_frame_count() const {
        return config.frames.size();
    }

    void Animation::set_start_frame(i32 frame_index) {
        if (frame_index == -1) {
            start_frame_index = 0;
            return;
        }
        ST_ASSERT(
            frame_index >= 0 && frame_index < config.frames.size(),
            "Frame index [" << frame_index << "] must be in range [0 - " << config.frames.size() - 1 << "]"
        );
        start_frame_index = frame_index;
    }

    void Animation::set_end_frame(i32 frame_index) {
        if (frame_index == -1) {
            end_frame_index = config.frames.size() - 1;
            return;
        }
        ST_ASSERT(
            frame_index >= 0 && frame_index < config.frames.size(),
            "Frame index [" << frame_index << "] must be in range [0 - " << config.frames.size() - 1 << "]"
        );
        end_frame_index = frame_index;
    }

    void Animation::set_frame_duration(u32 frame_index, f32 duration_ms) {
        ST_ASSERT(
            frame_index < config.frames.size(),
            "Frame index [" << frame_index << "] must be lower than frame count [" << config.frames.size() << "]"
        );
        AnimationFrame& frame = config.frames.at(frame_index);
        frame.duration_ms = duration_ms;
    }

    void Animation::set_looping(bool looping) {
        config.looping = looping;
    }

    void Animation::set_paused(bool paused) {
        config.paused = paused;
    }

    bool Animation::has_ended() const {
        if (frame_index < end_frame_index) {
            return false;
        }
        if (frame_index == end_frame_index) {
            const AnimationFrame& current_frame = config.frames.at(frame_index);
            if (frame_elapsed_ms < current_frame.duration_ms) {
                return false;
            }
        }
        return true;
    }

    void Animation::reset() {
        frame_index = 0;
        frame_elapsed_ms = 0.0;
    }

    void Animation::update(f64 timestep) {
        if (config.paused) {
            return;
        }
        u32 frame_count = config.frames.size();
        ST_ASSERT(
            frame_index < frame_count,
            "Frame index [" << frame_index << "] must be lower than frame count [" << config.frames.size() << "]"
        );
        AnimationFrame& current_frame = config.frames.at(frame_index);

        frame_elapsed_ms += timestep * 1000.0;
        if (frame_elapsed_ms < current_frame.duration_ms) {
            return;
        }

        u32 next_frame_index = frame_index + 1;
        if (next_frame_index > end_frame_index && !config.looping) {
            return;
        }
        frame_index = next_frame_index % (end_frame_index + 1);
        frame_elapsed_ms = 0.0;
    }

    void Animation::render(Renderer* renderer, const AnimationRenderConfig& render_config) {
        AnimationFrame& current_frame = get_current_frame();
        const Sprite& current_sprite = current_frame.sprite;
        current_sprite.render(renderer, render_config);
    }
}
