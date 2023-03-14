#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram {
public:
	ShaderProgram(const const std::string& vertPath, const const std::string& fragPath) {
		LinkProgram(CompileShader(vertPath, GL_VERTEX_SHADER), CompileShader(fragPath, GL_FRAGMENT_SHADER));
	}
	ShaderProgram(const const std::string& computePath) {
		LinkProgram(CompileShader(computePath, GL_COMPUTE_SHADER));
	}
	void Use() {
		glUseProgram(shaderProgramID);
	}
	static void Unuse() {
		glUseProgram(0);
	}
	void SetMat4(const std::string& uniformName, glm::mat4 value) {
		unsigned int location = glGetUniformLocation(shaderProgramID, uniformName.c_str());
		glProgramUniformMatrix4fv(shaderProgramID, location, 1, GL_FALSE, glm::value_ptr(value));
	}
	void SetVec3(const std::string& uniformName, glm::vec3 value) {
		unsigned int location = glGetUniformLocation(shaderProgramID, uniformName.c_str());
		glProgramUniform3fv(shaderProgramID, location, 1, glm::value_ptr(value));
	}
	void SetFloat(const std::string& uniformName, float value) {
		unsigned int location = glGetUniformLocation(shaderProgramID, uniformName.c_str());
		glProgramUniform1f(shaderProgramID, location, value);
	}
	void SetDouble(const std::string& uniformName, double value) {
		unsigned int location = glGetUniformLocation(shaderProgramID, uniformName.c_str());
		glProgramUniform1d(shaderProgramID, location, value);
	}
	void SetInt(const std::string& uniformName, int value) {
		unsigned int location = glGetUniformLocation(shaderProgramID, uniformName.c_str());
		glProgramUniform1i(shaderProgramID, location, value);
	}
	void SetUnsignedInt(const std::string& uniformName, unsigned int value) {
		unsigned int location = glGetUniformLocation(shaderProgramID, uniformName.c_str());
		glProgramUniform1ui(shaderProgramID, location, value);
	}
	void BindUniformBlock(const std::string& blockName, unsigned int bind) {
		unsigned int blockIndex = glGetUniformBlockIndex(shaderProgramID, blockName.c_str());
		glUniformBlockBinding(shaderProgramID, blockIndex, bind);
	}
	void BindStorageBlock(const std::string& blockName, unsigned int bind) {
		unsigned int blockIndex = glGetProgramResourceIndex(shaderProgramID, GL_SHADER_STORAGE_BLOCK, blockName.c_str());
		glShaderStorageBlockBinding(shaderProgramID, blockIndex, bind);
	}
private:
	unsigned int shaderProgramID;
private:
	unsigned int CompileShader(const std::string& filePath, GLenum type) {
		if (!(type == GL_VERTEX_SHADER || type == GL_FRAGMENT_SHADER || type == GL_COMPUTE_SHADER)) {
			std::cout << "ERROR: Cannot compile shader of type <" << std::to_string(type) << ">" << std::endl;
			glfwTerminate();
			exit(-1);
		}
		std::ifstream file = std::ifstream(filePath);
		std::stringstream stringstream;

		if (!file.is_open()) {
			std::cout << "ERROR: Could not open file at path <" << filePath << ">" << std::endl;
			file.close();
			glfwTerminate();
			exit(-1);
		}
		stringstream << file.rdbuf();
		const std::string shaderContents = stringstream.str();

		unsigned int shader = glCreateShader(type);
		const char* shaderContentsCString = shaderContents.c_str(); // glShaderSource() requires a const double pointer thingy.
		glShaderSource(shader, 1, &shaderContentsCString, NULL);
		glCompileShader(shader);
		ShaderCompilationErrorCheck(shader, filePath);

		file.close();

		return shader;

	}
	void LinkProgram(unsigned int vertShader, unsigned int fragShader) {
		shaderProgramID = glCreateProgram();

		glAttachShader(shaderProgramID, vertShader);
		glAttachShader(shaderProgramID, fragShader);

		glLinkProgram(shaderProgramID);
		ProgramLinkingErrorCheck(shaderProgramID);

		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
	}
	void LinkProgram(unsigned int computeShader) {
		shaderProgramID = glCreateProgram();

		glAttachShader(shaderProgramID, computeShader);

		glLinkProgram(shaderProgramID);
		ProgramLinkingErrorCheck(shaderProgramID);

		glDeleteShader(computeShader);
	}
	void ShaderCompilationErrorCheck(unsigned int shader, const std::string filePath) {
		int success;

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success) {
			char infoLog[1024];
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);

			std::cout << "Error: Could not compile shader at path <" << filePath << ">. Error message:\n" << infoLog << std::endl;
			glfwTerminate();
			exit(-1);
		}
	}
	void ProgramLinkingErrorCheck(unsigned int shaderProgramID) {
		int success;

		glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);

		if (!success) {
			char infoLog[1024];
			glGetProgramInfoLog(shaderProgramID, 1024, NULL, infoLog);

			std::cout << "Error: Could not link shader program. Error message:\n" << infoLog << std::endl;
			glfwTerminate();
			exit(-1);
		}
	}
};