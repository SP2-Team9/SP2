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
#include "pathFinding.h"


struct Shop{


    Shop(int Health, int Damage, int fireRate, int numberOfSmallShips, int numMediumShips, int numLargeShip);
    ~Shop();

    int playerShipHealth;
    int playerShipDamage;
    int playerShipFireRate;
    
    int numberOfSmallShips;
    int numberOfLargeShips;
    int numberOfMediumShips;

};

#endif _SHOP_H