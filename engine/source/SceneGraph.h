#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include <vector>
#include <memory>
#include "Node.h"



class Scene {
	public:
		Node *root;
		Scene();
		Scene(Node* _root) : root(_root) { }
		void update(float dt);
		void render();
		json serialize();
		void unload();
		void removeRoot();
		void start();
		void save(std::string file_name);
		static Scene load(std::string scene_path);
	private:
		void _load(json& j);
		void assignIds();
};

using ScenePtr = std::shared_ptr<Scene>;

#endif // !SCENE_GRAPH_H
