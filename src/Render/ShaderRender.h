#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
typedef unsigned int GLuint;
typedef int GLint;

namespace Render {
    class ShaderProgram {
    private:
        GLuint m_ID = 0;
        bool m_isCompiled = false;
    public:
        ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
        
        ShaderProgram(ShaderProgram&& shaderProgram)noexcept;
        ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
        
        ~ShaderProgram();
        
        bool use() const;
        bool isCompiled() const { return m_isCompiled; }
        GLuint getID() const { return m_ID; }
        
        // Методы для установки uniform-переменных
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setFloat3(const std::string& name, float v1, float v2, float v3) const;
        void setFloat4(const std::string& name, float v1, float v2, float v3, float v4) const;
        
    private:
        ShaderProgram() = delete;
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        bool createShader(const std::string& source, GLenum shaderType, GLuint& shaderID);
        GLint getUniformLocation(const std::string& name) const;
    };
}