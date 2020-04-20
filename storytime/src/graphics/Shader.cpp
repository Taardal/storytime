#include "system/Log.h"
#include "system/Core.h"
#include "Shader.h"
#include "GraphicsLog.h"
#include <glm/gtc/type_ptr.hpp>

namespace storytime {

    Shader::Shader(const char* vertexSource, const char* fragmentSource) {
        uint32_t vertexShaderId = CreateShader(vertexSource, GL_VERTEX_SHADER);
        uint32_t fragmentShaderId = CreateShader(fragmentSource, GL_FRAGMENT_SHADER);
        id = CreateProgram(vertexShaderId, fragmentShaderId);
    }

    Shader::~Shader() {
        ST_GL_CALL(ST_TAG, glDeleteProgram(id));
    }

    void Shader::Bind() const {
        ST_GL_CALL(ST_TAG, glUseProgram(id));
    }

    void Shader::Unbind() const {
        ST_GL_CALL(ST_TAG, glUseProgram(0));
    }

    void Shader::SetInt1(const char* key, uint32_t value) const {
        ST_GL_CALL(ST_TAG, int32_t location = glGetUniformLocation(id, key));
        ST_GL_CALL(ST_TAG, glUniform1i(location, value));
    }

    void Shader::SetMat4(const char* key, glm::mat4 value) const {
        ST_GL_CALL(ST_TAG, int32_t location = glGetUniformLocation(id, key));
        int32_t count = 1;
        bool transpose = GL_FALSE;
        ST_GL_CALL(ST_TAG, glUniformMatrix4fv(location, count, transpose, glm::value_ptr(value)));
    }

    void Shader::SetFloat4(const char* key, glm::vec4 value) const {
        ST_GL_CALL(ST_TAG, int32_t location = glGetUniformLocation(id, key));
        ST_GL_CALL(ST_TAG, glUniform4f(location, value.x, value.y, value.z, value.w));
    }

    void Shader::SetIntArray(const char* key, const int32_t* values, uint32_t count) const
    {
        ST_GL_CALL(ST_TAG, int32_t location = glGetUniformLocation(id, key));
        ST_GL_CALL(ST_TAG, glUniform1iv(location, count, values));
    }

    uint32_t Shader::CreateShader(const char* source, uint32_t type) const {
        const char* typeString = type == GL_VERTEX_SHADER ? ST_TO_STRING(GL_VERTEX_SHADER) : ST_TO_STRING(GL_FRAGMENT_SHADER);
        ST_LOG_DEBUG(ST_TAG, "Creating [{0}] shader", typeString);
        ST_GL_CALL(ST_TAG, uint32_t shaderId = glCreateShader(type));
        ST_LOG_TRACE(ST_TAG, "Created shader with id [{0}]", shaderId);
        SetShaderSource(source, shaderId);
        bool compiled = CompileShader(shaderId);
        if (compiled) {
            ST_LOG_DEBUG(ST_TAG, "Shader created successfully");
            return shaderId;
        } else {
            ST_LOG_TRACE(ST_TAG, "Deleting shader with id [{0}]", shaderId);
            ST_GL_CALL(ST_TAG, glDeleteShader(shaderId));
            return 0;
        }
    }

    void Shader::SetShaderSource(const char* source, uint32_t shaderId) const {
        ST_LOG_TRACE(ST_TAG, "Setting source on shader [{0}] \n{1}", shaderId, source);
        int32_t count = 1;
        int32_t* length = nullptr;
        ST_GL_CALL(ST_TAG, glShaderSource(shaderId, count, &source, length));
        ST_LOG_TRACE(ST_TAG, "Set source on shader [{0}]", shaderId);
    }

    bool Shader::CompileShader(uint32_t shaderId) const {
        ST_LOG_TRACE(ST_TAG, "Compiling shader with id [{0}]", shaderId);
        ST_GL_CALL(ST_TAG, glCompileShader(shaderId));
        int32_t status = 0;
        ST_GL_CALL(ST_TAG, glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status));
        bool compiled = status != 0;
        if (!compiled) {
            ST_LOG_ERROR(ST_TAG, "Could not compile shader with id [{0}]: [{1}]", shaderId, GetShaderLog(shaderId));
        }
        return compiled;
    }

    std::string Shader::GetShaderLog(uint32_t shaderId) const {
        int32_t length = 0;
        ST_GL_CALL(ST_TAG, glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length));
        std::vector<char> log(length);
        ST_GL_CALL(ST_TAG, glGetShaderInfoLog(shaderId, length, &length, &log[0]));
        return log.data();
    }

    uint32_t Shader::CreateProgram(uint32_t vertexShaderId, uint32_t fragmentShaderId) const {
        ST_LOG_TRACE(ST_TAG, "Creating program using vertex shader with id [{0}] and fragment shader with id [{1}]", vertexShaderId, fragmentShaderId);
        uint32_t programId = glCreateProgram();
        ST_LOG_TRACE(ST_TAG, "Attaching vertex shader with id [{0}]", vertexShaderId);
        ST_GL_CALL(ST_TAG, glAttachShader(programId, vertexShaderId));
        ST_LOG_TRACE(ST_TAG, "Attaching fragment shader with id [{0}]", fragmentShaderId);
        ST_GL_CALL(ST_TAG, glAttachShader(programId, fragmentShaderId));
        bool linked = LinkProgram(programId);
        if (linked) {
            ST_LOG_TRACE(ST_TAG, "Detaching vertex shader with id [{0}]", vertexShaderId);
            ST_GL_CALL(ST_TAG, glDetachShader(programId, vertexShaderId));
            ST_LOG_TRACE(ST_TAG, "Detaching fragment shader with id [{0}]", fragmentShaderId);
            ST_GL_CALL(ST_TAG, glDetachShader(programId, fragmentShaderId));
            ST_LOG_DEBUG(ST_TAG, "Shader program created successfully", programId);
            return programId;
        } else {
            ST_LOG_TRACE(ST_TAG, "Deleting shader program with id [{0}]", programId);
            ST_GL_CALL(ST_TAG, glDeleteProgram(programId));
            ST_LOG_TRACE(ST_TAG, "Deleting vertex shader with id [{0}]", vertexShaderId);
            ST_GL_CALL(ST_TAG, glDeleteShader(vertexShaderId));
            ST_LOG_TRACE(ST_TAG, "Deleting fragment shader with id [{0}]", fragmentShaderId);
            ST_GL_CALL(ST_TAG, glDeleteShader(fragmentShaderId));
            return 0;
        }
    }

    bool Shader::LinkProgram(uint32_t id) const {
        ST_LOG_TRACE(ST_TAG, "Linking shader program with id [{0}]", id);
        ST_GL_CALL(ST_TAG, glLinkProgram(id));
        int32_t status = 0;
        ST_GL_CALL(ST_TAG, glGetProgramiv(id, GL_LINK_STATUS, &status));
        bool linked = status != 0;
        if (!linked) {
            ST_LOG_ERROR(ST_TAG, "Could not link shader program with id [{0}]: [{1}]", id, GetProgramLog(id));
        }
        return linked;
    }

    std::string Shader::GetProgramLog(uint32_t programId) const {
        int32_t logLength = 0;
        ST_GL_CALL(ST_TAG, glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength));
        std::vector<char> log(logLength);
        ST_GL_CALL(ST_TAG, glGetProgramInfoLog(programId, logLength, &logLength, &log[0]));
        return log.data();
    }

}