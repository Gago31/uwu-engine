#pragma once

#include "Node2D.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "Renderer2D.h"


class SpriteNode : public VisualNode2D {
	public:
		inline static std::string className = "SpriteNode";
		SpriteNode() = default;
		SpriteNode(Sprite& s, std::string shader_name);
		SpriteNode(std::string node_name, Sprite& s, std::string shader_name);
		/*void render(glm::mat4 _transform) override {
			shader->setMatrix4f("model", _transform);
		}*/
		void _render(glm::mat4 _transform = glm::mat4(1.0f)) override;
		std::string getClassName() override {
			return className;
		}
};

void from_json(const json& j, SpriteNode& node);
void to_json(json& j, const SpriteNode& node);