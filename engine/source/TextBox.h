#pragma once

#include "SpriteNode.h"
#include "TextRenderer.h"
#include "ResourceManager.h"


class TextBox : public SpriteNode {
	private:	
		TextRenderer textRenderer;
	public:
		std::string text = "";
	
		TextBox() = default;
		TextBox(std::string node_name, SpritePtr s, std::string font_path, int font_size = 32, std::string shader_name = "spriteShader") {
			name = node_name;
			sprite = s;
			shaderName = shader_name;
			textRenderer.init(font_path, font_size);
			shader = std::make_shared<Shader>(ResourceManager::getShader(shader_name));
		}
		TextBox(SpritePtr s, std::string font_path, int font_size = 32, std::string shader_name = "spriteShader") {
			name = "TextBox";
			sprite = s;
			shaderName = shader_name;
			textRenderer.init(font_path, font_size);
			shader = std::make_shared<Shader>(ResourceManager::getShader(shader_name));
		}
		void setColor(float r, float g, float b) {
			color = { r, g, b };
		}
		void render(glm::mat4 _transform) override {
			if (visible && text != "") {
				//textRenderer.renderText()
				textRenderer.renderText(std::make_shared<Shader>(ResourceManager::getShader("textShader")), text, transform.position.x, transform.position.y, transform.size.x, color);
			}
		}
};