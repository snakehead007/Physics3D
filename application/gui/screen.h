#pragma once

#include "../../engine/debug.h"
#include "../../engine/profiling.h"
#include "../../engine/world.h"
#include "../../engine/math/vec2.h"
#include "../eventHandler.h"
#include "../util/properties.h"

#include "frameBuffer.h"
#include "camera.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class StandardInputHandler;

#include "indexedMesh.h"
#include "../standardInputHandler.h"

enum GraphicsProcess {
	UPDATE,
	SKYBOX,
	VECTORS,
	PHYSICALS,
	LIGHTING,
	ORIGIN,
	PROFILER,
	FINALIZE,
	OTHER,
	COUNT
};

bool initGLFW();
bool initGLEW();

void terminateGL();

class Screen {
private:
	GLFWwindow* window;
	std::vector<IndexedMesh*> meshes;

	void renderSkybox();
	void renderPhysicals();
	template<typename T>
	void renderDebugField(const char* fieldName, T value, const char* unit);
	int fieldIndex = 0;
public:
	World<ExtendedPart>* world;
	Vec2 screenSize;
	double aspect;
	Camera camera;
	EventHandler eventHandler;
	Properties properties;

	BreakdownAverageProfiler<60, GraphicsProcess> graphicsMeasure;

	FrameBuffer* modelFrameBuffer = nullptr;
	FrameBuffer* screenFrameBuffer = nullptr;

	// Picker
	Vec3 ray;
	ExtendedPart* intersectedPart = nullptr;
	Vec3 intersectedPoint;
	ExtendedPart* selectedPart = nullptr;
	Vec3 selectedPoint;

	Screen();
	Screen(int width, int height, World<ExtendedPart>* world);

	bool shouldClose();
	void init();
	void update();
	void refresh();
	void close();

	GLFWwindow* getWindow() { return window; }

	void setWorld(World<ExtendedPart>* world) { this->world = world; };

	int addMeshShape(Shape mesh);

	void toggleDebugVecType(Debug::VecType t);
};

extern bool renderPies;
extern StandardInputHandler* handler;
