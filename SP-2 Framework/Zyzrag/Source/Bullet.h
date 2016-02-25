#ifndef _BULLET_H
#define _BULLET_H

/////////////////////////////////////////////////////////////////
/*!

* \File Name: Bullet.h

* \author: Wong Keng Han Ashley

* \date: 22 feb 2016

* \description: functions and data for all bullets

*/
/////////////////////////////////////////////////////////////////
#include "Object.h"
#include "pathFinding.h"
#include "AABB.h"
#include "Vector3.h"
#include "Mtx44.h"

class Bullet : public Object{

public:

    Bullet(Vector3 newView, Vector3 playerPos, float newBulletDamage);
    ~Bullet();
  
    void bulletUpdate(double dt);
    void bulletMovement(double dt);


    float getBulletDamage();

    bool furtherThanBulletMaxRange();

private:

    float bulletSpeed = 450;
    float BulletDamage = 30;
    float bulletMaxRange = 400;


  

    Vector3 initialPosition;




};

#endif