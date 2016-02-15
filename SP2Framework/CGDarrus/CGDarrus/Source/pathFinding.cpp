#include "pathFinding.h"


pathFinding::pathFinding(){





}


pathFinding::~pathFinding(){





}


Vector3 pathFinding::pathRoute(Vector3 initialLocation, queue<Vector3>& wayPoints, float dt){

	if (!wayPoints.empty()){

		Vector3 view = wayPoints.front() - initialLocation;
		view.Normalize();


		float length = sqrt((wayPoints.front().x * initialLocation.x) + (wayPoints.front().y * initialLocation.y) + (wayPoints.front().z * initialLocation.z));

		initialLocation += view * 10 * dt;

		

		if (length < 1){

			wayPoints.pop();

		}

		return 

	}

}





float pathFinding::distanceBetween2points(Vector3 ){


	



}