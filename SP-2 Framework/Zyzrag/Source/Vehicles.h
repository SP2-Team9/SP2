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
#include "Asteroid.h"

class Vehicles : public Object{

public:

    bool checkMaxDistance(Vector3 playerPos);

    float maxDistance = 5000;
	int maxHealth, health;

	Vehicles();
    Vehicles(Vector3 endLocation);
    Vehicles(Vector3 position, Vector3 viewDirection, float newSpeed, int newHealth, float newFireRate, float newBulletDamage);

	~Vehicles();

    float getRotationAngle();
    float getRotationAngle(Vector3 newView);

   
	void update(double dt);
    void initialMoveDirection();
    void setThrust(float newThrust);
	void setNewWayPoint(float x, float z);
	void initialMoveDirection(float x, float z);


    bool fireBullets(double dt);

    double bulletCooldown;

    float Yaw, Pitch, delay, initialYaw;
    double bulletFireRate = 0;
    double bulletDamage = 0;
    double bulletCurrCooldown;


	bool board, isDead;

	pathFinding newVehicle;

    Asteroid* currAttackTarget;

  

};


#endif