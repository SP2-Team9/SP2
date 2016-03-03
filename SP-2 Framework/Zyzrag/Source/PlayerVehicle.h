#ifndef PLAYERVEHICLE_H
#define PLAYERVEHICLE_H
#include "Vehicles.h"


struct PlayerVehicle : Object {


	
	int health, maxHealth;

	bool isDead, board;
    void update(double dt, vector<AABB> hitbox);
	bool respawn(int time);
	void reset();

	double bulletCurrCooldown = 0;

    bool fireBullets();
   
    float thrust, pitch, yaw, delay, respawnTimer;
    
	int bulletFireRate;
	int bulletDamage;

	PlayerVehicle();
	~PlayerVehicle();



};


#endif