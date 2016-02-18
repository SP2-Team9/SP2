#include "AABB.h"

AABB::AABB()
{
}

AABB::AABB(const Vector3 min, const Vector3 max) : Min(min), Max(max) 
{
}

AABB::~AABB()
{
}

void AABB::Set(const Vector3 min, const Vector3 max)
{
	this->Max = max;
	this->Min = min;
}

void AABB::Set(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	this->Min = Vector3(minX, minY, minZ);
	this->Max = Vector3(maxX, maxY, maxZ);
}

bool AABB::AABBtoAABB(const AABB& box, Vector3& view)
{
	if (Max.x + view.x >= box.Min.x && Min.x + view.x <= box.Max.x &&
		Max.y + view.y >= box.Min.y && Min.y + view.y <= box.Max.y &&
		Max.z + view.z >= box.Min.z && Min.z + view.z <= box.Max.z)
	{
		return true;
	}
	return false;
}

bool AABB::AABBtoAABB(const vector<AABB>& box, Vector3& view)
{
	for (vector<AABB>::const_iterator cVit = box.begin(); cVit != box.end(); ++cVit)
	{
		if (Max.x + view.x >= cVit->Min.x && Min.x + view.x <= cVit->Max.x &&
			Max.y + view.y >= cVit->Min.y && Min.y + view.y <= cVit->Max.y &&
			Max.z + view.z >= cVit->Min.z && Min.z + view.z <= cVit->Max.z)
		{
			return true;
		}
	}
	return false;
}

bool AABB::PointToAABB(const Vector3& position)
{
	if (position.x >= Min.x && position.x <= Max.x &&
		position.y >= Min.y && position.y <= Max.y &&
		position.z >= Min.z && position.z <= Max.z)
	{
		return true;
	}
	return false;
}

bool AABB::RayToAABB(Vector3 rayOrigin, Vector3 ray)
{
	float T1, T2, Tnear, Tfar;
	T1 = T2 = Tnear = Tfar = 0.f;
	Vector3 rayFinal = ray * 1000.f;
	Vector3 rayDir = (rayFinal - rayOrigin).Normalized();

	// For X Axis
	Tnear = (Min.x - rayOrigin.x) / rayDir.x;
	Tfar = (Max.x - rayOrigin.x) / rayDir.x;

	// Swap
	if (Tnear > Tfar)
	{
		float temp = Tnear;
		Tnear = Tfar;
		Tfar = temp;
	}

	// For Y Axis
	T1 = (Min.y - rayOrigin.y) / rayDir.y;
	T2 = (Max.y - rayOrigin.y) / rayDir.y;

	if (T1 > T2)
	{
		float temp = T1;
		T1 = T2;
		T2 = temp;
	}

	if (Tnear > T2 || T1 > Tfar)
		return false;

	if (T1 > Tnear)
		Tnear = T1;
	if (T2 < Tfar)
		Tfar = T2;

	// For Z Axis
	T1 = (Min.z - rayOrigin.z) / rayDir.z;
	T2 = (Max.z - rayOrigin.z) / rayDir.z;

	if (T1 > T2)
	{
		float temp = T1;
		T1 = T2;
		T2 = temp;
	}

	if (Tnear > T2 || T1 > Tfar)
		return false;

	if (T1 > Tnear)
		Tnear = T1;
	if (T2 < Tfar)
		Tfar = T2;

	std::cout << "TRUE" << std::endl;
	return true;
}

Vector3 AABB::GetMax()
{
	return Max;
}

Vector3 AABB::GetMin()
{
	return Min;
}