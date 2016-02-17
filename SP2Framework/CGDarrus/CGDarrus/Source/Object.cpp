#include "Object.h"

Object::Object()
{

}

Object::Object(Vector3 pos, Vector3 view, Vector3 up)
{
	Pos = pos;
	View = view;
	Up = up;
	Right = view.Cross(up);
}

Object::~Object()
{

}

void Object::SetHitbox(AABB hitbox)
{
	this->hitbox = hitbox;
}

void Object::SetHitbox(float size)
{
	this->hitbox = AABB(Vector3(Pos * -size), Vector3(Pos * size));
}

void Object::SetInteraction(AABB interaction)
{
	this->interaction = interaction;
}

void SetInteraction(Vector3 distance)
{
	
}

void Object::SetPos(Vector3 position)
{
	Pos = position;
}

void Object::SetView(Vector3 view)
{
	View = view;
}

void Object::SetUp(Vector3 up)
{
	Up = up;
}

void Object::SetUp(Vector3 right, Vector3 view)
{
	Up = right.Cross(view);
}

void Object::SetRight(Vector3 right)
{
	Right = right;
}

void Object::SetRight(Vector3 view, Vector3 up)
{
	Right = view.Cross(up);
}