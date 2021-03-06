#pragma once

#include "fix.h"
#include "linalg/vec.h"

typedef Vector<Fix<32>, 3> Vec3Fix;

struct Position {
	Fix<32> x;
	Fix<32> y;
	Fix<32> z;
	inline Position() : x(), y(), z() {}
	inline Position(Fix<32> x, Fix<32> y, Fix<32> z) : x(x), y(y), z(z) {}
	inline Position(double x, double y, double z) : x(Fix<32>(x)), y(Fix<32>(y)), z(Fix<32>(z)) {}
};

inline Vec3Fix operator-(const Position& a, const Position& b) {
	return Vec3Fix(a.x - b.x, a.y - b.y, a.z - b.z);
}
inline Position operator+(const Position& a, const Vec3Fix& b) {
	return Position(a.x + b.x, a.y + b.y, a.z + b.z);
}
inline Position operator-(const Position& a, const Vec3Fix& b) {
	return Position(a.x - b.x, a.y - b.y, a.z - b.z);
}
inline void operator+=(Position& pos, const Vec3Fix& offset) {
	pos.x += offset.x;
	pos.y += offset.y;
	pos.z += offset.z;
}
inline void operator-=(Position& pos, const Vec3Fix& offset) {
	pos.x -= offset.x;
	pos.y -= offset.y;
	pos.z -= offset.z;
}

inline bool operator==(const Position& first, const Position& second) {return first.x == second.x && first.y == second.y && first.z == second.z;}
inline bool operator!=(const Position& first, const Position& second) { return first.x != second.x || first.y != second.y || first.z != second.z; }

inline Position max(const Position& p1, const Position& p2) {
	return Position(max<32>(p1.x, p2.x), max<32>(p1.y, p2.y), max<32>(p1.z, p2.z));
}
inline Position min(const Position& p1, const Position& p2) {
	return Position(min<32>(p1.x, p2.x), min<32>(p1.y, p2.y), min<32>(p1.z, p2.z));
}
inline Position avg(const Position& first, const Position& second) {
	Vec3Fix delta = second - first;
	return first + Vec3Fix(delta.x >> int64_t(1), delta.y >> int64_t(1), delta.z >> int64_t(1));
}

// cast Vec3 to Position, not recommended due to potential loss of precision
inline Position castVec3ToPosition(const Vec3& vec) {
	return Position(vec.x, vec.y, vec.z);
}
// case Position to Vec3, not recommended due to potential loss of precision
// If possible, compute relative positions, eg: pos1-pos2 = vec3 delta
inline Vec3 castPositionToVec3(const Position& pos) {
	return Vec3(pos.x, pos.y, pos.z);
}

// unconventional operator, compares xyz individually, returns true if all are true
inline bool operator>=(const Position& first, const Position& second) { return first.x >= second.x && first.y >= second.y && first.z >= second.z; }
// unconventional operator, compares xyz individually, returns true if all are true
inline bool operator<=(const Position& first, const Position& second) { return first.x <= second.x && first.y <= second.y && first.z <= second.z; }
// unconventional operator, compares xyz individually, returns true if all are true
inline bool operator>(const Position& first, const Position& second) { return first.x > second.x && first.y > second.y && first.z > second.z; }
// unconventional operator, compares xyz individually, returns true if all are true
inline bool operator<(const Position& first, const Position& second) { return first.x < second.x&& first.y < second.y&& first.z < second.z; }
