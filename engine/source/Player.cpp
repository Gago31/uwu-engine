#include "Player.h"
#include "GameController.h"
#include <iostream>
#include "Enemy.h"
#include <memory>
#include "ResourceManager.h"
#include <iostream>
#include "root_directory.h"
#include "SoundController.h"
#include "SoundEmitter3D.h"


Player::Player(glm::vec3 pos, float theta_d) {
    position = pos;
    theta = glm::radians<float>(theta_d);
	std::vector<std::string> actions = { "left", "right", "up", "down", "a", "b" };
	camera = std::make_shared<Camera>(Camera(pos, theta));
    Input = std::make_shared<InputListener>(InputListener(actions, true));
    InputController::addListener(&Input);
}

Player::Player(float x, float y, float z, float theta_d) {
    position = { x, y, z };
    theta = glm::radians<float>(theta_d);
    std::vector<std::string> actions = { "left", "right", "up", "down", "a", "b" };
    camera = std::make_shared<Camera>(Camera({ x, y, z }, theta));
    Input = std::make_shared<InputListener>(InputListener(actions, true));
    InputController::addListener(&Input);
}

void Player::update(float dt) {
    if (Input->actionPressed("b")) {
        glm::vec2 cam_dir = camera->getDirection();
        std::cout << "Direction: (" << cam_dir.x << ", " << cam_dir.y << "); position: (" << position.x << ", " << position.y << ")" << std::endl;
        Node *textbox = GameController::currentScene->root->findChild("TextBox");
        textbox->visible = !textbox->visible;
        std::cout << textbox->visible << std::endl;
    }
    if (accept_input) {
        h_axis = Input->actionStrength("right") - Input->actionStrength("left");
        v_axis = Input->actionStrength("up") - Input->actionStrength("down");
        h_axis = h_axis < 0.0f ? -1.0f : (h_axis > 0.0f ? 1.0f : 0.0f);
        v_axis = v_axis < 0.0f ? -1.0f : (v_axis > 0.0f ? 1.0f : 0.0f);
        h_axis = v_axis == 0.0f ? h_axis : 0.0f;
        glm::vec2 nextPos = {
            position.x + v_axis * glm::round(glm::sin(camera->theta)),
            -(position.y + v_axis * glm::round(glm::cos(camera->theta))) + GameController::currentGrid->height() - 1,
        };
        if (v_axis != 0.0f) {
            //std::cout << "pos: (" << position.x << ", " << position.y << "); next pos : (" << nextPos.x << ", " << nextPos.y << ") : " << GameController::currentGrid->coord(nextPos) << std::endl;
            //std::cout <<  << std::endl;
        }
        v_axis = GameController::currentGrid->coord(nextPos) ? 0.0f : v_axis;
        if (h_axis || v_axis) {
            accept_input = false;
            camera->setNextState({
                 position.x + v_axis * glm::round(glm::sin(camera->theta)),
                 position.y + v_axis * glm::round(glm::cos(camera->theta)),
                 0.0f }, theta + h_axis * glm::radians<float>(90)
            );
            GameController::turnStart((bool) v_axis);
            //std::cout << "turn start" << std::endl;
        }
        if (Input->actionPressed("a")) {
            //GameController::currentScene->unload();
            //GameController::currentScene->removeRoot();
            if (a) {
                if (GameController::currentScene->root->findChild("DCP2") == nullptr) {
                    std::vector<int> enemyPath = { WAIT | UP, WAIT | RIGHT, WAIT | DOWN, WAIT | LEFT };
                    Enemy* enemy = new Enemy({ 5, 4, 0 }, enemyPath);
                    enemy->name = "DCP2";
                    MeshNode* dcpNode = new MeshNode("DCP", "modelShader");
                    dcpNode->transform.scale(0.2f);
                    dcpNode->transform.rotateX(glm::pi<float>() * 0.5);
                    enemy->addChild(dcpNode);
                    SoundEmitter3D* dolphinSongNode = new SoundEmitter3D({ 0, 0, 0 }, "buenosDias", false);
                    dolphinSongNode->setLooping(false);
                    dolphinSongNode->name = "songNode";
                    enemy->addChild(dolphinSongNode);
                    GameController::currentScene->root->addChild(enemy);
                    std::cout << "DCP added" << std::endl;
                } else {
                    GameController::currentScene->root->findChild("DCP2")->visible = true;

                }
                a = false;
            } else {
                GameController::currentScene->root->findChild("DCP2")->remove();
                //GameController::currentScene->root.findChild("DCP2")->visible = false;
                std::cout << "DCP fucking killed" << std::endl;
                a = true;
            }
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
        //if (timer >= GameController::TURN_PERIOD) {
        //    /*camera->theta = theta + h_axis * glm::radians<float>(90);
        //    position.x += v_axis * glm::round(glm::sin(camera->theta));
        //    position.y += v_axis * glm::round(glm::cos(camera->theta));
        //    theta = camera->theta;
        //    h_axis = v_axis = 0.0f;
        //    timer = 0.0f;
        //    accept_input = true;*/
        //    turnEnd();
        //    //GameController::run_turn = false;
        //}
        camera->setPos(camPos);
    }
    SoundController::updateListener(position, theta);
    camera->update();
}

void Player::turnEnd() {
    camera->theta = theta + h_axis * glm::radians<float>(90);
    position.x += v_axis * glm::round(glm::sin(camera->theta));
    position.y += v_axis * glm::round(glm::cos(camera->theta));
    theta = camera->theta;
    camera->setNextState(camera->getPos(), camera->theta);
    h_axis = v_axis = 0.0f;
    timer = 0.0f;
    accept_input = true;
}

void Player::setInputListener(InputListenerPtr listener) {
    Input = listener;
}

void Player::setCamera(CameraPtr cam) {
    camera = cam;
    camera->setPos(position);
}