#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "GameObject.h"
#include "WindowInfo.h"

class Camera : public GameObject{
public:
	Camera(float yFOVInDegrees, WindowInfo windowInfo) {
		SetYFOV(yFOVInDegrees, windowInfo);
	}
	void SetNearClip(float nearClip) {
		this->nearClip = glm::max(0.0f, nearClip);
	}
	void SetFarClip(float farClip) {
		this->farClip = glm::max(nearClip, farClip);
	}
	void SetYFOV(float yFOVInDegrees, WindowInfo windowInfo) {
		this->yFOVInDegrees = yFOVInDegrees;
		focalLength = (float)windowInfo.width / (2.0f * glm::tan(yFOVInDegrees / 2.0f));
	}
	void SetFocalLength(float focalLength, WindowInfo windowInfo) {
		this->focalLength = focalLength;
		yFOVInDegrees = 2.0f * glm::atan((float)windowInfo.width / (2.0f * focalLength));
	}
	void ProcessInput(WindowInfo windowInfo, float deltaTime) {
		GLFWwindow* window = windowInfo.window;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) Translate(glm::vec3(0, 0, -1) * moveSpeed * deltaTime, Space::local);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) Translate(glm::vec3(0, 0, 1) * moveSpeed * deltaTime, Space::local);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) Translate(glm::vec3(-1, 0, 0) * moveSpeed * deltaTime, Space::local);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) Translate(glm::vec3(1, 0, 0) * moveSpeed * deltaTime, Space::local);


		double currMousePosX, currMousePosY;
		glfwGetCursorPos(window, &currMousePosX, &currMousePosY);

		if (!firstMouseEnter) {
			lastMousePos = glm::vec2(currMousePosX, currMousePosY);
			firstMouseEnter = true;
		}

		double xOffset = currMousePosX - lastMousePos.x;
		double yOffset = currMousePosY - lastMousePos.y;

		Rotate(-xOffset * lookSensitivity, glm::vec3(0, 1, 0), Space::global);
		Rotate(-yOffset * lookSensitivity, glm::vec3(1, 0, 0), Space::local);

		lastMousePos = glm::vec2(currMousePosX, currMousePosY);
	}
public:
	float moveSpeed = 15.0f;
	float lookSensitivity = 0.05f;

	float nearClip = 0.0f;
	float farClip = 100.0f;
private:
	float yFOVInDegrees;
	float focalLength;

	bool firstMouseEnter = false;
	glm::vec2 lastMousePos = glm::vec2(0.0f);
};