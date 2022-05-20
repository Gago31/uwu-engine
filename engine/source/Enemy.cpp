#include <vector>
#include <grafica/transformations.h>
#include <grafica/gpu_shape.h>
#include <grafica/easy_shaders.h>
#include <grafica/scene_graph.h>
#include <glad/glad.h>
#include "Enemy.h"

namespace gr = Grafica;
namespace tr = Grafica::Transformations;


Enemy::Enemy(gr::SceneGraphNodePtr _node) {
    //shape = _shape;
    //pipeline = _pipeline;
    node = _node;
	enemyPos = gr::Vector3f(0.0f, 0.0f, 0.0f);
}

Enemy::Enemy(gr::SceneGraphNodePtr _node, gr::Vector3f pos) {
    //shape = _shape;
    //pipeline = _pipeline;
    node = _node;
    enemyPos = pos;
}

Enemy::Enemy(gr::SceneGraphNodePtr _node, gr::Vector3f pos, std::vector<gr::Vector3f>& route) {
    //shape = _shape;
    //pipeline = _pipeline;
    node = _node;
    enemyPos = pos;
	enemyRoute = route;
}

void Enemy::update(float delta, const float movement_period) {
	if (enemyRoute.empty()) return;

    enemyMovementTime = std::clamp(enemyMovementTime + delta, 0.0f, movement_period);
    float enemyTranslationRatio = enemyMovementTime / movement_period;
    x = enemyPos[0] + (enemyRoute[enemyNextPos][0] - enemyPos[0]) * enemyTranslationRatio;
    y = enemyPos[1] + (enemyRoute[enemyNextPos][1] - enemyPos[1]) * enemyTranslationRatio;
    if (enemyMovementTime == movement_period) {
        enemyPos = enemyRoute[enemyNextPos];
        enemyNextPos = (enemyNextPos + 1) % enemyRoute.size();
        enemyMovementTime = 0.0f;
    }

    node->transform = tr::translate(x, y, 0.25);
    //render(x, y, _pipeline);
}

template <typename PipelineT>
void Enemy::render(const PipelineT& pipeline) {
    /*glUseProgram(pipeline.shaderProgram);
    gr::Matrix4f new_pos = tr::translate(x, y, 0.25f) * tr::uniformScale(0.5f);
    glUniformMatrix4fv(glGetUniformLocation(pipeline.shaderProgram, "model"), 1, GL_FALSE, new_pos.data());
    pipeline.drawCall(shape);*/
    gr::drawSceneGraphNode(node, pipeline, "model");
}

const gr::Vector3f Enemy::getPos() {
    return enemyPos;
}

const float Enemy::getX() {
    return x;
}

const float Enemy::getY() {
    return y;
}

const gr::SceneGraphNodePtr Enemy::getNode() {
    return node;
}
