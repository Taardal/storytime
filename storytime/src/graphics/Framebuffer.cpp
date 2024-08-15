#include "Framebuffer.h"
#include "system/environment.h"
#include "graphics/GraphicsLog.h"

namespace Storytime
{
    Framebuffer::Config::Config()
            : Width(0), Height(0), Samples(1), SwapChainTarget(false)
    {}
}

namespace Storytime
{
    Framebuffer::Framebuffer(const Framebuffer::Config& config)
            : config(config), id(0), colorAttachmentTexture(nullptr), depthAttachmentTexture(nullptr)
    {
        Init();
    }

    Framebuffer::~Framebuffer()
    {
        Delete();
    }

    const Framebuffer::Config& Framebuffer::GetConfig() const
    {
        return config;
    }

    Texture* Framebuffer::GetColorAttachmentTexture() const
    {
        return colorAttachmentTexture;
    }

    void Framebuffer::Bind()
    {
        ST_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, id));
        ST_GL_CALL(glViewport(0, 0, config.Width, config.Height));
    }

    void Framebuffer::Unbind()
    {
        ST_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void Framebuffer::Resize(uint32_t width, uint32_t height)
    {
        if (width > 0 && height > 0)
        {
            config.Width = width;
            config.Height = height;
            if (id)
            {
                Delete();
            }
            Init();
        }
    }

    void Framebuffer::Init()
    {
        ST_GL_CALL(glGenFramebuffers(1, &id));
        ST_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, id));
        {
            Texture::Config textureConfig;
            textureConfig.Width = config.Width;
            textureConfig.Height = config.Height;
            textureConfig.Format = GL_RGBA;
            textureConfig.InternalFormat = GL_RGBA8;
            colorAttachmentTexture = new Texture(textureConfig);
            AttachTexture(GL_COLOR_ATTACHMENT0, colorAttachmentTexture);
        }
        {
            Texture::Config textureConfig;
            textureConfig.Width = config.Width;
            textureConfig.Height = config.Height;
            textureConfig.Format = GL_DEPTH_COMPONENT;
            textureConfig.InternalFormat = GL_DEPTH_COMPONENT;
            depthAttachmentTexture = new Texture(textureConfig);
            AttachTexture(GL_DEPTH_ATTACHMENT, depthAttachmentTexture);
        }
        ST_GL_CALL(ST_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE));
        ST_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void Framebuffer::AttachTexture(uint32_t attachmentType, Texture* texture) const
    {
        ST_GL_CALL(glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                attachmentType,
                GL_TEXTURE_2D,
                texture->GetId(),
                texture->GetLevelOfDetail()
        ));
    }

    void Framebuffer::Delete() const
    {
        ST_GL_CALL(glDeleteFramebuffers(1, &id));
        delete colorAttachmentTexture;
        delete depthAttachmentTexture;
    }

}

