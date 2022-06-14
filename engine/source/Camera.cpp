#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 pos, float theta_, float aspect_ratio) {
    viewPos = pos;
    theta = theta_;
    ratio = aspect_ratio;
}

void Camera::setPos(glm::vec3 new_pos) {
    viewPos = new_pos;
}

void Camera::setPos(float x, float y, float z) {
    viewPos = { x, y, z };
}

void Camera::setAspectRatio(float new_ratio) {
    ratio = new_ratio;
}

void Camera::update() {
    eye = viewPos;
    at = {
        eye.x + glm::sin(theta),
        eye.y + glm::cos(theta),
        eye.z
    };
    up = { 0.0f, 0.0f, 1.0f };
}

void Camera::makeCurrent() {

}

glm::mat4 Camera::getView() {
    return glm::lookAt(eye, at, up);
}

glm::mat4 Camera::getProjection() {
    return glm::perspective(glm::radians<float>(60.0f), ratio, 0.1f, 20.0f);
}
