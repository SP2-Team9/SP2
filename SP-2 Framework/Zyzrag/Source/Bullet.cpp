/////////////////////////////////////////////////////////////////
/*!

* \File Name: Bullet.cpp

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
Bullet::Bullet(Vector3 newView, Vector3 playerPos, float newBulletDamage){

    Pos = playerPos;
    View = newView;
    View.Normalize();
    initialPosition = playerPos;
    BulletDamage = newBulletDamage;

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

* \method: furtherThanBulletMaxRange

* \author: Wong Keng Han Ashley

* \date: 22 feb 2016

* \description: check if the bullet is out of range

*/
/////////////////////////////////////////////////////////////////
bool Bullet::furtherThanBulletMaxRange(){

    if (initialPosition.distanceBetween2points(Pos) > bulletMaxRange){

        return true;

    }

    return false;

}

/////////////////////////////////////////////////////////////////
/*!

* \method: getBulletDamage

* \author: Wong Keng Han Ashley

* \date: 22 feb 2016

* \description: returns the bullet damage

*/
/////////////////////////////////////////////////////////////////
float Bullet::getBulletDamage(){

    return BulletDamage;

}