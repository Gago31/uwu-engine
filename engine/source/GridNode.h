#pragma once

#include "Node.h"
#include "Grid.h"


class GridNode : public Node3D {
	public:
		Grid grid;
		GridNode() = default;
		void render() override {

		}
};