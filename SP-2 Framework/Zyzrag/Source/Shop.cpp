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
Shop::Shop(int Health, int Damage, int fireRate, int numSmallShips, int numMediumShips, int numLargeShips){

   playerShipHealth = Health;
   playerShipDamage = Damage;
   playerShipFireRate = fireRate;

   numberOfSmallShips = numSmallShips;
   numberOfMediumShips = numMediumShips;
   numberOfLargeShips = numLargeShips;


}

Shop::~Shop()
{

}
