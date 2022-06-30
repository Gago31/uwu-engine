#pragma once

#include "Node.h"
#include "Transform.h"


class GUINode : public Node {
	Transform2D transform;
	void draw() {};
};