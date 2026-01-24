cmake_minimum_required(VERSION 3.10)
project(g)

# Указать путь к заголовкам GLFW
include_directories(external/glfw/include)

# Опции для GLFW (ОЧЕНЬ ВАЖНО: перед add_subdirectory!)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_INSTALL OFF)

# Подключить GLFW
add_subdirectory(external/glfw)

# Создать исполняемый файл
add_executable(g src/main.cpp)

# Связать с GLFW
target_link_libraries(g glfw)

# Для Linux добавить системные зависимости
if(UNIX AND NOT APPLE)
    target_link_libraries(g
        glfw
        pthread
        dl
        m
        X11
    )
endif()