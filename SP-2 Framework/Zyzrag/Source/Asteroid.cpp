/////////////////////////////////////////////////////////////////
/*!

* \file AABB.cpp

* \author: Goh ZHeng Yuan

* \date: 23 feb 2016

* \description: This class contains the Asteroid class.

*/
/////////////////////////////////////////////////////////////////

#include "Asteroid.h"

/******************************************************************************/
/*!
\brief
	Constructor for Asteroid.

\param size
	Sets size for asteroid.
*/
/******************************************************************************/
Asteroid::Asteroid(float size) : size(size), boom(false)
{
	health = 10 * size;
	speed = 600 / size;
	curRange = 0;
	maxRange = 5000;

    maxHealth = health;
}

/******************************************************************************/
/*!
\brief
	Destructor for Asteroid.
*/
/******************************************************************************/
Asteroid::~Asteroid()
{

}

/******************************************************************************/
/*!
\brief
	Update asteroid position, range, hitbox and health.

\param dt
	Delta time.
*/
/******************************************************************************/
void Asteroid::update(double dt)
{
	Pos += View.Normalize() * dt * speed;
	curRange += View.getMagnitude() * dt * speed;

	if (curRange > maxRange)
		boom = true;

	SetHitboxSize(size);
	updateHitbox();
	if (health < 0)
		boom = true;

}