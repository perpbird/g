#include "ResourceManager.h"
#include "../Graphics/ShaderRenderer.h"
#include "../Utils/FileUtils.h"
#include <iostream>

ResourceManager::ResourceManager(const std::string& Path) : m_path(Path) {
    if (!m_path.empty() && m_path.back() != '/') {
        m_path += '/';
    }
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShader(
    const std::string& shaderName, 
    const std::string& vertexPath, 
    const std::string& fragmentPath) {
    
    auto it = m_shaderPrograms.find(shaderName);
    if (it != m_shaderPrograms.end()) {
        return it->second;
    }
    
    std::string vertexCode = FileUtils::readFile(m_path + vertexPath);
    std::string fragmentCode = FileUtils::readFile(m_path + fragmentPath);
    
    if (vertexCode.empty() || fragmentCode.empty()) {
        std::cerr << "Failed to load shader files: " << shaderName << std::endl;
        return nullptr;
    }
    
    auto shader = std::make_shared<Renderer::ShaderProgram>(vertexCode, fragmentCode);
    
    if (shader->isCompiled()) {
        m_shaderPrograms[shaderName] = shader;
        std::cout << "Shader '" << shaderName << "' loaded successfully!" << std::endl;
    }
    
    return shader;
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShader(const std::string& shaderName) {
    auto it = m_shaderPrograms.find(shaderName);
    if (it != m_shaderPrograms.end()) {
        return it->second;
    }
    return nullptr;
}