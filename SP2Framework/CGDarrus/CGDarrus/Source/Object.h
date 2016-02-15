#ifndef OBJECT_H
#define OBJECT_H

#include "Vector3.h"
#include "AABB.h"

struct Object
{
	Object()
	{
		Warp = isDead = space = board = false;
		Thrust = Yaw = Pitch = Time = Cooldown = 0.f;
		Pos = View = Up = Right = Vector3(0, 0, 0);
	}
	AABB interaction;
	AABB hitbox;
	Vector3 Pos, View, Up, Right;
	float Thrust, Yaw, Pitch, Time, Cooldown, delay;
	bool Warp, board, isDead, space;
};

#endif