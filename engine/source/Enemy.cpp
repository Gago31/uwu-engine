#include <glad/glad.h>
#include "Enemy.h"

namespace gr = Grafica;
namespace tr = Grafica::Transformations;


Enemy::Enemy(ModelPtr _model, ShaderPtr _shader, float _square_size) {
    model = _model;
    shader = _shader;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
    square_size = _square_size;
}

Enemy::Enemy(ModelPtr _model, ShaderPtr _shader, glm::vec3 pos, float _square_size) {
    model = _model;
    shader = _shader;
    position = pos;
    x = pos.x;
    y = pos.y;
    square_size = _square_size;
}

Enemy::Enemy(ModelPtr _model, ShaderPtr _shader, glm::vec3 pos, std::vector<direction_t>& _path, float _square_size) {
    model = _model;
    shader = _shader;
    position = pos;
    //path = route;
    x = pos.x;
    y = pos.y;
    path = _path;
    square_size = _square_size;
}

void Enemy::update(float delta, const float movement_period) {
	if (path.empty()) return;

    movementTime = std::clamp(movementTime + delta, 0.0f, movement_period);
    float translationRatio = movementTime / movement_period;
    direction = (direction_t) (path[nextPos] & 3); // First two bits
    bool wait = path[nextPos] & WAIT;
    bool double_step = path[nextPos] & DOUBLE;
    float movementY = (direction & VERTICAL) ? ((direction & NEGATIVE) ? -1 : 1) : 0;
    float movementX = (direction & VERTICAL) ? 0 : ((direction & NEGATIVE) ? -1 : 1);
    movementY = wait ? 0 : movementY;
    movementX = wait ? 0 : movementX;
    x = position.x + movementX * square_size * translationRatio * (double_step ? 2.0f : 1.0f);
    y = position.y + movementY * square_size * translationRatio * (double_step ? 2.0f : 1.0f);
    if (movementTime == movement_period) {
        position.x = position.x + movementX * square_size * (double_step ? 2.0f : 1.0f);
        position.y = position.y + movementY * square_size * (double_step ? 2.0f : 1.0f);
        nextPos = (nextPos + 1) % path.size();
        movementTime = 0.0f;
    }

    //render(x, y);
}

void Enemy::render() {
    //this->transform.position = glm::vec3(x, y, 0.0f);
    glm::mat4 _transform = glm::mat4(1.0f);
    _transform = glm::translate(_transform, { x, y, 0.0f });
    _transform = glm::rotate(_transform, glm::pi<float>() * 0.5f * ((direction & 3) + 1), glm::vec3(0.0f, 0.0f, 1.0f));
    _transform = _transform * this->transform.rotation;
    _transform = glm::scale(_transform, this->transform.size);
    //_transform = glm::translate(_transform, this->transform.position);
    //gr::Matrix4f transform = tr::translate(x, y, 0.0f) * tr::rotationZ(3.14159f * 0.5f * ((direction & 3) + 1)) * tr::rotationX(3.14159f / 2) * tr::uniformScale(0.2f);
    shader->setMatrix4f("model", _transform);
    model->Draw(shader);
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
