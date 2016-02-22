#ifndef ASTEROID_H
#define ASTEROID_H

#include "Object.h"

class Asteroid : public Object
{
public:
	float speed, size, health;
	bool boom;


	Asteroid(float size);
	~Asteroid();

	void update(double dt);
};

#endif