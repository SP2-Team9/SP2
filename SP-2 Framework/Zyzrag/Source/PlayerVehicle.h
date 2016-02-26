#ifndef PLAYERVEHICLE_H
#define PLAYERVEHICLE_H

#include "Shop.h"
#include "Vehicles.h"


struct PlayerVehicle : Object {


	
	int health;
    int maxHealth;

	bool isDead, board;

    void update(double dt);
	bool respawn(int time);
	void reset();

    bool fireBullets(int bulletFireRate);
   

    double bulletCurrCooldown = 0;

    float thrust, pitch, yaw, delay, respawnTimer;
    

    PlayerVehicle() : thrust(0), pitch(0), yaw(0), delay(0), health(100), isDead(false), maxHealth(100){
    
    
    
    }
	
    ~PlayerVehicle(){}



};


#endif