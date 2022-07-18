#include "Sprite.h"
#include "ResourceManager.h"

Sprite::Sprite(std::string texture_name) {
	textureName = texture_name;
	texture = std::make_shared<Texture2D>(ResourceManager::getTexture(texture_name));
	transform = Transform2D();
}

Sprite::Sprite(std::string texture_name, Transform2D _transform) {
	textureName = texture_name;
	texture = std::make_shared<Texture2D>(ResourceManager::getTexture(texture_name));
	transform = _transform;
}

void from_json(const json& j, Sprite& sprite) {
	sprite.textureName = j["textureName"];
	sprite.texture = std::make_shared<Texture2D>(ResourceManager::getTexture(sprite.textureName));
	sprite.transform = j["transform"];
}

void to_json(json& j, const Sprite& sprite) {
	j["textureName"] = sprite.textureName;
	j["transform"] = sprite.transform;
}
