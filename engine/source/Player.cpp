#include "Player.h"
#include "GameController.h"
#include <iostream>

Player::Player(glm::vec3 pos, float theta_d) {
    position = pos;
    theta = glm::radians<float>(theta_d);
	std::vector<std::string> actions = { "left", "right", "up", "down"};
	camera = std::make_shared<Camera>(Camera(pos, theta));
    Input = std::make_shared<InputListener>(InputListener(actions, true));
    InputController::addListener(&Input);
}

Player::Player(float x, float y, float z, float theta_d) {
    position = { x, y, z };
    theta = glm::radians<float>(theta_d);
    std::vector<std::string> actions = { "left", "right", "up", "down" };
    camera = std::make_shared<Camera>(Camera({ x, y, z }, theta));
    Input = std::make_shared<InputListener>(InputListener(actions, true));
    InputController::addListener(&Input);
}

void Player::update(float dt) {
    if (accept_input) {
        h_axis = Input->actionStrength("right") - Input->actionStrength("left");
        v_axis = Input->actionStrength("up") - Input->actionStrength("down");
        h_axis = v_axis == 0.0f ? h_axis : 0.0f;
        glm::vec2 nextPos = {
            position.x + v_axis * glm::round(glm::sin(camera->theta)),
            -(position.y + v_axis * glm::round(glm::cos(camera->theta))) + GameController::currentGrid->height(),
        };
        /*if (v_axis != 0.0f) {
            std::cout << nextPos.x << ", " << nextPos.y << std::endl;
            std::cout << GameController::currentGrid->coord(nextPos) << std::endl;
        }*/
        v_axis = GameController::currentGrid->coord(nextPos) ? 0.0f : v_axis;
        if (h_axis || v_axis) {
            accept_input = false;
            GameController::run_turn = (bool) v_axis;
            //std::cout << "turn start" << std::endl;
        }
    } else {
        timer += dt;
        float time_ratio = glm::clamp(timer / GameController::TURN_PERIOD, 0.0f, 1.0f);
        camera->theta = theta + h_axis * glm::radians<float>(90) * time_ratio;
        glm::vec3 camPos = {
            position.x + v_axis * glm::sin(camera->theta) * time_ratio,
            position.y + v_axis * glm::cos(camera->theta) * time_ratio,
            position.z
        };
        /*position.x += walk_speed * v_axis * std::sin(camera->theta) * dt;
        position.y += walk_speed * v_axis * std::cos(camera->theta) * dt;*/
        if (timer >= GameController::TURN_PERIOD) {
            accept_input = true;
            position.x += v_axis * glm::round(glm::sin(camera->theta));
            position.y += v_axis * glm::round(glm::cos(camera->theta));
            theta = camera->theta;
            h_axis = v_axis = 0.0f;
            timer = 0.0f;
            //GameController::run_turn = false;
        }
        camera->setPos(camPos);
    }
    camera->update();

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
    Input = listener;
}

void Player::setCamera(CameraPtr cam) {
    camera = cam;
    camera->setPos(position);
}