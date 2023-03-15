#pragma once
#include<GLFW/glfw3.h>

struct WindowInfo {
	GLFWwindow* window;
	unsigned int width, height;

	WindowInfo(GLFWwindow* window, unsigned int width, unsigned int height) {
		this->window = window;
		this->width = width;
		this->height = height;
	}
};