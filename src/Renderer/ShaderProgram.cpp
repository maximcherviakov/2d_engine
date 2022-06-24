#include "ShaderProgram.h"
#include <iostream>

namespace Renderer {
	ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) {
		GLuint vertexShaderID;
		if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID)) {
			std::cerr << "VERTEX SHADER compilation error" << std::endl;
			return;
		}

		GLuint fragmentShaderID;
		if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID)) {
			std::cerr << "FRAGMENT SHADER compilation error" << std::endl;
			glDeleteShader(vertexShaderID);
			return;
		}

		ID = glCreateProgram();
		glAttachShader(ID, vertexShaderID);
		glAttachShader(ID, fragmentShaderID);
		glLinkProgram(ID);

		GLint status;
		glGetProgramiv(ID, GL_LINK_STATUS, &status);
		if (!status) {
			GLchar infoLog[1024];
			glGetProgramInfoLog(ID, 1024, nullptr, infoLog);
			std::cerr << "ERROR::PROGRAM: Link time error:\t" << infoLog << std::endl;
		}
		else {
			compiled = true;
		}

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	ShaderProgram::ShaderProgram(ShaderProgram& shaderProgram) {
		ID = shaderProgram.ID;
		compiled = shaderProgram.compiled;

		shaderProgram.ID = 0;
		shaderProgram.compiled = false;
	}

	bool ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID) {
		shaderID = glCreateShader(shaderType);
		int ID = glCreateShader(shaderType);
		const char* code = source.c_str();
		glShaderSource(shaderID, 1, &code, nullptr);
		glCompileShader(shaderID);

		GLint status;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

		if (!status) {
			GLchar infoLog[1024];
			glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
			std::cerr << "ERROR::SHADER: Compile time error:\t" << infoLog << std::endl;
			return false;
		}

		return true;
	}

	void ShaderProgram::use() {
		glUseProgram(ID);
	}

	ShaderProgram& ShaderProgram::operator = (ShaderProgram& shaderProgram) {
		glDeleteProgram(ID);
		ID = shaderProgram.ID;
		compiled = shaderProgram.compiled;

		shaderProgram.ID = 0;
		shaderProgram.compiled = false;

		return *this;
	}

	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(ID);
	}
}