#pragma once

#include "Node3D.h"
#include "Grid.h"


class GridNode : public Node3D {
	public:
		Grid grid;
		GridNode() = default;
		void render() override {

		}
};