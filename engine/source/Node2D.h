#pragma once

#include "Node.h"
#include "Shader.h"
#include "Transform.h"
#include "Sprite.h"


class Node2D : public Node {
	public:
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
};

class VisualNode2D : public Node2D {
	public:
		std::string shaderName;
		ShaderPtr shader;
		SpritePtr sprite;
		glm::vec3 color = { 1.0f, 1.0f, 1.0f };
		VisualNode2D() = default;
};