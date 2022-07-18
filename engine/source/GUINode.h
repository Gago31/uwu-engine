#pragma once

#include "Node.h"
#include "Transform.h"


class GUINode : public Node {
	inline static std::string className = "GUINode";
	Transform2D transform;
	void draw() {};
};