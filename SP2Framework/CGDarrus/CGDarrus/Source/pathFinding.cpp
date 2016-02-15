#include "pathFinding.h"


pathFinding::pathFinding():
initialLocation(Vector3(0, 0, 0))
{





}

pathFinding::pathFinding(Vector3 location):
initialLocation(location)
{





}

pathFinding::~pathFinding(){





}


void pathFinding::pathRoute(queue<Vector3>& wayPoints, float dt){

	if (!wayPoints.empty()){

		Vector3 view = wayPoints.front() - initialLocation;
		view.Normalize();


		initialLocation += view * 10 * dt;
	

		if (distanceBetween2points(initialLocation, wayPoints.front()) < 1){

			wayPoints.pop();

		}

	}

}





float pathFinding::distanceBetween2points(Vector3 Point1, Vector3 Point2){

	
	float length = sqrt((Point1.x - Point2.x) + (Point1.z - Point2.z));
	return length;


}