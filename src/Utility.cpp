#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "./Renderer/ShaderRenderer.h"
#include "./Resource/ResourceManager.h"
#include "ExceptionClass.cpp"
#include "Screen.cpp"
#include "Window.cpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <fstream>
#include <sstream>

GLfloat point[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.5f
};

GLfloat color[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

GLuint vao = 0;  // Глобальная или статическая переменная
GLuint points_vbo = 0;
GLuint colors_vbo = 0;


void Run(GLFWwindow* window) {
    if (!window) {
        glfwTerminate();
        throw ErrorException("Window creation failed!");
    }
    
    // Делаем контекст текущим
    glfwMakeContextCurrent(window);
    
    // Инициализация GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw ErrorException("GLAD init failed!");
    }
    
    // Теперь можно использовать OpenGL функции
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
}

void preRun() {
    if (!glfwInit()) {
        throw ErrorException("GLFW init failed!");
    }
    
    // Настройка OpenGL версии
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void shaderInit() {
    // Создание VBO для точек
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
    
    // Создание VBO для цветов
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
    
    // Создание VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // Настройка атрибутов для точек
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    
    // Настройка атрибутов для цветов
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    
    // Отвязываем
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

std::string readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filepath);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();  // Читаем весь файл в буфер
    return buffer.str();
}