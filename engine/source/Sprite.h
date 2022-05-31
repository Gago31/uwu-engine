#ifndef SPRITE_H
#define SPRITE_H

#include <memory>
#include "Texture.h"
#include "Transform.h"


class Sprite {
	public:
		TexturePtr texture;
		Transform2D transform;
		Sprite() = default;
		Sprite(TexturePtr _texture);
		Sprite(TexturePtr _texture, Transform2D _transform);
};

using SpritePtr = std::shared_ptr<Sprite>;

#endif // !SPRITE_H
