#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include <vector>
#include <map>
#include <forward_list>
#include <string>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


enum input_process_state { ACTIVE, LOCKED, BUFFERING };

class InputListener {
	public:
		InputListener(bool _passthrough=false) : passthrough(_passthrough) { }
		InputListener(std::vector<std::string> _actions, bool _passthrough=false);
		std::vector<std::string> actions;
		const bool isPassthrough() const;
		bool action(std::string action);
		bool actionPressed(std::string action);
		bool actionReleased(std::string action);
		bool actionHeld(std::string action);
		float actionStrength(std::string action);
		void setActionState(std::string action, bool current);
		void setActionState(std::string action, bool prev, bool current);
		void setActionState(std::string action, std::pair<bool, bool> value);
		void setActionStrength(std::string action, float value);
		const std::vector<std::string> getActions() const;
	private:
		bool passthrough;
		std::map<std::string, std::pair<bool, bool>> actionStates;
		std::map<std::string, float> actionStrengths;
};

using InputListenerPtr = std::shared_ptr<InputListener>;

class InputController {
	public:
		static void initialize(float deadzone=0.5f);
		static const std::vector<std::string> getActions();
		static void setKeyboardBinding(std::string action, int binding);
		static void setJoystickBinding(std::string action, int binding);
		static void setKeyboardBindings(std::map<std::string, int> action_bindings);
		static void setJoystickBindings(std::map<std::string, int> action_bindings);
		static void setAxisBinding(std::string action_negative, std::string action_positive, int joy_axis);
		static void setAxisBindings(std::map<std::string, std::pair<int, int>> action_bindings);
		static void addListener(InputListenerPtr *listener);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void joystick_callback(int jid, int event);
		static void pollJoysticks();
		static const std::map<std::string, int> getKeyboardBindings();
		static const std::map<std::string, int> getJoystickBindings();
		static const std::map<std::string, std::pair<int, int>> getAxisBindings();
	private:
		static int active_joystick;
		static float joystick_deadzone;
		static std::vector<int> joysticks;
		static std::vector<std::string> actions;
		static int buffer_size;
		static std::map<std::string, int> keyboard_bindings;
		static std::map<std::string, int> joystick_bindings;
		static std::map<std::string, std::pair<int, int>> axis_bindings;
		static std::map<int, std::pair<std::string, std::string>> axis_mappings;
		static std::map<std::string, std::pair<bool, bool>> actionStates;
		static std::forward_list<InputListenerPtr*> listeners;
		InputController() { };
};

#endif // !CONTROLLER_H

