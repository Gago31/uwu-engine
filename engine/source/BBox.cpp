
#include "Vector2D.h"
#include "BBox.h"


BBox::BBox() {
	pos = Vector2D();
	w = 1.0f;
	h = 1.0f;
}

BBox::BBox(float x_, float y_, float w_, float h_) {
	pos = { x_, y_ };
	w = w_;
	h = h_;
}

BBox::BBox(Vector2D pos_, float w_, float h_) {
	pos = pos_;
	w = w_;
	h = h_;
}

void BBox::setPos(Vector2D new_pos) {
	pos = new_pos;
}

void BBox::setPos(float x, float y) {
	pos = { x, y };
}

float BBox::getHeight() {
	return h;
}

float BBox::getWidth() {
	return w;
}

bool BBox::isColliding(BBox b) {
	return (pos.x < b.pos.x + b.getWidth() &&
		b.pos.x < pos.x + w &&
		pos.y < b.pos.y + b.getHeight() &&
		b.pos.y < pos.y + h);
}