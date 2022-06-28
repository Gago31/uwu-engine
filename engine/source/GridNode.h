#pragma once

#include "Node3D.h"
#include "Grid.h"
#include "Camera.h"
#include "GameController.h"
#include "Renderer.h"


class GridNode : public Node3D {
	public:
		Grid grid;
		std::vector<std::vector<Node*>> children;
		std::set<std::pair<int, int>> visible_children;
		GridNode() = default;
		GridNode(Grid& _grid) {
			grid = _grid;
			children.resize(grid.height());
			for (int j = 0; j < grid.height(); j++) {
				for (int i = 0; i < grid.width(); i++) {
					children[j].push_back(new Node());
					visible_children.insert({ i, j});
				}
			}
		}
		//void addChild(Node *child) override { return; }
		void addChild(Node *child, int x, int y) {
			children[y][x]->addChild(child);
		};
		void _render(glm::mat4 _transform) override {
			for (auto& [x, y] : visible_children) {
				children[y][x]->_render(_transform);
			}
		}
		virtual void _turnStart() {
			turnStart();
			for (std::vector<Node*> row : children) {
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
					if (children[next_y][next_x]) visible_children.insert({ next_x, next_y });
				}
			} else {
				if (next_y - pos_y != dir_y) {
					if (children[next_y][next_x]) visible_children.insert({ next_x, next_y });
				}
			}

			if (dir.x < 0) {
				for (int i = 0; i < far; i++) {
					for (int j = -(i + 1); j < i + 2; j++) {
						if (pos_x - i < 0) break;
						if (0 <= pos_y + j && pos_y + j < grid.height()) {
							if (children[pos_y + j][pos_x - i]) visible_children.insert({ pos_x - i, pos_y + j });
						}
					}
				}
			} else if (dir.x > 0) {
				for (int i = 0; i < far; i++) {
					for (int j = -(i + 1); j < i + 2; j++) {
						if (pos_x + i >= grid.width()) break;
						if (0 <= pos_y + j && pos_y + j < grid.height()) {
							if (children[pos_y + j][pos_x + i]) visible_children.insert({ pos_x + i, pos_y + j });
						}
					}
				}
			}
			if (dir.y > 0) {
				for (int j = 0; j < far; j++) {
					for (int i = -(j + 1); i < j + 2; i++) {
						if (pos_y + j >= grid.height()) break;
						if (0 <= pos_x + i && pos_x + i < grid.width()) {
							if (children[pos_y + j][pos_x + i]) visible_children.insert({ pos_x + i, pos_y + j });
						}
					}
				}
			} else if (dir.y < 0) {
				for (int j = 0; j < far; j++) {
					for (int i = -(j + 1); i < j + 2; i++) {
						if (pos_y - j < 0) break;
						if (0 <= pos_x + i && pos_x + i < grid.width()) {
							if (children[pos_y - j][pos_x + i]) visible_children.insert({ pos_x + i, pos_y - j });
						}
					}
				}
			}
		}
		virtual void _turnEnd() {
			turnEnd();
			for (std::vector<Node*> row : children) {
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
							if (children[pos_y + j][pos_x - i]) visible_children.insert({ pos_x - i, pos_y + j });
						}
					}
				}
			} else if (dir.x > 0) {
				for (int i = 0; i < far; i++) {
					for (int j = -(i + 1); j < i + 2; j++) {
						if (pos_x + i >= grid.width()) continue;
						if (0 <= pos_y + j && pos_y + j < grid.height()) {
							if (children[pos_y + j][pos_x + i]) visible_children.insert({ pos_x + i, pos_y + j });
						}
					}
				}
			} else if (dir.y > 0) {
				for (int j = 0; j < far; j++) {
					for (int i = -(j + 1); i < j + 2; i++) {
						if (pos_y + j >= grid.height()) break;
						if (0 <= pos_x + i && pos_x + i < grid.width()) {
							if (children[pos_y + j][pos_x + i]) visible_children.insert({ pos_x + i, pos_y + j });
						}
					}
				}
			} else if (dir.y < 0) {
				for (int j = 0; j < far; j++) {
					for (int i = -(j + 1); i < j + 2; i++) {
						if (pos_y - j < 0) continue;
						if (0 <= pos_x + i && pos_x + i < grid.width()) {
							if (children[pos_y - j][pos_x + i]) visible_children.insert({ pos_x + i, pos_y - j });
						}
					}
				}
			}
		}
};