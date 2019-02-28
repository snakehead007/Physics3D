#include "shape.h"

#include <stdlib.h>
#include <cstring>
#include <vector>
#include <math.h>

#include "../math/utils.h"

#include "../../util/Log.h"

bool Triangle::sharesEdgeWith(Triangle other) const {
	return firstIndex == other.secondIndex && secondIndex == other.firstIndex ||
		firstIndex == other.thirdIndex && secondIndex == other.secondIndex ||
		firstIndex == other.firstIndex && secondIndex == other.thirdIndex || 

		secondIndex == other.secondIndex && thirdIndex == other.firstIndex ||
		secondIndex == other.thirdIndex && thirdIndex == other.secondIndex ||
		secondIndex == other.firstIndex && thirdIndex == other.thirdIndex ||

		thirdIndex == other.secondIndex && firstIndex == other.firstIndex ||
		thirdIndex == other.thirdIndex && firstIndex == other.secondIndex ||
		thirdIndex == other.firstIndex && firstIndex == other.thirdIndex;
}

Triangle Triangle::operator~() const {
	return Triangle { firstIndex, thirdIndex, secondIndex };
}

bool Triangle::operator==(const Triangle & other) const {
	return firstIndex == other.firstIndex && secondIndex == other.secondIndex && thirdIndex == other.thirdIndex || 
		firstIndex == other.secondIndex && secondIndex == other.thirdIndex && thirdIndex == other.firstIndex ||
		firstIndex == other.thirdIndex && secondIndex == other.firstIndex && thirdIndex == other.secondIndex;
}

Triangle Triangle::rightShift() const { 
	return Triangle { thirdIndex, firstIndex, secondIndex };
}

Triangle Triangle::leftShift() const {
	return Triangle { secondIndex, thirdIndex, firstIndex };
}

Shape::Shape() : vertices(nullptr), triangles(nullptr), vertexCount(0), triangleCount(0) {}

Shape::Shape(Vec3* vertices, const Triangle* triangles, int vertexCount, int triangleCount) : vertices(vertices), triangles(triangles), vertexCount(vertexCount), triangleCount(triangleCount) {
	normals = getNormals();
}

Shape::Shape(Vec3* vertices, Vec3* normals, const Triangle* triangles, int vertexCount, int triangleCount) : vertices(vertices), normals(normals), triangles(triangles), vertexCount(vertexCount), triangleCount(triangleCount) {}

NormalizedShape Shape::normalized(Vec3* vecBuf, CFrame& backTransformation) const {
	backTransformation = getInertialEigenVectors();

	for (int i = 0; i < vertexCount; i++) {
		vecBuf[i] = backTransformation.globalToLocal(vertices[i]);
	}
	
	return NormalizedShape(vecBuf, triangles, vertexCount, triangleCount);
}

CenteredShape Shape::centered(Vec3* vecBuf, Vec3& backOffset) const {
	backOffset = getCenterOfMass();

	for(int i = 0; i < vertexCount; i++) {
		vecBuf[i] = vertices[i] - backOffset;
	}

	return CenteredShape(vecBuf, triangles, vertexCount, triangleCount);
}

CFrame Shape::getInertialEigenVectors() const {
	Vec3 centerOfMass = getCenterOfMass();
	Mat3 inertia = getInertia(centerOfMass);
	Mat3 basis = inertia.getEigenDecomposition().eigenVectors;

	return CFrame(centerOfMass, basis);
}

NormalizedShape::NormalizedShape(Vec3 * vertices, const Triangle * triangles, int vertexCount, int triangleCount) : CenteredShape(vertices, triangles, vertexCount, triangleCount) {
	// TODO add normalization verification
};
/*
	Creates a normalized shape

	Modifies `vertices`
*/
NormalizedShape::NormalizedShape(Vec3 * vertices, const Triangle * triangles, int vertexCount, int triangleCount, CFrame& transformation) : CenteredShape(vertices, triangles, vertexCount, triangleCount) {
	this->normalized(vertices, transformation);
}

