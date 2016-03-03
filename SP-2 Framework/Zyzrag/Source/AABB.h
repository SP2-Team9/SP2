/////////////////////////////////////////////////////////////////
/*!

* \file AABB.h

* \author: Goh Zheng Yuan

* \date: 15 feb 2016

* \description: This header contains the class AABB

*/
/////////////////////////////////////////////////////////////////

#ifndef AABB_H
#define AABB_H

#include "Vector3.h"
#include <vector>

using std::vector;


/******************************************************************************/
/*!
\brief
	Contains the class AABB hitbox, along with all it's functions
*/
/******************************************************************************/
class AABB
{
private:
	Vector3 Max;
	Vector3 Min;

public:
	AABB();
	AABB(const Vector3 min, const Vector3 max);
	AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
	~AABB();

	void Set(const Vector3 min, const Vector3 max);
	void Set(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
	bool AABBtoAABB(const AABB& box, Vector3& view);
	bool AABBtoAABB(const AABB& box);
	bool AABBtoAABB(const vector<AABB>& box, Vector3& view);
	bool RayToAABB(Vector3 rayOrigin, Vector3 ray);
	bool PointToAABB(const Vector3& position);
	bool withinPlane(Vector3 init, Vector3 end);

	Vector3 GetMax();
	Vector3 GetMin();
};

#endif