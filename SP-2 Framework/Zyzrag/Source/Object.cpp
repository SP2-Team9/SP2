/////////////////////////////////////////////////////////////////
/*!

* \file AABB.cpp

* \author: Goh ZHeng Yuan

* \date: 15 feb 2016

* \description: This class contains the object class which has a position, view and up.

*/
/////////////////////////////////////////////////////////////////

#include "Object.h"

/******************************************************************************/
/*!
\brief
	Default constructor.
*/
/******************************************************************************/
Object::Object()
{

}

/******************************************************************************/
/*!
\brief
	Constuctor

\param pos
	Sets position of object.

\param view
	Sets view of object.

\param up
	Sets up of object.
*/
/******************************************************************************/
Object::Object(Vector3 pos, Vector3 view, Vector3 up)
{
	Pos = pos;
	View = view;
	Up = up;
	Right = view.Cross(up);
}

/******************************************************************************/
/*!
\brief
	Destructor.
*/
/******************************************************************************/
Object::~Object()
{

}

/******************************************************************************/
/*!
\brief
	Sets object hitbox.

\param hitbox
	Takes in an AABB class hitbox.
*/
/******************************************************************************/
void Object::SetHitbox(AABB hitbox)
{
	this->hitbox.Set(hitbox.GetMin(), hitbox.GetMax());
}

/******************************************************************************/
/*!
\brief
	Gives a default size to hitbox size.

\param size
	Size of hitbox wanted.
*/
/******************************************************************************/
void Object::SetHitboxSize(float size)
{
	hitboxSize.Set(-size, -size, -size, size, size, size);
}

/******************************************************************************/
/*!
\brief
	Gives a default size to hitbox size.

\param minX
	Min X coordinates.
\param minY
	Min Y coordinates.
\param minZ
	Min Z coordinates
\param maxX
	Max X coordinates.
\param maxY
	Max Y coordinates.
\param maxZ
	Max Z coordinates.
*/
/******************************************************************************/
void Object::SetHitboxSize(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	hitboxSize.Set(minX, minY, minZ, maxX, maxY, maxZ);
}

/******************************************************************************/
/*!
\brief
	Updates hitbox by getting object pos added with hitbox size.
*/
/******************************************************************************/
void Object::updateHitbox()
{
	hitbox.Set(Pos + hitboxSize.GetMin(), Pos + hitboxSize.GetMax());
}

/******************************************************************************/
/*!
\brief
	Set interaction hitbox
	
\param interaction
	Gets an AABB class hitbox.
*/
/******************************************************************************/
void Object::SetInteraction(AABB interaction)
{
	this->interaction = interaction;
}

/******************************************************************************/
/*!
\brief
Gives a default size to interaction hitbox size.

\param minX
Min X coordinates.
\param minY
Min Y coordinates.
\param minZ
Min Z coordinates
\param maxX
Max X coordinates.
\param maxY
Max Y coordinates.
\param maxZ
Max Z coordinates.
*/
/******************************************************************************/
void Object::SetInteractionSize(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	InteractionMin = Vector3(minX, minY, minZ);
	InteractionMax = Vector3(maxX, maxY, maxZ);
}

/******************************************************************************/
/*!
\brief
Set object's position

\param position
	Vector3 position wanted.
*/
/******************************************************************************/
void Object::SetPos(Vector3 position)
{
	Pos = position;
}

/******************************************************************************/
/*!
\brief
Set object's position.

\param x
	X coordinates
\param y
	Y coordinates
\param Z
	Z coordinates
*/
/******************************************************************************/
void Object::SetPos(float x, float y, float z)
{
	Pos.Set(x, y, z);
}

/******************************************************************************/
/*!
\brief
Set View of object

\param view
	Vector3 view wanted.
*/
/******************************************************************************/
void Object::SetView(Vector3 view)
{
	View = view;
}

/******************************************************************************/
/*!
\brief
Set View of object.

\param x
	X coordinates
\param y
	Y coordinates
\param Z
	Z coordinates
*/
/******************************************************************************/
void Object::SetView(float x, float y, float z)
{
	View.Set(x, y, z);
}

/******************************************************************************/
/*!
\brief
Set up of object.

\param up
	Vector3 up wanted.
*/
/******************************************************************************/
void Object::SetUp(Vector3 up)
{
	Up = up;
}

/******************************************************************************/
/*!
\brief
Sets up of object by getting the cross of right and view.

\param right
	Passes in the Vector3 right.

\param view
	Passes in the Vector3 view.
*/
/******************************************************************************/
void Object::SetUp(Vector3 right, Vector3 view)
{
	Up = right.Cross(view);
}

/******************************************************************************/
/*!
\brief
Set up of object.

\param x
X coordinates
\param y
Y coordinates
\param Z
Z coordinates
*/
/******************************************************************************/
void Object::SetUp(float x, float y, float z)
{
	Up.Set(x, y, z);
}

/******************************************************************************/
/*!
\brief
Set right of object.

\para right
	Vector3 right wanted.
*/
/******************************************************************************/
void Object::SetRight(Vector3 right)
{
	Right = right;
}

/******************************************************************************/
/*!
\brief
Set right of object by getting the cross of view and up.

\param view
	Vector3 view of object.

\param up
	Vector3 up of object.
*/
/******************************************************************************/
void Object::SetRight(Vector3 view, Vector3 up)
{
	Right = view.Cross(up);
}

/******************************************************************************/
/*!
\brief
Set Right of object.

\param x
	X coordinates
\param y
	Y coordinates
\param Z
	Z coordinates
*/
/******************************************************************************/
void Object::SetRight(float x, float y, float z)
{
	Right.Set(x, y, z);
}