#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include <grafica/gpu_shape.h>
#include <grafica/easy_shaders.h>
#include <grafica/scene_graph.h>

namespace gr = Grafica;

class Enemy {
	private:
        //gr::GPUShape shape;
        //gr::ModelViewProjectionShaderProgram pipeline;
        std::vector<gr::Vector3f> enemyRoute;
        gr::Vector3f enemyPos;
        int enemyNextPos = 0;
        float enemyMovementTime = 0.0f;
        float x = 0.0f;
        float y = 0.0f;
        gr::SceneGraphNodePtr node;
        //gr::ModelViewProjectionShaderProgram& pipeline;
	public:
        Enemy(gr::SceneGraphNodePtr _node);
        Enemy(gr::SceneGraphNodePtr _node, gr::Vector3f pos);
        Enemy(gr::SceneGraphNodePtr _node, gr::Vector3f pos, std::vector<gr::Vector3f>& route);
        void update(float delta, const float movement_period);
        template <typename PipelineT>
        void render(const PipelineT& pipeline);
        const gr::Vector3f getPos();
        const float getX();
        const float getY();
        const gr::SceneGraphNodePtr getNode();
        //const gr::GPUShape getShape();
};

#endif // !ENEMY_H
