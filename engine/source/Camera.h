#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <memory>


class Camera {
	public:
		Camera(glm::vec3 pos, float theta_ = 0.0f, float aspect_ratio = 1.0f);
        void update();
        float theta = 0.0f;
        float phi = 0.0f;
        float far;
        float near;
        void setPos(glm::vec3 new_pos);
        void setPos(float x, float y, float z);
        void setAspectRatio(float new_ratio);
        void makeCurrent();
        const glm::vec3 getPos() const;
        glm::mat4 getView();
        glm::mat4 getProjection();
        glm::vec2 getDirection();
	private:
		glm::vec3 viewPos = { 0.0f, 0.0f, 0.0f };
		glm::vec3 eye = { 0.0f, 0.0f, 0.0f };
		glm::vec3 at = { 0.0f, 0.0f, 0.0f };
        glm::vec3 up = { 0.0f, 0.0f, 1.0f };
        float approxTrig(float value);
        float ratio;
};

using CameraPtr = std::shared_ptr<Camera>;

#endif // !CAMERA_H

