#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <memory>

class Texture2D {
	public:
		unsigned int ID;
		unsigned int width, height;
		unsigned int InternalFormat;
		unsigned int ImageFormat;
		unsigned int wrap_s;
		unsigned int wrap_t;
		unsigned int filter_min;
		unsigned int filter_max;

		Texture2D();
		void Generate(unsigned int w, unsigned int h, unsigned char *data);
		void Bind() const;
};

using TexturePtr = std::shared_ptr<Texture2D>;

#endif // !TEXTURE_H

