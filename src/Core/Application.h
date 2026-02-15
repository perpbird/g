#pragma once
#include "Window.h"
#include "Graphics/Shader.h"
#include "Graphics/Cube.h"
#include "../Resource/ResourceManager.h"
#include "../Utils/Logger.h"
#include "../Utils/ErrorHandler.h"
#include <memory>

class Application {
private:
    std::unique_ptr<Window> window;
    std::unique_ptr<ResourceManager> resources;
    std::shared_ptr<Renderer::ShaderProgram> shader;
    std::unique_ptr<Cube> cube;

    float lastFrameTime;
    float deltaTime;

    void initOpenGL();
    void initResources();
    void handleInput();

protected:
    virtual void onInit() {}
    virtual void onUpdate(float dt) {}
    virtual void onRender() {}
    virtual void onShutdown() {}

public:
    Application() : lastFrameTime(0), deltaTime(0) {}
    virtual ~Application() = default;

    void run();
};