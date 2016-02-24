#ifndef _EXPLOSION_H
#define _EXPLOSION_H

/////////////////////////////////////////////////////////////////
/*!

* \File Name: Explosion.h

* \author: Wong Keng Han Ashley

* \date: 22 feb 2016

* \description: functions and data for all explosions

*/
/////////////////////////////////////////////////////////////////
#include "Object.h"
#include "Vector3.h"




class Explosion: public Object
{

public:

    Explosion(float newExplosionSize, float newExplosionSpeed, Vector3 currPos);
    ~Explosion();

    float getYaw();
    float getPitch();
    float getExplosionSize();
    bool explosionEnd();

    void explosionUpdate(double dt);
    void setPitchandYaw(float newPitch, float newYaw);

private:

    float yaw = 0;
    float pitch = 0;
    float explosionMaxSize = 0;
    float maxExplosionSpeed = 0;
    float explosionCurrentSize = 0;
   

};



#endif _EXPLOSION_H