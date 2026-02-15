cmake_minimum_required(VERSION 3.28)
set(PROJECT_NAME g)
project(${PROJECT_NAME})

# Включение FetchContent
include(FetchContent)

# Скачивание GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master
)

FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
endif()

# Исполняемый файл
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/Renderer/ShaderRenderer.cpp
    src/Renderer/ShaderRenderer.h
    src/Resource/ResourceManager.cpp
    src/Resource/ResourceManager.h
    # Добавь сюда новые файлы, например:
    # src/Graphics/Cube.cpp
    # src/Graphics/Cube.h
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17) 

# Пути к заголовкам
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include
    ${glm_SOURCE_DIR}  # Путь к GLM
)

# Опции для GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# Подключить GLFW
add_subdirectory(external/glfw)

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

# Подключить glad
add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

# Явно не линкуем GLM - это header-only библиотека!

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Копирование ресурсов
if(EXISTS ${CMAKE_SOURCE_DIR}/res)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/res/
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/res/
            ${CMAKE_BINARY_DIR}/bin/res/
        COMMENT "Copying resources..."
    )
endif()