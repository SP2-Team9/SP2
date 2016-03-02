#ifndef _SHOP_H
#define _SHOP_H

/////////////////////////////////////////////////////////////////
/*!

* \File Name: Shop.h

* \author: Wong Keng Han Ashley

* \date: 24 feb 2016

* \description: functions and data for the Shop

*/
/////////////////////////////////////////////////////////////////
#include "Object.h"
#include "PlayerVehicle.h"
#include "pathFinding.h"


struct Shop{


	void increaseHealth();
	void increaseDamage();
	void increaseFireRate();
	
	int damageUpgradePrice;
	int healthUpgradePrice;
	int fireRateUpgradePrice;

	Shop(PlayerVehicle& playerShip);
	~Shop();
    

	PlayerVehicle* playerVehicle;


};

#endif _SHOP_H