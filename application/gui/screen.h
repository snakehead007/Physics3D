#pragma once

#include "../../engine/world.h"
#include "../../engine/math/vec2.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "indexedMesh.h"

bool initGLFW();
bool initGLEW();

void terminateGL();

class Screen {
private:
	GLFWwindow* window;
	World* world;
	std::vector<IndexedMesh*> meshes;
public:
	Vec2 screenSize;
	Physical* closestIntersect = nullptr;
	Physical* selectedPhysical = nullptr;

	Screen() {};
	Screen(int width, int height, World* world);

	bool shouldClose();
	void init();
	void update();
	void refresh();
	void close();

	void makeCurrent();

	void setWorld(World* world) { this->world = world; };

	int addMeshShape(Shape mesh);
};
