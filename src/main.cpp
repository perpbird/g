#include "Core/Application.h"
#include "Graphics/Cube.h"
#include "Utils/Logger.h"
#include <iostream>

class MyGame : public Application {
protected:
    void onInit() override {
        Logger::info("MyGame initialized!");
    }

    void onUpdate(float dt) override {
        // Вращаем куб
        if (cube) {
            cube->rotate(20.0f * dt, 30.0f * dt, 10.0f * dt);
        }
    }

    void onRender() override {
        // Здесь можно добавить дополнительный рендеринг
    }

    void onShutdown() override {
        Logger::info("MyGame shutting down!");
    }
};

int main() {
    try {
        MyGame game;
        game.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}