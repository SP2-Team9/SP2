#ifndef PLAYERVEHICLE_H
#define PLAYERVEHICLE_H

#include "Object.h"

struct PlayerVehicle : public Object
{
	float thrust, pitch, yaw, delay;
	bool isDead, board;

	PlayerVehicle() : thrust(0), pitch(0), yaw(0), delay(0), isDead(false){}
	~PlayerVehicle(){}
};


#endif