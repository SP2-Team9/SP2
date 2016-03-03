/////////////////////////////////////////////////////////////////
/*!

* \file Asteroid.h

* \author: Goh ZHeng Yuan

* \date: 23 feb 2016

* \description: This class contains the Asteroid class.

*/
/////////////////////////////////////////////////////////////////
#ifndef ASTEROID_H
#define ASTEROID_H

#include "Object.h"

/******************************************************************************/
/*!
\brief
	The class Asteroid along with it's position, health, size.
*/
/******************************************************************************/
class Asteroid : public Object
{
public:

	float speed, size, health, curRange;
	int maxRange;
	bool boom;

    int maxHealth;

	Asteroid(float size);
	~Asteroid();

	void update(double dt);

};

#endif