#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class Space {
	local,
	global
};

class GameObject {
public:
	void Translate(glm::vec3 translation, Space space = Space::global) {
		if (space == Space::local) translation = glm::mat3(rotationMatrix) * translation;
		position += translation;
		translationMatrix = glm::translate(translationMatrix, translation);
	}
	void Scale(glm::vec3 factor, Space space = Space::global) {
		if (space == Space::local) factor = glm::mat3(scaleMatrix) * factor;
		scaleMatrix = glm::scale(scaleMatrix, factor);
	}
	void Rotate(float angleInDegrees, glm::vec3 axis, Space space = Space::global) {
		if (space == Space::global) axis = glm::normalize(glm::inverse(rotationMatrix) * glm::vec4(axis, 0));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(angleInDegrees), axis);
	}
	glm::vec3 GetPosition() {
		return position;
	}
	glm::mat4 GetModelMatrix() {
		return translationMatrix * scaleMatrix * rotationMatrix;
	}
	glm::mat3 GetNormalMatrix() {
		return glm::transpose(glm::inverse(GetModelMatrix()));
	}
	glm::vec3 GetXAxis() {
		return glm::normalize(rotationMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	}
	glm::vec3 GetYAxis() {
		return glm::normalize(rotationMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	}
	glm::vec3 GetZAxis() {
		return glm::normalize(rotationMatrix * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	}
private:
	glm::mat4 translationMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMatrix = glm::mat4(1.0f);

	glm::vec3 position = glm::vec3(0.0f);
};