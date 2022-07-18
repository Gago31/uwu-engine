#include "Node.h"


Node::Node() {
	name = "Node";
}

Node::Node(std::string node_name) {
	name = node_name;
}

Node::~Node() {
	for (Node* child : children) {
		delete child;
	}
}

Node* Node::findChild(std::string child_name) {
	for (Node* child : children) {
		if (child == nullptr) continue;
		if (child->name == child_name) return child;
	}
	return nullptr;
}

Node* Node::getChild(int index) {
	if (children.size() >= index) return nullptr;
	return children[index];
}

void Node::addChild(Node* child) {
	//std::cout << "child added";
	children.push_back(child);
	child->parent = this;
}

void Node::remove() {
	deleted = true;
}

void Node::removeChild(std::string child_name) {
	for (int i = 0; i < parent->children.size(); i++) {
		if (parent->children[i]->name == child_name) {
			parent->children.erase(parent->children.begin() + i);
		}
	}
}

void Node::clearChildren() {
	for (Node* child : children) {
		child->clearChildren();
		if (child->deleted) {
			delete child;
		}
		/*if (child == nullptr) continue;
		delete child;*/
	}
	//children.clear();
}

void Node::_update(float dt) {
	update(dt);
	int count = 0;
	for (Node* child : children) {
		//std::shared_ptr<Node> child = *childPtr;
		if (child == nullptr) continue;
		child->_update(dt);
		count++;
	}
	//std::cout << count << std::endl;
}

void Node::_endUpdate() {
	for (int i = 0; i < children.size(); i++) {
		if (children[i] == nullptr) continue;
		if (children[i]->deleted) {
			delete children[i];
			children[i] = nullptr;
		} else {
			//clearChildren();
			children[i]->_endUpdate();
		}
	}
	//for (Node* child : children) {
	//	//child->deleted = deleted;
	//}
}

void Node::_render(glm::mat4 _transform) {
	if (!visible) return;
	//render();
	for (Node* child : children) {
		if (child == nullptr) continue;
		//std::shared_ptr<Node> child = *childPtr;
		child->_render();
	}
}

void Node::_turnStart() {
	turnStart();
	for (Node* child : children) {
		if (child == nullptr) continue;
		child->_turnStart();
	}
}

void Node::_turnEnd() {
	turnEnd();
	for (Node* child : children) {
		if (child == nullptr) continue;
		child->_turnEnd();
	}
}

void Node::hide() { visible = false; }

void Node::show() { visible = true; }

json Node::serialize() {
	json j;
	j["id"] = id;
	j["name"] = name;
	std::vector<int> children_ids;
	for (Node* child : children) {
		children_ids.push_back(child->id);
	}
	j["children"] = children_ids;
	j["className"] = getClassName();
	j["visible"] = visible;
	return j;
}

void Node::deserialize(json& j) {
	id = j["id"];
	name = j["name"];
	visible = j["visible"];
}

std::string Node::getClassName() {
	return className;
}

int Node::assignIds(int start) {
	id = start++;
	for (Node* child : children) {
		start = child->assignIds(start);
	}
	return start;
}

void Node::registerNode(json& j) {
	//j[getClassName()].push_back(serialize());
	j.push_back(serialize());
	for (Node* child : children) {
		child->registerNode(j);
	}
}

void Node::_start() {
	for (Node* child : children) {
		child->_start();
	}
	start();
}