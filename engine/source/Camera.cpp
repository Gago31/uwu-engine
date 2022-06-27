#include "Camera.h"
#include "GameController.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 pos, float theta_, float aspect_ratio) {
    viewPos = pos;
    theta = theta_; 
    ratio = aspect_ratio;
    near = 0.1f;
    far = 20.0f;
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
    GameController::currentCamera = this;
}

const glm::vec3 Camera::getPos() const {
    return viewPos;
}

glm::vec2 Camera::getDirection() {
    return { glm::sign(approxTrig(glm::cos(theta))), glm::sign(approxTrig(glm::sin(theta))) };
}

float Camera::approxTrig(float value) {
    return glm::abs(value) > 0.01f ? value : 0.0f;
}

glm::mat4 Camera::getView() {
    return glm::lookAt(eye, at, up);
}

glm::mat4 Camera::getProjection() {
    return glm::perspective(glm::radians<float>(60.0f), ratio, near, far);
}
