#include "ResourceManager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stb_image.h>
#include <dr_wav.h>
#include "root_directory.h"

#define OPENALCALL(function)\
    function;\
    {\
        ALenum error = alGetError();\
        assert(error != AL_NO_ERROR, "OpenAL Error");\
    }



std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Texture2D> ResourceManager::textures;
std::map<std::string, Model> ResourceManager::models;
std::map<std::string, Sound> ResourceManager::sounds;


void ResourceManager::clear() {
	for (auto& shader : shaders) {
		glDeleteProgram(shader.second.ID);
	}
	for (auto& texture : textures) {
		glDeleteTextures(1, &texture.second.ID);
	}
	for (auto& sound : sounds) {
		alDeleteBuffers(1, &sound.second.buffer);
	}
	models.clear();
}

// Shaders

Shader ResourceManager::loadShader(const char* vShaderPath, const char* fShaderPath, const char* gShaderPath, std::string name) {
	if (gShaderPath) {
		std::cout << "[ResourceManager] " << "Loading shader " << name << "from vertex path " << vShaderPath << ", fragment path " << fShaderPath << "and geometry path " << gShaderPath << std::endl;
	} else {
		std::cout << "[ResourceManager] " << "Loading shader " << name << "from vertex path " << vShaderPath << " and fragment path " << fShaderPath << std::endl;
	}
	shaders[name] = loadShaderFromFile(vShaderPath, fShaderPath, gShaderPath);
	return shaders[name];
}

