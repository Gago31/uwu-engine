#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include "Mesh.h"
#include "Shader.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;


unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model {
	public:
		std::string modelPath = "";
		std::vector<Texture> textures_loaded;
		std::vector<Mesh> meshes;
		std::string directory;
		bool gammaCorrection;

		Model() = default;

		Model(const std::filesystem::path &path, bool gamma=false) : gammaCorrection(gamma) {
			loadModel(path.string());
		}
		void Draw(ShaderPtr shader);
	private:
		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

void to_json(json& j, const Model& model);
void from_json(const json& j, Model& model);

using ModelPtr = std::shared_ptr<Model>;

#endif // !MODEL_H

