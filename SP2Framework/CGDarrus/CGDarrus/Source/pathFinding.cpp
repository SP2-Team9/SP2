#include "pathFinding.h"


pathFinding::pathFinding() :
initialLocation(Vector3(0, 0, 0))
{

	wayPoints.push(Vector3(1, 1, 1));


}

pathFinding::pathFinding(Vector3 location, Vector3 endlocation) :
initialLocation(location)
{
	wayPoints.push(endlocation);

}

pathFinding::~pathFinding(){





}


void pathFinding::pathRoute(double dt){


	if (!wayPoints.empty()){

		Vector3 view = (wayPoints.front() - initialLocation).Normalized();
		initialLocation += view * 10 * dt;


	}

	if (!wayPoints.empty() && distanceBetween2points(initialLocation, wayPoints.front()) < 1){
		wayPoints.pop();
		std::cout << wayPoints.size() << std::endl;
	}
	
}


float pathFinding::distanceBetween2points(Vector3 Point1, Vector3 Point2){

	
	return sqrt(((Point1.x - Point2.x) * (Point1.x - Point2.x)) + ((Point1.y - Point2.y) *  (Point1.y - Point2.y)) + ((Point1.z - Point2.z)) *  (Point1.z - Point2.z));



}

void pathFinding::setInitialWayPoints(Vector3 endLocation){

	Vector3 view = (endLocation - initialLocation).Normalized();
	Vector3 wayPointPosition = initialLocation;

	float length = distanceBetween2points(endLocation, wayPointPosition);
	length /= 10;

	for (int i = 0; i < 10; i++){

		wayPoints.push(wayPointPosition);
		wayPointPosition += (view * length);


	}

	std::cout << wayPoints.size() << std::endl;

}