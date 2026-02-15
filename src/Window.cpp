#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "./Renderer/ShaderRenderer.h"
#include "ExceptionClass.cpp"
#include "Screen.cpp"
#include <iostream>

class MyWindow{
	GLFWwindow* window;	
	ScreenOptions screen;
public:
	MyWindow(){
		screen = ScreenOptions();
		window = glfwCreateWindow(screen.get_width(),screen.get_height(),"Window",nullptr,nullptr);
	}

	MyWindow(float widthM, float heightM){
		screen = ScreenOptions();
		window = glfwCreateWindow(screen.get_width()*widthM,screen.get_height()*heightM,"Window",nullptr,nullptr);
	}

	GLFWwindow* get_window() const{
		return window;
	}

};