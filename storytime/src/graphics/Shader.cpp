#include "system/Log.h"
#include "Shader.h"
#include "system/Core.h"
#include "system/FileReader.h"
#include <glm/gtc/type_ptr.hpp>

namespace storytime {

    constexpr const char* TYPE_TOKEN = "#type";
    constexpr const char* VERTEX_TYPE = "vertex";
    constexpr const char* FRAGMENT_TYPE = "fragment";

    Shader::Shader(const char* vertexSource, const char* fragmentSource) {
        ST_TRACE(ST_TAG, "Creating");
        id = create(vertexSource, fragmentSource);
        ST_TRACE(ST_TAG, "Created");
    }

    Shader::Shader(const char* filepath) {
        ST_TRACE(ST_TAG, "Creating");
        ST_DEBUG(ST_TAG, "Creating shader from single file");
        std::string fileText = FileReader(filepath).read();
        std::unordered_map<GLenum, std::string> shaderSources = getShaderSources(fileText);
        const char* vertexSource = shaderSources[GL_VERTEX_SHADER].c_str();
        const char* fragmentSource = shaderSources[GL_FRAGMENT_SHADER].c_str();
        id = create(vertexSource, fragmentSource);
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
        GLint location = glGetUniformLocation(id, key);
        GLint count = 1;
        GLboolean transpose = GL_FALSE;
        glUniformMatrix4fv(location, count, transpose, glm::value_ptr(value));
    }

    void Shader::setFloat4(const char* key, glm::vec4 value) const {
        GLint location = glGetUniformLocation(id, key);
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    GLuint Shader::create(const char* vertexSource, const char* fragmentSource) const {
        ST_DEBUG(ST_TAG, "Creating [{0}] shader", ST_TO_STRING(GL_VERTEX_SHADER));
        GLuint vertexShaderId = createShader(vertexSource, GL_VERTEX_SHADER);
        ST_DEBUG(ST_TAG, "Creating [{0}] shader", ST_TO_STRING(GL_FRAGMENT_SHADER));
        GLuint fragmentShaderId = createShader(fragmentSource, GL_FRAGMENT_SHADER);
        ST_DEBUG(ST_TAG, "Creating shader program");
        return createProgram(vertexShaderId, fragmentShaderId);
    }

    GLuint Shader::createShader(const char* source, GLenum type) const {
        GLuint shaderId = glCreateShader(type);
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

    void Shader::setShaderSource(const GLchar* source, GLuint shaderId) const {
        ST_TRACE(ST_TAG, "Setting shader source \n{0}", source);
        GLsizei count = 1;
        GLint* length = nullptr;
        glShaderSource(shaderId, count, &source, length);
    }

    bool Shader::compileShader(GLuint shaderId) const {
        ST_TRACE(ST_TAG, "Compiling shader with id [{0}]", shaderId);
        glCompileShader(shaderId);
        GLint status = 0;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
        bool compiled = status != 0;
        if (!compiled) {
            ST_ERROR(ST_TAG, "Could not compile shader with id [{0}]: [{1}]", shaderId, getShaderLog(shaderId));
        }
        return compiled;
    }

    std::string Shader::getShaderLog(GLuint shaderId) const {
        GLint length = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetShaderInfoLog(shaderId, length, &length, &log[0]);
        return log.data();
    }

    GLuint Shader::createProgram(GLuint vertexShaderId, GLuint fragmentShaderId) const {
        ST_TRACE(ST_TAG, "Creating program using vertex shader with id [{0}] and fragment shader with id [{1}]", vertexShaderId, fragmentShaderId);
        GLuint programId = glCreateProgram();
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

    bool Shader::linkProgram(GLuint programId) const {
        ST_TRACE(ST_TAG, "Linking shader program with id [{0}]", programId);
        glLinkProgram(programId);
        GLint status = 0;
        glGetProgramiv(programId, GL_LINK_STATUS, &status);
        bool linked = status != 0;
        if (!linked) {
            ST_ERROR(ST_TAG, "Could not link shader program with id [{0}]: [{1}]", programId, getProgramLog(programId));
        }
        return linked;
    }

    std::string Shader::getProgramLog(GLuint programId) const {
        GLint logLength = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> log(logLength);
        glGetProgramInfoLog(programId, logLength, &logLength, &log[0]);
        return log.data();
    }

    std::unordered_map<GLenum, std::string> Shader::getShaderSources(const std::string& fileText) const {
        ST_TRACE(ST_TAG, "Getting shader sources from file \n{0}\n", fileText);
        std::unordered_map<GLenum, std::string> shaderSources;
        size_t typeTokenStart = fileText.find(TYPE_TOKEN);
        while (typeTokenStart != std::string::npos) {
            size_t typeStart = typeTokenStart + strlen(TYPE_TOKEN) + 1;
            size_t typeEnd = fileText.find_first_of("\r\n", typeTokenStart);
            std::string typeString = fileText.substr(typeStart, typeEnd - typeStart);
            ST_TRACE(ST_TAG, "Found shader type string [{0}]", typeString);
            size_t shaderSourceStart = fileText.find_first_not_of("\r\n", typeEnd);
            typeTokenStart = fileText.find(TYPE_TOKEN, shaderSourceStart);
            if (typeTokenStart == std::string::npos) {
                shaderSources[getType(typeString)] = fileText.substr(shaderSourceStart);
            } else {
                shaderSources[getType(typeString)] = fileText.substr(shaderSourceStart, typeTokenStart - shaderSourceStart);
            }
        }
        return shaderSources;
    }

    GLenum Shader::getType(const std::string& typeString) const {
        if (typeString == VERTEX_TYPE) {
            return GL_VERTEX_SHADER;
        } else if (typeString == FRAGMENT_TYPE) {
            return GL_FRAGMENT_SHADER;
        } else {
            ST_WARN(ST_TAG, "Could not find shader type enum by type string [{}]", typeString);
            return 0;
        }
    }

}