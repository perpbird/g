cmake_minimum_required(VERSION 3.28)
project(glad)

# Добавить библиотеку GLAD
add_library(glad STATIC src/glad.c)

# Установить пути к заголовкам (правильный синтаксис!)
target_include_directories(glad PUBLIC include)
