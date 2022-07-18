#pragma once

#include <iostream>
#include <map>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Shader.h"
#include "root_directory.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;


struct Character {
	unsigned int textureId;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};


class TextRenderer {
	private:
		std::map<char, Character> characters;
		FT_Library library;
		FT_Face face;
		unsigned int VAO;
		unsigned int VBO;
	public:
		int fontSize = 32;
		std::string fontPath;
		TextRenderer() {}
		int init(std::string path, int font_size = 32);
		void renderText(ShaderPtr shader, std::string text, float x, float y, float scale, glm::vec3 color);
};

void from_json(const json& j, TextRenderer& t);
void to_json(json& j, const TextRenderer& t);