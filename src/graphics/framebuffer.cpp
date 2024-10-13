#include "framebuffer.h"
#include "graphics/open_gl.h"

namespace Storytime {
    Framebuffer::Framebuffer(FramebufferConfig config) : config(std::move(config)) {
        initialize();
    }

    Framebuffer::~Framebuffer() {
        terminate();
    }

    const FramebufferConfig& Framebuffer::get_config() const {
        return config;
    }

    Texture* Framebuffer::get_color_attachment_texture() const {
        return color_attachment_texture;
    }

    void Framebuffer::bind() const {
        ST_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, id));
        ST_GL_CALL(glViewport(0, 0, config.width, config.height));
    }

    void Framebuffer::unbind() const {
        ST_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void Framebuffer::resize(u32 width, u32 height) {
        if (width > 0 && height > 0) {
            config.width = width;
            config.height = height;
            if (id) {
                terminate();
            }
            initialize();
        }
    }

    void Framebuffer::initialize() {
        ST_GL_CALL(glGenFramebuffers(1, &id));
        ST_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, id));
        {
            TextureConfig textureConfig;
            textureConfig.width = config.width;
            textureConfig.height = config.height;
            textureConfig.format = GL_RGBA;
            textureConfig.internal_format = GL_RGBA8;
            color_attachment_texture = new Texture(textureConfig);
            attach_texture(GL_COLOR_ATTACHMENT0, color_attachment_texture);
        }
        {
            TextureConfig textureConfig;
            textureConfig.width = config.width;
            textureConfig.height = config.height;
            textureConfig.format = GL_DEPTH_COMPONENT;
            textureConfig.internal_format = GL_DEPTH_COMPONENT;
            depth_attachment_texture = new Texture(textureConfig);
            attach_texture(GL_DEPTH_ATTACHMENT, depth_attachment_texture);
        }
        ST_GL_CALL(GLenum framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
        if (framebuffer_status != GL_FRAMEBUFFER_COMPLETE) {
            ST_THROW("Could not initialize framebuffer: initialization was incomplete");
        }
        ST_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void Framebuffer::attach_texture(u32 attachmentType, const Texture* texture) const {
        ST_GL_CALL(glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            attachmentType,
            GL_TEXTURE_2D,
            texture->get_id(),
            texture->get_level_of_detail()
        ));
    }

    void Framebuffer::terminate() const {
        ST_GL_CALL(glDeleteFramebuffers(1, &id));
        delete color_attachment_texture;
        delete depth_attachment_texture;
    }
}