CenteredShape::CenteredShape(Vec3 * vertices, const Triangle * triangles, int vertexCount, int triangleCount) : Shape(vertices, triangles, vertexCount, triangleCount) {
	// TODO add centering verification
};

CenteredShape::CenteredShape(Vec3 * vertices, const Triangle * triangles, int vertexCount, int triangleCount, Vec3& offset) : Shape(vertices, triangles, vertexCount, triangleCount) {
	this->centered(vertices, offset);
}

Shape Shape::translated(Vec3 offset, Vec3 * newVecBuf) const {
	for (int i = 0; i < this->vertexCount; i++) {
		newVecBuf[i] = offset + vertices[i];
	}

	return Shape(newVecBuf, triangles, vertexCount, triangleCount);
}

Shape Shape::rotated(RotMat3 rotation, Vec3* newVecBuf) const {
	for (int i = 0; i < this->vertexCount; i++) {
		newVecBuf[i] = rotation * vertices[i];
	}

	return Shape(newVecBuf, triangles, vertexCount, triangleCount);
}

Shape Shape::localToGlobal(CFrame frame, Vec3* newVecBuf) const {
	for (int i = 0; i < this->vertexCount; i++) {
		newVecBuf[i] = frame.localToGlobal(vertices[i]);
	}

	return Shape(newVecBuf, triangles, vertexCount, triangleCount);
}

Shape Shape::globalToLocal(CFrame frame, Vec3* newVecBuf) const {
	for (int i = 0; i < this->vertexCount; i++) {
		newVecBuf[i] = frame.globalToLocal(vertices[i]);
	}

	return Shape(newVecBuf, triangles, vertexCount, triangleCount);
}

BoundingBox Shape::getBounds() const {
	double xmin = vertices[0].x, xmax = vertices[0].x;
	double ymin = vertices[0].y, ymax = vertices[0].y;
	double zmin = vertices[0].z, zmax = vertices[0].z;

	for (int i = 1; i < vertexCount; i++) {
		const Vec3 current = vertices[i];

		if (current.x < xmin) xmin = current.x;
		if (current.x > xmax) xmax = current.x;
		if (current.y < ymin) ymin = current.y;
		if (current.y > ymax) ymax = current.y;
		if (current.z < zmin) zmin = current.z;
		if (current.z > zmax) zmax = current.z;
	}

	return BoundingBox { xmin, ymin, zmin, xmax, ymax, zmax };
}

// for every edge, of every triangle, check that it coincides with exactly one other triangle, in reverse order
bool isComplete(const Triangle* triangles, int triangleCount) {
	for (int i = 0; i < triangleCount; i++) {
		Triangle a = triangles[i];
		
		for (int j = 0; j < triangleCount; j++) {
			if (j == i) continue;

			Triangle b = triangles[j];

			if (a.sharesEdgeWith(b)) {  // correctly oriented
				goto endOfLoop;
			} else if (a.sharesEdgeWith(~b)) {	// wrongly oriented
				Log::warn("triangles[%d](%d, %d, %d) and triangles[%d](%d, %d, %d) are joined wrongly", i, a.firstIndex, a.secondIndex, a.thirdIndex, j, b.firstIndex, b.secondIndex, b.thirdIndex);
				return false;
			}
		}
		Log::warn("No triangle found that shares an edge with triangles[%d]", i);
		return false;
		endOfLoop:;
	}
	return true;
}

bool Shape::isValid() const {
	return isComplete(triangles, triangleCount);
}

Vec3 Shape::getNormalVecOfTriangle(Triangle triangle) const {
	Vec3 v0 = vertices[triangle.firstIndex];
	return (vertices[triangle.secondIndex] - v0) % (vertices[triangle.thirdIndex] - v0);
}

