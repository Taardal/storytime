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

        void bind() const;

        void unbind() const;

        void setUniform1i(const std::string& key, int value);

    private:
        unsigned int createShader(unsigned int shaderType, const std::string& shaderSource);

        void setShaderSource(unsigned int shaderId, const std::string& shaderSource);

        bool compileShader(unsigned int shaderId);

        std::string getShaderLog(unsigned int shaderId);

        unsigned int createProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId);

        bool linkProgram(unsigned int id);

        std::string getProgramLog(unsigned int programId);

        int getUniformLocation(const std::string& key);
    };

}

