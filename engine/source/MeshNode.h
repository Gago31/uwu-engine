#pragma once

#include "Node3D.h"
#include "Mesh.h"
#include "Model.h"

class MeshNode : public Node3D {
	public:
		ModelPtr model;
		ShaderPtr shader;
		MeshNode() = default;
		MeshNode(ModelPtr m, ShaderPtr _shader) : model(m), shader(_shader) {}
		void render(glm::mat4 _transform) override {
			//std::cout << "uwu" << std::endl;
			shader->use();
			shader->setMatrix4f("model", _transform * transform.getTransform());
			model->Draw(shader);
		}
};