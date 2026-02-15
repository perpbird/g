#include "Texture.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"

Texture::Texture(const std::string& path) : m_path(path) {
    stbi_set_flip_vertically_on_load(true); // OpenGL ожидает текстуры перевернутыми
    
    unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
    if (!data) {
        Logger::error("Failed to load texture: " + path);
        return;
    }
    
    Logger::info("Texture loaded: " + path + " (" + 
                 std::to_string(m_width) + "x" + std::to_string(m_height) + 
                 ", " + std::to_string(m_channels) + " channels)");
    
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);
    
    // Определяем формат текстуры
    GLenum format = GL_RGB;
    if (m_channels == 1) format = GL_RED;
    else if (m_channels == 3) format = GL_RGB;
    else if (m_channels == 4) format = GL_RGBA;
    
    glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // Настройки текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_image_free(data);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_ID);
}

void Texture::bind(int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}