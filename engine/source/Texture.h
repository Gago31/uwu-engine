#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>
#include <memory>
#include "nlohmann/json.hpp"

using json = nlohmann::json;


class Texture2D {
	public:
		std::string texturePath = "";
		bool isModelTexture = false;
		unsigned int ID;
		unsigned int width, height;
		unsigned int InternalFormat;
		unsigned int ImageFormat;
		unsigned int wrap_s;
		unsigned int wrap_t;
		unsigned int filter_min;
		unsigned int filter_max;

		Texture2D();
		void Generate(unsigned int w, unsigned int h, unsigned char *data, bool model_texture = false);
		void Bind() const;
};

void to_json(json& j, const Texture2D& texture);
void from_json(const json& j, Texture2D& texture);

using TexturePtr = std::shared_ptr<Texture2D>;

#endif // !TEXTURE_H

