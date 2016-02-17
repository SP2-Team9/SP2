#ifndef VEHICLES_H
#define VEHICLES_H

#include "Object.h"

class Vehicles : public Object
{
public:
	Vehicles();
	~Vehicles();

	float Thrust, Yaw, Pitch, delay;
	bool board, isDead;
};

#endif