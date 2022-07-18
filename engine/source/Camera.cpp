#include "Camera.h"
#include "GameController.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 pos, float theta_, float aspect_ratio) {
    viewPos = pos;
    nextPos = pos;
    theta = theta_;
    nextTheta = theta_;
    ratio = aspect_ratio;
    near = 0.1f;
    far = 10.0f;
}

void Camera::setPos(glm::vec3 new_pos) {
    viewPos = new_pos;
}

void Camera::setPos(float x, float y, float z) {
    viewPos = { x, y, z };
}

void Camera::setNextState(glm::vec3 next_pos, float next_theta) {
    nextPos = next_pos;
    nextTheta = next_theta;
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

const glm::vec3 Camera::getNextPos() const {
    return nextPos;
}

glm::vec2 Camera::getDirection() {
    return { 
        glm::sign(approxTrig(glm::sin(theta)) + approxTrig(glm::sin(nextTheta))), 
        glm::sign(approxTrig(glm::cos(theta)) + approxTrig(glm::cos(nextTheta))) 
    };
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
