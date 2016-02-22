/////////////////////////////////////////////////////////////////
/*!

* \File Name: Vehicles.cpp

* \author: Wong Keng Han Ashley

* \date: 22 feb 2016

* \description: functions and data for all bullets

*/
/////////////////////////////////////////////////////////////////

#include "Bullet.h"


/////////////////////////////////////////////////////////////////
/*!

* \method: constructor

* \author: Wong Keng Han Ashley

* \date: 22 feb 2016

* \description: constructor for Bullets

*/
/////////////////////////////////////////////////////////////////
Bullet::Bullet(Vector3 newView, Vector3 playerPos)
{

    Pos = playerPos;
    View = newView;
    View.Normalize();
    initialPosition = playerPos;

}

/////////////////////////////////////////////////////////////////
/*!

* \method: Destructor

* \author: Wong Keng Han Ashley

* \date: 22 feb 2016

* \description: Destructor for Bullets

*/
/////////////////////////////////////////////////////////////////
Bullet::~Bullet(){





}

/////////////////////////////////////////////////////////////////
/*!

* \method: bulletUpdate

* \author: Wong Keng Han Ashley

* \date: 22 feb 2016

* \description: update Bullets

*/
/////////////////////////////////////////////////////////////////
void Bullet::bulletUpdate(double dt){

    bulletMovement(dt);

}

/////////////////////////////////////////////////////////////////
/*!

* \method: bulletMovement

* \author: Wong Keng Han Ashley

* \date: 22 feb 2016

* \description: changes the position of the bullet

*/
/////////////////////////////////////////////////////////////////
void Bullet::bulletMovement(double dt){


    Pos += View * bulletSpeed * dt;


}


/////////////////////////////////////////////////////////////////
/*!

* \method: bulletMovement

* \author: Wong Keng Han Ashley

* \date: 22 feb 2016

* \description: changes the position of the bullet

*/
/////////////////////////////////////////////////////////////////
bool Bullet::furtherThanBulletMaxRange(){


    if (initialPosition.distanceBetween2points(Pos) > bulletMaxRange){

        return true;

    }


    return false;

}