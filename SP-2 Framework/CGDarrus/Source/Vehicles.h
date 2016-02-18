#ifndef VEHICLES_H
#define VEHICLES_H

#include "Object.h"
#include "pathFinding.h"
#include "AABB.h"
#include "Vector3.h"


class Vehicles : public Object{

public:

	Vehicles();
	Vehicles(Vector3 endLocation);
	~Vehicles();

    float getRotationAngle();

	void update(double dt);
	void setNewWayPoint(float x, float z);
	void initialMoveDirection(float x, float z);
    


	float Thrust, Yaw, Pitch, delay;
	double interactionCooldown;
	bool board, isDead;

	pathFinding newVehicle;

   

};


#endif