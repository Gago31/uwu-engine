#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <glm/glm.hpp>


class Node {
	public:
		Node *parent = nullptr;
		std::vector<Node*> children;
		std::string name;
		bool visible = true;
		Node() {
			name = "Node";
		};
		Node(std::string node_name) {
			name = node_name;
		}
		virtual void update(float dt) { }
		virtual void render(glm::mat4 _transform = glm::mat4(1.0)) { }
		/*void addChild(Node& child) {
			children.push_back(std::make_shared<Node>(child));
		}*/
		virtual Node* findChild(std::string child_name) {
			for (Node* child : children) {
				if (child->name == child_name) return child;
			}
			return nullptr;
		}
		virtual Node* getChild(int index) {
			if (children.size() >= index) return nullptr;
			return children[index];
		}
		virtual void addChild(Node *child) final {
			//std::cout << "child added";
			children.push_back(child);
			child->parent = this;
		}
		virtual void remove() final {
			for (int i = 0; i < parent->children.size(); i++) {
				if (parent->children[i] == this) {
					parent->children.erase(parent->children.begin() + i);
				}
			}
			clearChildren();
			delete this;
		}
		virtual void removeChild(std::string child_name) {
			for (int i = 0; i < parent->children.size(); i++) {
				if (parent->children[i]->name == child_name) {
					parent->children.erase(parent->children.begin() + i);
				}
			}
		}
		virtual void clearChildren() final {
			for (Node* child : children) {
				if (child == nullptr) continue;
				child->clearChildren();
				delete child;
			}
			children.clear();
		}
		virtual void _update(float dt) final {
			update(dt);
			int count = 0;
			for (Node *child : children) {
				//std::shared_ptr<Node> child = *childPtr;
				if (child == nullptr) continue;
				child->_update(dt);
				count++;
			}
			//std::cout << count << std::endl;
		}
		virtual void _render(glm::mat4 _transform = glm::mat4(1.0)) {
			if (!visible) return;
			//render();
			for (Node *child : children) {
				//std::shared_ptr<Node> child = *childPtr;
				child->_render();
			}
		}
		virtual void _turnStart() {
			turnStart();
			for (Node* child : children) {
				child->_turnStart();
			}
		}
		virtual void turnStart() { }
		virtual void _turnEnd() {
			turnEnd();
			for (Node* child : children) {
				child->_turnEnd();
			}
		}
		virtual void turnEnd() { }
		void hide() { visible = false; }
		void show() { visible = true; }
};

using NodePtr = std::shared_ptr<Node>;