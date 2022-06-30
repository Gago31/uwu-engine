#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <set>
#include <map>
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include "Node2D.h"


struct Renderable2D {
	VisualNode2D* node;
	glm::mat4 transform;
};

class Renderer2D {
	public:
		static void initialize(int width, int height);
		static void DrawSprite(VisualNode2D* node, glm::mat4 _transform = glm::mat4(1.0f));
		static void setResolution(int width, int height);
		static void enqueue(VisualNode2D* node, glm::mat4 _transform);
		static void clear();
		static void render();
	private:
		inline static int WIDTH;
		inline static int HEIGHT;
		inline static glm::mat4 projection;
		inline static unsigned int quadVAO;
		inline static std::set<std::string> shaderNames;
		inline static std::map<std::string, std::vector<Renderable2D>> nodes;
		Renderer2D();
};

#endif // !SPRITE_RENDERER_H
