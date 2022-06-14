#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "Model.h"
#include "Camera.h"

class Renderer {
	public:
		static void render();
		static void setCamera(Camera& new_cam);
	private:
		static CameraPtr currentCamera;
		static std::vector<ModelPtr> models;
		Renderer() { }
};

#endif // !RENDERER_H