Vec3* Shape::getNormals() const {
	std::vector<Vec3>* normals = new std::vector<Vec3>();
	for (int i = 0; i < vertexCount; i++) {
		Vec3 vertex = vertices[i];
		Vec3 vertexNormal;
		for (int j = 0; j < triangleCount; j++) {
			Triangle triangle = triangles[j];
			if (triangle.firstIndex == i || triangle.secondIndex == i || triangle.thirdIndex == i) {

				while (triangle.firstIndex != i)
					triangle = triangle.rightShift();

				Vec3 v0 = vertices[triangle.firstIndex];
				Vec3 v1 = vertices[triangle.secondIndex];
				Vec3 v2 = vertices[triangle.thirdIndex];

				Vec3 D1 = v1 - v0;
				Vec3 D2 = v2 - v0;

				Vec3 faceNormal = D1 % D2;

				double sin = faceNormal.length() / (D1.length() * D2.length());
				vertexNormal += faceNormal.normalize() * asin(sin);
			}
		}
		vertexNormal = vertexNormal.normalize();
		normals->push_back(vertexNormal);
	}

	return &(*normals)[0];
}

/*
Checks that for every triangle, the outward-facing normal vector *dot* the vector from any of the points to the given point is negative
If at least one of these values is positive, then the point must be on the outside of that triangle, and hence, outside of the shape
only for convex shapes
*/
/*bool Shape::containsPoint(Vec3 point) const {
	for (int i = 0; i < triangleCount; i++) {
		Triangle triangle = triangles[i];
		Vec3 normalVec = getNormalVecOfTriangle(triangle);
		if((point - vertices[triangle.firstIndex]) * normalVec > 0) return false;
	}
	return true;
}*/


bool Shape::containsPoint(Vec3 point) const {
	Vec3 ray = Vec3(1, 0, 0);

	bool isExiting = false;
	double bestD = static_cast<double>(INFINITY);

	for(const Triangle& tri : iterTriangles()) {
		RayIntersection r = rayTriangleIntersection(point, ray, vertices[tri.firstIndex], vertices[tri.secondIndex], vertices[tri.thirdIndex]);
		if(r.d >= 0 && r.lineIntersectsTriangle()) {
			if(r.d < bestD) {
				bestD = r.d;
				isExiting = (getNormalVecOfTriangle(tri) * ray >= 0);
			}
		}
	}

	return isExiting;
}

//Shape Shape::getRubbingShape(const Shape& other, Vec3* newVecBuf, Triangle* newTriangleBuf) const {
	/*for(Triangle t : iterTriangles()) {

	}*/
	/*for(Vec3 v : iterVertices()) {

	}*/
//}


double Shape::getVolume() const {
	double total = 0;
	for (Triangle triangle : iterTriangles()) {
		Vec3 v0 = vertices[triangle.firstIndex]; 
		Vec3 v1 = vertices[triangle.secondIndex];
		Vec3 v2 = vertices[triangle.thirdIndex];

		Vec3 D1 = v1 - v0; 
		Vec3 D2 = v2 - v0;
		
		double Tf = (D1.x * D2.y - D1.y * D2.x);

		total += Tf * ((D1.z + D2.z) / 6 + v0.z / 2);
	}

	return total;
}

Vec3 Shape::getCenterOfMass() const {
	Vec3 total = Vec3(0,0,0);
	for (Triangle triangle : iterTriangles()) {
		Vec3 v0 = vertices[triangle.firstIndex];
		Vec3 v1 = vertices[triangle.secondIndex];
		Vec3 v2 = vertices[triangle.thirdIndex];

		Vec3 D1 = v1 - v0;
		Vec3 D2 = v2 - v0;

		Vec3 dFactor = D1 % D2;
		Vec3 vFactor = v0.squared() + v1.squared() + v2.squared() + v0.mul(v1) + v1.mul(v2) + v2.mul(v0);

		total += dFactor.mul(vFactor);
	}
	
	return total / (24 * getVolume());
}

