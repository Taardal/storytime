#include "Framebuffer.h"

#include "system/Log.h"

namespace storytime
{
    Framebuffer::Config::Config()
            : Width(0), Height(0), Samples(1), SwapChainTarget(false)
    {}
}

namespace storytime
{
    Framebuffer::Framebuffer(const Framebuffer::Config& config)
            : config(config), id(0), colorAttachmentId(0), depthAttachmentId(0)
    {
        Invalidate();
    }

    Framebuffer::~Framebuffer()
    {
        ST_GL_CALL(ST_TAG, glDeleteFramebuffers(1, &id));
        ST_GL_CALL(ST_TAG, glDeleteTextures(1, &colorAttachmentId));
        ST_GL_CALL(ST_TAG, glDeleteTextures(1, &depthAttachmentId));
    }

    const Framebuffer::Config& Framebuffer::GetConfig() const
    {
        return config;
    }

    uint32_t Framebuffer::GetColorAttachmentId() const
    {
        return colorAttachmentId;
    }

    void Framebuffer::Bind()
    {
        ST_GL_CALL(ST_TAG, glBindFramebuffer(GL_FRAMEBUFFER, id));
        ST_GL_CALL(ST_TAG, glViewport(0, 0, config.Width, config.Height));
    }

    void Framebuffer::Unbind()
    {
        ST_GL_CALL(ST_TAG, glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void Framebuffer::Invalidate()
    {
        if (id)
        {
            ST_GL_CALL(ST_TAG, glDeleteFramebuffers(1, &id));
            ST_GL_CALL(ST_TAG, glDeleteTextures(1, &colorAttachmentId));
            ST_GL_CALL(ST_TAG, glDeleteTextures(1, &depthAttachmentId));
        }

        ST_GL_CALL(ST_TAG, glGenFramebuffers(1, &id));
        ST_GL_CALL(ST_TAG, glBindFramebuffer(GL_FRAMEBUFFER, id));

        int levelOfDetail = 0;
        int border = 0;

        ST_GL_CALL(ST_TAG, glGenTextures(1, &colorAttachmentId));
        ST_GL_CALL(ST_TAG, glBindTexture(GL_TEXTURE_2D, colorAttachmentId));
        ST_GL_CALL(ST_TAG, glTexImage2D(
                GL_TEXTURE_2D,
                levelOfDetail,
                GL_RGBA8,
                config.Width,
                config.Height,
                border,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                nullptr
        ));
        ST_GL_CALL(ST_TAG, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        ST_GL_CALL(ST_TAG, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        ST_GL_CALL(ST_TAG, glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D,
                colorAttachmentId,
                levelOfDetail
        ));
        ST_GL_CALL(ST_TAG, glBindTexture(GL_TEXTURE_2D, 0));

        ST_GL_CALL(ST_TAG, glGenTextures(1, &depthAttachmentId));
        ST_GL_CALL(ST_TAG, glBindTexture(GL_TEXTURE_2D, depthAttachmentId));
        ST_GL_CALL(ST_TAG, glTexImage2D(
                GL_TEXTURE_2D,
                levelOfDetail,
                GL_DEPTH_COMPONENT,
                config.Width,
                config.Height,
                border,
                GL_DEPTH_COMPONENT,
                GL_UNSIGNED_BYTE,
                nullptr
        ));
        ST_GL_CALL(ST_TAG, glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_DEPTH_ATTACHMENT,
                GL_TEXTURE_2D,
                depthAttachmentId,
                levelOfDetail
        ));
        ST_GL_CALL(ST_TAG, glBindTexture(GL_TEXTURE_2D, 0));

        ST_GL_CALL(ST_TAG, uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
        ST_ASSERT(ST_TAG, status == GL_FRAMEBUFFER_COMPLETE);

        ST_GL_CALL(ST_TAG, glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void Framebuffer::Resize(uint32_t width, uint32_t height)
    {
        config.Width = width;
        config.Height = height;
        Invalidate();
    }

}

