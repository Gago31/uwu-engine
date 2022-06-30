#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <set>
#include "Model.h"
#include "Camera.h"
#include "Node.h"
#include "Node3D.h"


struct Renderable {
	VisualNode *node;
	glm::mat4 transform;
};

class Renderer {
	public:
		static std::set<std::string> shaderNames;
		static std::map<std::string, std::vector<Renderable>> nodes;
		static glm::mat4 projection;
		static glm::mat4 view;
		static void render();
		//static void setCamera(Camera& new_cam);
		static void enqueue(VisualNode *node, glm::mat4 _transform);
	private:
		Renderer() { }
};

#endif // !RENDERER_H
