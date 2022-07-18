#include "Transform.h"


Transform::Transform(glm::vec3 _position, glm::mat4 _rotation, glm::vec3 _scale) {
	position = _position;
	rotation = _rotation;
	size = _scale;
	matrix = glm::mat4(1.0f);
}

void Transform::setMatrix(glm::mat4 newMatrix) {
	matrix = newMatrix;
}

glm::mat4 Transform::getTransform() {
	return matrix;
}

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

glm::mat4 Transform::rotateX(float t) {
	matrix = glm::rotate(matrix, t, { 1.0f, 0.0f, 0.0f });
	rotation = glm::rotate(rotation, t, { 1.0f, 0.0f, 0.0f });
	return matrix;
}

glm::mat4 Transform::rotateY(float t) {
	matrix = glm::rotate(matrix, t, { 0.0f, 1.0f, 0.0f });
	rotation = glm::rotate(rotation, t, { 0.0f, 1.0f, 0.0f });
	return matrix;
}

glm::mat4 Transform::rotateZ(float t) {
	matrix = glm::rotate(matrix, t, { 0.0f, 0.0f, 1.0f });
	rotation = glm::rotate(rotation, t, { 0.0f, 0.0f, 1.0f });
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

void from_json(const json& j, Transform& transform) {
	transform.position = { j["position"][0], j["position"][1], j["position"][2] };
	transform.rotation[0][0] = j["rotation"][0][0]; transform.rotation[1][0] = j["rotation"][1][0]; transform.rotation[2][0] = j["rotation"][2][0]; transform.rotation[3][0] = j["rotation"][3][0];
	transform.rotation[0][1] = j["rotation"][0][1]; transform.rotation[1][1] = j["rotation"][1][1]; transform.rotation[2][1] = j["rotation"][2][1]; transform.rotation[3][1] = j["rotation"][3][1];
	transform.rotation[0][2] = j["rotation"][0][2]; transform.rotation[1][2] = j["rotation"][1][2]; transform.rotation[2][2] = j["rotation"][2][2]; transform.rotation[3][2] = j["rotation"][3][2];
	transform.rotation[0][3] = j["rotation"][0][3]; transform.rotation[1][3] = j["rotation"][1][3]; transform.rotation[2][3] = j["rotation"][2][3]; transform.rotation[3][3] = j["rotation"][3][3];
	transform.size= { j["size"][0], j["size"][1], j["size"][2] };
	transform.matrix[0][0] = j["matrix"][0][0]; transform.matrix[1][0] = j["matrix"][1][0]; transform.matrix[2][0] = j["matrix"][2][0]; transform.matrix[3][0] = j["matrix"][3][0];
	transform.matrix[1][0] = j["matrix"][0][1]; transform.matrix[1][1] = j["matrix"][1][1]; transform.matrix[2][1] = j["matrix"][2][1]; transform.matrix[3][1] = j["matrix"][3][1];
	transform.matrix[2][0] = j["matrix"][0][2]; transform.matrix[1][2] = j["matrix"][1][2]; transform.matrix[2][2] = j["matrix"][2][2]; transform.matrix[3][2] = j["matrix"][3][2];
	transform.matrix[3][0] = j["matrix"][0][3]; transform.matrix[1][3] = j["matrix"][1][3]; transform.matrix[2][3] = j["matrix"][2][3]; transform.matrix[3][3] = j["matrix"][3][3];
}

void to_json(json& j, const Transform& transform) {
	j["position"] = { transform.position.x, transform.position.y , transform.position.z };
	j["rotation"] = {
		{ transform.rotation[0][0], transform.rotation[0][1], transform.rotation[0][2], transform.rotation[0][3] },
		{ transform.rotation[1][0], transform.rotation[1][1], transform.rotation[1][2], transform.rotation[1][3] },
		{ transform.rotation[2][0], transform.rotation[2][1], transform.rotation[2][2], transform.rotation[2][3] },
		{ transform.rotation[3][0], transform.rotation[3][1], transform.rotation[3][2], transform.rotation[3][3] },
	};
	j["size"] = { transform.size.x, transform.size.y , transform.size.z };
	j["matrix"] = {
		{ transform.matrix[0][0], transform.matrix[0][1], transform.matrix[0][2], transform.matrix[0][3] },
		{ transform.matrix[1][0], transform.matrix[1][1], transform.matrix[1][2], transform.matrix[1][3] },
		{ transform.matrix[2][0], transform.matrix[2][1], transform.matrix[2][2], transform.matrix[2][3] },
		{ transform.matrix[3][0], transform.matrix[3][1], transform.matrix[3][2], transform.matrix[3][3] },
	};
}

//==================== Transform2D =======================

Transform2D::Transform2D(glm::vec2 _position, float _rotation, glm::vec2 _scale) {
	position = _position;
	rotation = _rotation;
	size = _scale;
}

glm::mat4 Transform2D::getTransform() {
	//return matrix;
	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
	model = glm::scale(model, glm::vec3(size, 1.0f));
	return model;
}

glm::vec2 Transform2D::translate(glm::vec2 translation) {
	//matrix = glm::translate(matrix, glm::vec3(translation, 0.0f));
	position += translation;
	return position;
}

glm::vec2 Transform2D::translate(float x, float y) {
	//matrix = glm::translate(matrix, glm::vec3(x, y, 0.0f));
	position.x += x;
	position.y += y;
	return position;
}

float Transform2D::rotate(float t) {
	//matrix = glm::rotate(matrix, t, { 0.0f, 0.0f, 1.0f});
	rotation += t;
	return rotation;
}
glm::vec2 Transform2D::scale(float value) {
	//matrix = glm::scale(matrix, glm::vec3(value, value, 1.0f));
	size *= value;
	return size;
}

glm::vec2 Transform2D::scale(float x, float y) {
	//matrix = glm::scale(matrix, glm::vec3(x, y, 1.0f));
	size.x *= x;
	size.y *= y;
	return size;
}

glm::vec2 Transform2D::scale(glm::vec2 value) {
	//matrix = glm::scale(matrix, glm::vec3(value.x, value.y, 1.0f));
	size.x *= value.x;
	size.y *= value.y;
	return size;
}

void Transform2D::resetPosition() {
	//matrix = glm::translate(matrix, glm::vec3(-position, 0.0f));
	position = { 0.0f, 0.0f };
}

void Transform2D::resetRotation() {
	//matrix = glm::rotate(matrix, -rotation, { 0.0f, 0.0f, 1.0f });
	rotation = 0.0f;
}

void Transform2D::resetScale() {
	//matrix = glm::scale(matrix, glm::vec3(1.0f/size.x, 1.0f/size.y, 1.0f));
	size = { 1.0f, 1.0f };
}

void from_json(const json& j, Transform2D& transform) {
	transform.position = { j["position"][0], j["position"][1] };
	transform.rotation = j["rotation"];
	transform.size = { j["size"][0], j["size"][1] };
	transform.matrix[0][0] = j["matrix"][0][0]; transform.matrix[1][0] = j["matrix"][1][0]; transform.matrix[2][0] = j["matrix"][2][0]; transform.matrix[3][0] = j["matrix"][3][0];
	transform.matrix[0][1] = j["matrix"][0][1]; transform.matrix[1][1] = j["matrix"][1][1]; transform.matrix[2][1] = j["matrix"][2][1]; transform.matrix[3][1] = j["matrix"][3][1];
	transform.matrix[0][2] = j["matrix"][0][2]; transform.matrix[1][2] = j["matrix"][1][2]; transform.matrix[2][2] = j["matrix"][2][2]; transform.matrix[3][2] = j["matrix"][3][2];
	transform.matrix[0][3] = j["matrix"][0][3]; transform.matrix[1][3] = j["matrix"][1][3]; transform.matrix[2][3] = j["matrix"][2][3]; transform.matrix[3][3] = j["matrix"][3][3];
}

void to_json(json& j, const Transform2D& transform) {
	j["position"] = { transform.position.x, transform.position.y };
	j["rotation"] = transform.rotation;
	j["size"] = { transform.size.x, transform.size.y };
	j["matrix"] = {
		{ transform.matrix[0][0], transform.matrix[1][0], transform.matrix[2][0], transform.matrix[3][0] },
		{ transform.matrix[0][1], transform.matrix[1][1], transform.matrix[2][1], transform.matrix[3][1] },
		{ transform.matrix[0][2], transform.matrix[1][2], transform.matrix[2][2], transform.matrix[3][2] },
		{ transform.matrix[0][3], transform.matrix[1][3], transform.matrix[2][3], transform.matrix[3][3] },
	};
}
