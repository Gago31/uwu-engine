#include "Transform.h"


Transform::Transform(glm::vec3 _position, glm::mat4 _rotation, glm::vec3 _scale) {
	this->position = _position;
	this->rotation = _rotation;
	this->size = _scale;
}

glm::mat4 Transform::getTransform() {
	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, this->position);
	model = glm::translate(model, glm::vec3(0.5f * this->size.x, 0.5f * this->size.y, 0.5f * this->size.z));
	model = model * rotation;
	model = glm::translate(model, glm::vec3(-0.5f * this->size.x, -0.5f * this->size.y, 0.0f));
	model = glm::scale(model, this->size);

	return model;
}

glm::vec3 Transform::translate(glm::vec3 translation) {
	this->position += translation;
	return this->position;
}

glm::vec3 Transform::translate(float x, float y, float z) {
	this->position.x += x;
	this->position.y += y;
	this->position.z += z;
	return this->position;
}

glm::mat4 Transform::rotateX(float t) {
	this->rotation = glm::rotate(this->rotation, t, { 1.0f, 0.0f, 0.0f });
	return this->rotation;
}

glm::mat4 Transform::rotateY(float t) {
	this->rotation = glm::rotate(this->rotation, t, { 0.0f, 1.0f, 0.0f });
	return this->rotation;
}

glm::mat4 Transform::rotateZ(float t) {
	this->rotation = glm::rotate(this->rotation, t, { 0.0f, 0.0f, 1.0f });
	return this->rotation;
}

glm::vec3 Transform::scale(float value) {
	this->size *= value;
	return this->size;
}

glm::vec3 Transform::scale(float x, float y, float z) {
	this->size.x *= x;
	this->size.y *= y;
	this->size.z *= z;
	return this->size;
}

glm::vec3 Transform::scale(glm::vec3 value) {
	this->size.x *= value.x;
	this->size.y *= value.y;
	this->size.z *= value.z;
	return this->size;
}

void Transform::resetPosition() {
	this->position = { 0.0f,0.0f,0.0f };
}

void Transform::resetRotation() {
	this->rotation = glm::mat4(1.0f);
}

void Transform::resetScale() {
	this->size = { 1.0f,1.0f,1.0f };
}


Transform2D::Transform2D(glm::vec2 _position, float _rotation, glm::vec2 _scale) {
	this->position = _position;
	this->rotation = _rotation;
	this->size = _scale;
}

glm::mat4 Transform2D::getTransform() {
	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(this->position, 0.0f));
	model = glm::translate(model, glm::vec3(0.5f * this->size.x, 0.5f * this->size.y, 0.0f));
	model = glm::rotate(model, glm::radians(this->rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * this->size.x, -0.5f * this->size.y, 0.0f));
	model = glm::scale(model, glm::vec3(this->size, 1.0f));
	return model;
}

glm::vec2 Transform2D::translate(glm::vec2 translation) {
	this->position += translation;
	return this->position;
}

glm::vec2 Transform2D::translate(float x, float y) {
	this->position.x += x;
	this->position.y += y;
	return this->position;
}

float Transform2D::rotate(float t) {
	this->rotation += t;
	return this->rotation;
}
glm::vec2 Transform2D::scale(float value) {
	this->size *= value;
	return this->size;
}

glm::vec2 Transform2D::scale(float x, float y) {
	this->size.x *= x;
	this->size.y *= y;
	return this->size;
}

glm::vec2 Transform2D::scale(glm::vec2 value) {
	this->size.x *= value.x;
	this->size.y *= value.y;
	return this->size;
}

void Transform2D::resetPosition() {
	this->position = { 0.0f,0.0f };
}

void Transform2D::resetRotation() {
	this->rotation = 0.0f;
}

void Transform2D::resetScale() {
	this->size = { 1.0f,1.0f };
}
