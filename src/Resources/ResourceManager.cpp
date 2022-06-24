#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"

#include <sstream>
#include <fstream>
#include <iostream>

ResourceManager::ResourceManager(const std::string& exePath) {
	size_t found = exePath.find_last_of("/\\");
	path = exePath.substr(0, found);
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath) {
	std::string vertexString = getFileString(vertexPath);
	if (vertexString.empty()) {
		std::cerr << "No vertex shader!" << std::endl;
		return nullptr;
	}

	std::string fragmentString = getFileString(fragmentPath);
	if (fragmentString.empty()) {
		std::cerr << "No fragment shader!" << std::endl;
		return nullptr;
	}

	std::shared_ptr<Renderer::ShaderProgram>& newShader = shaderPrograms.emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexString, fragmentString)).first->second;

	if (!newShader->isCompiled()) {
		std::cerr << "Cannot load shader program\n"
			<< "Vertex: " << vertexPath << "\n"
			<< "Fragment: " << fragmentPath << std::endl;
		return nullptr;
	}

	return newShader;
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShader(const std::string& shaderName) {
	ShaderProgramsMap::const_iterator iterator = shaderPrograms.find(shaderName);
	if (iterator != shaderPrograms.end()) {
		return iterator->second;
	}

	std::cerr << "Cannot find the shader program: " << shaderName << std::endl;

	return nullptr;
}

std::string ResourceManager::getFileString(const std::string& relativeFilePath) {
	std::ifstream f;
	f.open(path + "/" + relativeFilePath, std::ios::in | std::ios::binary);

	if (!f.is_open()) {
		std::cerr << "Failed to open file by path: " << relativeFilePath << std::endl;
		return std::string{};
	}

	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}