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
	SetHitbox(AABB(Vector3(Pos.x - HitboxSize, Pos.y - HitboxSize, Pos.z - HitboxSize), Vector3(Pos.x + HitboxSize, Pos.y + HitboxSize, Pos.z + HitboxSize)));
	SetInteraction(AABB(Vector3(Pos.x - InteractionMin.x, Pos.y - InteractionMin.y, Pos.z - InteractionMin.z), Vector3(Pos.x + InteractionMax.x, Pos.y + InteractionMax.y, Pos.z + InteractionMax.z)));
	Yaw = getRotationAngle();
}


void Vehicles::setNewWayPoint(float x, float z){


	newVehicle.updateWayPoints(Vector3(x, 0, z));


}


void Vehicles::initialMoveDirection(float x, float z){


	newVehicle.setInitialWayPoints(Vector3(x, 0, z));


}

void Vehicles::initialMoveDirection(){

	newVehicle.setInitialWayPoints(Pos, View);

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

