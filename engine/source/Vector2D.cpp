
#include <cmath>
#include <ostream>
#include "Vector2D.h"


Vector2D::Vector2D() {
	x = 0.0f;
	y = 0.0f;
}

Vector2D::Vector2D(float x_, float y_) {
	x = x_;
	y = y_;
}

Vector2D Vector2D::operator+(const Vector2D& b) {
	return { x + b.x, y + b.y };
}

Vector2D Vector2D::operator-(const Vector2D& b) {
	return { x - b.x, y - b.y };
}

Vector2D& Vector2D::operator+=(const Vector2D& b) {
	this->x += b.x;
	this->y += b.y;
	return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& b) {
	this->x -= b.x;
	this->y -= b.y;
	return *this;
}

float Vector2D::operator*(const Vector2D& b) {
	return x * b.x + y * b.y;
}

Vector2D Vector2D::operator*(float a) {
	return { a * x, a * y };
}

std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
	return os << "(" << v.x << ", " << v.y << ")";
}

float Vector2D::distance() {
	return sqrt(x * x + y * y);
}