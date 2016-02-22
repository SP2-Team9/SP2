/////////////////////////////////////////////////////////////////
/*!

* \File Name: Vehicles.h

* \author: Wong Keng Han Ashley

* \date: 14 feb 2016

* \description: functions and data for all vehicles

*/
/////////////////////////////////////////////////////////////////

#ifndef VEHICLES_H
#define VEHICLES_H

#include "Object.h"
#include "pathFinding.h"
#include "AABB.h"
#include "Vector3.h"
#include "Mtx44.h"
#include "Bullet.h"

class Vehicles : public Object{

public:

	Vehicles();
    Vehicles(Vector3 endLocation);
    Vehicles(Vector3 position, Vector3 moveDirection, float speed, int health);

	~Vehicles();

    float getRotationAngle();
    float getRotationAngle(Vector3 newView);


	void update(double dt);
    void initialMoveDirection();
    void setThrust(float newThrust);
	void setNewWayPoint(float x, float z);
	void initialMoveDirection(float x, float z);
	


	float Yaw, Pitch, delay, initialYaw;

	bool board, isDead;

	pathFinding newVehicle;

   
};


#endif