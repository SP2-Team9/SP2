#ifndef OBJECT_H
#define OBJECT_H

#include "Vector3.h"
#include "AABB.h"

class Object
{
public:
	Object();
	Object(Vector3 pos, Vector3 view, Vector3 up);
	~Object();
	AABB interaction;
	AABB hitbox;
	Vector3 Pos, View, Up, Right;

	void SetHitbox(AABB hitbox);
	void SetHitbox(float size);
	void SetInteraction(AABB interaction);
	void SetInteraction(Vector3 distance);
	void SetPos(Vector3 position);
	void SetView(Vector3 view);
	void SetUp(Vector3 up);
	void SetUp(Vector3 right, Vector3 view);
	void SetRight(Vector3 right);
	void SetRight(Vector3 view, Vector3 up);
};

#endif