#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>
#include <glad/glad.h>

namespace storytime
{
    class Shader
    {
    private:
        uint32_t id = 0;

    public:
        Shader(const char* vertexSource, const char* fragmentSource);

        ~Shader();

        void Bind() const;

        void Unbind() const;

        void SetInt1(const char* key, uint32_t value) const;

        void SetMat4(const char* key, glm::mat4 value) const;

        void SetFloat4(const char* key, glm::vec4 value) const;

        void SetIntArray(const char* key, const int32_t* values, uint32_t count) const;

    private:
        [[nodiscard]] uint32_t CreateShader(const char* source, uint32_t type) const;

        void SetShaderSource(const char* source, uint32_t shaderId) const;

        [[nodiscard]] bool CompileShader(uint32_t shaderId) const;

        [[nodiscard]] std::string GetShaderLog(uint32_t shaderId) const;

        [[nodiscard]] uint32_t CreateProgram(uint32_t vertexShaderId, uint32_t fragmentShaderId) const;

        [[nodiscard]] bool LinkProgram(uint32_t id) const;

        [[nodiscard]] std::string GetProgramLog(uint32_t programId) const;

    };

}

