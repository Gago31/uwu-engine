#pragma once

#include "Node.h"
#include "Shader.h"
#include "Transform.h"
#include "Sprite.h"
#include "ResourceManager.h"


class Node2D : public Node {
	public:
		inline static std::string className = "Node2D";
		Transform2D transform;
		Node2D() {
			name = "Node2D";
		}
		Node2D(std::string node_name) {
			name = node_name;
		}
		virtual void _render(glm::mat4 _transform = glm::mat4(1.0)) override {
			if (!visible) return;
			for (Node* child : children) {
				child->_render(_transform * transform.getTransform());
			}
		}
		virtual json serialize() override {
			json j = Node::serialize();
			j["transform"] = transform;
			return j;
		}
		void deserialize(json& j) override {
			Node::deserialize(j);
			transform = j["transform"];
		}
		std::string getClassName() override {
			return className;
		}
};

class VisualNode2D : public Node2D {
	public:
		inline static std::string className = "VisualNode2D";
		std::string shaderName;
		ShaderPtr shader;
		SpritePtr sprite;
		glm::vec3 color = { 1.0f, 1.0f, 1.0f };
		VisualNode2D() = default;
		virtual json serialize() override {
			json j = Node2D::serialize();
			j["shaderName"] = shaderName;
			j["sprite"] = *sprite.get();
			j["color"] = { color.x, color.y, color.z };
			return j;
		}
		void deserialize(json& j) override {
			Node2D::deserialize(j);
			shaderName = j["shaderName"];
			shader = std::make_shared<Shader>(ResourceManager::getShader(shaderName));
			Sprite sprt = j["sprite"];
			sprite = std::make_shared<Sprite>(sprt);
			color = { j["color"][0], j["color"][1], j["color"][2] };
		}
		std::string getClassName() override {
			return className;
		}
};