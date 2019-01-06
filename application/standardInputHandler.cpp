#include "standardInputHandler.h"
#include "../engine/math/mathUtil.h"
#include "application.h"

StandardInputHandler::StandardInputHandler(GLFWwindow* window, Screen* screen, Camera* camera) : InputHandler(window) {
	this->screen = screen;
	this->camera = camera;
}

void StandardInputHandler::framebufferResize(int width, int height) {
	/*int left, top, right, bottom;
	glfwGetWindowFrameSize(window, &left, &top, &right, &bottom);
	glViewport(left, bottom, width - right, height - top);*/
	glViewport(0, 0, width, height);
	screen->screenSize = Vec2(width, height);
}

void StandardInputHandler::keyDownOrRepeat(int key, int modifiers) {
	switch (key) {
	case GLFW_KEY_PAGE_UP:
		setSpeed(getSpeed() * 1.5);
		Log::info("TPS is now: %f", getSpeed());
		break;
	case GLFW_KEY_PAGE_DOWN:
		setSpeed(getSpeed() / 1.5);
		Log::info("TPS is now: %f", getSpeed());
		break;
	case GLFW_KEY_T:
		runTick();
		break;
	}
}

void StandardInputHandler::keyDown(int key, int modifiers) {
	if (key == GLFW_KEY_P) {
		togglePause();
	}
};
void StandardInputHandler::keyUp(int key, int modifiers) {};
void StandardInputHandler::keyRepeat(int key, int modifiers) {};
// void StandardInputHandler::mouseEnter() {};

void StandardInputHandler::mouseDown(int button, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT) rightDragging = true;
	if (button == GLFW_MOUSE_BUTTON_LEFT) leftDragging = true;
};

void StandardInputHandler::mouseUp(int button, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT) rightDragging = false;
	if (button == GLFW_MOUSE_BUTTON_LEFT) leftDragging = false;

	screen->selectedPhysical = screen->closestIntersect;
};

void StandardInputHandler::mouseMove(double x, double y) {
	if (rightDragging) {
		camera->rotate((y - curPos.y) * 0.5, (x - curPos.x) * 0.5, 0);
	}

	if (leftDragging) {
		camera->move((x - curPos.x) * -0.5, (y - curPos.y) * 0.5, 0);
	}

	curPos = Vec2(x, y);
};

void StandardInputHandler::scroll(double xOffset, double yOffset) {
	camera->move(0, 0, -5 * yOffset);
};

void StandardInputHandler::mouseExit() {
	rightDragging = false;
	leftDragging = false;
};
