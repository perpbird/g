#include "Application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void Application::initOpenGL() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        THROW_ENGINE_EXCEPTION("Failed to initialize GLAD");
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, window->getWidth(), window->getHeight());

    Logger::info("OpenGL initialized");
    Logger::info("Renderer: " + std::string((char*)glGetString(GL_RENDERER)));
    Logger::info("Version: " + std::string((char*)glGetString(GL_VERSION)));
}

void Application::initResources() {
    resources = std::make_unique<ResourceManager>("res/");
    
    shader = resources->loadShader("basic", "vertex.txt", "fragment.txt");
    if (!shader || !shader->isCompiled()) {
        THROW_ENGINE_EXCEPTION("Failed to load shader");
    }

    cube = std::make_unique<Cube>();

    // Устанавливаем матрицы камеры
    glm::mat4 view = glm::lookAt(
        glm::vec3(3.0f, 2.0f, 5.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (float)window->getWidth() / (float)window->getHeight(),
        0.1f,
        100.0f
    );

    shader->use();
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
}

void Application::handleInput() {
    GLFWwindow* glfwWindow = window->getGLFWwindow();
    
    if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        window->setShouldClose(true);
    }

    // Вращение куба по нажатию клавиш
    if (glfwGetKey(glfwWindow, GLFW_KEY_UP) == GLFW_PRESS) {
        cube->rotate(50.0f * deltaTime, 0.0f, 0.0f);
    }
    if (glfwGetKey(glfwWindow, GLFW_KEY_DOWN) == GLFW_PRESS) {
        cube->rotate(-50.0f * deltaTime, 0.0f, 0.0f);
    }
    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT) == GLFW_PRESS) {
        cube->rotate(0.0f, 50.0f * deltaTime, 0.0f);
    }
    if (glfwGetKey(glfwWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        cube->rotate(0.0f, -50.0f * deltaTime, 0.0f);
    }
}

void Application::run() {
    try {
        Logger::init();
        Logger::info("Application starting...");

        window = std::make_unique<Window>(1280, 720, "Game Engine");
        window->setVSync(true);

        initOpenGL();
        initResources();
        onInit();

        Logger::info("Entering main loop");

        while (!window->shouldClose()) {
            float currentTime = glfwGetTime();
            deltaTime = currentTime - lastFrameTime;
            lastFrameTime = currentTime;

            handleInput();
            onUpdate(deltaTime);

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            cube->draw(*shader);
            onRender();

            window->update();
        }

        onShutdown();
        Logger::info("Application terminated");
        Logger::shutdown();
    }
    catch (const EngineException& e) {
        Logger::error(std::string("Engine exception: ") + e.what());
        Logger::shutdown();
        throw;
    }
}