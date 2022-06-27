#pragma once

#include "Node3D.h"
#include "Mesh.h"
#include "Model.h"
#include "Renderer.h"
#include "ResourceManager.h"


class MeshNode : public VisualNode {
	public:
		ModelPtr model;
		MeshNode() = default;
		MeshNode(ModelPtr m, std::string shader_name) : model(m) {
			shaderName = shader_name;
			shader = std::make_shared<Shader>(ResourceManager::getShader(shader_name));
		}
		void render(glm::mat4 _transform) override {
			//std::cout << "uwu" << std::endl;
			//shader->use();
			shader->setMatrix4f("model", _transform * transform.getTransform());
			model->Draw(shader);
		}
		virtual void _render(glm::mat4 _transform = glm::mat4(1.0)) override {
			//render(_transform);
			Renderer::enqueue(this, _transform);
			for (Node* child : children) {
				child->_render(_transform * transform.getTransform());
			}
		}
};