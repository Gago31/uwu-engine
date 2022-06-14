#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <glm/glm.hpp>

class Node {
	public:
		std::vector<Node*> children;
		Node() = default;
		virtual void update(float dt) { }
		virtual void render(glm::mat4 _transform = glm::mat4(1.0)) { }
		/*void addChild(Node& child) {
			children.push_back(std::make_shared<Node>(child));
		}*/
		virtual void addChild(Node *child) final {
			children.push_back(child);
		}
		virtual void _update(float dt) final {
			update(dt);
			int count = 0;
			for (Node *child : children) {
				//std::shared_ptr<Node> child = *childPtr;
				child->_update(dt);
				count++;
			}
			//std::cout << count << std::endl;
		}
		virtual void _render(glm::mat4 _transform = glm::mat4(1.0)) {
			render();
			for (Node *child : children) {
				//std::shared_ptr<Node> child = *childPtr;
				child->_render();
			}
		}
};

using NodePtr = std::shared_ptr<Node>;