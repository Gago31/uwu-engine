#pragma once

#include "Node3D.h"
#include "Grid.h"
#include "Camera.h"
#include "GameController.h"
#include "Renderer.h"
#include "MeshNode.h"
#include <nlohmann/json.hpp>
#include <map>
#include "ResourceManager.h"


class GridNode : public Node3D {
	/*private:
		std::map<unsigned int, std::string> resourceMap = {
			{ 0, "greenPlane" },
			{ 1, "greenCube" }
		};*/
	public:
		inline static std::string className = "GridNode";
		Grid grid;
		//bool updateGridChildren = false;
		/*Grid collisionGrid;
		Grid attributeGrid;*/
		std::vector<std::vector<Node*>> gridChildren;
		std::set<std::pair<int, int>> visible_children;
		GridNode() {
			name = "Grid";
		}
		GridNode(Grid& layout_grid) {
			name = "Grid";
			grid = layout_grid;
			gridChildren.resize(grid.height());
			for (int j = 0; j < grid.height(); j++) {
				for (int i = 0; i < grid.width(); i++) {
					gridChildren[j].push_back(new Node());
					visible_children.insert({ i, j });
				}
			}
			build();
			setCollision();
		}
		GridNode(std::string node_name, Grid& layout_grid) {
			name = node_name;
			grid = layout_grid;
			gridChildren.resize(grid.height());
			for (int j = 0; j < grid.height(); j++) {
				for (int i = 0; i < grid.width(); i++) {
					gridChildren[j].push_back(new Node());
					visible_children.insert({ i, j });
				}
			}
		}
		/*GridNode(std::string node_name, Grid& layout_grid, Grid& collision_grid, Grid& attribute_grid) {
			name = node_name;
			grid = layout_grid;
			collisionGrid = collision_grid;
			attributeGrid = collisionGrid;
			gridChildren.resize(grid.height());
			for (int j = 0; j < grid.height(); j++) {
				for (int i = 0; i < grid.width(); i++) {
					gridChildren[j].push_back(new Node());
					visible_children.insert({ i, j});
				}
			}
		}
		GridNode(Grid& layout_grid, Grid& collision_grid, Grid& attribute_grid) : GridNode("Grid", layout_grid, collision_grid, attribute_grid) {}
		GridNode(std::string node_name, Grid& _grid) {
			name = node_name;
			grid = _grid;
			gridChildren.resize(grid.height());
			for (int j = 0; j < grid.height(); j++) {
				for (int i = 0; i < grid.width(); i++) {
					gridChildren[j].push_back(new Node());
					visible_children.insert({ i, j });
				}
			}
		}*/
		void build() {
			gridChildren.clear();
			gridChildren.resize(grid.height());
			for (int j = 0; j < grid.height(); j++) {
				for (int i = 0; i < grid.width(); i++) {
					gridChildren[j].push_back(new Node());
					visible_children.insert({ i, j });
				}
			}
			for (int j = 0; j < grid.height(); j++) {
				for (int i = 0; i < grid.width(); i++) {
					if (grid.coord(i, j) == 1) {
						MeshNode* wall = new MeshNode("greenCube", "modelShaderL");
						wall->transform.translate({ i, -j + grid.height() - 1, 0.5f });
						wall->transform.scale(0.5f);
						addChild(wall, i, -j + grid.height() - 1);
					} else {
						MeshNode* floor = new MeshNode("greenPlane", "modelShaderL");
						floor->transform.translate({ i, -j + grid.height() - 1, 0.0f });
						floor->transform.scale(0.5f);
						floor->transform.rotateX(glm::radians<float>(90));
						addChild(floor, i, -j + grid.height() - 1);
					}
				}
			}
		}
		void setCollision() {
			GameController::currentGrid = std::make_shared<Grid>(grid);
		}
		virtual Node* findChild(std::string child_name) {
			for (std::vector<Node*>& row : gridChildren) {
				for (Node* child : row) {
					if (child->name == child_name) return child;
				}
			}
			return nullptr;
		}
		virtual Node* getChild(int x, int y) {
			if (gridChildren.size() >= y || gridChildren[y].size() >= x) return nullptr;
			return gridChildren[y][x];
		}
		//void addChild(Node *child) override { return; }
		void addChild(Node *child, int x, int y) {
			gridChildren[y][x]->addChild(child);
		};
		void _render(glm::mat4 _transform) override {
			for (auto& [x, y] : visible_children) {
				gridChildren[y][x]->_render(_transform);
			}
		}
		virtual void _turnStart() {
			turnStart();
			for (std::vector<Node*> row : gridChildren) {
				for (Node* child : row) {
					child->_turnStart();
				}
			}
		}
		virtual void turnStart() {
			visible_children.clear();
			int far = (int)GameController::currentCamera->far;
			glm::vec2 pos = GameController::currentCamera->getPos();
			glm::vec2 nextPos = GameController::currentCamera->getNextPos();
			int pos_x = (int)glm::round(pos.x);
			int pos_y = (int)glm::round(pos.y);
			int next_x = (int)glm::round(nextPos.x);
			int next_y = (int)glm::round(nextPos.y);
			glm::vec2 dir = GameController::currentCamera->getDirection();
			int dir_x = (int)glm::round(dir.x);
			int dir_y = (int)glm::round(dir.y);
			/*std::cout << "turn start" << std::endl;
			std::cout << "Cam dir: (" << dir.x << ", " << dir.y << ")" << std::endl;
			std::cout << "Pos: (" << pos_x << ", " << pos_y << ")" << "Next pos: (" << next_x << ", " << next_y << ")" << std::endl;
			std::cout << ((dir_x != 0) ? (next_x - pos_x == dir_x) : (next_y - pos_y == dir_y)) << std::endl;*/

			if (dir_x != 0) {
				if (next_x - pos_x != dir_x) {
					if (gridChildren[next_y][next_x]) visible_children.insert({ next_x, next_y });
				}
			} else {
				if (next_y - pos_y != dir_y) {
					if (gridChildren[next_y][next_x]) visible_children.insert({ next_x, next_y });
				}
			}

			if (dir.x < 0) {
				for (int i = 0; i < far; i++) {
					for (int j = -(i + 1); j < i + 2; j++) {
						if (pos_x - i < 0) break;
						if (0 <= pos_y + j && pos_y + j < grid.height()) {
							if (gridChildren[pos_y + j][pos_x - i]) visible_children.insert({ pos_x - i, pos_y + j });
						}
					}
				}
			} else if (dir.x > 0) {
				for (int i = 0; i < far; i++) {
					for (int j = -(i + 1); j < i + 2; j++) {
						if (pos_x + i >= grid.width()) break;
						if (0 <= pos_y + j && pos_y + j < grid.height()) {
							if (gridChildren[pos_y + j][pos_x + i]) visible_children.insert({ pos_x + i, pos_y + j });
						}
					}
				}
			}
			if (dir.y > 0) {
				for (int j = 0; j < far; j++) {
					for (int i = -(j + 1); i < j + 2; i++) {
						if (pos_y + j >= grid.height()) break;
						if (0 <= pos_x + i && pos_x + i < grid.width()) {
							if (gridChildren[pos_y + j][pos_x + i]) visible_children.insert({ pos_x + i, pos_y + j });
						}
					}
				}
			} else if (dir.y < 0) {
				for (int j = 0; j < far; j++) {
					for (int i = -(j + 1); i < j + 2; i++) {
						if (pos_y - j < 0) break;
						if (0 <= pos_x + i && pos_x + i < grid.width()) {
							if (gridChildren[pos_y - j][pos_x + i]) visible_children.insert({ pos_x + i, pos_y - j });
						}
					}
				}
			}
		}
		virtual void _turnEnd() {
			turnEnd();
			for (std::vector<Node*> row : gridChildren) {
				for (Node* child : row) {
					child->_turnEnd();
				}
			}
		}
		virtual void turnEnd() {
			visible_children.clear();
			int far = (int) GameController::currentCamera->far;
			glm::vec2 pos = GameController::currentCamera->getPos();
			int pos_x = (int) glm::round(pos.x);
			int pos_y = (int) glm::round(pos.y);
			glm::vec2 dir = GameController::currentCamera->getDirection();
			//std::cout << "Cam dir: (" << dir.x << ", " << dir.y << ")" << std::endl;

			if (dir.x < 0) {
				for (int i = 0; i < far; i++) {
					for (int j = -(i + 1); j < i + 2; j++) {
						if (pos_x - i < 0) continue;
						if (0 <= pos_y + j && pos_y + j < grid.height()) {
							if (gridChildren[pos_y + j][pos_x - i]) visible_children.insert({ pos_x - i, pos_y + j });
						}
					}
				}
			} else if (dir.x > 0) {
				for (int i = 0; i < far; i++) {
					for (int j = -(i + 1); j < i + 2; j++) {
						if (pos_x + i >= grid.width()) continue;
						if (0 <= pos_y + j && pos_y + j < grid.height()) {
							if (gridChildren[pos_y + j][pos_x + i]) visible_children.insert({ pos_x + i, pos_y + j });
						}
					}
				}
			} else if (dir.y > 0) {
				for (int j = 0; j < far; j++) {
					for (int i = -(j + 1); i < j + 2; i++) {
						if (pos_y + j >= grid.height()) break;
						if (0 <= pos_x + i && pos_x + i < grid.width()) {
							if (gridChildren[pos_y + j][pos_x + i]) visible_children.insert({ pos_x + i, pos_y + j });
						}
					}
				}
			} else if (dir.y < 0) {
				for (int j = 0; j < far; j++) {
					for (int i = -(j + 1); i < j + 2; i++) {
						if (pos_y - j < 0) continue;
						if (0 <= pos_x + i && pos_x + i < grid.width()) {
							if (gridChildren[pos_y - j][pos_x + i]) visible_children.insert({ pos_x + i, pos_y - j });
						}
					}
				}
			}
		}
		virtual json serialize() override {
			json j = Node3D::serialize();
			j["grid"] = grid;
			//json grid_children = json::array();
			/*for (std::vector<Node*>& row : gridChildren) {
				json children_row;
				for (Node* child : row) {
					children_row.push_back(child->id);
				}
				grid_children.push_back(children_row);
			}
			j["gridChildren"] = grid_children;*/
			return j;
		}
		void deserialize(json& j) override {
			Node3D::deserialize(j);
			grid = j["grid"];
			build();
			setCollision();
		}
		int assignIds(int start) override {
			id = start++;
			/*for (std::vector<Node*>& row : gridChildren) {
				for (Node* child : row) {
					start = child->assignIds(start);
				}
			}*/
			for (Node* child : children) {
				start = child->assignIds(start);
			}
			return start;
		}
		void registerNode(json& j) override {
			j.push_back(serialize());
			/*for (std::vector<Node*>& row : gridChildren) {
				for (Node* child : row) {
					child->registerNode(j);
				}
			}*/
			for (Node* child : children) {
				child->registerNode(j);
			}
		}
		std::string getClassName() override {
			return className;
		}
};