Shader ResourceManager::getShader(std::string name) {
	return shaders.at(name);
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderPath, const char* fShaderPath, const char* gShaderPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	try {
		std::ifstream vShaderFile(UWU::getPath(vShaderPath).string().c_str());
		std::ifstream fShaderFile(UWU::getPath(fShaderPath).string().c_str());
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		if (gShaderPath != nullptr) {
			std::ifstream gShaderFile(UWU::getPath(gShaderPath).string().c_str());
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
	shader.vertexPath = vShaderPath;
	shader.fragmentPath = fShaderPath;
	if (gShaderPath) {
		shader.geometryPath = gShaderPath;
	}
	shader.compile(vShaderCode, fShaderCode, gShaderPath != nullptr ? gShaderCode : nullptr);
	return shader;
}


// Textures

Texture2D ResourceManager::loadTexture(const char* path, bool alpha, std::string name, bool modelTexture) {
	if (!textures.count(name)) {
		std::cout << "[ResourceManager] " << "Loading texture " << name << "from path " << path << std::endl;
		textures[name] = loadTextureFromFile(path, alpha, modelTexture);
	}
	return textures[name];
}

Texture2D ResourceManager::getTexture(std::string name) {
	return textures.at(name);
}

Texture2D ResourceManager::loadTextureFromFile(const char* path, bool alpha, bool modelTexture) {
	Texture2D texture;
	texture.texturePath = path;
	if (alpha) {
		texture.InternalFormat = GL_RGBA;
		texture.ImageFormat = GL_RGBA;
	}

	int width, height, nrChannels;
	unsigned char* data = stbi_load(UWU::getPath(path).string().c_str(), &width, &height, &nrChannels, 0);
	if (modelTexture) {
		if (nrChannels == 1) {
			texture.ImageFormat = GL_RED;
		} else if (nrChannels == 3) {
			texture.ImageFormat = GL_RGB;
		} else if (nrChannels == 4) {
			texture.ImageFormat = GL_RGBA;
		}
		texture.filter_min = GL_LINEAR_MIPMAP_LINEAR;
	}
	texture.Generate(width, height, data, modelTexture);
	stbi_image_free(data);
	return texture;
}

// Models

Model ResourceManager::loadModel(const char* path, std::string name) {
	if (!models.count(name)) {
		std::cout << "[ResourceManager] " << "Loading model " << name << "from path " << path << std::endl;
		models[name] = loadModelFromFile(path);
	}
	return models[name];
}

Model ResourceManager::getModel(std::string name) {
	return models.at(name);
}

Model ResourceManager::loadModelFromFile(const char* path) {
	Model model(UWU::getPath(path).string().c_str());
	model.modelPath = path;
	return model;
}

json ResourceManager::serialize() {
	json j;
	for (std::pair<std::string, Shader> p : shaders) {
		j["shaders"][p.first] = p.second;
	}
	for (std::pair<std::string, Texture2D> p : textures) {
		if (p.second.isModelTexture) continue;
		j["textures"][p.first] = p.second;
	}
	for (std::pair<std::string, Model> p : models) {
		j["models"][p.first] = p.second;
	}
	for (std::pair<std::string, Sound> p : sounds) {
		j["sounds"][p.first] = { { "path", p.second.path }, { "type", p.second.fileType } };
	}
	return j;
}

void ResourceManager::deserialize(json& j) {
	//std::cout << j.dump(2) << std::endl;
	std::map<std::string, json> jshaders = j["shaders"];
	for (auto& [key, value] : jshaders) {
		std::string vertexPath = value["vertexPath"];
		std::string fragmentPath = value["fragmentPath"];
		std::string geometryPath = value["geometryPath"];
		loadShader(vertexPath.c_str(), fragmentPath.c_str(), geometryPath.empty() ? nullptr : geometryPath.c_str(), key);
	}
	std::map<std::string, json> jtexture = j["textures"];
	for (auto& [key, value] : jtexture) {
		std::string path = value["path"];
		loadTexture(path.c_str(), true, key);
	}
	std::map<std::string, json> jmodels = j["models"];
	for (auto& [key, value] : jmodels) {
		std::string path = value["path"];
		loadModel(path.c_str(), key);
	}
	std::map<std::string, json> jsounds = j["sounds"];
	for (auto& [key, value] : jsounds) {
		std::string ftype = value["type"];
		std::string path = value["path"];
		if (ftype == "WAV") {
			loadSoundWAV(path.c_str(), key);
		} else if (ftype == "OGG") {
			//loadSoundOGG(path.c_str(), key);
		}
	}
}

void ResourceManager::loadSoundWAV(const char* soundPath, std::string name){
	std::cout << "[ResourceManager] " << "Loading wav file " << name << "from path " << soundPath << std::endl;
	ALuint buffer = loadWAVFromFile(UWU::getPath(soundPath).string().c_str());
	sounds[name] = {
		buffer,
		"WAV",
		soundPath
	};
}

ALuint ResourceManager::getSound(std::string name) {
	return sounds.at(name).buffer;
}

//void ResourceManager::loadSoundOGG(const char* soundPath, std::string name){
//
//}

void ResourceManager::playSound(std::string name){

}

ALuint ResourceManager::loadWAVFromFile(const char* path){
	ALuint buffer;
	OPENALCALL(alGenBuffers((ALuint)1, &buffer));
	if (!load_wav_file(path, buffer)) return -1;
	//if (!load_wav_file("/assets/music/moths_bgm.wav", buffer)) return -1;

	std::cout << "[ResourceManager] " << "Loaded wav file succesfully" << std::endl;
	return buffer;
}

//ALuint ResourceManager::loadOGGFromFile(const char* path) {
//
//}

bool ResourceManager::load_wav_file(const char* audiofile, ALuint bufferId) {
	struct WavData {
		unsigned int channels = 0;
		unsigned int sampleRate = 0;
		drwav_uint64 totalPCMFrameCount = 0;
		std::vector<uint16_t> pcmData;
		drwav_uint64 getTotalSamples() const { return totalPCMFrameCount * channels; }
	};

	WavData audioData;
	drwav_int16* sampleData = drwav_open_file_and_read_pcm_frames_s16(
		audiofile,
		&audioData.channels,
		&audioData.sampleRate,
		&audioData.totalPCMFrameCount,
		nullptr
	);

	if (!sampleData) {
		std::cerr << "Audio Clip Error: Failed to load file " << audiofile << std::endl;
		drwav_free(sampleData, nullptr);
		return false;
	} else if (audioData.getTotalSamples() > drwav_uint64(std::numeric_limits<size_t>::max())) {
		std::cerr << "Audio Clip Error: File " << audiofile << " is too big to be loaded." << std::endl;
		drwav_free(sampleData, nullptr);
		return false;
	}

	audioData.pcmData.resize(size_t(audioData.getTotalSamples()));

	std::memcpy(audioData.pcmData.data(), sampleData, audioData.pcmData.size() * 2);
	drwav_free(sampleData, nullptr);

	assert(bufferId != 0);

	OPENALCALL(alBufferData(
		bufferId,
		audioData.channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
		audioData.pcmData.data(),
		audioData.pcmData.size() * 2,
		audioData.sampleRate
	));
}