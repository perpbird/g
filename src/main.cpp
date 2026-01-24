#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "./Render/ShaderRender.h"
#include "ExceptionClass.cpp"
#include "Screen.cpp"
#include "Window.cpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>

GLfloat point[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, -0.5f, 0.5f
};

GLfloat color[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

const char* vertex_shader = 
"#version 460\n"
"layout(location = 0) in vec3 vertex_position;\n"
"layout(location = 1) in vec3 vertex_color;\n"
"out vec3 color;\n"
"void main(){\n"
"    color = vertex_color;\n"
"    gl_Position = vec4(vertex_position, 1.0);\n"
"}\n";

const char* fragment_shader = 
"#version 460\n"
"in vec3 color;\n"
"out vec4 frag_color;\n"
"void main(){\n"
"    frag_color = vec4(color, 1.0);\n"
"}\n";

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

int main() {
    try {
        preRun();

        MyWindow* w = new MyWindow(0.5, 0.5);
        GLFWwindow* window = w->get_window();
        
        if (!window) {
            throw ErrorException("Window is null!");
        }

        // Инициализация OpenGL
        Run(window);
        
        // Создание шейдерной программы
        std::string vertex_str(vertex_shader);
        std::string fragment_str(fragment_shader);
        
        Render::ShaderProgram shaderProgram(vertex_str, fragment_str);
        
        if (!shaderProgram.isCompiled()) {
            throw ErrorException("Can't create shader program");
        }
        
        std::cout << "Shader program created successfully! ID: " << shaderProgram.getID() << std::endl;
        
        // Инициализация геометрии
        shaderInit();
        
        // Включение VSync
        glfwSwapInterval(1);
        
        // Главный цикл
        while (!glfwWindowShouldClose(window)) {
            // Очистка экрана
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            // Использование шейдера
            shaderProgram.use();
            
            // Отрисовка
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);
            
            // Обмен буферов
            glfwSwapBuffers(window);
            
            // Обработка событий
            glfwPollEvents();
        }
        
        // Очистка OpenGL ресурсов
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &points_vbo);
        glDeleteBuffers(1, &colors_vbo);
        
        // Очистка GLFW
        glfwDestroyWindow(window);
        delete w;
        glfwTerminate();
        
        std::cout << "Program terminated successfully!" << std::endl;
    }
    catch (ErrorException& e) {
        std::cout << "Error: " << e.what() << std::endl;
        glfwTerminate();
        return -1;
    }
    catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
        glfwTerminate();
        return -1;
    }
    catch (...) {
        std::cout << "Unknown error!" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    return 0;
}