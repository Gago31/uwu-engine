#include <vector>
#include <iostream>
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

void Grid::print() {
	for (int j = 0; j < h; j++) {
		for (int i = 0; i < w; i++) {
			std::cout << coord(i, j) << ' ';
		}
		std::cout << std::endl;
	}
}

void Grid::resize(int new_width, int new_height) {
	w = new_width;
	h = new_height;
	grid.resize(w * h, 0);
}

void from_json(const json& j, Grid& g) {
	int _w = j["w"];
	int _h = j["h"];
	g.resize(_w, _h);
	for (int i = 0; i < _w; i++) {
		for (int k = 0; k < _h; k++) {
			g.set_coord(i, k, j["grid"][i + k * _w]);
		}
	}
}

void to_json(json& j, const Grid& g) {
	j["w"] = g.width();
	j["h"] = g.height();
	std::vector<int> v(g.width() * g.height());
	for (int i = 0; i < g.width(); i++) {
		for (int k = 0; k < g.height(); k++) {
			v[i + k * g.width()] = g.coord(i, k);
		}
	}
	j["grid"] = v;
}