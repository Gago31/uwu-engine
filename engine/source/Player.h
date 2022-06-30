#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include "Camera.h"
#include "Node.h"
#include "InputController.h"

class Player : public Node {
	public:
		CameraPtr camera;
		InputListenerPtr Input;
		glm::vec3 position;
		bool accept_input = true;
		float timer = 0.0f;
		float theta = 0.0f;
		float h_axis = 0.0f;
		float v_axis = 0.0f;
		bool a = true;
		Player(glm::vec3 pos, float theta_d = 0.0f);
		Player(float x, float y, float z, float theta_d = 0.0f);
		void setInputListener(InputListenerPtr listener);
		void setCamera(CameraPtr cam);
		void update(float delta) override;
		void turnEnd() override;
};

#endif // !PLAYER_H

