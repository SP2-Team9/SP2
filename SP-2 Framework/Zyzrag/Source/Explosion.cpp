/////////////////////////////////////////////////////////////////
/*!

* \File Name: Explosion.cpp

* \author: Wong Keng Han Ashley

* \date: 23 feb 2016

* \description: functions and data for all Explosions

*/
/////////////////////////////////////////////////////////////////

#include "Explosion.h"


/////////////////////////////////////////////////////////////////
/*!

* \method: constructor

* \author: Wong Keng Han Ashley

* \date: 23 feb 2016

* \description: constructor for Explosion

*/
/////////////////////////////////////////////////////////////////
Explosion::Explosion(float newExplosionSize, float newExplosionSpeed, Vector3 currPos){


    explosionMaxSize = newExplosionSize;
    maxExplosionSpeed = newExplosionSpeed;

    Pos = currPos;


}

/////////////////////////////////////////////////////////////////
/*!

* \method: Destructor

* \author: Wong Keng Han Ashley

* \date: 23 feb 2016

* \description: Destructor for Explosion

*/
/////////////////////////////////////////////////////////////////
Explosion::~Explosion(){






}


/////////////////////////////////////////////////////////////////
/*!

* \method: explosionUpdate

* \author: Wong Keng Han Ashley

* \date: 23 feb 2016

* \description: update Bullets

*/
/////////////////////////////////////////////////////////////////
void Explosion::explosionUpdate(double dt){

    if (explosionCurrentSize < explosionMaxSize){
    
        explosionCurrentSize += dt * maxExplosionSpeed;

    }


}

/////////////////////////////////////////////////////////////////
/*!

* \method: set Pitch and Yaw

* \author: Wong Keng Han Ashley

* \date: 23 feb 2016

* \description:  set Pitch and Yaw

*/
/////////////////////////////////////////////////////////////////
void Explosion::setPitchandYaw(float newPitch, float newYaw){

    pitch = newPitch;
    yaw = newYaw;


}


/////////////////////////////////////////////////////////////////
/*!

* \method: get Pitch

* \author: Wong Keng Han Ashley

* \date: 23 feb 2016

* \description:  get pitch
*/
/////////////////////////////////////////////////////////////////
float Explosion::getPitch(){

    return pitch;


}


/////////////////////////////////////////////////////////////////
/*!

* \method:get yaw

* \author: Wong Keng Han Ashley

* \date: 23 feb 2016

* \description: get yaw

*/
/////////////////////////////////////////////////////////////////
float Explosion::getYaw(){

    return yaw;


}

/////////////////////////////////////////////////////////////////
/*!

* \method: explosionEnd

* \author: Wong Keng Han Ashley

* \date: 23 feb 2016

* \description: check if explosion ended

*/
/////////////////////////////////////////////////////////////////
bool Explosion::explosionEnd(){

    if (explosionCurrentSize > explosionMaxSize){

        return true;

    }

    return false;

}


/////////////////////////////////////////////////////////////////
/*!

* \method: getExplosionSize

* \author: Wong Keng Han Ashley

* \date: 23 feb 2016

* \description: check if explosion ended

*/
/////////////////////////////////////////////////////////////////
float Explosion::getExplosionSize(){

    return explosionCurrentSize;

}
