#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include <vector>
#include <memory>
#include "Node.h"


class Scene {
	public:
		Node root;
		Scene() = default;
		Scene(Node& _root) : root(_root) { }
		void update(float dt) { root._update(dt); }
		void render() { root._render(); }
};


#endif // !SCENE_GRAPH_H
