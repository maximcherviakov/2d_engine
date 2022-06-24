#pragma once

#include <string>
#include <glad/glad.h>

namespace Renderer {
	class ShaderProgram {
	public:
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		ShaderProgram(ShaderProgram& );
		ShaderProgram() = delete;
		~ShaderProgram();

		ShaderProgram& operator = (ShaderProgram& shaderProgram);

		bool isCompiled() {
			return compiled;
		}

		void use();

	private:
		bool compiled = false;
		GLuint ID = 0;

		bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);
	};
}