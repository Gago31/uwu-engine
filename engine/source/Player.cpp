#include "Player.h"
#include <iostream>

Player::Player(glm::vec3 pos) {
    this->position = pos;
	std::vector<std::string> actions = { "left", "right", "up", "down"};
	this->camera = std::make_shared<Camera>(Camera(pos));
    this->Input = std::make_shared<InputListener>(InputListener(actions, true));
    InputController::addListener(&this->Input);
}

Player::Player(float x, float y, float z) {
    this->position = { x, y, z };
    std::vector<std::string> actions = { "left", "right", "up", "down" };
    this->camera = std::make_shared<Camera>(Camera({ x, y, z }));
    this->Input = std::make_shared<InputListener>(InputListener(actions, true));
    InputController::addListener(&this->Input);
}

void Player::update(float dt) {
    float h_axis = this->Input->actionStrength("right") - this->Input->actionStrength("left");
    float v_axis = this->Input->actionStrength("up") - this->Input->actionStrength("down");
    this->camera->theta += 2 * h_axis * dt;
    this->position.x += 2 * v_axis * std::sin(this->camera->theta) * dt;
    this->position.y += 2 * v_axis * std::cos(this->camera->theta) * dt;
    this->camera->setPos(this->position);
    this->camera->update();

    /*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPhi -= 2 * dt;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPhi += 2 * dt;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPhi -= 2 * dt;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPhi += 2 * dt;
    }*/
}

void Player::setInputListener(InputListenerPtr listener) {
    this->Input = listener;
}

void Player::setCamera(CameraPtr cam) {
    this->camera = cam;
    this->camera->setPos(this->position);
}