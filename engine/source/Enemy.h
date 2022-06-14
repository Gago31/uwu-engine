#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include <memory>
#include <grafica/transformations.h>
#include "Shader.h"
#include "Model.h"
#include "Transform.h"
#include "MeshNode.h"

namespace gr = Grafica;


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

class Enemy : public MeshNode {
	private:
        std::vector<direction_t> path;
        glm::vec3 position;
        int nextPos = 0;
        float movementTime = 0.0f;
        bool action_ready = true;
        direction_t direction = RIGHT;
        float x = 0.0f;
        float y = 0.0f;
        //float square_size = 1.0f;
        //ModelPtr model;
	public:
        ShaderPtr shader;
        Enemy() = default;
        //Transform transform;
        /*Enemy(ModelPtr _model, ShaderPtr _shader, float _square_size = 1.0f);
        Enemy(ModelPtr _model, ShaderPtr _shader, glm::vec3 pos, float _square_size = 1.0f);
        Enemy(ModelPtr _model, ShaderPtr _shader, glm::vec3 pos, std::vector<direction_t>& _path, float _square_size = 1.0f);*/
        Enemy(glm::vec3 pos, std::vector<direction_t>& _path);
        void update(float dt) override;
        void render(glm::mat4 _transform) override;
        const glm::vec3 getPos();
        const float getX();
        const float getY();
};

#endif // !ENEMY_H
