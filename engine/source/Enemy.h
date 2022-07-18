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
        bool action_ready = true;
        float x = 0.0f;
        float y = 0.0f;
	public:
        int direction = RIGHT;
        int nextPos = 0;
        inline static std::string className = "Enemy";
        glm::vec3 position;
        std::vector<int> path;
        ShaderPtr shader;
        Enemy() = default;
        Enemy(glm::vec3 pos, std::vector<int>& _path);
        void update(float dt) override;
        const glm::vec3 getPos();
        void setPos(glm::vec3 new_pos);
        void setPos(float _x, float _y, float _z = 0.0f);
        const float getX();
        const float getY();
        virtual json serialize() override;
        void deserialize(json& j) override;
        void turnStart() override;
        std::string getClassName() override {
            return className;
        }
};

//void from_json(const json& j, Enemy& node);
//void to_json(json& j, const Enemy& node);

#endif // !ENEMY_H
