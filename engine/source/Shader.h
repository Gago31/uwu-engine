#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <glad/glad.h>


class Shader {
	public:
		unsigned int ID;
		Shader(const std::filesystem::path vertexPath, const std::filesystem::path fragmentPath);
		void use();
		void setBool(const std::string &name, bool value) const;
		void setInt(const std::string &name, int value) const;
		void setFloat(const std::string &name, float value) const;
		void setMatrix4f(const std::string& name, float *value) const;
};

#endif // !SHADER_H
