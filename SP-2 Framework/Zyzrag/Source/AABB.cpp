/////////////////////////////////////////////////////////////////
/*!

* \file AABB.cpp

* \author: Goh Zheng Yuan

* \date: 15 feb 2016

* \description: This cpp contains the functions of the class AABB

*/
/////////////////////////////////////////////////////////////////

#include "AABB.h"

/******************************************************************************/
/*!
\brief
	Default constructor
*/
/******************************************************************************/
AABB::AABB()
{
}

/******************************************************************************/
/*!
\brief
	Constructor that takes in Vector3 min and max
		
\param min - Gets the Vector3 minimum value
\param max - Gets the Vector3 maximum value
*/
/******************************************************************************/
AABB::AABB(const Vector3 min, const Vector3 max) : Min(min), Max(max) 
{
}

/******************************************************************************/
/*!
\brief
	Constructor that takes it float of min X, Y, Z and max X, Y, Z.

\param minX - Mininum X coordinates
\param minY - Mininum Y coordinates
\param minZ - Mininum Z coordinates
\param maxX - Maximum X coordinates
\param maxY - Maximum Y coordinates
\param maxZ - Maximum Z coordinates
*/
/******************************************************************************/
AABB::AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	Set(minX, minY, minZ, maxX, maxY, maxZ);
}

/******************************************************************************/
/*!
\brief
	Destructor
*/
/******************************************************************************/
AABB::~AABB()
{
}

/******************************************************************************/
/*!
\brief
	Sets the AABB's Vector3 min and max.

\param min - Gets the Vector3 mininum value
\param max - Gets the Vector3 maximum value
*/
/******************************************************************************/
void AABB::Set(const Vector3 min, const Vector3 max)
{
	this->Max = max;
	this->Min = min;
}

/******************************************************************************/
/*!
\brief
	Sets the AABB's Vector3 min and max.

\param minX - Mininum X coordinates
\param minY - Mininum Y coordinates
\param minZ - Mininum Z coordinates
\param maxX - Maximum X coordinates
\param maxY - Maximum Y coordinates
\param maxZ - Maximum Z coordinates
*/
/******************************************************************************/
void AABB::Set(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	this->Min = Vector3(minX, minY, minZ);
	this->Max = Vector3(maxX, maxY, maxZ);
}

/******************************************************************************/
/*!
\brief
	Checks if current AABB is colliding with another AABB

\param box - The other AABB class to check with
\param view - To check which direction the current AABB is heading.

\return 
	Returns true if colliding else false.
*/
/******************************************************************************/
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

/******************************************************************************/
/*!
\brief
	Checks if current AABB is colliding with another AABB

\param box - The other AABB class to check with

\return
	Returns true if colliding else false.
*/
/******************************************************************************/
bool AABB::AABBtoAABB(const AABB& box)
{
	if (Max.x >= box.Min.x && Min.x <= box.Max.x &&
		Max.y >= box.Min.y && Min.y <= box.Max.y &&
		Max.z >= box.Min.z && Min.z <= box.Max.z)
	{
		return true;
	}
	return false;
}

/******************************************************************************/
/*!
\brief
	Checks if current AABB is colliding with a vector of AABBs

\param box - vector AABB class to check with
\param view - To check which direction the current AABB is heading.

\return
	Returns true if colliding else false.
*/
/******************************************************************************/
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

/******************************************************************************/
/*!
\brief
	Checks if current AABB is colliding with a point

\param position - Position of the point

\return
	Returns true if colliding else false.
*/
/******************************************************************************/
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

/******************************************************************************/
/*!
\brief
	Checks if current AABB is colliding with a ray

\param rayOrigin - Get's the ray origin
\pram ray - Get's where the ray ends at

\return
	Returns true if colliding else false.
*/
/******************************************************************************/
bool AABB::RayToAABB(Vector3 rayOrigin, Vector3 ray)
{
	float T1, T2, Tnear, Tfar;
	T1 = T2 = Tnear = Tfar = 0.f;
	Vector3 rayFinal = ray * 100000.f;
	Vector3 rayDir = rayFinal - rayOrigin;
	rayDir.Normalize();

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

	return true;
}

/******************************************************************************/
/*!
\brief
	Get's Vector3 max of class.

\return
	Returns Vector3 max
*/
/******************************************************************************/
Vector3 AABB::GetMax()
{
	return Max;
}

/******************************************************************************/
/*!
\brief
	Get's Vector3 min of class.

\return
	Returns Vector3 min
*/
/******************************************************************************/
Vector3 AABB::GetMin()
{
	return Min;
}

/******************************************************************************/
/*!
\brief
	Checks if AABB is within the plane

\param init - Get's the initial starting point of the plane
\param end - Get's the ending point of the plane

\return
	True if AABB is within the plane
*/
/******************************************************************************/
bool AABB::withinPlane(Vector3 init, Vector3 end)
{
	float swap;
	if (init.x > end.x)
	{
		swap = init.x;
		init.x = end.x;
		end.x = swap;
	}
	if (init.y > end.y)
	{
		swap = init.y;
		init.y = end.y;
		end.y = swap;
	}
	if (init.z > end.z)
	{
		swap = init.z;
		init.z = end.z;
		end.z = swap;
	}
	if (Min.x > init.x && Max.x < end.x &&
		Min.z > init.z && Max.z < end.z)
	{
		return true;
	}
	return false;
}