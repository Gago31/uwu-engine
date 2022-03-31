#ifndef VECTOR2D_H
#define VECTOR2D_H
#include <ostream>


class Vector2D {
	public:
		float x, y;

		Vector2D();

		Vector2D(float x_, float y_);

		Vector2D operator+(const Vector2D& b);

		Vector2D operator-(const Vector2D& b);

		Vector2D& operator+=(const Vector2D& b);
		
		Vector2D& operator-=(const Vector2D& b);

		float operator*(const Vector2D& b);

		Vector2D operator*(const float a);

		friend std::ostream& operator<<(std::ostream& os, const Vector2D& v);

		float distance();
};
#endif
