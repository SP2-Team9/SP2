#ifndef VEHICLES_H
#define VEHICLES_H

#include "Object.h"
#include "pathFinding.h"


class Vehicles : public Object
{
public:

	Vehicles();
	~Vehicles();

	void update();

	float Thrust, Yaw, Pitch, delay;
	bool board, isDead;

};

#endif