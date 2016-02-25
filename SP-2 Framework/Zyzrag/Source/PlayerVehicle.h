#ifndef PLAYERVEHICLE_H
#define PLAYERVEHICLE_H

#include "Shop.h"
#include "Vehicles.h"


struct PlayerVehicle : Object {


	float thrust, pitch, yaw, delay;
	bool isDead, board;

    void update(double dt);

    bool fireBullets(int bulletFireRate);
   

    double bulletCurrCooldown = 0;



	PlayerVehicle() : thrust(100), pitch(0), yaw(0), delay(0), isDead(false){}
	
    ~PlayerVehicle(){}



};


#endif