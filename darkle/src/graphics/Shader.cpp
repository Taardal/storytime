#include "Shader.h"
#include "Log.h"
#include <glad/glad.h>

namespace Darkle {

    Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource) {
        unsigned int vertexShaderId = CreateShader(GL_VERTEX_SHADER, vertexSource);
        unsigned int fragmentShaderId = CreateShader(GL_FRAGMENT_SHADER, fragmentSource);
        if (vertexShaderId != 0 && fragmentShaderId != 0) {
            id = CreateProgram(vertexShaderId, fragmentShaderId);
        }
    }

    Shader::~Shader() {
        glDeleteProgram(id);
    }

    void Shader::Bind() const {
        glUseProgram(id);
    }

    void Shader::Unbind() const {
        glUseProgram(0);
    }

    void Shader::SetUniform1i(const std::string& key, int value) {
        glUniform1i(GetUniformLocation(key), value);
    }

    unsigned int Shader::CreateShader(unsigned int shaderType, const std::string& shaderSource) {
        unsigned int shaderId = glCreateShader(shaderType);
        SetShaderSource(shaderId, shaderSource);
        bool compiled = CompileShader(shaderId);
        if (compiled) {
            return shaderId;
        } else {
            glDeleteShader(shaderId);
            return 0;
        }
    }

    void Shader::SetShaderSource(unsigned int shaderId, const std::string& shaderSource) {
        char* source = (char*) shaderSource.c_str();
        unsigned int count = 1;
        int* length = nullptr;
        glShaderSource(shaderId, count, &source, length);
    }

    bool Shader::CompileShader(unsigned int shaderId) {
        glCompileShader(shaderId);
        int status = 0;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
        bool compiled = status != 0;
        if (!compiled) {
            LOG_ERROR(TAG, "Could not compile shader [{0}]", GetShaderLog(shaderId));
        }
        return compiled;
    }

    std::string Shader::GetShaderLog(unsigned int shaderId) {
        int logLength = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> log(logLength);
        glGetShaderInfoLog(shaderId, logLength, &logLength, &log[0]);
        return log.data();
    }

    unsigned int Shader::CreateProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId) {
        unsigned int programId = glCreateProgram();
        glAttachShader(programId, vertexShaderId);
        glAttachShader(programId, fragmentShaderId);
        bool linked = LinkProgram(programId);
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

    bool Shader::LinkProgram(unsigned int programId) {
        glLinkProgram(programId);
        int status = 0;
        glGetProgramiv(programId, GL_LINK_STATUS, (int*) &status);
        bool linked = status != 0;
        if (!linked) {
            LOG_ERROR(TAG, "Could not link shader program [{0}]", GetProgramLog(programId));
        }
        return linked;
    }

    std::string Shader::GetProgramLog(unsigned int programId) {
        int logLength = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> log(logLength);
        glGetProgramInfoLog(programId, logLength, &logLength, &log[0]);
        return log.data();
    }

    int Shader::GetUniformLocation(const std::string& name) {
        if (uniformLocations.find(name) != uniformLocations.end()) {
            return uniformLocations[name];
        } else {
            int location = glGetUniformLocation(id, name.c_str());
            if (location == -1) {
                LOG_WARN(TAG, "Uniform [{0}] does not exist", name);
            }
            uniformLocations[name] = location;
            return location;
        }
    }
}