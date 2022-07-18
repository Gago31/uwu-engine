#include <fstream>
#include "SceneGraph.h"
#include "GameController.h"
#include "ResourceManager.h"
#include "NodeRegistry.h"


Scene::Scene() {
	root = new Node();
}

void Scene::update(float dt) {
	if (root == nullptr || root->deleted) {
		removeRoot();
		return;
	}
	root->_update(dt);
	root->_endUpdate();
}

void Scene::render() { root->_render(); }

json Scene::serialize() {
	assignIds();
	json j;
	j["resources"] = ResourceManager::serialize();
	std::cout << "Resources serialized" << std::endl;
	root->registerNode(j["nodes"]);
	j["root"] = 0;
	return j;
}

void Scene::save(std::string file_name) {
	std::fstream fs;
	fs.open(file_name, std::ios_base::out);
	fs << serialize().dump(2) << std::endl;
	fs.close();
}

Scene Scene::load(std::string scene_path) {
	std::fstream fs;
	fs.open(scene_path, std::ios_base::in);
	json j;
	fs >> j;
	fs.close();
	Scene scene;
	scene._load(j);
	return scene;
}

void Scene::_load(json& j) {
	ResourceManager::deserialize(j["resources"]);
	std::map<unsigned int, Node*> nodes;
	for (json jnode : j["nodes"]) {
		std::string class_name = jnode["className"];
		std::cout << "className: " << class_name << std::endl;
		Node* node = NodeRegistry::instanceNode(class_name);
		node->deserialize(jnode);
		nodes[node->id] = node;
	}
	for (json jnode : j["nodes"]) {
		std::vector<int> childrenIds = jnode["children"];
		unsigned int nodeId = jnode["id"];
		for (int i : childrenIds) {
			nodes[nodeId]->addChild(nodes.at(i));
		}
	}
	int rootId = j["root"];
	root = nodes[rootId];
}

void Scene::unload() {
	root->deleted = true;
}

void Scene::removeRoot() {
	GameController::currentCamera = nullptr;
	GameController::currentGrid = nullptr;
	delete root;
	root = new Node();
	ResourceManager::clear();
}

void Scene::assignIds() {
	std::cout << "Total nodes: " << root->assignIds(0) << std::endl;
}

void Scene::start() {
	root->_start();
}