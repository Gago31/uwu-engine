#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "nlohmann/json.hpp"

using json = nlohmann::json;


class Shader {
	public:
		unsigned int ID;
		std::string vertexPath = "";
		std::string fragmentPath = "";
		std::string geometryPath = "";
		Shader() = default;
		void use();
		void compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);
		void setBool(const std::string &name, bool value) const;
		void setInt(const std::string &name, int value) const;
		void setFloat(const std::string &name, float value) const;
		void setVector2f(const std::string& name, float x, float y);
		void setVector2f(const std::string& name, const glm::vec2& value);
		void setVector3f(const std::string& name, float x, float y, float z);
		void setVector3f(const std::string& name, const glm::vec3& value);
		void setVector4f(const std::string& name, float x, float y, float z, float w);
		void setVector4f(const std::string& name, const glm::vec4& value);
		void setMatrix4f(const std::string& name, float* value) const;
		void setMatrix4f(const std::string& name, glm::mat4 value) const;
};

void to_json(json& j, const Shader& shader);
void from_json(const json& j, Shader& shader);

using ShaderPtr = std::shared_ptr<Shader>;

#endif // !SHADER_H
