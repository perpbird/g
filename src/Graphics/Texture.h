#pragma once
#include <glad/glad.h>
#include <string>
#include "../Utils/Logger.h"

class Texture {
private:
    GLuint m_ID;
    int m_width, m_height, m_channels;
    std::string m_path;

public:
    Texture(const std::string& path);
    ~Texture();
    
    void bind(int slot = 0) const;
    void unbind() const;
    
    GLuint getID() const { return m_ID; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
};