#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "Model.h"

class Renderer {
	public:
		static void render();
	private:
		static std::vector<ModelPtr> models;
		Renderer() { }
};

#endif // !RENDERER_H
