#ifndef OBJECT_H
#define OBJECT_H

#include "AABB.h"

class Object
{
public:

	Object();
	Object(Vector3 pos, Vector3 view, Vector3 up, int health);
	~Object();
	AABB interaction;
	AABB hitbox;
	Vector3 Pos, View, Up, Right, InteractionMin, InteractionMax;
	float HitboxSize;

    int health;

	void SetHitbox(AABB hitbox);
	void SetHitboxSize(float size);
	void SetInteraction(AABB interaction);
	void SetInteractionSize(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
	void SetPos(Vector3 position);
	void SetPos(float x, float y, float z);
	void SetView(Vector3 view);
	void SetView(float x, float y, float z);
	void SetUp(Vector3 up);
	void SetUp(Vector3 right, Vector3 view);
	void SetUp(float x, float y, float z);
	void SetRight(Vector3 right);
	void SetRight(Vector3 view, Vector3 up);
	void SetRight(float x, float y, float z);

};

#endif