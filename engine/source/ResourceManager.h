#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <filesystem>
#include <glad/glad.h>
#include "Texture.h"
#include "Shader.h"
#include "Model.h"


class ResourceManager {
	public:
		static std::map<std::string, Shader> shaders;
		static std::map<std::string, Texture2D> textures;
		static std::map<std::string, Model> models;
		static Shader loadShader(const char *vShaderPath, const char *fShaderPath, const char *gShaderPath, std::string name);
		static Shader getShader(std::string name);
		static Texture2D loadTexture(const char* path, bool alpha, std::string name);
		static Texture2D getTexture(std::string name);
		static Model loadModel(const char* path, std::string name);
		static Model getModel(std::string name);
		static void clear();
	private:
		ResourceManager() { }
		static Shader loadShaderFromFile(const char* vShaderPath, const char* fShaderPath, const char* gShaderPath = nullptr);
		static Texture2D loadTextureFromFile(const char* path, bool alpha);
		static Model loadModelFromFile(const char *path);
};

#endif // !RESOURCE_MANAGER_H
