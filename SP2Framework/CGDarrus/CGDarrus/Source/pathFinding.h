/////////////////////////////////////////////////////////////////
/*!

* \file pathFinding.h

* \author Wong Keng Han Ashley

* \date 15 feb 2016

* This cpp file contains all the declarations for use in pathFinding.cpp

*/
/////////////////////////////////////////////////////////////////

#include <vector>
#include <queue>
#include "Vector3.h"
#include "MyMath.h"



using std::vector;
using std::queue;


class pathFinding{
public:
	pathFinding();
	pathFinding(Vector3 location, Vector3 endlocation);
	virtual ~pathFinding();


	void pathRoute(double dt);
	void setInitialWayPoints(Vector3 endLocation);
	void updateWayPoints(Vector3 endLocation);
	float distanceBetween2points(Vector3 Point1, Vector3 Point2);





	queue<Vector3> wayPoints;
	Vector3 initialLocation;
	


};

