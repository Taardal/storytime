#include "Shader.h"
#include "system/Log.h"
#include <glad/glad.h>

namespace Darkle {

    Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource) {
        LOG_TRACE(TAG, "Creating");
        unsigned int vertexShaderId = createShader(GL_VERTEX_SHADER, vertexSource);
        unsigned int fragmentShaderId = createShader(GL_FRAGMENT_SHADER, fragmentSource);
        if (vertexShaderId != 0 && fragmentShaderId != 0) {
            id = createProgram(vertexShaderId, fragmentShaderId);
        }
        LOG_TRACE(TAG, "Created");
    }

    Shader::~Shader() {
        LOG_TRACE(TAG, "Destroying");
        glDeleteProgram(id);
        LOG_TRACE(TAG, "Destroyed");
    }

    void Shader::bind() const {
        glUseProgram(id);
        LOG_DEBUG(TAG, "Bound shader [{0}]", id);
    }

    void Shader::unbind() const {
        glUseProgram(0);
        LOG_DEBUG(TAG, "Unbound shader [{0}]", id);
    }

    void Shader::setUniform1i(const std::string& key, int value) {
        glUniform1i(getUniformLocation(key), value);
    }

    unsigned int Shader::createShader(unsigned int shaderType, const std::string& shaderSource) {
        unsigned int shaderId = glCreateShader(shaderType);
        setShaderSource(shaderId, shaderSource);
        bool compiled = compileShader(shaderId);
        if (compiled) {
            return shaderId;
        } else {
            glDeleteShader(shaderId);
            return 0;
        }
    }

    void Shader::setShaderSource(unsigned int shaderId, const std::string& shaderSource) {
        char* source = (char*) shaderSource.c_str();
        unsigned int count = 1;
        int* length = nullptr;
        glShaderSource(shaderId, count, &source, length);
    }

    bool Shader::compileShader(unsigned int shaderId) {
        glCompileShader(shaderId);
        int status = 0;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
        bool compiled = status != 0;
        if (!compiled) {
            LOG_ERROR(TAG, "Could not compile shader [{0}]", getShaderLog(shaderId));
        }
        return compiled;
    }

    std::string Shader::getShaderLog(unsigned int shaderId) {
        int logLength = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> log(logLength);
        glGetShaderInfoLog(shaderId, logLength, &logLength, &log[0]);
        return log.data();
    }

    unsigned int Shader::createProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId) {
        unsigned int programId = glCreateProgram();
        glAttachShader(programId, vertexShaderId);
        glAttachShader(programId, fragmentShaderId);
        bool linked = linkProgram(programId);
        if (linked) {
            glDetachShader(programId, vertexShaderId);
            glDetachShader(programId, fragmentShaderId);
            return programId;
        } else {
            glDeleteProgram(programId);
            glDeleteShader(vertexShaderId);
            glDeleteShader(fragmentShaderId);
            return 0;
        }
    }

    bool Shader::linkProgram(unsigned int programId) {
        glLinkProgram(programId);
        int status = 0;
        glGetProgramiv(programId, GL_LINK_STATUS, (int*) &status);
        bool linked = status != 0;
        if (!linked) {
            LOG_ERROR(TAG, "Could not link shader program [{0}]", getProgramLog(programId));
        }
        return linked;
    }

    std::string Shader::getProgramLog(unsigned int programId) {
        int logLength = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> log(logLength);
        glGetProgramInfoLog(programId, logLength, &logLength, &log[0]);
        return log.data();
    }

    int Shader::getUniformLocation(const std::string& key) {
        if (uniformLocations.find(key) != uniformLocations.end()) {
            return uniformLocations[key];
        } else {
            int location = glGetUniformLocation(id, key.c_str());
            if (location == -1) {
                LOG_WARN(TAG, "Uniform [{0}] does not exist", key);
            }
            uniformLocations[key] = location;
            return location;
        }
    }
}