#include "SpriteNode.h"


SpriteNode::SpriteNode(Sprite& s, std::string shader_name) {
	sprite = std::make_shared<Sprite>(s);
	name = "Sprite";
	shaderName = shader_name;
	shader = std::make_shared<Shader>(ResourceManager::getShader(shader_name));
}

SpriteNode::SpriteNode(std::string node_name, Sprite& s, std::string shader_name) {
	sprite = std::make_shared<Sprite>(s);
	name = node_name;
	shaderName = shader_name;
	shader = std::make_shared<Shader>(ResourceManager::getShader(shader_name));
}

void SpriteNode::_render(glm::mat4 _transform) {
	//std::cout << "a" << std::endl;
	if (!visible) return;
	Renderer2D::enqueue(this, _transform);
	for (Node* child : children) {
		child->_render(_transform * sprite->transform.getTransform());
	}
}

void from_json(const json& j, SpriteNode& node) {
	Sprite s = j["sprite"];
	node.sprite = std::make_shared<Sprite>(s);
	node.name = j["name"];
	node.shaderName = j["shaderName"];
	node.shader = std::make_shared<Shader>(ResourceManager::getShader(node.shaderName));
}

void to_json(json& j, const SpriteNode& node) {
	j["sprite"] = *node.sprite.get();
	j["name"] = node.name;
	j["shaderName"] = node.shaderName;
}
