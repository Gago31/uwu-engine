#include "Shader.h"


void Shader::use() {
	glUseProgram(this->ID);
}

void Shader::compile(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode) {
	unsigned int vertex, fragment, geometry;
	int success;
	char infoLog[1024];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
		std::cout << "ERROR - Shader compilation failed\n" << infoLog << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
		std::cout << "ERROR - Shader compilation failed\n" << infoLog << std::endl;
	}

	if (gShaderCode != nullptr) {
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
			std::cout << "ERROR - Shader compilation failed\n" << infoLog << std::endl;
		}
	}

	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertex);
	glAttachShader(this->ID, fragment);
	if (gShaderCode != nullptr) {
		glAttachShader(this->ID, geometry);
	}

	glLinkProgram(this->ID);

	glGetProgramiv(this->ID, GL_LINK_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
		std::cout << "ERROR - Shader program linking failed\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (gShaderCode != nullptr) {
		glDeleteShader(geometry);
	}
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int) value);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setMatrix4f(const std::string& name, float *value) const {
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::setMatrix4f(const std::string& name, glm::mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVector2f(const std::string& name, float x, float y){
	glUniform2f(glGetUniformLocation(this->ID, name.c_str()), x, y);
}

void Shader::setVector2f(const std::string& name, const glm::vec2& value){
	glUniform2f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y);
}

void Shader::setVector3f(const std::string& name, float x, float y, float z){
	glUniform3f(glGetUniformLocation(this->ID, name.c_str()), x, y, z);
}

void Shader::setVector3f(const std::string& name, const glm::vec3& value){
	glUniform3f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::setVector4f(const std::string& name, float x, float y, float z, float w){
	glUniform4f(glGetUniformLocation(this->ID, name.c_str()), x, y, z, w);
}

void Shader::setVector4f(const std::string& name, const glm::vec4& value){
	glUniform4f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z, value.w);
}

void to_json(json& j, const Shader& shader) {
	j = {
		{ "vertexPath", shader.vertexPath },
		{ "fragmentPath", shader.fragmentPath },
		{ "geometryPath", shader.geometryPath }
	};
}

void from_json(const json& j, Shader& shader) {
	shader.vertexPath = j["vertexPath"];
	shader.fragmentPath = j["fragmentPath"];
	shader.geometryPath = j["geometryPath"];
}
