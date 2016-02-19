#ifndef VEHICLES_H
#define VEHICLES_H

#include "Object.h"
#include "pathFinding.h"
#include "AABB.h"
#include "Vector3.h"
#include "Mtx44.h"


class Vehicles : public Object{

public:

	Vehicles();
    Vehicles(Vector3 endLocation);
    Vehicles(Vector3 position, Vector3 moveDirection, float speed);

	~Vehicles();

    float getRotationAngle();
    float getRotationAngle(Vector3 newView);

	void update(double dt);
    void initialMoveDirection();
    void setThrust(float newThrust);
	void setNewWayPoint(float x, float z);
	void initialMoveDirection(float x, float z);
	


	float Yaw, delay, initialYaw;

	bool board, isDead;

	pathFinding newVehicle;

   

};


#endif