#ifndef ACTOR_H
#define ACTOR_H
#include "Vector2D.h"
#include "BBox.h"

class Actor {
	private:
		BBox bbox;
		Vector2D pos;
	public:
		Vector2D speed;

		Actor(float x, float y, float w, float h, Vector2D speed);
		
		Actor(Vector2D pos_, float w, float h, Vector2D speed);
		
		Actor(Vector2D pos_, BBox bbox_, Vector2D speed);

		void update(float delta);

		void move(float delta);

		bool isColliding(Actor other);

		Vector2D& getPos();

		BBox& getBBox();
};
#endif
