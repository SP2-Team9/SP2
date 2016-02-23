/////////////////////////////////////////////////////////////////
/*!

* \file: pathFinding.cpp

* \author: Wong Keng Han Ashley

* \date: 15 feb 2016

* \description: This cpp file contains all the methods for setting way points

*/
/////////////////////////////////////////////////////////////////


#include "pathFinding.h"

/////////////////////////////////////////////////////////////////

/*!

* \method pathFinding:

* \author Wong Keng Han Ashley:

* \date 15 feb 2016:

* \description: Default constructor

*/

/////////////////////////////////////////////////////////////////
pathFinding::pathFinding() :
currentLocation(Vector3(0, 0, 0)),
lastWayPointDirection(Vector3(0, 0, 0)),
speed(0)
{

}


/////////////////////////////////////////////////////////////////

/*!

* \method: pathFinding

* \author: Wong Keng Han Ashley

* \date: 15 feb 2016

* \description: overloaded constructor that takes in and sets initial location and final location

*/

/////////////////////////////////////////////////////////////////
pathFinding::pathFinding(Vector3 location, Vector3 endlocation) :
currentLocation(location),
speed(0)
{
	wayPoints.push(endlocation);

}


/////////////////////////////////////////////////////////////////

/*!

* \method: pathFinding

* \author: Wong Keng Han Ashley

* \date: 15 feb 2016

* \description: default constructor

*/

/////////////////////////////////////////////////////////////////
pathFinding::~pathFinding(){


    while (!wayPoints.empty()){

        wayPoints.pop();

    }
}

/////////////////////////////////////////////////////////////////

/*!

* \method: pathFinding

* \author: Wong Keng Han Ashley

* \date: 15 feb 2016

* \description: moves the object towards the waypoint

*/

/////////////////////////////////////////////////////////////////
void pathFinding::pathRoute(double dt){

	if (!wayPoints.empty()){

		Vector3 view = (wayPoints.front() - currentLocation).Normalized();
		currentLocation += view * speed * dt;
		lastWayPointDirection = view;
		

	}
	if (!wayPoints.empty() && distanceBetween2points(currentLocation, wayPoints.front()) < 1){

		wayPoints.pop();
		

	}

	if (wayPoints.empty()){

        currentLocation += lastWayPointDirection * speed * dt;

	}

    currentLocation.y = 0;
	
}

/////////////////////////////////////////////////////////////////

/*!

* \method: pathFinding

* \author: Wong Keng Han Ashley

* \date: 15 feb 2016

* \description: checks the distance between 2 vectors

*/

/////////////////////////////////////////////////////////////////
float pathFinding::distanceBetween2points(Vector3 Point1, Vector3 Point2){

	return sqrt(((Point1.x - Point2.x) * (Point1.x - Point2.x)) + ((Point1.y - Point2.y) *  (Point1.y - Point2.y)) + ((Point1.z - Point2.z)) *  (Point1.z - Point2.z));

}



/////////////////////////////////////////////////////////////////

/*!

* \method: setInitialWayPoints

* \author: Wong Keng Han Ashley

* \date: 15 feb 2016

* \description: setting initial waypoints for the object

*/

/////////////////////////////////////////////////////////////////
void pathFinding::setInitialWayPoints(Vector3 endLocation){

	Vector3 view = (endLocation - currentLocation).Normalized();
	Vector3 wayPointPosition = currentLocation;

	float length = distanceBetween2points(endLocation, wayPointPosition);
	
    length /= 10;

	for (int i = 0; i < 10; i++){

		wayPoints.push(wayPointPosition);
		wayPointPosition += (view * length);


	}

}

/////////////////////////////////////////////////////////////////

/*!

* \method: overLoaded setInitialWayPoints

* \author: Wong Keng Han Ashley

* \date: 18 feb 2016

* \description: overloaded function for setting way points

*/

/////////////////////////////////////////////////////////////////

void pathFinding::setInitialWayPoints(Vector3 location, Vector3 view){

	currentLocation = location;
	wayPoints.push(currentLocation + view);

}


/////////////////////////////////////////////////////////////////

/*!

* \method: updateWayPoints

* \author: Wong Keng Han Ashley

* \date: 15 feb 2016

* \description: setting initial waypoints for the object

*/

/////////////////////////////////////////////////////////////////
void pathFinding::updateWayPoints(Vector3 endLocation){

    if (currentLocation.distanceBetween2points(endLocation) > 25){

        Vector3 view = (endLocation - currentLocation).Normalized();
        Vector3 wayPointPosition = currentLocation;

        resetWayPoints();


        float length = distanceBetween2points(endLocation, wayPointPosition);
        length /= 15;

        for (int i = 0; i < 15; i++){

            wayPointPosition += (view * length);
            wayPoints.push(Vector3(wayPointPosition.x, 0, wayPointPosition.z));

        }
    }


}

/////////////////////////////////////////////////////////////////

/*!

* \method: resetWayPoints

* \author: Wong Keng Han Ashley

* \date: 16 feb 2016

* \description: remove all current waypoints

*/

/////////////////////////////////////////////////////////////////
void pathFinding::resetWayPoints(){

	while (!wayPoints.empty()){

        lastWayPointDirection = wayPoints.front();
		wayPoints.pop();

	}

}

/////////////////////////////////////////////////////////////////

/*!

* \method: getCurrentLocation

* \author: Wong Keng Han Ashley

* \date: 16 feb 2016

* \description: return current location

*/

/////////////////////////////////////////////////////////////////
Vector3 pathFinding::getCurrentLocation(){

	return currentLocation;

}

/////////////////////////////////////////////////////////////////

/*!

* \method: setCurrentLocation

* \author: Wong Keng Han Ashley

* \date: 19 feb 2016

* \description: return current location

*/

/////////////////////////////////////////////////////////////////
void pathFinding::setCurrentLocation(Vector3 newCurrLocation){

    currentLocation = newCurrLocation;

}



/////////////////////////////////////////////////////////////////

/*!

* \method: getwayPoints

* \author: Wong Keng Han Ashley

* \date: 17 feb 2016

* \description: returns the vector of way points

*/

/////////////////////////////////////////////////////////////////
queue<Vector3>  pathFinding::getwayPoints(){

	return wayPoints;

}


/////////////////////////////////////////////////////////////////

/*!

* \method: getLastWayPointDirection

* \author: Wong Keng Han Ashley

* \date: 17 feb 2016

* \description: return the direction of the previous waypoint

*/

/////////////////////////////////////////////////////////////////
Vector3 pathFinding::getLastWayPointDirection(){

	return lastWayPointDirection;

}

////////////////////////////////////////////////////////////////

/*!

* \method: setSpeed

* \author: Wong Keng Han Ashley

* \date: 18 feb 2016

* \description: set the speed of the vehicle
*/

/////////////////////////////////////////////////////////////////
void pathFinding::setSpeed(float newSpeed){


    speed = newSpeed;


}

////////////////////////////////////////////////////////////////

/*!

* \method: getSpeed

* \author: Wong Keng Han Ashley

* \date: 19 feb 2016

* \description: return speed

*/

/////////////////////////////////////////////////////////////////
float pathFinding::getSpeed(){


    return speed;


}






