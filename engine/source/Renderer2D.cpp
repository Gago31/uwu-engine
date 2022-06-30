#include "Renderer2D.h"
#include "root_directory.h"
#include "GameController.h"
#include "ResourceManager.h"



void Renderer2D::initialize(int width, int height) {
	WIDTH = width;
	HEIGHT = height;
	projection = glm::ortho(0.0f, static_cast<float>(WIDTH), static_cast<float>(HEIGHT), 0.0f, -1.0f, 1.0f);
	unsigned int VBO;
	float vertices[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer2D::clear() {
	glDeleteVertexArrays(1, &quadVAO);
}

void Renderer2D::setResolution(int width, int height) {
	projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
}

void Renderer2D::enqueue(VisualNode2D* node, glm::mat4 _transform) {
	shaderNames.insert(node->shaderName);
	nodes[node->shaderName].push_back({ node, _transform });
}

void Renderer2D::render() {
	for (std::string shader_name : shaderNames) {
		ResourceManager::getShader(shader_name).use();
		ResourceManager::getShader(shader_name).setMatrix4f("projection", projection);
		
		for (Renderable2D renderable : nodes[shader_name]) {
			if (renderable.node->visible) {
				DrawSprite(renderable.node);
				renderable.node->render();
			}
		}
	}
	shaderNames.clear();
	nodes.clear();
}

void Renderer2D::DrawSprite(VisualNode2D* node, glm::mat4 _transform) {
	glm::vec2 position = node->sprite->transform.position;
	glm::vec2 size = { 
		node->sprite->texture->width * node->sprite->transform.size.x, 
		node->sprite->texture->height * node->sprite->transform.size.y 
	};
	float rotation = node->sprite->transform.rotation;
	glm::mat4 model(1.0f);
	model = _transform * model;
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
	model = glm::scale(model, glm::vec3(size, 1.0f));

	ResourceManager::getShader(node->shaderName).setMatrix4f("model", model);
	ResourceManager::getShader(node->shaderName).setVector3f("spriteColor", node->color);

	glActiveTexture(GL_TEXTURE0);
	node->sprite->texture->Bind();

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

