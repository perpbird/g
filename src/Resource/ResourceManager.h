#pragma once
#include <string>
#include <memory>
#include <map>
#include "../Graphics/ShaderRenderer.h" 

namespace Renderer{
    class ShaderProgram;
}

class ResourceManager{
private:
    typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramMap;
    ShaderProgramMap m_shaderPrograms;

    std::string m_path;

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(const ResourceManager&&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&&) = delete;
public:
    ResourceManager(const std::string& Path);
    
    std::shared_ptr<Renderer::ShaderProgram> loadShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
    std::shared_ptr<Renderer::ShaderProgram> getShader(const std::string& shaderName);

    ~ResourceManager() = default;
};