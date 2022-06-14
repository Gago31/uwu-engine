#include "InputController.h"
#include <ranges>
#include <iostream>
#include <set>
#include <algorithm>
#include "GameController.h"

// InputListener

InputListener::InputListener(std::vector<std::string> _actions, bool _passthrough) {
	actions = _actions;
	passthrough = _passthrough;
	for (std::string action : actions) {
		actionStates[action] = { false, false };
		actionStrengths[action] = 0.0f;
	}
	//InputController::addListener(std::make_shared<InputListener>(*this));
}

const bool InputListener::isPassthrough() const {
	return passthrough;
}

bool InputListener::action(std::string action) {
	return actionStates[action].second;
}

bool InputListener::actionPressed(std::string action) {
	return actionStates[action].second && !actionStates[action].second;
}

bool InputListener::actionReleased(std::string action) {
	return !actionStates[action].second && actionStates[action].second;
}

bool InputListener::actionHeld(std::string action) {
	return actionStates[action].second && actionStates[action].second;
}

void InputListener::setActionState(std::string action, bool current) {
	actionStates[action].first = actionStates[action].second;
	actionStates[action].second = current;
}

float InputListener::actionStrength(std::string action) {
	//std::cout << "request action " << action << ": " << actionStrengths[action] << std::endl;
	return actionStrengths[action];
}

void InputListener::setActionState(std::string action, bool prev, bool current) {
	actionStates[action] = { prev, current };
}

void InputListener::setActionState(std::string action, std::pair<bool, bool> value) {
	actionStates[action] = value;
}

void InputListener::setActionStrength(std::string action, float value) {
	actionStrengths[action] = value;
	//std::cout << "set action " << action << ": " << actionStrength(action) << std::endl;
}

const std::vector<std::string> InputListener::getActions() const {
	return actions;
}

// InputController

std::vector<std::string> InputController::actions;
std::map<std::string, int> InputController::keyboard_bindings;
std::map<std::string, int> InputController::joystick_bindings;
std::map<std::string, std::pair<int, int>> InputController::axis_bindings;
std::map<int, std::pair<std::string, std::string>> InputController::axis_mappings;
std::map<std::string, std::pair<bool, bool>> InputController::actionStates;
std::forward_list<InputListenerPtr*> InputController::listeners;
std::vector<int> InputController::joysticks;
int InputController::active_joystick = -1;
float InputController::joystick_deadzone = 0.0f;


void InputController::initialize(float deadzone) {
	active_joystick = -1;
	joystick_deadzone = deadzone;

	for (int i = GLFW_JOYSTICK_1; i < GLFW_JOYSTICK_LAST; i++) {
		if (glfwJoystickIsGamepad(i)) {
			std::cout << "Joystick found" << std::endl;
			joysticks.push_back(i);
			if (active_joystick == -1) {
				active_joystick = i;
				std::cout << "Joystick set (" << active_joystick << ")" << std::endl;
			}
		}
	}
}

const std::vector<std::string> InputController::getActions() {
	return actions;
}

void InputController::setKeyboardBinding(std::string action, int binding) {
	keyboard_bindings[action] = binding;
}

void InputController::setJoystickBinding(std::string action, int binding) {
	joystick_bindings[action] = binding;
}

void InputController::setKeyboardBindings(std::map<std::string, int> action_bindings) {
	keyboard_bindings = action_bindings;
	std::cout << "keyboard actions set" << std::endl;
}

void InputController::setJoystickBindings(std::map<std::string, int> action_bindings) {
	joystick_bindings = action_bindings;
	std::cout << "joystick actions set" << std::endl;
}

void InputController::setAxisBinding(std::string action_negative, std::string action_positive, int joy_axis) {
	axis_bindings[action_negative] = { joy_axis, -1 };
	axis_bindings[action_positive] = { joy_axis, 1 };
	joystick_bindings[action_negative] = joy_axis;
	joystick_bindings[action_positive] = joy_axis;
	axis_mappings[joy_axis] = { action_negative, action_positive };
}

