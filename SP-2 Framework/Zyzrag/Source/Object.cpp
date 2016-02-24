#include "Object.h"

Object::Object()
{

}

Object::Object(Vector3 pos, Vector3 view, Vector3 up, int newHealth)
{
	Pos = pos;
	View = view;
	Up = up;
	Right = view.Cross(up);
    health = newHealth;
}

Object::~Object()
{

}

void Object::SetHitbox(AABB hitbox)
{
    this->hitbox.Set(hitbox.GetMin(), hitbox.GetMax());
}

void Object::SetHitboxSize(float size)
{
	HitboxSize = size;
	SetHitbox(AABB(Pos.x - size, Pos.y - size, Pos.z - size, Pos.x + size, Pos.y + size, Pos.z + size));
}

void Object::SetInteraction(AABB interaction)
{
	this->interaction = interaction;
}

void Object::SetInteractionSize(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	InteractionMin = Vector3(minX, minY, minZ);
	InteractionMax = Vector3(maxX, maxY, maxZ);
}

void Object::SetPos(Vector3 position)
{
	Pos = position;
}

void Object::SetPos(float x, float y, float z)
{
	Pos.Set(x, y, z);
}

void Object::SetView(Vector3 view)
{
	View = view;
}

void Object::SetView(float x, float y, float z)
{
	View.Set(x, y, z);
}

void Object::SetUp(Vector3 up)
{
	Up = up;
}

void Object::SetUp(Vector3 right, Vector3 view)
{
	Up = right.Cross(view);
}

void Object::SetUp(float x, float y, float z)
{
	Up.Set(x, y, z);
}

void Object::SetRight(Vector3 right)
{
	Right = right;
}

void Object::SetRight(Vector3 view, Vector3 up)
{
	Right = view.Cross(up);
}

void Object::SetRight(float x, float y, float z)
{
	Right.Set(x, y, z);
}