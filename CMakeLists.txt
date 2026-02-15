cmake_minimum_required(VERSION 3.28)
set(PROJECT_NAME g)
project(${PROJECT_NAME})

include(FetchContent)

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glm)

# Исполняемый файл
add_executable(${PROJECT_NAME} 
    src/main.cpp
    
    src/Core/Application.cpp
    src/Core/Window.cpp
    
    src/Graphics/Cube.cpp
    src/Graphics/ShaderRenderer.cpp
    src/Graphics/Vertex.cpp
    src/Graphics/Textures.cpp
    
    
    src/Resource/ResourceManager.cpp
    
    src/Utils/FileUtils.cpp
    src/Utils/Logger.cpp
    src/Utils/PathUtils.cpp
    
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17) 

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include
    ${glm_SOURCE_DIR}
)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

target_link_libraries(${PROJECT_NAME} glfw)

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME}
        glfw
        glad
        pthread
        dl
        m
        X11
    )
endif()

# glad
add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

set_target_properties(${PROJECT_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Копирование ресурсов
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res/
        ${CMAKE_BINARY_DIR}/bin/res/
    COMMENT "Copying resources..."
)