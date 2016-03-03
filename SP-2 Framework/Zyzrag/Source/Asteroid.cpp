/////////////////////////////////////////////////////////////////
/*!

* \file Asteroid.cpp

* \author: Goh Zheng Yuan

* \date: 23 feb 2016

* \description: This class contains the Asteroid class.

*/
/////////////////////////////////////////////////////////////////

#include "Asteroid.h"

/******************************************************************************/
/*!
\brief
	Constructor for Asteroid.

\param size - Sets size for asteroid.
*/
/******************************************************************************/
Asteroid::Asteroid(float size) : size(size), boom(false)
{
	health = 10 * size;
	speed = 500 / size;
	curRange = 0;
	maxRange = 2500;
	SetHitboxSize(size);
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

\param dt - Delta time.
*/
/******************************************************************************/
void Asteroid::update(double dt)
{
	
	
	Pos += View.Normalize() * dt * speed;
	curRange += View.getMagnitude() * dt * speed;
	updateHitbox();

}