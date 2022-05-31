#include "ResourceManager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stb_image.h>

std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Texture2D> ResourceManager::textures;
std::map<std::string, Model> ResourceManager::models;


void ResourceManager::clear() {
	for (auto shader : shaders) {
		glDeleteProgram(shader.second.ID);
	}
	for (auto texture : textures) {
		glDeleteTextures(1, &texture.second.ID);
	}
}

// Shaders

Shader ResourceManager::loadShader(const char* vShaderPath, const char* fShaderPath, const char* gShaderPath, std::string name) {
	shaders[name] = loadShaderFromFile(vShaderPath, fShaderPath, gShaderPath);
	return shaders[name];
}

Shader ResourceManager::getShader(std::string name) {
	return shaders[name];
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderPath, const char* fShaderPath, const char* gShaderPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	try {
		std::ifstream vShaderFile(vShaderPath);
		std::ifstream fShaderFile(fShaderPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		if (gShaderPath != nullptr) {
			std::ifstream gShaderFile(gShaderPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	} catch (std::ifstream::failure& e) {
		std::cout << "ERROR - Couldn't read shader" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();

	Shader shader;
	shader.compile(vShaderCode, fShaderCode, gShaderPath != nullptr ? gShaderCode : nullptr);
	return shader;
}


// Textures

Texture2D ResourceManager::loadTexture(const char* path, bool alpha, std::string name) {
	textures[name] = loadTextureFromFile(path, alpha);
	return textures[name];
}

Texture2D ResourceManager::getTexture(std::string name) {
	return textures[name];
}

Texture2D ResourceManager::loadTextureFromFile(const char* path, bool alpha) {
	Texture2D texture;
	if (alpha) {
		texture.InternalFormat = GL_RGBA;
		texture.ImageFormat = GL_RGBA;
	}

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	texture.Generate(width, height, data);
	stbi_image_free(data);
	return texture;
}

// Models

Model ResourceManager::loadModel(const char* path, std::string name) {
	models[name] = loadModelFromFile(path);
	return models[name];
}

Model ResourceManager::getModel(std::string name) {
	return models[name];
}

Model ResourceManager::loadModelFromFile(const char* path) {
	Model model(path);
	return model;
}
