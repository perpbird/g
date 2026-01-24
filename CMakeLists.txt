cmake_minimum_required(VERSION 3.28)
set(PROJECT_NAME g)
project(${PROJECT_NAME})

# Указать путь к заголовкам GLFW
include_directories(external/glfw/include)

# Опции для GLFW (ОЧЕНЬ ВАЖНО: перед add_subdirectory!)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# Подключить GLFW
add_subdirectory(external/glfw)

# Создать исполняемый файл
add_executable(${PROJECT_NAME} src/main.cpp)

# Связать с GLFW
target_link_libraries(${PROJECT_NAME} glfw)

# Для Linux добавить системные зависимости
if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME}
        glfw
        pthread
        dl
        m
        X11
    )
endif()

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)