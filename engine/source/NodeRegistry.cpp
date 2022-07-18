#include "Node2D.h"
#include "Node3D.h"
#include "SpriteNode.h"
#include "GridNode.h"
#include "MeshNode.h"
#include "TextBox.h"
#include "Enemy.h"
#include "Player.h"
#include "SoundEmitter.h"
#include "SoundEmitter2D.h"
#include "SoundEmitter3D.h"
#include "NodeRegistry.h"


void NodeRegistry::initialize() {
	REGISTER_NODE("Node", Node);
	REGISTER_NODE("Node2D", Node2D);
	REGISTER_NODE("Node3D", Node3D);
	REGISTER_NODE("SpriteNode", SpriteNode);
	REGISTER_NODE("GridNode", GridNode);
	REGISTER_NODE("MeshNode", MeshNode);
	REGISTER_NODE("TextBox", TextBox);
	REGISTER_NODE("Enemy", Enemy);
	REGISTER_NODE("Player", Player);
	REGISTER_NODE("SoundEmitter", SoundEmitter);
	REGISTER_NODE("SoundEmitter2D", SoundEmitter2D);
	REGISTER_NODE("SoundEmitter3D", SoundEmitter3D);
}

template<typename T>
void NodeRegistry::registerNode(std::string name) {
	REGISTER_NODE(name, T);
}