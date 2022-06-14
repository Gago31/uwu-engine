#include <vector>
#include "Grid.h"

Grid::Grid() {
	w = 10;
	h = 10;
	grid = std::vector<int>(w * h, 0);
}

Grid::Grid(int _w, int _h) {
	w = _w;
	h = _h;
	grid = std::vector<int>(w * h, 0);
}

Grid::Grid(int _w, int _h, std::vector<int> _grid) {
	w = _w;
	h = _h;
	grid = _grid;
}

int Grid::width() const {
	return w;
}

int Grid::height() const {
	return h;
}

int Grid::coord(int x, int y) const {
	if (x < 0 || w < x || y < 0 || h < y) {
		return -1;
	}
	return grid[y * w + x];
}

int Grid::coord(glm::vec2 pos) const {
	if (pos.x < 0 || w < pos.x || pos.y < 0 || h < pos.y) {
		return -1;
	}
	return grid[pos.y * w + pos.x];
}

void Grid::set_coord(int x, int y, int value) {
	if (x < 0 || w < x || y < 0 || h < y) {
		return;
	}
	grid[y * w + x] = value;
}
