#pragma once
#include <vector>
#include <glm/glm.hpp>

const std::vector<glm::vec3> QUAD_VERTS = { glm::vec3(-1.0, -1.0, 0.0), glm::vec3(-1.0, 1.0, 0.0), glm::vec3(1.0, 1.0, 0.0), glm::vec3(1.0, -1.0, 0.0) };
const std::vector<unsigned int> QUAD_INDICES = { 0, 1, 2, 0, 2, 3 };