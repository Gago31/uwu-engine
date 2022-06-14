#include "Transform.h"


Transform::Transform(glm::vec3 _position, glm::mat4 _rotation, glm::vec3 _scale) {
	position = _position;
	rotation = _rotation;
	size = _scale;
	matrix = glm::mat4(1.0f);
}

//glm::mat4 Transform::getTransform() {
//	glm::mat4 model = glm::mat4(1.0);
//	model = glm::translate(model, position);
//	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z));
//	model = model * rotation;
//	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
//	model = glm::scale(model, size);
//
//	return model;
//}

void Transform::setMatrix(glm::mat4 newMatrix) {
	matrix = newMatrix;
}

glm::mat4 Transform::getTransform() {
	return matrix;
}

//glm::vec3 Transform::translate(glm::vec3 translation) {
//	position += translation;
//	return position;
//}

glm::vec3 Transform::translate(glm::vec3 translation) {
	matrix = glm::translate(matrix, translation);
	position += translation;
	return position;
}

glm::vec3 Transform::translate(float x, float y, float z) {
	position.x += x;
	position.y += y;
	position.z += z;
	matrix = glm::translate(matrix, { x, y, z });
	return position;
}

//glm::mat4 Transform::rotateX(float t) {
//	rotation = glm::rotate(rotation, t, { 1.0f, 0.0f, 0.0f });
//	return rotation;
//}

glm::mat4 Transform::rotateX(float t) {
	matrix = glm::rotate(matrix, t, { 1.0f, 0.0f, 0.0f });
	return matrix;
}

glm::mat4 Transform::rotateY(float t) {
	matrix = glm::rotate(matrix, t, { 0.0f, 1.0f, 0.0f });
	return matrix;
}

glm::mat4 Transform::rotateZ(float t) {
	matrix = glm::rotate(matrix, t, { 0.0f, 0.0f, 1.0f });
	return matrix;
}

glm::vec3 Transform::scale(float value) {
	matrix = glm::scale(matrix, { value, value, value });
	size *= value;
	return size;
	/*size *= value;
	return size;*/
}

glm::vec3 Transform::scale(float x, float y, float z) {
	size.x *= x;
	size.y *= y;
	size.z *= z;
	matrix = glm::scale(matrix, { x, y, z });
	return size;
}

glm::vec3 Transform::scale(glm::vec3 value) {
	size.x *= value.x;
	size.y *= value.y;
	size.z *= value.z;
	matrix = glm::scale(matrix, { value.x, value.y, value.z });
	return size;
}

void Transform::resetPosition() {
	matrix = glm::translate(matrix, -position);
	position = { 0.0f, 0.0f, 0.0f };
}

void Transform::resetRotation() {
	rotation = glm::mat4(1.0f);
}

void Transform::resetScale() {
	size = { 1.0f,1.0f,1.0f };
}


Transform2D::Transform2D(glm::vec2 _position, float _rotation, glm::vec2 _scale) {
	position = _position;
	rotation = _rotation;
	size = _scale;
}

glm::mat4 Transform2D::getTransform() {
	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
	model = glm::scale(model, glm::vec3(size, 1.0f));
	return model;
}

glm::vec2 Transform2D::translate(glm::vec2 translation) {
	position += translation;
	return position;
}

glm::vec2 Transform2D::translate(float x, float y) {
	position.x += x;
	position.y += y;
	return position;
}

float Transform2D::rotate(float t) {
	rotation += t;
	return rotation;
}
glm::vec2 Transform2D::scale(float value) {
	size *= value;
	return size;
}

glm::vec2 Transform2D::scale(float x, float y) {
	size.x *= x;
	size.y *= y;
	return size;
}

glm::vec2 Transform2D::scale(glm::vec2 value) {
	size.x *= value.x;
	size.y *= value.y;
	return size;
}

void Transform2D::resetPosition() {
	position = { 0.0f,0.0f };
}

void Transform2D::resetRotation() {
	rotation = 0.0f;
}

void Transform2D::resetScale() {
	size = { 1.0f,1.0f };
}
