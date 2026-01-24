#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "./Render/ShaderRender.h"
#include "ExceptionClass.cpp"
#include <iostream>

class ScreenOptions{
protected:
	int height{0};
	int width{0};
	int refreshRate{0};
public:
	ScreenOptions(){
		// Получаем основной монитор
    	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

		height = mode->height;
		width = mode->width;
		refreshRate = mode->refreshRate;
	}

	ScreenOptions& operator=(const ScreenOptions& other){
		height = other.height;
		width = other.width;
		refreshRate = other.refreshRate;
		return *this;
	}

	int get_width() const{
		return width;
	}
	int get_height() const{
		return height;
	}
	
};