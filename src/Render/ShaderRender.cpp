#include "ShaderRender.h"
#include <iostream>
#include <utility>

namespace Render {
    ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) {
        GLuint vertexShaderID;
        if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID)) {
            std::cerr << "VERTEX SHADER compile time error" << std::endl;
            return;
        }

        GLuint fragmentShaderID;
        if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID)) {
            std::cerr << "FRAGMENT SHADER compile time error" << std::endl;
            glDeleteShader(vertexShaderID);
            return;
        }

        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertexShaderID);
        glAttachShader(m_ID, fragmentShaderID);
        glLinkProgram(m_ID);

        GLint success;
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar infoLog[1024];
            glGetProgramInfoLog(m_ID, 1024, nullptr, infoLog);  // Исправлено: glGetProgramInfoLog
            std::cerr << "ERROR::SHADER: Link time error:\n" << infoLog << std::endl;
        } else {
            m_isCompiled = true;
            std::cout << "Shader program compiled successfully! ID: " << m_ID << std::endl;
        }
        
        // Всегда удаляем шейдеры, даже если линковка не удалась
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
    }

    ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram)noexcept{
        m_ID = shaderProgram.m_ID;
        m_isCompiled = shaderProgram.m_isCompiled;

        shaderProgram.m_ID = 0;
        shaderProgram.m_isCompiled = false;
    }

    ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept{
        glDeleteProgram(m_ID);
        m_ID = shaderProgram.m_ID;
        m_isCompiled = shaderProgram.m_isCompiled;

        shaderProgram.m_ID = 0;
        shaderProgram.m_isCompiled = false;

        return *this;
    }
        

    bool ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID) {
        shaderID = glCreateShader(shaderType);
        const char* code = source.c_str();
        glShaderSource(shaderID, 1, &code, nullptr);
        glCompileShader(shaderID);

        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[1024];
            glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER: Compile time error:\n" << infoLog << std::endl;
            glDeleteShader(shaderID);
            return false;
        }
        return true;
    }

    bool ShaderProgram::use() const {
        if (!m_isCompiled) {
            std::cerr << "Shader program is not compiled!" << std::endl;
            return false;
        }
        glUseProgram(m_ID);
        return true;
    }

    ShaderProgram::~ShaderProgram() {
        if (m_ID != 0) {
            glDeleteProgram(m_ID);
        }
    }

    // Методы для установки uniform-переменных
    GLint ShaderProgram::getUniformLocation(const std::string& name) const {
        return glGetUniformLocation(m_ID, name.c_str());
    }

    void ShaderProgram::setBool(const std::string& name, bool value) const {
        glUniform1i(getUniformLocation(name), (int)value);
    }

    void ShaderProgram::setInt(const std::string& name, int value) const {
        glUniform1i(getUniformLocation(name), value);
    }

    void ShaderProgram::setFloat(const std::string& name, float value) const {
        glUniform1f(getUniformLocation(name), value);
    }

    void ShaderProgram::setFloat3(const std::string& name, float v1, float v2, float v3) const {
        glUniform3f(getUniformLocation(name), v1, v2, v3);
    }

    void ShaderProgram::setFloat4(const std::string& name, float v1, float v2, float v3, float v4) const {
        glUniform4f(getUniformLocation(name), v1, v2, v3, v4);
    }
}