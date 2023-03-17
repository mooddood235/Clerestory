#pragma once
#include <glm/glm.hpp>

class Volume{
public:
	Volume(glm::vec3 cornerMin, glm::vec3 cornerMax) {
		this->cornerMin = cornerMin;
		this->cornerMax = cornerMax;
	}
public:
	glm::vec3 cornerMin;
	glm::vec3 cornerMax;
};