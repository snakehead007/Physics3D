#pragma once

#include "../../engine/math/mat4.h"
#include "../../engine/math/vec4.h"
#include "../../engine/math/vec3.h"
#include "../../engine/math/vec2.h"
#include "../../engine/physical.h"

#include <vector>

Vec2 getNormalizedDeviceSpacePosition(Vec2 viewportSpacePosition, Vec2 screenSize);
Vec3 calcRay(Vec2 mousePosition, Vec2 screenSize, Mat4f viewMatrix, Mat4f projectionMatrix);
Physical* getIntersectedPhysical(std::vector<Physical>& physicals, Vec3 cameraPosition, Vec2 mousePosition, Vec2 screenSize, Mat4f rotatedViewMatrix, Mat4f projectionMatrix);