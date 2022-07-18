#ifndef GRID_H
#define GRID_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


class Grid {
	private:
		int w, h;
		std::vector<int> grid;

	public:
		Grid();

		Grid(int _w, int _h);
	
		Grid(int _w, int _h, std::vector<int> _grid);

		void resize(int new_width, int new_height);

		int width() const;

		int height() const;

		int coord(int x, int y) const;

		int coord(glm::vec2 pos) const;

		void set_coord(int x, int y, int value);

		void print();
};

void from_json(const json& j, Grid& g);
void to_json(json& j, const Grid& g);

using GridPtr = std::shared_ptr<Grid>;

#endif // !GRID_H