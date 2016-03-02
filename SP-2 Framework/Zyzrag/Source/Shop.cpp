/////////////////////////////////////////////////////////////////
/*!

* \File Name: Shop.cpp

* \author: Wong Keng Han Ashley

* \date: 24 feb 2016

* \description: functions and data for the Shop

*/
/////////////////////////////////////////////////////////////////

#include "Shop.h"


/////////////////////////////////////////////////////////////////
/*!

* \method: constructor

* \author: Wong Keng Han Ashley

* \date: 24 feb 2016

* \description: constructor for Shop

*/
/////////////////////////////////////////////////////////////////
Shop::Shop(PlayerVehicle& playerShip):
damageUpgradePrice(250),
healthUpgradePrice(75),
fireRateUpgradePrice(300)
{

	playerVehicle = &playerShip;

}

Shop::~Shop(){



}

void Shop::increaseHealth(){

	healthUpgradePrice *= 2;
	playerVehicle->maxHealth += 10;
	playerVehicle->health = playerVehicle->maxHealth;

}

void Shop::increaseDamage(){

	damageUpgradePrice *= 2;
	playerVehicle->bulletDamage += 10;

}

void Shop::increaseFireRate(){

	fireRateUpgradePrice *= 2;
	playerVehicle->bulletFireRate += 100;


}