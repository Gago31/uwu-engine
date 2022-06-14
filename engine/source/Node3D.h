#pragma once

#include "Node.h"
#include "Transform.h"


class Node3D : public Node {
	public:
		Transform transform;
		virtual void _render(glm::mat4 _transform = glm::mat4(1.0)) override {
			render(_transform);
			for (Node* child : children) {
				child->_render(_transform * transform.getTransform());
			}
		}
};