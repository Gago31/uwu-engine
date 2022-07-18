#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include <vector>
#include <map>
#include <forward_list>
#include <string>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Keyboard keys

#define KEYBOARD_KEY_ESCAPE GLFW_KEY_ESCAPE
#define KEYBOARD_KEY_SPACE GLFW_KEY_SPACE
#define KEYBOARD_KEY_ENTER GLFW_KEY_ENTER
#define KEYBOARD_KEY_BACKSPACE GLFW_KEY_BACKSPACE
#define KEYBOARD_KEY_LEFT GLFW_KEY_LEFT
#define KEYBOARD_KEY_DOWN GLFW_KEY_DOWN
#define KEYBOARD_KEY_RIGHT GLFW_KEY_RIGHT
#define KEYBOARD_KEY_UP GLFW_KEY_UP
#define KEYBOARD_KEY_0 GLFW_KEY_0
#define KEYBOARD_KEY_1 GLFW_KEY_1
#define KEYBOARD_KEY_2 GLFW_KEY_2
#define KEYBOARD_KEY_3 GLFW_KEY_3
#define KEYBOARD_KEY_4 GLFW_KEY_4
#define KEYBOARD_KEY_5 GLFW_KEY_5
#define KEYBOARD_KEY_6 GLFW_KEY_6
#define KEYBOARD_KEY_7 GLFW_KEY_7
#define KEYBOARD_KEY_8 GLFW_KEY_8
#define KEYBOARD_KEY_9 GLFW_KEY_9
#define KEYBOARD_KEY_A GLFW_KEY_A
#define KEYBOARD_KEY_B GLFW_KEY_B
#define KEYBOARD_KEY_C GLFW_KEY_C
#define KEYBOARD_KEY_D GLFW_KEY_D
#define KEYBOARD_KEY_E GLFW_KEY_E
#define KEYBOARD_KEY_F GLFW_KEY_F
#define KEYBOARD_KEY_G GLFW_KEY_G
#define KEYBOARD_KEY_H GLFW_KEY_H
#define KEYBOARD_KEY_I GLFW_KEY_I
#define KEYBOARD_KEY_J GLFW_KEY_J
#define KEYBOARD_KEY_K GLFW_KEY_K
#define KEYBOARD_KEY_L GLFW_KEY_L
#define KEYBOARD_KEY_M GLFW_KEY_M
#define KEYBOARD_KEY_N GLFW_KEY_N
#define KEYBOARD_KEY_O GLFW_KEY_O
#define KEYBOARD_KEY_P GLFW_KEY_P
#define KEYBOARD_KEY_Q GLFW_KEY_Q
#define KEYBOARD_KEY_R GLFW_KEY_R
#define KEYBOARD_KEY_S GLFW_KEY_S
#define KEYBOARD_KEY_T GLFW_KEY_T
#define KEYBOARD_KEY_U GLFW_KEY_U
#define KEYBOARD_KEY_V GLFW_KEY_V
#define KEYBOARD_KEY_W GLFW_KEY_W
#define KEYBOARD_KEY_X GLFW_KEY_X
#define KEYBOARD_KEY_Y GLFW_KEY_Y
#define KEYBOARD_KEY_Z GLFW_KEY_Z

// Gamepad buttons
enum gamepad_button {
	NINTENDO_A    = GLFW_GAMEPAD_BUTTON_B,
	NINTENDO_B    = GLFW_GAMEPAD_BUTTON_A,
	NINTENDO_X    = GLFW_GAMEPAD_BUTTON_X,
	NINTENDO_Y    = GLFW_GAMEPAD_BUTTON_Y,
	XBOX_A        = GLFW_GAMEPAD_BUTTON_A,
	XBOX_B        = GLFW_GAMEPAD_BUTTON_B,
	XBOX_X        = GLFW_GAMEPAD_BUTTON_X,
	XBOX_Y        = GLFW_GAMEPAD_BUTTON_Y,
	PS_X          = GLFW_GAMEPAD_BUTTON_A,
	PS_CIRCLE     = GLFW_GAMEPAD_BUTTON_B,
	PS_SQUARE     = GLFW_GAMEPAD_BUTTON_X,
	PS_TRIANGLE   = GLFW_GAMEPAD_BUTTON_Y,
	DPAD_LEFT     = GLFW_GAMEPAD_BUTTON_DPAD_LEFT,
	DPAD_DOWN     = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
	DPAD_RIGHT    = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
	DPAD_UP       = GLFW_GAMEPAD_BUTTON_DPAD_UP,
	BUTTON_L1     = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
	BUTTON_L2     = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
	BUTTON_R1     = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
	BUTTON_R2     = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,
	BUTTON_START  = GLFW_GAMEPAD_BUTTON_START,
	BUTTON_SELECT = GLFW_GAMEPAD_BUTTON_BACK
};

// Gamepad axis
using axis_type = std::pair<int, int>;
const axis_type LEFT_STICK_LEFT   = { GLFW_GAMEPAD_AXIS_LEFT_X, -1 };
const axis_type LEFT_STICK_RIGHT  = { GLFW_GAMEPAD_AXIS_LEFT_X, 1 };
const axis_type LEFT_STICK_UP     = { GLFW_GAMEPAD_AXIS_LEFT_Y, -1 };
const axis_type LEFT_STICK_DOWN   = { GLFW_GAMEPAD_AXIS_LEFT_Y, 1 };
const axis_type RIGHT_STICK_LEFT  = { GLFW_GAMEPAD_AXIS_RIGHT_X, -1 };
const axis_type RIGHT_STICK_RIGHT = { GLFW_GAMEPAD_AXIS_RIGHT_X, 1 };
const axis_type RIGHT_STICK_UP    = { GLFW_GAMEPAD_AXIS_RIGHT_Y, -1 };
const axis_type RIGHT_STICK_DOWN  = { GLFW_GAMEPAD_AXIS_RIGHT_Y, 1 };


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
		static void setAxisBindings(std::map<std::string, axis_type> action_bindings);
		static void addListener(InputListenerPtr *listener);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void joystick_callback(int jid, int event);
		static void pollJoysticks();
		static void pollKeys(GLFWwindow* window);
		static const std::map<std::string, int> getKeyboardBindings();
		static const std::map<std::string, int> getJoystickBindings();
		static const std::map<std::string, axis_type> getAxisBindings();
	private:
		static int active_joystick;
		static float joystick_deadzone;
		static std::vector<int> joysticks;
		static std::vector<std::string> actions;
		static int buffer_size;
		static std::map<std::string, int> keyboard_bindings;
		static std::map<std::string, int> joystick_bindings;
		static std::map<std::string, axis_type> axis_bindings;
		static std::map<int, std::pair<std::string, std::string>> axis_mappings;
		static std::map<std::string, std::pair<bool, bool>> actionStates;
		static std::forward_list<InputListenerPtr*> listeners;
		InputController() { };
};

#endif // !CONTROLLER_H

