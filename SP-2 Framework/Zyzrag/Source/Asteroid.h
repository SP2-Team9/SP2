#ifndef ASTEROID_H
#define ASTEROID_H

#include "Object.h"

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