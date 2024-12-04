#pragma once

#include "texture.h"

namespace Storytime {
    struct FramebufferConfig {
        u32 width = 0;
        u32 height = 0;
        u32 samples = 1;
        bool swap_chain_target = false;
    };

    class Framebuffer {
    private:
        FramebufferConfig config;
        u32 id = 0;
        Texture* color_attachment_texture = nullptr;
        Texture* depth_attachment_texture = nullptr;

    public:
        explicit Framebuffer(const FramebufferConfig& config);

        virtual ~Framebuffer();

        const FramebufferConfig& get_config() const;

        Texture* get_color_attachment_texture() const;

        void bind() const;

        void unbind() const;

        void resize(u32 width, u32 height);

    private:
        void initialize();

        void terminate() const;

        void attach_texture(u32 attachment, const Texture* texture) const;
    };
}
