#pragma once

#include "Node2D.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "Renderer2D.h"


class SpriteNode : public VisualNode2D {
	public:
		//SpritePtr sprite;
		SpriteNode() = default;
		SpriteNode(SpritePtr s, std::string shader_name) {
			sprite = s;
			name = "Sprite";
			shaderName = shader_name;
			shader = std::make_shared<Shader>(ResourceManager::getShader(shader_name));
		}
		SpriteNode(std::string node_name, SpritePtr s, std::string shader_name) {
			sprite = s;
			name = node_name;
			shaderName = shader_name;
			shader = std::make_shared<Shader>(ResourceManager::getShader(shader_name));
		}
		/*void render(glm::mat4 _transform) override {
			shader->setMatrix4f("model", _transform);
		}*/
		void _render(glm::mat4 _transform = glm::mat4(1.0f)) override {
			//std::cout << "a" << std::endl;
			if (!visible) return;
			Renderer2D::enqueue(this, _transform);
			for (Node* child : children) {
				child->_render(_transform * sprite->transform.getTransform());
			}
		}
};
