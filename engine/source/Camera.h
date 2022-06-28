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
        float far = 20.0f;
        float near = 0.0f;
        void setPos(glm::vec3 new_pos);
        void setPos(float x, float y, float z);
        void setNextState(glm::vec3 next_pos, float next_theta);
        void setAspectRatio(float new_ratio);
        void makeCurrent();
        const glm::vec3 getPos() const;
        const glm::vec3 getNextPos() const;
        glm::mat4 getView();
        glm::mat4 getProjection();
        glm::vec2 getDirection();
	private:
		glm::vec3 viewPos = { 0.0f, 0.0f, 0.0f };
        glm::vec3 nextPos;
        float nextTheta;
		glm::vec3 eye = { 0.0f, 0.0f, 0.0f };
		glm::vec3 at = { 0.0f, 0.0f, 0.0f };
        glm::vec3 up = { 0.0f, 0.0f, 1.0f };
        float approxTrig(float value);
        float ratio;
};

using CameraPtr = std::shared_ptr<Camera>;

#endif // !CAMERA_H

