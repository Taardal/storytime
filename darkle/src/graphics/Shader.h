#pragma once

#include <unordered_map>

namespace Darkle {

    class Shader {
    private:
        unsigned int id;
        std::unordered_map<std::string, int> uniformLocations;

    public:
        Shader(const std::string& vertexSource, const std::string& fragmentSource);

        ~Shader();

        void Bind() const;

        void Unbind() const;

        void SetUniform1i(const std::string& key, int value);

    private:
        unsigned int CreateShader(unsigned int shaderType, const std::string& shaderSource);

        void SetShaderSource(unsigned int shaderId, const std::string& shaderSource);

        bool CompileShader(unsigned int shaderId);

        std::string GetShaderLog(unsigned int shaderId);

        unsigned int CreateProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId);

        bool LinkProgram(unsigned int id);

        std::string GetProgramLog(unsigned int programId);

        int GetUniformLocation(const std::string& key);
    };

}

