#include "open_gl.h"
#include "open_gl_log.h"

namespace Storytime {
    OpenGL::OpenGL(const OpenGLConfig& config) : config(config) {
        ST_LOG_TRACE("Initializing GLAD");
        bool glad_initialized = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) != 0;
        if (!glad_initialized) {
            ST_THROW("Could not initialize GLAD");
        }
        ST_LOG_DEBUG("Initialized GLAD");

        ST_LOG_TRACE("Initializing OpenGL logger");
        OpenGLLog::initialize({
            .log_level = config.log_level,
            .version_major = config.major_version,
            .version_minor = config.minor_version,
        });
        ST_LOG_DEBUG("Initialized OpenGL logger");

        ST_LOG_INFO("Using renderer [{}, {}]", glGetString(GL_RENDERER), glGetString(GL_VERSION));
    }
}
