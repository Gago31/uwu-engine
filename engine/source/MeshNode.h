#pragma once

#include "Node3D.h"
#include "Mesh.h"
#include "Model.h"
#include "Renderer.h"
#include "ResourceManager.h"


class MeshNode : public VisualNode {
	public:
		inline static std::string className = "MeshNode";
		std::string modelName;
		ModelPtr model;
		MeshNode() = default;
		MeshNode(std::string model_name, std::string shader_name) {
			name = "Mesh";
			shaderName = shader_name;
			modelName = model_name;
			shader = std::make_shared<Shader>(ResourceManager::getShader(shader_name));
			model = std::make_shared<Model>(ResourceManager::getModel(model_name));
		}
		MeshNode(std::string node_name, std::string model_name, std::string shader_name) {
			name = node_name;
			shaderName = shader_name;
			shader = std::make_shared<Shader>(ResourceManager::getShader(shader_name));
			modelName = model_name;
			model = std::make_shared<Model>(ResourceManager::getModel(model_name));
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
		virtual json serialize() override {
			json j = VisualNode::serialize();
			j["modelName"] = modelName;
			return j;
		}
		void deserialize(json& j) override {
			Node3D::deserialize(j);
			shaderName = j["shaderName"];
			shader = std::make_shared<Shader>(ResourceManager::getShader(shaderName));
			modelName = j["modelName"];
			model = std::make_shared<Model>(ResourceManager::getModel(modelName));
		}
		std::string getClassName() override {
			return className;
		}
};