Vec3 Shape::furthestInDirection(Vec3 direction) const {
	double bestDot = vertices[0] * direction;
	Vec3 bestVertex = vertices[0];
	for(int i = 1; i < vertexCount; i++) {
		double newD = vertices[i] * direction;
		if(newD > bestDot) {
			bestDot = newD;
			bestVertex = vertices[i];
		}
	}

	return bestVertex;
}

struct Simplex {
	Vec3 A, B, C, D;
	Vec3 At, Bt, Ct, Dt;
	int order;
	Simplex(Vec3 A, Vec3 At) : A(A), At(At), order(1) {}
	Simplex(Vec3 A, Vec3 B, Vec3 At, Vec3 Bt) : A(A), B(B), At(At), Bt(Bt), order(2) {}
	Simplex(Vec3 A, Vec3 B, Vec3 C, Vec3 At, Vec3 Bt, Vec3 Ct) : A(A), B(B), C(C), At(At), Bt(Bt), Ct(Ct), order(3) {}
	Simplex(Vec3 A, Vec3 B, Vec3 C, Vec3 D, Vec3 At, Vec3 Bt, Vec3 Ct, Vec3 Dt) : A(A), B(B), C(C), D(D), At(At), Bt(Bt), Ct(Ct), Dt(Dt), order(4) {}
	void insert(Vec3 newA, Vec3 newAt) {
		D = C;
		C = B;
		B = A;
		A = newA;
		Dt = Ct;
		Ct = Bt;
		Bt = At;
		At = newAt;
		order++;
	}
};

#define MAX_ITER 20

