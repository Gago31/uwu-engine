#include <glad/glad.h>
#include "Enemy.h"
#include "GameController.h"

namespace gr = Grafica;
namespace tr = Grafica::Transformations;


//Enemy::Enemy(ModelPtr _model, ShaderPtr _shader, float _square_size) {
//    model = _model;
//    shader = _shader;
//	position = glm::vec3(0.0f, 0.0f, 0.0f);
//    square_size = _square_size;
//}
//
//Enemy::Enemy(ModelPtr _model, ShaderPtr _shader, glm::vec3 pos, float _square_size) {
//    model = _model;
//    shader = _shader;
//    position = pos;
//    x = pos.x;
//    y = pos.y;
//    square_size = _square_size;
//}
//
//Enemy::Enemy(ModelPtr _model, ShaderPtr _shader, glm::vec3 pos, std::vector<direction_t>& _path, float _square_size) {
//    model = _model;
//    shader = _shader;
//    position = pos;
//    //path = route;
//    x = pos.x;
//    y = pos.y;
//    path = _path;
//    square_size = _square_size;
//}

Enemy::Enemy(glm::vec3 pos, std::vector<direction_t>& _path) {
    x = pos.x;
    y = pos.y;
    position = pos;
    path = _path;
}

void Enemy::update(float dt) {
	if (path.empty()) return;
    //float movement_period = GameController::MOVEMENT_PERIOD;
    if (!(GameController::run_turn && action_ready)) return;
    /*movementTime = std::clamp(movementTime + dt, 0.0f, movement_period);
    float translationRatio = movementTime / movement_period;*/
    direction = (direction_t) (path[nextPos] & 3); // First two bits
    bool wait = path[nextPos] & WAIT;
    bool double_step = path[nextPos] & DOUBLE;
    float movementY = (direction & VERTICAL) ? ((direction & NEGATIVE) ? -1 : 1) : 0;
    float movementX = (direction & VERTICAL) ? 0 : ((direction & NEGATIVE) ? -1 : 1);
    movementY = wait ? 0 : movementY;
    movementX = wait ? 0 : movementX;
    x = position.x + movementX * GameController::SQUARE_SIZE * GameController::TURN_PROGRESS * (double_step ? 2.0f : 1.0f);
    y = position.y + movementY * GameController::SQUARE_SIZE * GameController::TURN_PROGRESS * (double_step ? 2.0f : 1.0f);
    if (GameController::TURN_PROGRESS == 1.0f) {
        position.x = position.x + movementX * GameController::SQUARE_SIZE * (double_step ? 2.0f : 1.0f);
        position.y = position.y + movementY * GameController::SQUARE_SIZE * (double_step ? 2.0f : 1.0f);
        nextPos = (nextPos + 1) % path.size();
    }

    glm::mat4 __transform = glm::mat4(1.0f);
    __transform = glm::translate(__transform, { x, y, 0.0f });
    __transform = glm::rotate(__transform, glm::pi<float>() * 0.5f * ((direction & 3) + 1), glm::vec3(0.0f, 0.0f, 1.0f));
    transform.setMatrix(__transform);
}

void Enemy::render(glm::mat4 _transform) {
   /* this->transform.position = glm::vec3(x, y, 0.0f);
    glm::mat4 __transform = glm::mat4(1.0f);
    __transform = glm::translate(__transform, { x, y, 0.0f });
    __transform = glm::rotate(__transform, glm::pi<float>() * 0.5f * ((direction & 3) + 1), glm::vec3(0.0f, 0.0f, 1.0f));*/
}

const glm::vec3 Enemy::getPos() {
    return position;
}

const float Enemy::getX() {
    return x;
}

const float Enemy::getY() {
    return y;
}
