#pragma once

#include "../engine/math/mat3.h"
#include "../engine/math/mat4.h"
#include "../engine/math/vec2.h"
#include "../engine/math/vec3.h"
#include "../engine/math/vec4.h"

template<typename Num1, typename Num2, typename Tol>
bool tolerantEquals(const Num1& first, const Num2& second, Tol tolerance) {
	auto diff = first - second;
	return diff <= tolerance && -diff <= tolerance;
}

template<typename Num1, typename Num2, typename Tol>
bool tolerantNotEquals(const Num1& first, const Num2& second, Tol tolerance) {
	auto diff = first - second;
	return diff > tolerance && -diff > tolerance;
}

template<typename Num1, typename Num2, typename Tol>
bool tolerantLessThan(const Num1& first, const Num2& second, Tol tolerance) {
	return first < second + tolerance;
}

template<typename Num1, typename Num2, typename Tol>
bool tolerantGreaterThan(const Num1& first, const Num2& second, Tol tolerance) {
	return first + tolerance > second;
}

template<typename Num1, typename Num2, typename Tol>
bool tolerantLessOrEqual(const Num1& first, const Num2& second, Tol tolerance) {
	return first <= second + tolerance;
}

template<typename Num1, typename Num2, typename Tol>
bool tolerantGreaterOrEqual(const Num1& first, const Num2& second, Tol tolerance) {
	return first + tolerance >= second;
}


template<typename Num1, typename Num2, typename Tol>
bool tolerantEquals(const Mat3Template<Num1>& first, const Mat3Template<Num2>& second, Tol tolerance) {
	for (int i = 0; i < 9; i++)
		if (!tolerantEquals(first.m[i], second.m[i], tolerance))
			return false;
	return true;
}
template<typename Num1, typename Num2, typename Tol>
bool tolerantNotEquals(const Mat3Template<Num1>& first, const Mat3Template<Num2>& second, Tol tolerance) {
	for (int i = 0; i < 9; i++)
		if (tolerantNotEquals(first.m[i], second.m[i], tolerance))
			return true;
	return false;
}

template<typename Num1, typename Num2, typename Tol>
bool tolerantEquals(const Mat4Template<Num1>& first, const Mat4Template<Num2>& second, Tol tolerance) {
	for (int i = 0; i < 16; i++)
		if (!tolerantEquals(first.m[i], second.m[i], tolerance))
			return false;
	return true;
}
template<typename Num1, typename Num2, typename Tol>
bool tolerantNotEquals(const Mat4Template<Num1>& first, const Mat4Template<Num2>& second, Tol tolerance) {
	for (int i = 0; i < 16; i++)
		if (tolerantNotEquals(first.m[i], second.m[i], tolerance))
			return true;
	return false;
}

template<typename Num1, typename Num2, typename Tol>
bool tolerantEquals(const Vec2Template<Num1>& first, const Vec2Template<Num2>& second, Tol tolerance) {
	return tolerantEquals(first.x, second.x, tolerance) &&
		tolerantEquals(first.y, second.y, tolerance);
}
template<typename Num1, typename Num2, typename Tol>
bool tolerantNotEquals(const Vec2Template<Num1>& first, const Vec2Template<Num2>& second, Tol tolerance) {
	return tolerantNotEquals(first.x, second.x, tolerance) ||
		tolerantNotEquals(first.y, second.y, tolerance);
}

template<typename Num1, typename Num2, typename Tol>
bool tolerantEquals(const Vec3Template<Num1>& first, const Vec3Template<Num2>& second, Tol tolerance) {
	return tolerantEquals(first.x, second.x, tolerance) &&
		tolerantEquals(first.y, second.y, tolerance) &&
		tolerantEquals(first.z, second.z, tolerance);
}
template<typename Num1, typename Num2, typename Tol>
bool tolerantNotEquals(const Vec3Template<Num1>& first, const Vec3Template<Num2>& second, Tol tolerance) {
	return tolerantNotEquals(first.x, second.x, tolerance) ||
		tolerantNotEquals(first.y, second.y, tolerance) ||
		tolerantNotEquals(first.z, second.z, tolerance);
}

template<typename Num1, typename Num2, typename Tol>
bool tolerantEquals(const Vec4Template<Num1>& first, const Vec4Template<Num2>& second, Tol tolerance) {
	return tolerantEquals(first.x, second.x, tolerance) &&
		tolerantEquals(first.y, second.y, tolerance) &&
		tolerantEquals(first.z, second.z, tolerance) &&
		tolerantEquals(first.w, second.w, tolerance);
}
template<typename Num1, typename Num2, typename Tol>
bool tolerantNotEquals(const Vec4Template<Num1>& first, const Vec4Template<Num2>& second, Tol tolerance) {
	return tolerantNotEquals(first.x, second.x, tolerance) ||
		tolerantNotEquals(first.y, second.y, tolerance) ||
		tolerantNotEquals(first.z, second.z, tolerance) ||
		tolerantNotEquals(first.w, second.w, tolerance);
}