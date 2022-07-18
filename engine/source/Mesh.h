#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
		~Mesh();
		void Draw(ShaderPtr shader);
	private:
		unsigned int VAO, VBO, EBO;
		void setupMesh();
};

//void from_json(const json& j, Vertex& v);
//void to_json(json& j, const Vertex& v);
//
//void from_json(const json& j, Texture& t);
//void to_json(json& j, const Texture& t);
//
//void from_json(const json& j, Mesh& m);
//void to_json(json& j, const Mesh& m);

#endif // !MESH_H
