#pragma once

#include "Node.h"
#include "Transform.h"


class Node3D : public Node {
	public:
		Transform transform;
		Node3D() {
			name = "Node3D";
		}
		Node3D(std::string node_name) {
			name = node_name;
		}
		virtual void _render(glm::mat4 _transform = glm::mat4(1.0)) override {
			//render(_transform);
			for (Node* child : children) {
				child->_render(_transform * transform.getTransform());
			}
		}
};

class VisualNode : public Node3D {
	public:
		std::string shaderName;
		ShaderPtr shader;
		VisualNode() = default;
};