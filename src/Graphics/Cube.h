#pragma once
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Vertex.h"
#include "../Graphics/ShaderRenderer.h"
#include "Texture.h"
#include <memory>

class Cube {
private:
    GLuint VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    std::unique_ptr<Texture> m_texture;
    
public:
    Cube(const glm::vec3& pos = glm::vec3(0.0f), 
         const glm::vec3& rot = glm::vec3(0.0f),
         const glm::vec3& scl = glm::vec3(1.0f)) 
        : position(pos), rotation(rot), scale(scl) {
        initCubeData();
        setupBuffers();
    }
    
    ~Cube() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void setTexture(std::unique_ptr<Texture> texture) { 
        m_texture = std::move(texture); 
    }
    
    void draw(Renderer::ShaderProgram& shader) {
        shader.use();
        
        // Если есть текстура - используем её
        if (m_texture) {
            m_texture->bind(0);
            shader.setInt("ourTexture", 0);
        }
        
        // Создаем матрицу трансформации
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        
        shader.setMat4("model", model);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    
    void rotate(float x, float y, float z) {
        rotation.x += x;
        rotation.y += y;
        rotation.z += z;
    }
    
    void setPosition(const glm::vec3& pos) { position = pos; }
    void setRotation(const glm::vec3& rot) { rotation = rot; }
    void setScale(const glm::vec3& scl) { scale = scl; }
    
private:
    void initCubeData() {
        // Вершины куба с текстурными координатами
        vertices = {
            // Передняя грань (z = 0.5)
            {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
            
            // Задняя грань (z = -0.5)
            {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
            {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
            {{ 0.5f,  0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}},
        };
        
        // Индексы остаются теми же
        indices = {
            0, 1, 2,  2, 3, 0,  // перед
            4, 5, 6,  6, 7, 4,  // зад
            1, 4, 7,  7, 2, 1,  // право
            5, 0, 3,  3, 6, 5,  // лево
            3, 2, 7,  7, 6, 3,  // верх
            5, 4, 1,  1, 0, 5   // низ
        };
    }
    
    void setupBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), 
                    vertices.data(), GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), 
                    indices.data(), GL_STATIC_DRAW);
        
        // Позиция (location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Цвет (location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                            (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);
        
        // Текстурные координаты (location = 2)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                            (void*)offsetof(Vertex, texCoord));
        glEnableVertexAttribArray(2);
        
        glBindVertexArray(0);
    }
};