#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <filesystem>
#include <glad/glad.h>
#include "Texture.h"
#include "Shader.h"
#include "Model.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <cassert>
#include "nlohmann/json.hpp"



using json = nlohmann::json;


struct Sound {
	ALuint buffer;
	std::string fileType;
	std::string path;
};

class ResourceManager {
	public:
		static std::map<std::string, Shader> shaders;
		static std::map<std::string, Texture2D> textures;
		static std::map<std::string, Model> models;
		static std::map<std::string, Sound> sounds;
		static Shader loadShader(const char* vShaderPath, const char* fShaderPath, const char* gShaderPath, std::string name);
		static Shader getShader(std::string name);
		static Texture2D loadTexture(const char* path, bool alpha, std::string name, bool modelTexture = false);
		static Texture2D getTexture(std::string name);
		static Model loadModel(const char* path, std::string name);
		static Model getModel(std::string name);
		static void clear();
		static json serialize();
		static void deserialize(json& j);
		static void loadSoundWAV(const char* soundPath, std::string name);
		static ALuint getSound(std::string name);
		//static void loadSoundOGG(const char* soundPath, std::string name);
		static void playSound(std::string name);
	private:
		ResourceManager() { }
		static Shader loadShaderFromFile(const char* vShaderPath, const char* fShaderPath, const char* gShaderPath = nullptr);
		static Texture2D loadTextureFromFile(const char* path, bool alpha, bool modelTexture = false);
		static Model loadModelFromFile(const char *path);
		static ALuint loadWAVFromFile(const char* path);
		//static ALuint loadOGGFromFile(const char *path);
		static bool load_wav_file(const char* audiofile, ALuint bufferId);
};

#endif // !RESOURCE_MANAGER_H
