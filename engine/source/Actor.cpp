
#include "Vector2D.h"
#include "BBox.h"
#include "Actor.h"


Actor::Actor(float x, float y, float w, float h, Vector2D speed_) {
	pos = { x, y };
	bbox = { x, y, w, h };
	speed = speed_;
}

Actor::Actor(Vector2D pos_, float w, float h, Vector2D speed_) {
	pos = pos_;
	bbox = { pos_, w, h };
	speed = speed_;
}

Actor::Actor(Vector2D pos_, BBox bbox_, Vector2D speed_) {
	pos = pos_;
	bbox = bbox_;
	speed = speed_;
}

void Actor::update(float delta) {
	move(delta);
}

void Actor::move(float delta) {
	pos += speed * delta;
	bbox.setPos(pos);
}

Vector2D& Actor::getPos() {
	return pos;
}

BBox& Actor::getBBox() {
	return bbox;
}

bool Actor::isColliding(Actor other) {
	return bbox.isColliding(other.getBBox());
}
