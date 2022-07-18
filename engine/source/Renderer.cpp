#include <memory>
#include <glad/glad.h>
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameController.h"


std::set<std::string> Renderer::shaderNames;
std::map<std::string, std::vector<Renderable>> Renderer::nodes;
glm::mat4 Renderer::projection;
glm::mat4 Renderer::view;


void Renderer::render() {
	updateView();
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GameController::fillPolygon ? GL_FILL : GL_LINE);
	for (std::string shader_name : shaderNames) {
		ResourceManager::getShader(shader_name).use();
		ResourceManager::getShader(shader_name).setMatrix4f("projection", projection);
		ResourceManager::getShader(shader_name).setMatrix4f("view", view);
		ResourceManager::getShader("modelShaderL").setVector3f("lightColor", { 1.0f, 1.0f, 1.0f });
		ResourceManager::getShader("modelShaderL").setVector3f("lightPos", { 5.0f, 5.0f, 2.0f });
		ResourceManager::getShader("modelShaderL").setVector3f("viewPos", GameController::currentCamera->getPos());

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

void Renderer::updateView() {
	if (GameController::currentCamera == nullptr) return;
	glm::mat4 player_view = GameController::currentCamera->getView();
	glm::mat4 player_projection = GameController::currentCamera->getProjection();

	Renderer::projection = player_projection;
	Renderer::view = player_view;
}

//void Renderer::setCamera(Camera& new_cam) {
//	currentCamera = std::make_shared<Camera>(new_cam);
//}
