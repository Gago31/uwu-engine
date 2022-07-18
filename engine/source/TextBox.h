#pragma once

#include <locale>
#include <codecvt>
#include "SpriteNode.h"
#include "TextRenderer.h"
#include "ResourceManager.h"


class TextBox : public SpriteNode {
	private:
		std::string to_utf8(std::string& string) {
			return string;
			//std::wstring wide = std::wstring(string.begin(), string.end());
			////std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			////std::wstring wide = converter.from_bytes(string);
			////std::string narrow = converter.to_bytes(wide);
			//std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
			//return utf8_conv.to_bytes(wide);
		}
	public:
		TextRenderer textRenderer;
		inline static std::string className = "TextBox";
		std::string text = "";
	
		TextBox() = default;
		TextBox(std::string node_name, Sprite& s, std::string font_path, int font_size = 32, std::string shader_name = "spriteShader") {
			name = node_name;
			sprite = std::make_shared<Sprite>(s);
			shaderName = shader_name;
			textRenderer.init(font_path, font_size);
			shader = std::make_shared<Shader>(ResourceManager::getShader(shader_name));
		}
		TextBox(Sprite& s, std::string font_path, int font_size = 32, std::string shader_name = "spriteShader") {
			name = "TextBox";
			sprite = std::make_shared<Sprite>(s);
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
		virtual json serialize() override {
			json j = SpriteNode::serialize();
			j["text"] = to_utf8(text);
			j["textRenderer"] = textRenderer;
			return j;
		}
		void deserialize(json& j) override {
			SpriteNode::deserialize(j);
			text = j["text"];
			textRenderer = j["textRenderer"];
		}
		std::string getClassName() override {
			return className;
		}
};