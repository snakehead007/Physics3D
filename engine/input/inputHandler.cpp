#include "core.h"

#include "inputHandler.h"

#include "keyboard.h"
#include "mouse.h"

#include "event/event.h"
#include "event/keyEvent.h"
#include "event/mouseEvent.h"
#include "event/windowEvent.h"

namespace Engine {

#pragma region callbacks

void InputHandler::keyCallback(int code, int action, int mods) {
	Key key = Keyboard::getKey(code);
	Modifiers modifiers = Modifiers(mods);

	if (action == Keyboard::KEY_PRESS) {
		keys[key.getCode()] = true;
		timestamp[key.getCode()] = glfwGetTime();
		anyKey++;

		if (keys[key.getCode()] == false && glfwGetTime() - timestamp[key.getCode()] < keyInterval) {
			DoubleKeyPressEvent event(key, mods);
			onEvent(event);
		}  else {
			KeyPressEvent event(key, mods);
			onEvent(event);
		}
	} else if (action == Keyboard::KEY_RELEASE) {
		keys[key.getCode()] = false;
		anyKey--;

		KeyReleaseEvent event(key, mods);
		onEvent(event);
	} else if (action == Keyboard::KEY_REPEAT) {
		KeyPressEvent event(key, mods, true);
		onEvent(event);
	}
}

void InputHandler::cursorCallback(double x, double y) {
	MouseMoveEvent event(mousePosition.x, mousePosition.y, x, y);
	onEvent(event);

	if (leftDragging) {
		MouseDragEvent event(mousePosition.x, mousePosition.y, x, y, Mouse::LEFT, Modifiers(0));
		onEvent(event);
	}

	if (middleDragging) {
		MouseDragEvent event(mousePosition.x, mousePosition.y, x, y, Mouse::MIDDLE, Modifiers(0));
		onEvent(event);
	}

	if (rightDragging) {
		MouseDragEvent event(mousePosition.x, mousePosition.y, x, y, Mouse::RIGHT, Modifiers(0));
		onEvent(event);
	}

	mousePosition = Vec2(x, y);
}

void InputHandler::cursorEnterCallback(int entered) {
	if (entered) {
		MouseEnterEvent event(mousePosition.x, mousePosition.y);
		onEvent(event);
	} else {
		leftDragging = false;
		middleDragging = false;
		rightDragging = false;

		MouseExitEvent event(mousePosition.x, mousePosition.y);
		onEvent(event);
	}
}

void InputHandler::scrollCallback(double xOffset, double yOffset) {
	MouseScrollEvent event(mousePosition.x, mousePosition.y, xOffset, yOffset);
	onEvent(event);
}

void InputHandler::mouseButtonCallback(int button, int action, int mods) {
	if (action == Mouse::PRESS) {
		if (Mouse::RIGHT == button) rightDragging = true;
		if (Mouse::MIDDLE == button) middleDragging = true;
		if (Mouse::LEFT == button) leftDragging = true;

		MousePressEvent event(mousePosition.x, mousePosition.y, Mouse::getButton(button), Modifiers(mods));
		onEvent(event);

	} else if (action == Mouse::RELEASE) {
		if (Mouse::RIGHT == button) rightDragging = false;
		if (Mouse::MIDDLE == button) middleDragging = false;
		if (Mouse::LEFT == button) leftDragging = false;

		MouseReleaseEvent event(mousePosition.x, mousePosition.y, Mouse::getButton(button), Modifiers(mods));
		onEvent(event);
	}
}

void InputHandler::windowSizeCallback(int width, int height) {
	WindowResizeEvent event(width, height);
	onEvent(event);
}

void InputHandler::windowDropCallback(const char* path) {
	std::string stdPath(path);
	WindowDropEvent event(stdPath);
	onEvent(event);
}

void InputHandler::framebufferSizeCallback(int width, int height) {
	FrameBufferResizeEvent event(width, height);
	onEvent(event);
}

#pragma endregion

Vec2 InputHandler::getMousePosition() const {
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	return Vec2(x, y);
}

bool InputHandler::getKey(const Key& key) const {
	if (key.getCode() < Keyboard::KEY_FIRST || key.getCode() > Keyboard::KEY_LAST)
		return false;

	return keys[key.getCode()];
}

void InputHandler::setKey(const Key& key, bool pressed) {
	if (key.getCode() < Keyboard::KEY_FIRST || key.getCode() > Keyboard::KEY_LAST)
		return;
	
	keys[key.getCode()] = pressed;
}

InputHandler::InputHandler(GLFWwindow* window) : window(window) {
	for(bool& k : this->keys){
		k = false;
	}


	glfwSetWindowUserPointer(window, this);

	glfwSetKeyCallback(window, [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
		static_cast<InputHandler*>(glfwGetWindowUserPointer(window))->keyCallback(key, action, mods);
	});

	glfwSetCursorPosCallback(window, [] (GLFWwindow* window, double x, double y) {
		static_cast<InputHandler*>(glfwGetWindowUserPointer(window))->cursorCallback(x, y);
	});

	glfwSetCursorEnterCallback(window, [] (GLFWwindow* window, int entered) {
		static_cast<InputHandler*>(glfwGetWindowUserPointer(window))->cursorEnterCallback(entered);
	});

	glfwSetScrollCallback(window, [] (GLFWwindow* window, double xOffset, double yOffset) {
		static_cast<InputHandler*>(glfwGetWindowUserPointer(window))->scrollCallback(xOffset, yOffset);
	});

	glfwSetMouseButtonCallback(window, [] (GLFWwindow* window, int button, int action, int mods) {
		static_cast<InputHandler*>(glfwGetWindowUserPointer(window))->mouseButtonCallback(button, action, mods);
	});

	glfwSetWindowSizeCallback(window, [] (GLFWwindow* window, int width, int height) {
		static_cast<InputHandler*>(glfwGetWindowUserPointer(window))->windowSizeCallback(width, height);
	});

	glfwSetFramebufferSizeCallback(window, [] (GLFWwindow* window, int width, int height) {
		static_cast<InputHandler*>(glfwGetWindowUserPointer(window))->framebufferSizeCallback(width, height);
	});

	glfwSetDropCallback(window, [] (GLFWwindow* window, int count, const char** paths) {
		InputHandler* inputHandler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
		for (int i = 0; i < count; i++)
			inputHandler->windowDropCallback(paths[i]);
	});
}

};