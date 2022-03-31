#ifndef BBOX_H
#define BBOX_H
#include "Vector2D.h"

class BBox {
	private:
		Vector2D pos;
		float w;
		float h;

	public:
		BBox();

		BBox(float x, float y, float w_, float h_);

		BBox(Vector2D pos_, float w_, float h_);

		void setPos(Vector2D new_pos);

		void setPos(float x, float y);

		float getHeight();

		float getWidth();

		bool isColliding(BBox b);
};
#endif
