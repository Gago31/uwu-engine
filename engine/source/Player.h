#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include "Camera.h"
#include "Controller.h"

class Player {
	public:
		CameraPtr camera;
		InputListenerPtr Input;
		glm::vec3 position;
		Player(glm::vec3 pos);
		Player(float x, float y, float z);
		void setInputListener(InputListenerPtr listener);
		void setCamera(CameraPtr cam);
		void update(float delta);
};

#endif // !PLAYER_H

