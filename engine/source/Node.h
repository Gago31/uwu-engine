#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


class Node {
	public:
		inline static std::string className = "Node";
		unsigned int id;
		bool deleted = false;
		Node *parent = nullptr;
		std::vector<Node*> children;
		std::string name;
		bool visible = true;
		Node();
		~Node();
		Node(std::string node_name);
		virtual void update(float dt) { }
		virtual void render(glm::mat4 _transform = glm::mat4(1.0)) { }
		virtual Node* findChild(std::string child_name);
		virtual Node* getChild(int index);
		virtual void addChild(Node *child) final;
		virtual void remove() final;
		virtual void removeChild(std::string child_name);
		virtual void clearChildren() final;
		virtual void _update(float dt) final;
		void _endUpdate();
		virtual void _render(glm::mat4 _transform = glm::mat4(1.0));
		virtual void _turnStart();
		virtual void turnStart() { }
		virtual void _turnEnd();
		virtual void turnEnd() { }
		void hide();
		void show();
		virtual json serialize();
		virtual void deserialize(json& j);
		virtual std::string getClassName();
		virtual int assignIds(int start);
		virtual void registerNode(json& j);
		virtual void _start();
		virtual void start() {}
		void deleteNode() {

		}
};


using NodePtr = std::shared_ptr<Node>;