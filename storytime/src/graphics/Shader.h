#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>
#include <glad/glad.h>

namespace storytime {

    class Shader {
    private:
        uint32_t id = 0;
        std::unordered_map<std::string, int> uniformLocations;

    public:
        explicit Shader(const char* filepath);

        Shader(const char* vertexSource, const char* fragmentSource);

        ~Shader();

        void bind() const;

        void unbind() const;

        void setMat4(const char* key, glm::mat4 value) const;

        void setFloat4(const char* key, glm::vec4 value) const;

    private:
        [[nodiscard]] GLuint create(const char* vertexSource, const char* fragmentSource) const;

        [[nodiscard]] GLuint createShader(const char* source, GLenum type) const;

        void setShaderSource(const GLchar* source, GLuint shaderId) const;

        [[nodiscard]] bool compileShader(GLuint shaderId) const;

        [[nodiscard]] std::string getShaderLog(GLuint shaderId) const;

        [[nodiscard]] GLuint createProgram(GLuint vertexShaderId, GLuint fragmentShaderId) const;

        [[nodiscard]] bool linkProgram(GLuint id) const;

        [[nodiscard]] std::string getProgramLog(GLuint programId) const;

        [[nodiscard]] std::unordered_map<GLenum, std::string> getShaderSources(const std::string& fileText) const;

        [[nodiscard]] GLenum getType(const std::string& typeString) const;
    };

}