/*
	GJK algorythm, google it
*/
bool Shape::intersects(const Shape& other, Vec3& intersection) const {
	Vec3 searchDirection = Vec3(1.0, 0.0, 0.0);

	Vec3 furthestInDir = this->furthestInDirection(searchDirection);

	// first point
	Simplex s(furthestInDir - other.furthestInDirection(-searchDirection), furthestInDir);

	// set new searchdirection to be straight at the origin
	searchDirection = -s.A;

	for(int iteration=0; iteration < MAX_ITER; iteration++) {
		Vec3 furthest = this->furthestInDirection(searchDirection);
		Vec3 newPoint = furthest - other.furthestInDirection(-searchDirection);
		if(newPoint * searchDirection < 0) {
			return false; // the best point in the direction does not go past the origin, therefore the entire difference must be on this side of the origin, not containing it, thus no collision
		}

		//simplex[simplexLength] = newPoint;
		//simplexLength++;

		s.insert(newPoint, furthest);

		Vec3 AO = -s.A;
		switch(s.order) {
		case 4: {
			// A is new point, at the top of the tetrahedron
			Vec3 AB = s.B - s.A;
			Vec3 AC = s.C - s.A;
			Vec3 AD = s.D - s.A;
			Vec3 nABC = AB % AC;
			Vec3 nACD = AC % AD;
			Vec3 nADB = AD % AB;

			if(nACD * AO > 0) {
				// remove B and continue with triangle
				s = Simplex(s.A, s.C, s.D, s.At, s.Ct, s.Dt);
			} else {
				if(nABC * AO > 0) {
					// remove D and continue with triangle
					s = Simplex(s.A, s.B, s.C, s.At, s.Bt, s.Ct);
				} else {
					if(nADB * AO > 0) {
						// remove C and continue with triangle
						s = Simplex(s.A, s.D, s.B, s.At, s.Dt, s.Bt);
					} else {
						// GOTCHA! TETRAHEDRON COVERS THE ORIGIN!
						Vec3 vecs[]{s.A, s.B, s.C, s.D};
						Vec3 worldVecs[]{s.At, s.Bt, s.Ct, s.Dt};
						int bestI = 0;
						double bestDist = s.A.lengthSquared();

						for(int i = 1; i < 4; i++) {
							double newDist = vecs[i].lengthSquared();
							if(newDist < bestDist) {
								bestDist = newDist;
								bestI = i;
							}
						}

						intersection = worldVecs[bestI] - vecs[bestI] / 2; // guesstimate

						return true;
					}
				}
			}
		}

			
		case 3: { // triangle, check if closest to one of the edges, point, or face
			Vec3 AB = s.B - s.A;
			Vec3 AC = s.C - s.A;
			Vec3 normal = AB % AC;
			Vec3 nAB = AB % normal;
			Vec3 nAC = normal % AC;

			if(AO * nAB > 0) {
				if(AO*AB > 0) {
					// edge of AB is closest, searchDirection perpendicular to AB towards O
					s = Simplex(s.A, s.B, s.At, s.Bt);
					searchDirection = (AO % AB) % AB;
				} else {
					if(AO*AC > 0) {
						// edge AC is closest, searchDirection perpendicular to AC towards O
						s = Simplex(s.A, s.C, s.At, s.Ct);
						searchDirection = (AO % AC) % AC;
					} else {
						// Point A is closest, searchDirection is straight towards O
						s = Simplex(s.A, s.At);
						searchDirection = AO;
					}
				}
			} else {
				if(AO*nAC > 0) {
					if(AO*AC > 0) {
						// edge AC is closest, searchDirection perpendicular to AC towards O
						s = Simplex(s.A, s.C, s.At, s.Ct);
						searchDirection = (AO % AC) % AC;
					} else {
						// Point A is closest, searchDirection is straight towards O
						s = Simplex(s.A, s.At);
						searchDirection = AO;
					}
				} else {
					// hurray! best shape is tetrahedron
					// just find which direction to look in
					if(normal * AO > 0) {
						searchDirection = normal;
					} else {
						searchDirection = -normal;
						s = Simplex(s.A, s.C, s.B, s.At, s.Ct, s.Bt); // invert triangle
					}
				}
			}
			break;
		}

		case 2: { // line segment, check if line, or either point closer
				  // B can't be closer since we picked a point towards the origin
				  // Just one test, to see if the line segment or A is closer
			Vec3 BA = s.A - s.B;
			if(AO*BA > 0) { // AO*BA > 0 means that BA and AO are in the same-ish direction, so O must be closest to A and not the line
				s = Simplex(s.A, s.At);
				searchDirection = AO;
			} else {
				// simplex remains the same
				// new searchdirection perpendicular to the line, towards O as much as possible
				searchDirection = (AO % BA) % BA;
			}
			break;
		}
		case 1: // single point, just go towards the origin
			searchDirection = AO;
			break;
		}
	}
	return false;
}

Vec4 Shape::getCircumscribedSphere() const {
	BoundingBox bounds = getBounds();
	Vec3 center = Vec3(bounds.xmax - bounds.xmin, bounds.ymax - bounds.ymin, bounds.zmax - bounds.zmin) / 2.0;
	double radius = (Vec3(bounds.xmax, bounds.ymax, bounds.zmax) - center).length();
	return Vec4(center.x, center.y, center.z, radius);
}

