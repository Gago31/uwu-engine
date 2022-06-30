#include <iostream>
#include "Texture.h"


Texture2D::Texture2D() : width(0), height(0), InternalFormat(GL_RGB), ImageFormat(GL_RGB), wrap_s(GL_REPEAT), wrap_t(GL_REPEAT), filter_min(GL_LINEAR), filter_max(GL_LINEAR) {
	glGenTextures(1, &this->ID);
}

void Texture2D::Generate(unsigned int w, unsigned int h, unsigned char* data, bool model_texture) {
	this->width = w;
	this->height = h;
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->InternalFormat, w, h, 0, this->ImageFormat, GL_UNSIGNED_BYTE, data);
	if (model_texture) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filter_max);
	if (!model_texture) {
		glBindBuffer(GL_TEXTURE_2D, 0);
	}
}

void Texture2D::Bind() const {
	glBindTexture(GL_TEXTURE_2D, this->ID);
}