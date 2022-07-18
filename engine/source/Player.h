#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include "Camera.h"
#include "Node.h"
#include "InputController.h"
#include "GameController.h"

class Player : public Node {
	public:
		inline static std::string className = "Player";
		CameraPtr camera;
		InputListenerPtr Input;
		glm::vec3 position;
		bool accept_input = true;
		float timer = 0.0f;
		float theta = 0.0f;
		float h_axis = 0.0f;
		float v_axis = 0.0f;
		bool a = true;
		Player(glm::vec3 pos = { 0.0f, 0.0f, 0.0f }, float theta_d = 0.0f);
		Player(float x, float y, float z, float theta_d = 0.0f);
		void setInputListener(InputListenerPtr listener);
		void setCamera(CameraPtr cam);
		void update(float delta) override;
		void turnEnd() override;
		std::string getClassName() override {
			return className;
		}
		json serialize() override {
			json j = Node::serialize();
			j["position"] = { position.x, position.y, position.z };
			j["theta"] = theta;
			j["actions"] = Input->getActions();
			return j;
		}
		void deserialize(json& j) override {
			Node::deserialize(j);
			position = { j["position"][0], j["position"][1], j["position"][2] };
			theta = j["theta"];
			std::vector<std::string> actions = j["actions"];
			Input->actions = actions;
			camera->setPos(position);
			camera->theta = theta;
			camera->setAspectRatio(GameController::SCREEN_RATIO);
			camera->makeCurrent();
		}
};

#endif // !PLAYER_H

