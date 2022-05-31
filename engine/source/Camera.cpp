#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 pos, float aspect_ratio) {
    this->viewPos = pos;
    this->ratio = aspect_ratio;
}

void Camera::setPos(glm::vec3 new_pos) {
    this->viewPos = new_pos;
}

void Camera::setPos(float x, float y, float z) {
    this->viewPos = { x, y, z };
}

void Camera::setAspectRatio(float new_ratio) {
    this->ratio = new_ratio;
}

void Camera::update() {
    this->eye = this->viewPos;
    this->at = {
        this->eye.x + glm::sin(this->theta),
        this->eye.y + glm::cos(this->theta),
        this->eye.z
    };
    this->up = { 0.0f, 0.0f, 1.0f };
}

glm::mat4 Camera::getView() {
    return glm::lookAt(this->eye, this->at, this->up);
}

glm::mat4 Camera::getProjection() {
    return glm::perspective(glm::radians<float>(60.0f), this->ratio, 0.1f, 80.0f);
}
