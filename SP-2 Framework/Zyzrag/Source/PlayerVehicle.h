#ifndef PLAYERVEHICLE_H
#define PLAYERVEHICLE_H

#include "Shop.h"
#include "Vehicles.h"


struct PlayerVehicle : Object {


	float thrust, pitch, yaw, delay, respawnTimer;
	int health;
	bool isDead, board;

    void update(double dt);
	bool respawn(int time);
	void reset();

    bool fireBullets(int bulletFireRate);
   

    double bulletCurrCooldown = 0;



	PlayerVehicle() : thrust(0), pitch(0), yaw(0), delay(0), health(100), isDead(false){}
	
    ~PlayerVehicle(){}



};


#endif