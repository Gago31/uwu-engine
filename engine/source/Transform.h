#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Transform {
	public:
		/*gr::Matrix4f position = tr::translate(0.0f, 0.0f, 0.0f);
		gr::Matrix4f rotation = tr::rotationZ(0.0f);
		gr::Matrix4f scale = tr::uniformScale(1.0f);*/
		glm::vec3 position;
		glm::mat4 rotation;
		glm::vec3 size;
		glm::mat4 matrix;
		//Transform(gr::Matrix4f _translation, gr::Matrix4f _rotation, gr::Matrix4f _scale);
		Transform(glm::vec3 _translation = { 0.0f, 0.0f, 0.0f }, glm::mat4 _rotation = glm::mat4(1.0f), glm::vec3 _scale = { 1.0f, 1.0f, 1.0f });
		glm::mat4 getTransform();
		glm::vec3 translate(glm::vec3 translation);
		glm::vec3 translate(float x, float y, float z);
		glm::mat4 rotateX(float t);
		glm::mat4 rotateY(float t);
		glm::mat4 rotateZ(float t);
		glm::vec3 scale(float value);
		glm::vec3 scale(float x, float y, float z);
		glm::vec3 scale(glm::vec3 value);
		void setMatrix(glm::mat4 newMatrix);
		void resetPosition();
		void resetRotation();
		void resetScale();
};

class Transform2D {
	public:
		glm::vec2 position;
		float rotation;
		glm::vec2 size;
		Transform2D(glm::vec2 _position = { 0.0f, 0.0f }, float _rotation = 0.0f, glm::vec2 _scale = { 1.0f, 1.0f });
		glm::mat4 getTransform();
		glm::vec2 translate(glm::vec2 translation);
		glm::vec2 translate(float x, float y);
		float rotate(float t);
		glm::vec2 scale(float value);
		glm::vec2 scale(float x, float y);
		glm::vec2 scale(glm::vec2 value);
		void resetPosition();
		void resetRotation();
		void resetScale();
};

using TransformPtr = std::shared_ptr<Transform>;
using Transform2DPtr = std::shared_ptr<Transform2D>;

#endif // !TRANSFORM_H
