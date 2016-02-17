#ifndef AABB_H
#define AABB_H

#include "Vector3.h"
#include <vector>

using std::vector;

class AABB
{
private:
	Vector3 Max;
	Vector3 Min;

public:
	AABB();
	AABB(const Vector3 min, const Vector3 max);
	~AABB();

	void Set(const Vector3 min, const Vector3 max);
	void Set(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
	bool AABBtoAABB(const AABB& box, Vector3& view);
	bool AABBtoAABB(const vector<AABB>& box, Vector3& view);
	bool RayToAABB(Vector3 rayOrigin, Vector3 ray);
	bool PointToAABB(const Vector3& position);

	Vector3 GetMax();
	Vector3 GetMin();
};

#endif