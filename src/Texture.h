#pragma once
#include <iostream>
#include <stb_image/stb_image.h>
#include <string>
#include <assimp/scene.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

struct Texture {
public:
	Texture(const std::string& texturePath, aiTextureType type) {
		glGenTextures(1, &textureID);

		int width, height, numChannels;

		unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &numChannels, 0);

		if (!data) {
			std::cout << "ERROR: Could not load texture at path <" << texturePath << ">" << std::endl;
			stbi_image_free(data);
			glfwTerminate();
			exit(-1);
		}
		this->width = width;
		this->height = height;

		GLenum inFormat = GL_RED, outFormat = GL_RED;

		if (numChannels == 3) {
			if (type == aiTextureType_DIFFUSE) inFormat = GL_SRGB;
			else inFormat = GL_RGB;
			outFormat = GL_RGB;
		}
		else if (numChannels == 4) {
			if (type == aiTextureType_DIFFUSE) inFormat = GL_SRGB_ALPHA;
			else inFormat = GL_RGBA;
			outFormat = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, inFormat, width, height, 0, outFormat, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
	}
	Texture(unsigned int width, unsigned int height) {
		this->width = width;
		this->height = height;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		std::vector<unsigned char> emptyData(width * height * 3, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, emptyData.data());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
private:
	unsigned int textureID;

	unsigned int width;
	unsigned int height;
};