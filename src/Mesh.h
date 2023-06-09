#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

class Mesh {
public:
	Mesh(const std::vector<glm::vec3>& verts, const std::vector<unsigned int>& indices) {
		indicesSize = indices.size();

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * verts.size(), verts.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	void Draw() {
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
protected:
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;

	unsigned int indicesSize;
};