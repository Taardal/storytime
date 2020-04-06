#include "system/Log.h"
#include "system/Core.h"
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

namespace storytime {

    Shader::Shader(const char* vertexSource, const char* fragmentSource) {
        ST_TRACE(ST_TAG, "Creating");
        uint32_t vertexShaderId = createShader(vertexSource, GL_VERTEX_SHADER);
        uint32_t fragmentShaderId = createShader(fragmentSource, GL_FRAGMENT_SHADER);
        id = createProgram(vertexShaderId, fragmentShaderId);
        ST_TRACE(ST_TAG, "Created");
    }

    Shader::~Shader() {
        ST_TRACE(ST_TAG, "Destroying");
        glDeleteProgram(id);
        ST_TRACE(ST_TAG, "Destroyed");
    }

    void Shader::bind() const {
        glUseProgram(id);
    }

    void Shader::unbind() const {
        glUseProgram(0);
    }

    void Shader::setMat4(const char* key, glm::mat4 value) const {
        int32_t location = glGetUniformLocation(id, key);
        int32_t count = 1;
        bool transpose = GL_FALSE;
        glUniformMatrix4fv(location, count, transpose, glm::value_ptr(value));
    }

    void Shader::setFloat4(const char* key, glm::vec4 value) const {
        int32_t location = glGetUniformLocation(id, key);
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    uint32_t Shader::createShader(const char* source, uint32_t type) const {
        const char* typeString = type == GL_VERTEX_SHADER ? ST_TO_STRING(GL_VERTEX_SHADER) : ST_TO_STRING(GL_FRAGMENT_SHADER);
        ST_DEBUG(ST_TAG, "Creating [{0}] shader", typeString);
        uint32_t shaderId = glCreateShader(type);
        ST_TRACE(ST_TAG, "Created shader with id [{0}]", shaderId);
        setShaderSource(source, shaderId);
        bool compiled = compileShader(shaderId);
        if (compiled) {
            ST_DEBUG(ST_TAG, "Shader created successfully");
            return shaderId;
        } else {
            ST_TRACE(ST_TAG, "Deleting shader with id [{0}]", shaderId);
            glDeleteShader(shaderId);
            return 0;
        }
    }

    void Shader::setShaderSource(const char* source, uint32_t shaderId) const {
        ST_TRACE(ST_TAG, "Setting source on shader [{0}] \n{1}", shaderId, source);
        int32_t count = 1;
        int32_t* length = nullptr;
        glShaderSource(shaderId, count, &source, length);
        ST_TRACE(ST_TAG, "Set source on shader [{0}]", shaderId);
    }

    bool Shader::compileShader(uint32_t shaderId) const {
        ST_TRACE(ST_TAG, "Compiling shader with id [{0}]", shaderId);
        glCompileShader(shaderId);
        int32_t status = 0;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
        bool compiled = status != 0;
        if (!compiled) {
            ST_ERROR(ST_TAG, "Could not compile shader with id [{0}]: [{1}]", shaderId, getShaderLog(shaderId));
        }
        return compiled;
    }

    std::string Shader::getShaderLog(uint32_t shaderId) const {
        int32_t length = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetShaderInfoLog(shaderId, length, &length, &log[0]);
        return log.data();
    }

    uint32_t Shader::createProgram(uint32_t vertexShaderId, uint32_t fragmentShaderId) const {
        ST_TRACE(ST_TAG, "Creating program using vertex shader with id [{0}] and fragment shader with id [{1}]", vertexShaderId, fragmentShaderId);
        uint32_t programId = glCreateProgram();
        ST_TRACE(ST_TAG, "Attaching vertex shader with id [{0}]", vertexShaderId);
        glAttachShader(programId, vertexShaderId);
        ST_TRACE(ST_TAG, "Attaching fragment shader with id [{0}]", fragmentShaderId);
        glAttachShader(programId, fragmentShaderId);
        bool linked = linkProgram(programId);
        if (linked) {
            ST_TRACE(ST_TAG, "Detaching vertex shader with id [{0}]", vertexShaderId);
            glDetachShader(programId, vertexShaderId);
            ST_TRACE(ST_TAG, "Detaching fragment shader with id [{0}]", fragmentShaderId);
            glDetachShader(programId, fragmentShaderId);
            ST_DEBUG(ST_TAG, "Shader program created successfully", programId);
            return programId;
        } else {
            ST_TRACE(ST_TAG, "Deleting shader program with id [{0}]", programId);
            glDeleteProgram(programId);
            ST_TRACE(ST_TAG, "Deleting vertex shader with id [{0}]", vertexShaderId);
            glDeleteShader(vertexShaderId);
            ST_TRACE(ST_TAG, "Deleting fragment shader with id [{0}]", fragmentShaderId);
            glDeleteShader(fragmentShaderId);
            return 0;
        }
    }

    bool Shader::linkProgram(uint32_t programId) const {
        ST_TRACE(ST_TAG, "Linking shader program with id [{0}]", programId);
        glLinkProgram(programId);
        int32_t status = 0;
        glGetProgramiv(programId, GL_LINK_STATUS, &status);
        bool linked = status != 0;
        if (!linked) {
            ST_ERROR(ST_TAG, "Could not link shader program with id [{0}]: [{1}]", programId, getProgramLog(programId));
        }
        return linked;
    }

    std::string Shader::getProgramLog(uint32_t programId) const {
        int32_t logLength = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> log(logLength);
        glGetProgramInfoLog(programId, logLength, &logLength, &log[0]);
        return log.data();
    }

}