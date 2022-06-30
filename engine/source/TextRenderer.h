#pragma once

#include <iostream>
#include <map>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Shader.h"



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
		TextRenderer() {}
		int init(std::string path, int font_size = 32) {
			int err = FT_Init_FreeType(&library);
			if (err) {
				std::cout << "Error initializing Freetype" << std::endl;
				return -1;
			}
			err = FT_New_Face(library, UWU::getPath(path).string().c_str(), 0, &face);
			if (err) {
				std::cout << "Error loading font" << std::endl;
				return -1;
			}

			FT_Set_Pixel_Sizes(face, 0, font_size);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			for (unsigned char c = 0; c < 255; c++) {
				if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
					std::cout << "Error: failed to load glyph" << std::endl;
					continue;
				}

				unsigned int texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(
					GL_TEXTURE_2D,
					0, GL_RED,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					0, GL_RED,
					GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer
				);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

				Character character = {
					texture,
					glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					face->glyph->advance.x
				};
				characters.insert(std::pair<char, Character>(c, character));
			}

			FT_Done_Face(face);
			FT_Done_FreeType(library);

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		void renderText(ShaderPtr shader, std::string text, float x, float y, float scale, glm::vec3 color) {
			shader->use();
			shader->setVector3f("textColor", color.x, color.y, color.z);
			glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f);
			shader->setMatrix4f("projection", projection);
			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(VAO);

			std::string::const_iterator c;

			for (c = text.begin(); c != text.end(); c++) {
				Character ch = characters[*c];

				float xpos = x + ch.bearing.x * scale;
				float ypos = y - (ch.size.y - ch.bearing.y) * scale;

				float w = ch.size.x * scale;
				float h = ch.size.y * scale;

				float vertices[6][4] = {
					{ xpos,     ypos + h, 0.0f, 0.0f },
					{ xpos,     ypos,     0.0f, 1.0f },
					{ xpos + w, ypos,     1.0f, 1.0f },
					{ xpos,     ypos + h, 0.0f, 0.0f },
					{ xpos + w, ypos,     1.0f, 1.0f },
					{ xpos + w, ypos + h, 1.0f, 0.0f }
				};

				glBindTexture(GL_TEXTURE_2D, ch.textureId);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				x += (ch.advance >> 6) * scale;
			}
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
};