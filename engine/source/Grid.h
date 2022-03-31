#ifndef GRID_H
#define GRID_H

#include <vector>


class Grid {
	private:
		int w, h;
		std::vector<int> grid;

	public:
		Grid();

		Grid(int _w, int _h);
	
		Grid(int _w, int _h, std::vector<int> _grid);

		int width() const;

		int height() const;

		int coord(int x, int y) const;

		void set_coord(int x, int y, int value);
};
#endif // !GRID_H