void InputController::setAxisBindings(std::map<std::string, std::pair<int, int>> action_bindings) {
	axis_bindings = action_bindings;
	std::cout << "axis actions set" << std::endl;
}

void InputController::addListener(InputListenerPtr *listener) {
	listeners.emplace_front(listener);
	std::cout << "input listener added" << std::endl;
}

void InputController::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_REPEAT) return;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		return;
	} else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		GameController::fillPolygon = not GameController::fillPolygon;
		return;
	}

	bool consumed = false;
	for (InputListenerPtr *l: listeners) {
		InputListenerPtr listener = *l;
		for (std::string game_action : listener->getActions()) {
			if (key == keyboard_bindings[game_action]) {
				//std::cout << "action: " << game_action << std::endl;
				consumed = true;
				bool action_pressed = action == GLFW_PRESS;
				listener->setActionState(game_action, action_pressed);
				listener->setActionStrength(game_action, action_pressed ? 1.0f : 0.0f);
				break;
			}
		}
		if (consumed || !listener->isPassthrough()) break;
	}
}

void InputController::joystick_callback(int jid, int event) {
	if (event == GLFW_CONNECTED && glfwJoystickIsGamepad(jid)) {
		std::cout << "Joystick connected" << std::endl;
		if (active_joystick == -1) {
			active_joystick = jid;
			std::cout << "Joystick set (" << active_joystick << ")" << std::endl;
		}
		joysticks.push_back(jid);
	} else if (event == GLFW_DISCONNECTED && glfwJoystickIsGamepad(jid)) {
		std::cout << "Joystick disconnected" << std::endl;
		joysticks.erase(std::remove(joysticks.begin(), joysticks.end(), jid), joysticks.end());
		if (active_joystick == jid) {
			active_joystick = joysticks.empty() ? -1 : joysticks[0];
			std::cout << "Joystick set (" << active_joystick << ")" << std::endl;
		}
	}
}

void InputController::pollJoysticks() {
	if (active_joystick == -1 || !glfwJoystickIsGamepad(active_joystick)) return;

	GLFWgamepadstate state;
	if (glfwGetGamepadState(active_joystick, &state)) {
		bool consumed = false;
		for (InputListenerPtr* l : listeners) {
			const InputListenerPtr listener = *l;
			for (std::string game_action : listener->getActions()) {
				int event = joystick_bindings[game_action];
				//consumed = true;
				if (axis_bindings.count(game_action)) {
					float action_strength = std::abs(state.axes[event]) < joystick_deadzone ? state.axes[event] : 0.0f;
					consumed = action_strength > 0.0f;
					//float action_strength = state.axes[event <= GLFW_GAMEPAD_AXIS_LAST ? event : GLFW_GAMEPAD_AXIS_LAST];
					listener->setActionState(axis_mappings[event].first, action_strength < 0.0f);
					listener->setActionState(axis_mappings[event].second, action_strength > 0.0f);
					listener->setActionStrength(axis_mappings[event].first, action_strength < 0.0f ? abs(action_strength) : 0.0f);
					listener->setActionStrength(axis_mappings[event].second, action_strength > 0.0f ? action_strength : 0.0f);
				} else {
					consumed = state.buttons[event];
					bool action_pressed = state.buttons[event];
					//bool action_pressed = state.buttons[event <= GLFW_GAMEPAD_BUTTON_LAST ? event : GLFW_GAMEPAD_BUTTON_LAST];
					listener->setActionState(game_action, action_pressed);
					listener->setActionStrength(game_action, action_pressed ? 1.0f : 0.0f);
				}
				if (consumed) break;
			}
			if (consumed || !listener->isPassthrough()) break;
		}
	}
	
}



const std::map<std::string, int> InputController::getKeyboardBindings() {
	return keyboard_bindings;
}

const std::map<std::string, int> InputController::getJoystickBindings() {
	return joystick_bindings;
}

const std::map<std::string, std::pair<int, int>> InputController::getAxisBindings() {
	return axis_bindings;
}