#pragma once
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    // Позже добавим texture coordinates, normals и т.д.
    
    Vertex(const glm::vec3& pos, const glm::vec3& col) 
        : position(pos), color(col) {}
};