#pragma once

#include "Node.h"
#include "Transform.h"
#include "ResourceManager.h"


class Node3D : public Node {
	public:
		inline static std::string className = "Node3D";
		Transform transform;
		Node3D() {
			name = "Node3D";
		}
		Node3D(std::string node_name) {
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

class VisualNode : public Node3D {
	public:
		inline static std::string className = "VisualNode";
		std::string shaderName;
		ShaderPtr shader;
		VisualNode() = default;
		virtual json serialize() override {
			json j = Node3D::serialize();
			j["shaderName"] = shaderName;
			return j;
		}
		void deserialize(json& j) override {
			Node3D::deserialize(j);
			shaderName = j["shaderName"];
			shader = std::make_shared<Shader>(ResourceManager::getShader(shaderName));
		}
		std::string getClassName() override {
			return className;
		}
};