/*
	The total inertial matrix is given by the integral over the volume of the shape of the following matrix:
	[[
	[y^2+z^2,    xy,    xz],
	[xy,    x^2+z^2,    yz],
	[xz,    yz,    x^2+y^2]
	]]

	This has been reworked to a surface integral resulting in the given formulae
*/
Mat3 Shape::getInertia(CFrame reference) const {
	Mat3 total = Mat3(0, 0, 0, 0, 0, 0, 0, 0, 0);
	for (Triangle triangle : iterTriangles()) {
		Vec3 v0 = reference.globalToLocal(vertices[triangle.firstIndex]);
		Vec3 v1 = reference.globalToLocal(vertices[triangle.secondIndex]);
		Vec3 v2 = reference.globalToLocal(vertices[triangle.thirdIndex]);

		Vec3 D1 = v1 - v0;
		Vec3 D2 = v2 - v0;

		Vec3 dFactor = D1 % D2;

		// Diagonal Elements
		Vec3 squaredIntegral = v0.cubed() + v1.cubed() + v2.cubed() + v0.squared().mul(v1 + v2) + v1.squared().mul(v0 + v2) + v2.squared().mul(v0 + v1) + v0.mul(v1).mul(v2);
		Vec3 diagonalElementParts = dFactor.mul(squaredIntegral) / 60;

		total.m00 += diagonalElementParts.y + diagonalElementParts.z;
		total.m11 += diagonalElementParts.z + diagonalElementParts.x;
		total.m22 += diagonalElementParts.x + diagonalElementParts.y;

		// Other Elements
		double selfProducts  =	v0.x*v0.y*v0.z + v1.x*v1.y*v1.z + v2.x*v2.y*v2.z;
		double twoSames      =	v0.x*v0.y*v1.z + v0.x*v1.y*v0.z + v0.x*v1.y*v1.z + v0.x*v0.y*v2.z + v0.x*v2.y*v0.z + v0.x*v2.y*v2.z +
								v1.x*v0.y*v0.z + v1.x*v1.y*v0.z + v1.x*v0.y*v1.z + v1.x*v1.y*v2.z + v1.x*v2.y*v1.z + v1.x*v2.y*v2.z +
								v2.x*v0.y*v0.z + v2.x*v1.y*v2.z + v2.x*v0.y*v2.z + v2.x*v1.y*v1.z + v2.x*v2.y*v0.z + v2.x*v2.y*v1.z;
		double allDifferents =	v0.x*v1.y*v2.z + v0.x*v2.y*v1.z + v1.x*v0.y*v2.z + v1.x*v2.y*v0.z + v2.x*v0.y*v1.z + v2.x*v1.y*v0.z;

		double xyzIntegral = -(3 * selfProducts + twoSames + allDifferents / 2) / 60;

		total.m01 += dFactor.z * xyzIntegral;
		total.m10 += dFactor.z * xyzIntegral;
		total.m02 += dFactor.y * xyzIntegral;
		total.m20 += dFactor.y * xyzIntegral;
		total.m12 += dFactor.x * xyzIntegral;
		total.m21 += dFactor.x * xyzIntegral;
	}
	
	return total;
}

Mat3 Shape::getInertia(Vec3 reference) const {
	return this->getInertia(CFrame(reference));
}

Mat3 Shape::getInertia(Mat3 reference) const {
	return this->getInertia(CFrame(reference));
}

Mat3 Shape::getInertia() const {
	return this->getInertia(CFrame());
}

double Shape::getIntersectionDistance(Vec3 origin, Vec3 direction) {
	const float EPSILON = 0.0000001f;
	double t = INFINITY;
	for (Triangle triangle : iterTriangles()) {
		Vec3 v0 = vertices[triangle.firstIndex];
		Vec3 v1 = vertices[triangle.secondIndex];
		Vec3 v2 = vertices[triangle.thirdIndex];

		Vec3 edge1, edge2, h, s, q;
		float a, f, u, v;

		edge1 = v1 - v0;
		edge2 = v2 - v0;

		h = direction % edge2;
		a = edge1 * h;

		if (a > -EPSILON && a < EPSILON) continue;   
		
		f = 1.0 / a;
		s = origin - v0;
		u = f * (s * h);

		if (u < 0.0 || u > 1.0) continue;
		
		q = s % edge1;
		v = f * direction * q;

		if (v < 0.0 || u + v > 1.0) continue;

		double r = f * edge2 * q;
		if (r > EPSILON) { 
			if (r < t) t = r;
		} else {
			//Log::debug("Line intersection but not a ray intersection");
			continue;
		}
	}

	return t;
}



/*
Two parts:
edge-edge intersections
face-vertex intersections
*/
/*void CenteredShape::getFutureIntersection(const CenteredShape& other, Vec3 offset, Vec3 relVel, Vec3 rotation1, Vec3 rotation2, Vec3& intersection, double& time) const {

}*/