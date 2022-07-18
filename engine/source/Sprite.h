#ifndef SPRITE_H
#define SPRITE_H

#include <memory>
#include <string>
#include "Texture.h"
#include "Transform.h"
#include "nlohmann/json.hpp"


using json = nlohmann::json;

class Sprite {
	public:
		std::string textureName;
		TexturePtr texture;
		Transform2D transform;
		Sprite() = default;
		Sprite(std::string texture_name);
		Sprite(std::string texture_name, Transform2D _transform);
};

void from_json(const json& j, Sprite& sprite);
void to_json(json& j, const Sprite& sprite);

using SpritePtr = std::shared_ptr<Sprite>;

#endif // !SPRITE_H
