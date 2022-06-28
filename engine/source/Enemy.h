#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include <memory>
#include "Shader.h"
#include "Model.h"
#include "Transform.h"
#include "MeshNode.h"


enum direction_t { 
    RIGHT = 0,
    UP = 1,
    LEFT = 2,
    DOWN = 3,
    WALK = 0,
    WAIT = 1 << 2,
    DOUBLE = 1 << 3,
    POSITIVE = 0,
    NEGATIVE = 1 << 1,
    HORIZONTAL = 0,
    VERTICAL = 1
};

class Enemy : public Node3D {
	private:
        std::vector<direction_t> path;
        glm::vec3 position;
        int nextPos = 0;
        bool action_ready = true;
        direction_t direction = RIGHT;
        float x = 0.0f;
        float y = 0.0f;
	public:
        ShaderPtr shader;
        Enemy() = default;
        Enemy(glm::vec3 pos, std::vector<direction_t>& _path);
        void update(float dt) override;
        const glm::vec3 getPos();
        const float getX();
        const float getY();
};

#endif // !ENEMY_H
