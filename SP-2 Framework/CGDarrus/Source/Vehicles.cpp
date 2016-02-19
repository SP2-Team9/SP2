#include "Vehicles.h"

Vehicles::Vehicles() :
Yaw(0),
delay(0),
board(false),
isDead(false)
{




}

Vehicles::Vehicles(Vector3 moveDirection) :
Yaw(0),
delay(0),
board(false),
isDead(false)
{
	
}


Vehicles::Vehicles(Vector3 position, Vector3 viewDirection, float newSpeed) :
board(false),
isDead(false)
{

    Vector3 initialPosition = position + viewDirection.Normalize();
    Pos = position;
    View = viewDirection;
    newVehicle.setSpeed(newSpeed);
    initialMoveDirection();
    newVehicle.setCurrentLocation(position);
    initialYaw = getRotationAngle(viewDirection);



}


Vehicles::~Vehicles(){





}

void Vehicles::update(double dt){


	newVehicle.pathRoute(dt);
    Pos = newVehicle.getCurrentLocation();
	SetHitbox(AABB(Vector3(Pos.x - HitboxSize, Pos.y - HitboxSize, Pos.z - HitboxSize), Vector3(Pos.x + HitboxSize, Pos.y + HitboxSize, Pos.z + HitboxSize)));

	SetInteraction(AABB(Vector3(Pos.x - InteractionMin.x, Pos.y - InteractionMin.y, Pos.z - InteractionMin.z), Vector3(Pos.x + InteractionMax.x, Pos.y + InteractionMax.y, Pos.z + InteractionMax.z)));
	Yaw = getRotationAngle();

    SetInteraction(AABB(Vector3(Pos.x - InteractionMin.x, Pos.y - InteractionMin.y, Pos.z - InteractionMin.z), Vector3(Pos.x + InteractionMax.x, Pos.y + InteractionMax.y, Pos.z + InteractionMax.z)));
    getRotationAngle();

    std::cout << View << std::endl;

    /*if (!newVehicle.getwayPoints().empty()){

        View = newVehicle.getwayPoints().front() - Pos;
        View.Normalize();

    }

    std::cout << View << std::endl;*/

    Pos.y = 0;

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

* \method: getRotationAngle

* \author: Wong Keng Han Ashley

* \date: 18 feb 2016

* \description: setting initial waypoints for the object

*/
/////////////////////////////////////////////////////////////////
float Vehicles::getRotationAngle(){

    float degree = Math::RadianToDegree(acos(View.Dot(newVehicle.getLastWayPointDirection())));

    
    Vector3 n = View.Cross(newVehicle.getLastWayPointDirection());
         
    if ((n.Dot(Vector3(0, 1, 0))) < 0){

        degree *= -1;

    }

    Yaw = initialYaw + degree;

    return degree;

}


/////////////////////////////////////////////////////////////////
/*!

* \method: getRotationAngle

* \author: Wong Keng Han Ashley

* \date: 18 feb 2016

* \description: sets the speed of the vehicle

*/
/////////////////////////////////////////////////////////////////
void Vehicles::setThrust(float newThrust){

    newVehicle.setSpeed(newThrust);

}

/////////////////////////////////////////////////////////////////
/*!

* \method: overloaded getRotationAngle

* \author: Wong Keng Han Ashley

* \date: 18 feb 2016

* \description: setting initial waypoints for the object

*/
/////////////////////////////////////////////////////////////////
float Vehicles::getRotationAngle(Vector3 newView){

    float degree = Math::RadianToDegree(acos(newView.Dot(Vector3(0, 0, 1))));

    if (newView.x < 0){

        degree *= -1;

    }

    return degree;

}