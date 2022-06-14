#include <memory>
#include <glad/glad.h>
#include "Renderer.h"


std::vector<ModelPtr> Renderer::models;


void Renderer::render() {

}

void Renderer::setCamera(Camera& new_cam) {
	currentCamera = std::make_shared<Camera>(new_cam);
}
