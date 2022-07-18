#include <glad/glad.h>
#include "Enemy.h"
#include "GameController.h"
#include "SoundEmitter3D.h"


Enemy::Enemy(glm::vec3 pos, std::vector<int>& _path) {
    x = pos.x;
    y = pos.y;
    position = pos;
    path = _path;
    glm::mat4 __transform = glm::mat4(1.0f);
    __transform = glm::translate(__transform, { x, y, 0.0f });
    __transform = glm::rotate(__transform, glm::pi<float>() * 0.5f * ((direction & 3) + 1), glm::vec3(0.0f, 0.0f, 1.0f));
    transform.setMatrix(__transform);
}

void Enemy::update(float dt) {
	if (path.empty()) return;
    if (!(GameController::update_world && action_ready)) return;
    direction = path[nextPos] & 3; // First two bits
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

void Enemy::setPos(glm::vec3 new_pos) {
    position = new_pos;
    x = position.x;
    y = position.y;
}

void Enemy::setPos(float _x, float _y, float _z) {
    position = { _x, _y, _z };
    x = position.x;
    y = position.y;
}

void Enemy::turnStart() {
    SoundEmitter3D* song = (SoundEmitter3D*) findChild("songNode");
    if (!song->isPlaying()) song->play();
}

//void from_json(const json& j, Enemy& node) {
//    node.setPos({ j["position"][0], j["position"][1] , j["position"][2] });
//    node.path = j["path"].get<std::vector<int>>();
//    node.transform = j["transform"];
//    node.name = j["name"];
//}
//
//void to_json(json& j, const Enemy& node) {
//    j["position"] = { node.position.x, node.position.y, node.position.z };
//    j["path"] = node.path;
//    j["transform"] = node.transform;
//    j["name"] = node.name;
//}

json Enemy::serialize() {
    json j = Node3D::serialize();
    j["position"] = { position.x, position.y, position.z };
    j["path"] = path;
    j["nextPos"] = nextPos;
    j["direction"] = direction;
    return j;
}

void Enemy::deserialize(json& j) {
    Node3D::deserialize(j);
    setPos({ j["position"][0], j["position"][1], j["position"][2] });
    std::vector<int> jpath = j["path"];
    path = jpath;
    nextPos = j["nextPos"];
    direction = j["direction"];
    glm::mat4 __transform = glm::mat4(1.0f);
    __transform = glm::translate(__transform, { x, y, 0.0f });
    __transform = glm::rotate(__transform, glm::pi<float>() * 0.5f * ((direction & 3) + 1), glm::vec3(0.0f, 0.0f, 1.0f));
    transform.setMatrix(__transform);
}
