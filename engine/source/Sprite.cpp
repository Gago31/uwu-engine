#include "Sprite.h"

Sprite::Sprite(TexturePtr _texture) {
	this->texture = _texture;
	this->transform = Transform2D();
}

Sprite::Sprite(TexturePtr _texture, Transform2D _transform) {
	this->texture = _texture;
	this->transform = _transform;
}