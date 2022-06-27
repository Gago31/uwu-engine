#include <memory>
#include <glad/glad.h>
#include "Renderer.h"
#include "ResourceManager.h"


std::set<std::string> Renderer::shaderNames;
std::map<std::string, std::vector<Renderable>> Renderer::nodes;
glm::mat4 Renderer::projection;
glm::mat4 Renderer::view;


void Renderer::render() {
	for (std::string shader_name : shaderNames) {
		ResourceManager::getShader(shader_name).use();
		ResourceManager::getShader(shader_name).setMatrix4f("projection", projection);
		ResourceManager::getShader(shader_name).setMatrix4f("view", view);

		for (Renderable renderable : nodes[shader_name]) {
			renderable.node->render(renderable.transform);
		}
	}
	shaderNames.clear();
	nodes.clear();
}

void Renderer::enqueue(VisualNode* node, glm::mat4 _transform) {
	shaderNames.insert(node->shaderName);
	nodes[node->shaderName].push_back({ node, _transform });
}

//void Renderer::setCamera(Camera& new_cam) {
//	currentCamera = std::make_shared<Camera>(new_cam);
//}
