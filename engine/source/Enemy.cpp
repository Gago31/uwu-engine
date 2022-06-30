#include <glad/glad.h>
#include "Enemy.h"
#include "GameController.h"


Enemy::Enemy(glm::vec3 pos, std::vector<direction_t>& _path) {
    x = pos.x;
    y = pos.y;
    position = pos;
    path = _path;
}

void Enemy::update(float dt) {
	if (path.empty()) return;
    if (!(GameController::update_world && action_ready)) return;
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

const glm::vec3 Enemy::getPos() {
    return position;
}

const float Enemy::getX() {
    return x;
}

const float Enemy::getY() {
    return y;
}
