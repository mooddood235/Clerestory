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

		this->width = width;
		this->height = height;
		this->inFormat = inFormat;

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
	Texture(const std::string& hdrTexturePath) {
		glGenTextures(1, &textureID);

		stbi_set_flip_vertically_on_load(true);

		int width, height, numChannels;
		float* data = stbi_loadf(hdrTexturePath.c_str(), &width, &height, &numChannels, 0);

		if (!data) {
			std::cout << "ERROR: Could not load enviroment map with path <" << hdrTexturePath << ">" << std::endl;
			glfwTerminate();
			exit(-1);
		}
		this->width = width;
		this->height = height;
		inFormat = GL_RGB32F;

		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);

		stbi_set_flip_vertically_on_load(false);
	}
	Texture(unsigned int width, unsigned int height) {
		this->width = width;
		this->height = height;
		inFormat = GL_RGBA32F;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	unsigned int GetID() {
		return textureID;
	}
	void BindImageTexture(unsigned int bindUnit, GLenum access) {
		glBindImageTexture(bindUnit, textureID, 0, GL_FALSE, 0, access, inFormat);
	}
private:
	unsigned int textureID;

	unsigned int width;
	unsigned int height;

	GLenum inFormat;
};