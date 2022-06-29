#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"


class SpriteRenderer {
	public:
		SpriteRenderer(ShaderPtr _shader);
		~SpriteRenderer();

		void DrawSprite(TexturePtr texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), 
						float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
		void DrawSprite(SpritePtr sprite);
	private:
		ShaderPtr shader;
		unsigned int quadVAO;
		void initRenderData();
};

#endif // !SPRITE_RENDERER_H
