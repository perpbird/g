#pragma once
#include <glad/glad.h>
#include <vector>
#include "Vertex.h"
#include "Shader.h"

class Cube {
private:
    GLuint VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    
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
    
    void draw(Renderer::ShaderProgram& shader) {
        shader.use();
        
        // Создаем матрицу трансформации
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        
        // Передаем матрицу в шейдер
        shader.setMat4("model", model);
        
        // Рисуем куб
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
        // Вершины куба (8 вершин с разными цветами)
        vertices = {
            // Позиция                // Цвет
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}}, // 0 - красный
            {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}}, // 1 - зеленый
            {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}, // 2 - синий
            {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}}, // 3 - желтый
            
            {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f}}, // 4 - пурпурный
            {{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 1.0f}}, // 5 - голубой
            {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}, // 6 - белый
            {{-0.5f,  0.5f,  0.5f}, {0.5f, 0.5f, 0.5f}}  // 7 - серый
        };
        
        // Индексы для 12 треугольников (6 граней * 2 треугольника)
        indices = {
            // Задняя грань
            0, 1, 2,  2, 3, 0,
            // Передняя грань
            4, 5, 6,  6, 7, 4,
            // Левая грань
            0, 3, 7,  7, 4, 0,
            // Правая грань
            1, 5, 6,  6, 2, 1,
            // Нижняя грань
            0, 1, 5,  5, 4, 0,
            // Верхняя грань
            3, 2, 6,  6, 7, 3
        };
    }
    
    void setupBuffers() {
        // Создаем VAO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);
        
        // VBO для вершин
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), 
                     vertices.data(), GL_STATIC_DRAW);
        
        // EBO для индексов
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), 
                     indices.data(), GL_STATIC_DRAW);
        
        // Настройка атрибутов
        // Позиция (location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Цвет (location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                             (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);
        
        glBindVertexArray(0);
    }
};