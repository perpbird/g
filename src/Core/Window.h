#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include "../Utils/Logger.h"
#include "../Utils/ErrorHandler.h"

class Window {
private:
    GLFWwindow* window;
    int width;
    int height;
    std::string title;

public:
    Window(int w = 800, int h = 600, const std::string& t = "Engine") 
        : width(w), height(h), title(t) {
        if (!glfwInit()) {
            THROW_ENGINE_EXCEPTION("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            THROW_ENGINE_EXCEPTION("Failed to create window");
        }

        glfwMakeContextCurrent(window);
        Logger::info("Window created: " + std::to_string(width) + "x" + 
                    std::to_string(height));
    }

    ~Window() {
        if (window) {
            glfwDestroyWindow(window);
        }
        glfwTerminate();
        Logger::info("Window destroyed");
    }

    void update() {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    bool shouldClose() const {
        return glfwWindowShouldClose(window);
    }

    void setShouldClose(bool close) {
        glfwSetWindowShouldClose(window, close);
    }

    GLFWwindow* getGLFWwindow() const { return window; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void setVSync(bool enabled) {
        glfwSwapInterval(enabled ? 1 : 0);
    }
};