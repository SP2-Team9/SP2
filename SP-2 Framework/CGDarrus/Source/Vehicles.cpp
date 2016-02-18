#include "Vehicles.h"

Vehicles::Vehicles() :
Thrust(0),
Yaw(0),
Pitch(0),
delay(0),
interactionCooldown(0),
board(false),
isDead(false)
{




}



Vehicles::Vehicles(Vector3 moveDirection) :
Thrust(0),
Yaw(0),
Pitch(0),
delay(0),
interactionCooldown(0),
board(false),
isDead(false)
{

	initialMoveDirection(moveDirection.x, moveDirection.z);


}


Vehicles::~Vehicles(){



	


}

void Vehicles::update(double dt){


	newVehicle.pathRoute(dt);

	SetPos(newVehicle.getCurrentLocation().x, 0, newVehicle.getCurrentLocation().z);
	
	SetHitbox(AABB(Vector3(Pos.x - 5, Pos.y - 5, Pos.z - 5), Vector3(Pos.x + 5, Pos.y + 5, Pos.z + 5)));


}


void Vehicles::setNewWayPoint(float x, float z){


	newVehicle.updateWayPoints(Vector3(x, 0, z));


}


void Vehicles::initialMoveDirection(float x, float z){


	newVehicle.setInitialWayPoints(Vector3(x, 0, z));


}

/////////////////////////////////////////////////////////////////
/*!

* \method: getwayPoints

* \author: Wong Keng Han Ashley

* \date: 18 feb 2016

* \description: setting initial waypoints for the object

*/
/////////////////////////////////////////////////////////////////
float Vehicles::getRotationAngle(){

    float degree = Math::RadianToDegree(acos(View.Dot(newVehicle.getLastWayPointDirection())));

    if (newVehicle.getLastWayPointDirection().x < 0){

        degree *= -1;

    }

    return degree;

}

