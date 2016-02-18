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
lastWayPointDirection(Vector3(0, 0, 0))
{

	wayPoints.push(Vector3(1, 1, 1));


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
currentLocation(location)
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
		currentLocation += view * 10 * dt;
		lastWayPointDirection = view;
		

	}
	if (!wayPoints.empty() && distanceBetween2points(currentLocation, wayPoints.front()) < 1){

		wayPoints.pop();
		

	}

	if (wayPoints.empty()){

		currentLocation += lastWayPointDirection * 10 * dt;


	}

	
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

* \method: updateWayPoints

* \author: Wong Keng Han Ashley

* \date: 15 feb 2016

* \description: setting initial waypoints for the object

*/

/////////////////////////////////////////////////////////////////
void pathFinding::updateWayPoints(Vector3 endLocation){

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




